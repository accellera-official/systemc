/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/
/*****************************************************************************

  Original Author: Mark Burton, GreenSocs

 *****************************************************************************/

#ifndef ASYNCTESTNODE_H
#define ASYNCTESTNODE_H

//#define DEBUG
#include <sstream>
#include <systemc>
#include <tlm>
#include <tlm_utils/tlm_quantumkeeper.h>
#include <queue>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include <tlm_utils/multi_passthrough_initiator_socket.h>
#include <tlm_utils/multi_passthrough_target_socket.h>

#include "async_event.h"
#include "collector.h"

const char *sycname="SystemC";

#define SPEEDSYSTEMC 1000
#define SPEEDNODE 100
#define SPEEDBTRANS 10

/* Each 'node' consists of a SystemC thread, and a std::thread component.
 *
 * The std::thread gets transactions from a queue (or creates new ones), and 'sends'
 * them by posting them to a 'mailbox' (txn), and then signalling SystemC using
 * an async_events (sendIt). It then uses a 'semaphore' (txnSent) to wait till
 * the txn is sent, before sending another one.
 * The std::thread has it's own notion of 'time' (myTime)
 *
 * Meanwhile, the SystemC thread receives the async event (sendIt), in a
 * SC_THREAD (sendTxns). This must be a thread, so that 'wait' can be called.
 * The txn's are sent to a 'random' other node, where they are processed by a
 * b_transport which calls wait().
 * Once done, sendTxns releases the semaphore allowing the std::thread to continue.
 *
 * In order to maintain synchronisation, the SystemC thread can do 2 things:
 *  1. If the 'nodes' local time (myTime) is ahead of SystemC, the std::thread
 *  semaphore is released by notifying a method at the appropriate myTime.
 *  2. Else SystemC is requested to suspend (waiting for the node to catch up).
 *
 * The b_transport call is protected as 'unsuspendable', to ensure that all
 * b_transport calls can complete.
 */

