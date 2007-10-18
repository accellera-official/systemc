/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

#ifndef SIMPLE_AT_SLAVE1_H
#define SIMPLE_AT_SLAVE1_H

//#include "tlm.h"
#include "simple_slave_socket.h"
//#include <systemc.h>
#include <cassert>
#include <vector>
#include <queue>
//#include <iostream>

class SimpleATSlave1 : public sc_module
{
public:
  typedef tlm::tlm_generic_payload transaction_type;
  typedef tlm::tlm_phase phase_type;
  typedef SimpleSlaveSocket<> slave_socket_type;

public:
  slave_socket_type socket;

public:
  SC_HAS_PROCESS(SimpleATSlave1);
  SimpleATSlave1(sc_module_name name) :
    sc_module(name),
    socket("socket"),
    ACCEPT_DELAY(25, SC_NS),
    RESPONSE_DELAY(100, SC_NS)
  {
    // register nb_transport method
    REGISTER_SOCKETPROCESS(socket, myNBTransport);

    SC_METHOD(endRequest)
    sensitive << mEndRequestEvent;
    dont_initialize();

    SC_METHOD(beginResponse)
    sensitive << mBeginResponseEvent;
    dont_initialize();

    SC_METHOD(endResponse)
    sensitive << mEndResponseEvent;
    dont_initialize();
  }

  //
  // Simple AT slave
  // - Request is accepted after ACCEPT delay (relative to end of prev request phase)
  // - Response is started after RESPONSE delay (relative to end of prev resp phase)
  //
  bool myNBTransport(transaction_type& trans, phase_type& phase, sc_time& t)
  {
    if (phase == tlm::BEGIN_REQ) {
      sc_dt::uint64 address = trans.get_address();
      assert(address < 400);

      unsigned int& data = *reinterpret_cast<unsigned int*>(trans.get_data_ptr());
      if (trans.get_command() == tlm::TLM_WRITE_COMMAND) {
        std::cerr << name() << ": Received write request: A = "
                  << (void*)(int)address << ", D = " << (void*)data
                  << " @ " << sc_time_stamp() << std::endl;

        *reinterpret_cast<unsigned int*>(&mMem[address]) = data;

      } else {
        std::cerr << name() << ": Received read request: A = "
                  << (void*)(int)address << " @ " << sc_time_stamp() << std::endl;

        data = *reinterpret_cast<unsigned int*>(&mMem[address]);
      }

      // Notify end of request phase after ACCEPT delay
      if (mEndRequestQueue.empty()) {
        mEndRequestEvent.notify(t + ACCEPT_DELAY);
      }
      mEndRequestQueue.push(&trans);

      // AT-noTA slave
      // - always return false
      // - seperate call to indicate end of phase (do not update phase or t)
      return false;

    } else if (phase == tlm::END_RESP) {

      // response phase ends after t
      mEndResponseEvent.notify(t);

      return true;
    }

    // Not possible
    assert(0); exit(1);
    return false;
  }

  void endRequest()
  {
    assert(!mEndRequestQueue.empty());
    // end request phase of oldest transaction
    phase_type phase = tlm::END_REQ;
    sc_time t = SC_ZERO_TIME;
    transaction_type* trans = mEndRequestQueue.front();
    assert(trans);
    mEndRequestQueue.pop();
    bool r = socket->nb_transport(*trans, phase, t);
    assert(!r); // FIXME: master should return false?
    assert(t == SC_ZERO_TIME); // t must be SC_ZERO_TIME

    // Notify end of request phase for next transaction after ACCEPT delay
    if (!mEndRequestQueue.empty()) {
      mEndRequestEvent.notify(ACCEPT_DELAY);
    }

    if (mResponseQueue.empty()) {
      // Start processing transaction
      // Notify begin of response phase after RESPONSE delay
      mBeginResponseEvent.notify(RESPONSE_DELAY);
    }
    mResponseQueue.push(trans);
  }

  void beginResponse()
  {
    assert(!mResponseQueue.empty());
    // start response phase of oldest transaction
    phase_type phase = tlm::BEGIN_RESP;
    sc_time t = SC_ZERO_TIME;
    transaction_type* trans = mResponseQueue.front();
    assert(trans);

    // Set response data
    trans->set_response_status(tlm::TLM_OK_RESP);
    if (trans->get_command() == tlm::TLM_READ_COMMAND) {
       sc_dt::uint64 address = trans->get_address();
       assert(address < 400);
      *reinterpret_cast<unsigned int*>(trans->get_data_ptr()) =
        *reinterpret_cast<unsigned int*>(&mMem[address]);
    }

    if (socket->nb_transport(*trans, phase, t)) {
      // response phase ends after t
      mEndResponseEvent.notify(t);

    } else {
      // master will call nb_transport to indicate end of response phase
    }
  }

  void endResponse()
  {
    assert(!mResponseQueue.empty());
    mResponseQueue.pop();

    if (!mResponseQueue.empty()) {
      // Start processing next transaction
      // Notify begin of response phase after RESPONSE delay
      mBeginResponseEvent.notify(RESPONSE_DELAY);
    }
  }

private:
  const sc_time ACCEPT_DELAY;
  const sc_time RESPONSE_DELAY;

private:
  unsigned char mMem[400];
  std::queue<transaction_type*> mEndRequestQueue;
  sc_event mEndRequestEvent;
  std::queue<transaction_type*> mResponseQueue;
  sc_event mBeginResponseEvent;
  sc_event mEndResponseEvent;
};

#endif
