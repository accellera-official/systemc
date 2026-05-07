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

 sc_ob_event.h -- Event that fires only when the simulator arrives at the
 notification time, or there are subsequent events

  Original Author: Mark burton

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable
  CHANGE LOG AT END OF FILE
 *****************************************************************************/

#ifndef _SC_OB_EVENT_
#define _SC_OB_EVENT_

#include <systemc>

namespace sc_core {
class sc_ob_event : public sc_core::sc_module,
                    public sc_core::sc_event,
                    public sc_core::sc_stage_callback_if {

private:
  sc_core::sc_time m_scheduled;
  sc_core::sc_process_handle m_th;

  void future_events_notify_th() {
    while (true) {
      m_th.suspend();

      auto now = sc_core::sc_time_stamp();
      if (now >= m_scheduled) {
        sc_core::sc_event::notify();
      } else {
        sc_core::sc_event::notify(m_scheduled - now);
      }
    }
  }

public:
  sc_ob_event(const sc_core::sc_module_name &n =
                  sc_core::sc_gen_unique_name("sc_ob_event"))
      : sc_module(n), m_scheduled(sc_core::SC_ZERO_TIME) {
    SC_THREAD(future_events_notify_th);
    m_th = sc_core::sc_get_current_process_handle();
  }
  void notify() {
    sc_core::sc_event::notify();
    sc_core::sc_unregister_stage_callback(*this, sc_core::SC_POST_UPDATE);
  }
  void notify(double delay, sc_core::sc_time_unit unit) {
    notify(sc_core::sc_time(delay, unit));
  }
  void notify(sc_core::sc_time delay) {
    sc_core::sc_event::cancel();
    m_scheduled = sc_core::sc_time_stamp() + delay;
    sc_core::sc_register_stage_callback(*this, sc_core::SC_POST_UPDATE);
  }
  void cancel() {
    sc_core::sc_event::cancel();
    sc_core::sc_unregister_stage_callback(*this, sc_core::SC_POST_UPDATE);
  }
  void stage_callback(const sc_core::sc_stage &stage) {
    sc_core::sc_time pending = sc_core::sc_time_stamp();
    if (sc_core::sc_pending_activity_at_future_time()) {
      pending += sc_core::sc_time_to_pending_activity();
    }

    if (pending >= m_scheduled) {
      m_th.resume();
      sc_core::sc_unregister_stage_callback(*this, sc_core::SC_POST_UPDATE);
    }
  }

  ~sc_ob_event() {}
};
} // namespace sc_core

#endif // _SC_OB_EVENT_