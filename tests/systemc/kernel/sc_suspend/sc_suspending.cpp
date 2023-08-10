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

  callbacks.cpp -- Test of stage callbacks

  Note: requires sc_suspending support in the kernel

  Original Author: Mark Burton, Qualcomm, 2023-08-10

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
#include <future>
#include <queue>

#include <mutex>
#include <systemc>
#include <thread>

class async_event : public sc_core::sc_prim_channel, public sc_core::sc_event
{
private:
    sc_core::sc_time m_delay;
    std::thread::id tid;
    std::mutex mutex; // Belt and braces
    int outstanding;

public:
    async_event(bool start_attached = true): outstanding(0)
    {
        register_simulation_phase_callback(sc_core::SC_PAUSED | sc_core::SC_START_OF_SIMULATION);
        tid = std::this_thread::get_id();
        outstanding = 0;
        enable_attach_suspending(start_attached);
    }

    void async_notify() { notify(); }

    void notify(sc_core::sc_time delay = sc_core::sc_time(sc_core::SC_ZERO_TIME))
    {
        if (tid == std::this_thread::get_id()) {
            sc_core::sc_event::notify(delay);
        } else {
            mutex.lock();
            m_delay = delay;
            outstanding++;
            mutex.unlock();
            async_request_update();
        }
    }

    void async_attach_suspending() { this->sc_core::sc_prim_channel::async_attach_suspending(); }

    void async_detach_suspending() { this->sc_core::sc_prim_channel::async_detach_suspending(); }

    void enable_attach_suspending(bool e)
    {
        mutex.lock();
        e ? this->async_attach_suspending() : this->async_detach_suspending();
        mutex.unlock();
    }

private:
    void update(void)
    {
        mutex.lock();
        // we should be in SystemC thread
        if (outstanding) {
            sc_event::notify(m_delay);
            outstanding--;
            if (outstanding) request_update();
        }
        mutex.unlock();
    }

    void simulation_phase_callback()
    {
        mutex.lock();
        if (outstanding) request_update();
        mutex.unlock();
    }
};

SC_MODULE (tester) {
    sc_core::sc_event ctrlev;
    async_event start;
    std::thread t;
    std::mutex mutex;
    std::condition_variable condition;

    void ticker()
    {
        for (int i = 0; i < 10; i++) {
            sc_core::wait(100, sc_core::SC_US);
            SC_REPORT_INFO("tester", ("Tick SystemC time: " + sc_core::sc_time_stamp().to_string()).c_str());
        }
    }

    // NB suspend/unsuspend must come from SAME process
    void control()
    {
        if (sc_core::sc_time_stamp() < sc_core::sc_time(400, sc_core::SC_US)) {
            SC_REPORT_INFO("tester", ("Suspend all SystemC time: " + sc_core::sc_time_stamp().to_string()).c_str());
            sc_core::sc_suspend_all();
            start.async_attach_suspending(); // If we dont attach here, the simulation will finish at this point because
                                             // everything is suspended.

            // triggure the other thread to release us !
            std::lock_guard<decltype(mutex)> lock(mutex);
            condition.notify_one();
        } else {
            sc_core::sc_unsuspend_all();
        }
    }
    /* this is a thread, so it can call wait */
    void starter()
    {
        wait(start);                     // wait for the other thread to (async) notify us.
        t.join();                        // We're done with that thread now.
        start.async_detach_suspending(); // we dont need this any more, we should now just finish the SystemC simulation

        SC_REPORT_INFO("tester", ("Restart SystemC time: " + sc_core::sc_time_stamp().to_string()).c_str());
        sc_core::sc_unsuspendable(); // If we dont mark this as unsuspendable, the simulation will end at this point!

        SC_REPORT_INFO("tester", ("Start to advance SystemC time: " + sc_core::sc_time_stamp().to_string()).c_str());
        wait(110, sc_core::SC_US);
        SC_REPORT_INFO("tester", ("SystemC time: " + sc_core::sc_time_stamp().to_string()).c_str());
        sc_core::sc_suspendable();
        ctrlev.notify();
    }
    SC_CTOR (tester) :
        t([&] {
            std::unique_lock<decltype(mutex)> lock(mutex);
            condition.wait(lock);
            SC_REPORT_INFO("child thread", "Release child thread");
            start.notify();
        }),
        start(false)
        {
            SC_THREAD(ticker);
            SC_METHOD(control);
            dont_initialize();
            sensitive << ctrlev;
            SC_THREAD(starter);
            ctrlev.notify(sc_core::sc_time(350, sc_core::SC_US)); // notify in the future to 'hang'.
        }
};
int sc_main(int, char*[])
{
    tester t1("mytester");
    sc_core::sc_start();
    SC_REPORT_INFO("tester", ("test complete SystemC time: " + sc_core::sc_time_stamp().to_string()).c_str());
    sc_core::sc_stop();
    return 0;
}
