/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at
 law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_ob_event.h -- Event that fires only when there are subsequent events

  Original Author: Mark burton

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable
  CHANGE LOG AT END OF FILE
 *****************************************************************************/

#ifndef _SC_OB_EVENT_
#define _SC_OB_EVENT_

#include <systemc>
#include <tlm>

#include <string>

namespace sc_core {
class sc_ob_event : public sc_core::sc_module, public sc_core::sc_event, public sc_core::sc_stage_callback_if
{

private:
    sc_core::sc_event m_update;
    sc_core::sc_time m_scheduled;
    sc_core::sc_process_handle m_th;

    void update_m()
    {
        auto now = sc_core::sc_time_stamp();
        if (now >= m_scheduled) {
            sc_core::sc_event::notify();
        } else {
            sc_core::sc_time pending = now;
            if (sc_core::sc_pending_activity_at_future_time()) {
                pending = now + sc_core::sc_time_to_pending_activity();
            }
            if (pending >= m_scheduled) {
                sc_core::sc_event::notify(m_scheduled - now);
            } else {
                if (pending > now) {
                    m_update.notify(pending - now);
                } else {
                    sc_core::sc_register_stage_callback(*this, sc_core::SC_POST_UPDATE);
                }
            }
        }
    }

    void future_events_notify_th()
    {
        while (true) {
            m_th.suspend();
            update_m();
        }
    }

public:
    sc_ob_event(const sc_core::sc_module_name& n = sc_core::sc_gen_unique_name("sc_ob_event"))
        : sc_module(n)
        , sc_core::sc_event((std::string(n) + "_notifier_event").c_str())
        , m_update((std::string(n) + "_update_event").c_str())
        , m_scheduled(sc_core::SC_ZERO_TIME)
    {
        SC_METHOD(update_m);
        sensitive << m_update;
        dont_initialize();
        SC_THREAD(future_events_notify_th);
        m_th = sc_core::sc_get_current_process_handle();
    }
    void notify()
    {
        m_scheduled = sc_core::sc_time_stamp();
        m_update.notify(sc_core::SC_ZERO_TIME);
    }
    void notify(double delay, sc_core::sc_time_unit unit)
    {
        m_scheduled = sc_core::sc_time_stamp() + sc_core::sc_time(delay, unit);
        m_update.notify(sc_core::SC_ZERO_TIME);
    }
    void notify(sc_core::sc_time delay)
    {
        m_scheduled = sc_core::sc_time_stamp() + delay;
        m_update.notify(sc_core::SC_ZERO_TIME);
    }

    void stage_callback(const sc_core::sc_stage& stage)
    {
        if (sc_core::sc_pending_activity_at_future_time()) {
            m_th.resume();
            sc_core::sc_unregister_stage_callback(*this, sc_core::SC_POST_UPDATE);
        }
    }

    ~sc_ob_event() {}
};
} // namespace 

#endif // _SC_OB_EVENT_