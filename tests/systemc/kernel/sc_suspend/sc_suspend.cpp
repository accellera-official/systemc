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
#include <sstream>
#include <mutex>
#include <systemc>
#include <thread>

class async_event : public sc_core::sc_prim_channel,
                    public sc_core::sc_event,
                    public sc_core::sc_stage_callback_if 
{
private:
    sc_core::sc_time m_delay;
    std::thread::id tid;
    std::mutex mutex; // Belt and braces
    bool outstanding=false;

public:
    async_event(bool start_attached = true) {
        sc_core::sc_register_stage_callback(*this, sc_core::SC_PRE_PAUSE | sc_core::SC_PRE_SUSPEND | sc_core::SC_POST_SUSPEND);
        tid = std::this_thread::get_id();
        enable_attach_suspending(start_attached);
    }

    void async_notify() { notify(); }

    void notify() {
        if (tid == std::this_thread::get_id()) {
            sc_core::sc_event::notify();
        } else {
            notify(sc_core::SC_ZERO_TIME);
        }
    }
    void notify(double d, sc_core::sc_time_unit u) {
        notify(sc_core::sc_time(d, u));
    }
    void notify(sc_core::sc_time delay) {
        if (tid == std::this_thread::get_id()) {
            sc_core::sc_event::notify(delay);
        } else {
            mutex.lock();
            m_delay = delay;
            outstanding = true;
            mutex.unlock();
            async_request_update();
        }
    }

    bool triggered() const {
        if (tid == std::this_thread::get_id()) {
            return sc_core::sc_event::triggered();
        } else {
            SC_REPORT_ERROR("async_event",
                            "It is an error to call triggered() from outside "
                            "the SystemC thread");
        }
        return false;
    }
    void async_attach_suspending() {
        this->sc_core::sc_prim_channel::async_attach_suspending();
    }

    void async_detach_suspending() {
        this->sc_core::sc_prim_channel::async_detach_suspending();
    }

    void enable_attach_suspending(bool e) {
        e ? this->async_attach_suspending() : this->async_detach_suspending();
    }

private:
    void update(void) {
        mutex.lock();
        // we should be in SystemC thread
        if (outstanding) {
            sc_event::notify(m_delay);
            outstanding = false;
        }
        mutex.unlock();
    }
    void start_of_simulation() {
        // we should be in SystemC thread
        if (outstanding) {
            request_update();
        }
    }
    void stage_callback(const sc_core::sc_stage& stage)
    {
        std::ostringstream stage_str;
        stage_str << stage;
        SC_REPORT_INFO("async_event", ("Called stage_callback at stage: " + stage_str.str()).c_str());
        switch (stage) {
            case sc_core::SC_PRE_PAUSE:    // execute for SC_PRE_PAUSE...
            case sc_core::SC_PRE_SUSPEND:  // ...and SC_PRE_SUSPEND
              mutex.lock();
              if (outstanding) { request_update(); } 
              mutex.unlock();
              break;
            default: // no action for other callbacks
              break;
        }
    }
};

SC_MODULE (tester) {
    bool released = false;
    std::mutex mutex;
    std::condition_variable condition;
    std::thread t;
    async_event start;
    sc_core::sc_event ctrlev;

    SC_CTOR (tester) :
        t([&] {
            std::unique_lock<decltype(mutex)> lock(mutex);
            condition.wait(lock, [&](){return released;});
            start.notify();
        }),
        start(false)
    {
        SC_THREAD(ticker);
        SC_METHOD(control);
        dont_initialize();
        sensitive << ctrlev;
        SC_THREAD(starter);
        ctrlev.notify(sc_core::sc_time(350, sc_core::SC_US)); // notify in the future to 'hang'
    }

    void ticker()
    {
        for (int i = 0; i < 10; i++) {
            sc_core::wait(100, sc_core::SC_US);
            SC_REPORT_INFO("tester", ("Tick SystemC time: " + sc_core::sc_time_stamp().to_string()).c_str());
        }
    }

    void control()                       // NB suspend/unsuspend must come from SAME process
    {
        if (sc_core::sc_time_stamp() < sc_core::sc_time(400, sc_core::SC_US)) {
            SC_REPORT_INFO("tester", ("Suspend at SystemC time: " + sc_core::sc_time_stamp().to_string()).c_str());
            sc_core::sc_suspend_all();
            start.async_attach_suspending(); // if we don't attach here, the simulation will finish at this point because
                                             // everything is suspended

            // triggure the other thread to release us !
            std::lock_guard<decltype(mutex)> lock(mutex);
            released = true;
            condition.notify_one();
        } else {
            SC_REPORT_INFO("tester", ("Unsuspend at SystemC time: " + sc_core::sc_time_stamp().to_string()).c_str());
            sc_core::sc_unsuspend_all();
        }
    }

    void starter()                       // this is a thread, so it can call wait
    {
        wait(start);                     // wait for the other thread to (async) notify us
        t.join();                        // we're done with that thread now
        start.async_detach_suspending(); // we don't need this any more, we should now just finish the SystemC simulation

        SC_REPORT_INFO("tester", ("Restart SystemC time: " + sc_core::sc_time_stamp().to_string()).c_str());
        sc_core::sc_unsuspendable(); // If we don't mark this as unsuspendable, the simulation will end at this point!

        SC_REPORT_INFO("tester", ("Start to advance SystemC time: " + sc_core::sc_time_stamp().to_string()).c_str());
        wait(110, sc_core::SC_US);
        SC_REPORT_INFO("tester", ("SystemC time: " + sc_core::sc_time_stamp().to_string()).c_str());
        sc_core::sc_suspendable();
        ctrlev.notify();
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