SC_MODULE (asynctestnode)
{
public:
    // Each node can send/receive from each other node.
    tlm_utils::multi_passthrough_initiator_socket<asynctestnode> init_socket;
    tlm_utils::multi_passthrough_target_socket<asynctestnode> target_socket;

    // Queue of TXN's protected by a mutex.
    std::mutex tQueue;
    std::queue<tlm::tlm_generic_payload *> queue;

    // C++ semaphore structure (used to signal when a txn has been sent by the
    // SystemC thread)
    std::mutex txnSent_m;
    std::condition_variable txnSent_cv;
    int txnSent_c;


    std::thread m_thread;

    async_event sendIt;            // request from std::thread to SystemC to send
                                   // the txn in the mailbox. async_event is
                                   // like a normal event, but thread safe.

    tlm::tlm_generic_payload *txn; // The txn in flight (mailbox)
    sc_core::sc_time myTime;

    sc_event txnSentEvent;         // when we're done

    bool suspendReq;               // To ensure that we only request a suspend
                                   // once from this thread.

    collector &col;                // just used to print out the results


    bool running;
    bool finished;

    asynctestnode(sc_core::sc_module_name name, collector &c) :
        init_socket("output"),
        target_socket("input"),
        txnSent_c(0),
        suspendReq(false),
        col(c),
        running(true),
        finished(false)
    {
        myTime = sc_core::SC_ZERO_TIME;

        target_socket.register_b_transport( this, &asynctestnode::b_transport);

        SC_THREAD(startProcessQueue);
        SC_THREAD(timeconsumer);
        SC_THREAD(sendTxns);

        SC_METHOD(txnSentMethod);
        dont_initialize();
        sensitive << txnSentEvent;
    }

    ~asynctestnode()
    {
        running = false;
        while (!finished)
            txnSentMethod();
        if (m_thread.joinable()) m_thread.join();
    }

    // This will cause SystemC time to be driven forwards. But, if we're not
    // careful, it could go too fast !
    void timeconsumer()
    {
        while(1)
        {
            wait(rand() % SPEEDSYSTEMC, SC_NS);
            col.add(sycname, sc_time_stamp());

#ifdef DEBUG
            std::stringstream msg;
            msg << name() << " sc_time now " << sc_time_stamp() <<"\n";
            std::cout << msg.str();
#endif

        }
    }

    // We are in SystemC
    void b_transport(int from, tlm::tlm_generic_payload &trans, sc_core::sc_time &delay)
    {
        wait(rand() % SPEEDBTRANS, SC_NS); // we'll do stuff, it takes time....
        col.add(sycname, sc_time_stamp());

        // return the txn into the queue - this will be picked up by the other
        // thread to be re-used...
        {
            std::lock_guard<std::mutex> guard(tQueue);
            queue.push(&trans);
        }
    }

    void startProcessQueue()
    {
        wait(SC_ZERO_TIME);
        col.add(sycname, sc_time_stamp());
        // Start the other thread once SystemC really has got going....
        m_thread = std::thread([&]{processQueue();});
    }

    // We are NOT in SystemC - separate thread - DONT CALL WAIT!  
    void processQueue()
    {
        while (running) {
            {
                std::lock_guard<std::mutex> guard(tQueue);
                if (queue.empty())
                {
                    txn = new tlm::tlm_generic_payload();
                }
                else
                {
                    txn = queue.front();
                    queue.pop();
                }
            }

            myTime += sc_core::sc_time(rand() % SPEEDNODE, sc_core::SC_NS);
            col.add(name(), myTime);

#ifdef DEBUG
            std::stringstream msg;
            msg << name() << " sending txn at sc_time "<< sc_time_stamp() << " our time " << myTime <<" ( "<<queue.size()<<" txn's in my queue)\n";
            std::cout << msg.str();
#endif

            sendIt.notify(); // Ask SystemC to send the actual transaction
            {
                std::unique_lock<std::mutex> lock(txnSent_m);
                while (txnSent_c == 0)
                {
                    txnSent_cv.wait(lock);
                }
                txnSent_c--;
            }
        }

        finished=true;

    }

    // This could be an 'SC_NSMETHOD' or some such
    // We are in SystemC
    void sendTxns()
    {
        while(1)
        {
            wait(sendIt);
            col.add(sycname, sc_time_stamp());
            // NB other thread is BLOCKED
            sc_unsuspendable();
            {

                // Send transaction to a random place
                init_socket[rand() % init_socket.size()]->b_transport(*txn, myTime);

#ifdef DEBUG
                std::stringstream msg;
                msg << name() << " txn completed at sc_time "<< sc_time_stamp() << " our time " << myTime <<"\n";
                std::cout << msg.str();
#endif

                // Sync policy - tightly coupled:
                // Allow us to continue when SystemC time is ahead (or
                // equal) to our time. Ask to slow down SystemC if it's ahead
                // NB if a node 'slows down' (and not send a txn) it could
                // get (significantly) behind SystemC which would not stop
                // e.g. this sync policy is just a one-sided example.
                if (sc_time_stamp() > myTime)
                {
                    txnSentEvent.notify();  // The notify will happen before the
                                            // end of this delta, so will happen
                                            // before we suspend.
                    if (!suspendReq)
                    {
                        suspendReq = true;
                        sc_suspend_all();
                    }
                }
                else
                {
                    if (suspendReq)
                    {
                        sc_unsuspend_all();
                        suspendReq = false;
                    }

                    txnSentEvent.notify(myTime - sc_time_stamp());
                }
            }
            sc_suspendable();
        }
    }

    void txnSentMethod()
    {
        col.add(sycname,sc_time_stamp());
        // RELEASE other thread
        {
            std::unique_lock<std::mutex> lock(txnSent_m);
            txnSent_c++;
            txnSent_cv.notify_one();
        }
    }

};

#endif
