
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

  sc_sync_window.h -- keeps SystemC within a 'window' of time

  Original Author: Mark burton

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable
  CHANGE LOG AT END OF FILE
 *****************************************************************************/

#include <mutex>
#include <sysc/utils/sc_ob_event.h>
#include <systemc>
#include <tlm_utils/tlm_quantumkeeper.h>

namespace sc_core {

/**
 * @brief Base class which gives the quantum (dynamically) and specifies if the
 * window should stay open indefinitely or detach when there are no more events.
 * A SC_ZERO_TIME quantum means sync on every time step
 */
struct sc_sync_policy_base {
  virtual sc_core::sc_time quantum() = 0;
  virtual bool keep_alive() = 0;
};
struct sc_sync_policy_sc_zero_time : sc_sync_policy_base {
  sc_core::sc_time quantum() { return SC_ZERO_TIME; }
  bool keep_alive() { return false; }
};
struct sc_sync_policy_tlm_quantum : sc_sync_policy_base {
  sc_core::sc_time quantum() {
    return tlm_utils::tlm_quantumkeeper::get_global_quantum();
  }
  bool keep_alive() { return true; }
};

/**
 * @brief windowed synchronizer, template sc_sync_policy gives the quantum
 * (dynamically) and specifies if the window should stay open indefinitely or
 * detach when there are no more events.
 *
 * @tparam sc_sync_policy
 */
template <class sc_sync_policy = sc_sync_policy_sc_zero_time>
class sc_sync_windowed : public sc_core::sc_module,
                         public sc_core::sc_prim_channel {
  static_assert(std::is_base_of<sc_sync_policy_base, sc_sync_policy>::value,
                "sc_sync_policy must derive from sc_sync_policy_base");

  sc_core::sc_event m_sweep_ev;
  sc_ob_event m_step_ev;
  std::mutex m_mutex;
  sc_sync_policy policy;

public:
  struct window {
    sc_core::sc_time from;
    sc_core::sc_time to;
    bool operator==(window other) {
      return other.to == to && other.from == from;
    }
  };

  const struct window zero_window = {sc_core::SC_ZERO_TIME,
                                     sc_core::SC_ZERO_TIME};
  const struct window open_window = {sc_core::SC_ZERO_TIME,
                                     sc_core::sc_max_time()};

private:
  window m_window;
  std::function<void(const window &)> m_other_async_set_window_fn;

  void do_other_async_set_window_fn(const window &p_w) {
    if (m_other_async_set_window_fn) {
      m_other_async_set_window_fn(p_w);
    }
  }

  /* Handle suspending/resuming at the end of the window, also inform other side
   * if we reach end of window */
  void step_helper() {
    auto now = sc_core::sc_time_stamp();
    auto to = m_window.to;

    if (now >= to) {
      sc_core::sc_unsuspend_all(); // such that pending activity is valid if
                                   // it's needed below.

      /* We should suspend at this point, and wait for the other side to catch
       * up */
      auto q = (policy.quantum() == sc_core::SC_ZERO_TIME)
                   ? sc_core::sc_time_to_pending_activity()
                   : policy.quantum();

      do_other_async_set_window_fn({now, now + q});

      if (!policy.keep_alive())
        async_attach_suspending();
      sc_core::sc_suspend_all();

    } else {
      sc_core::sc_unsuspend_all();
      if (!policy.keep_alive())
        async_detach_suspending();

      auto q = (policy.quantum() == sc_core::SC_ZERO_TIME)
                   ? sc_core::sc_time_to_pending_activity()
                   : policy.quantum();
      do_other_async_set_window_fn({now, now + q});
      /* Re-notify event - maybe presumably moved */
      m_step_ev.notify(std::min(q, to - now));
    }
  }

  /*
   * Handle all sweep requests, once we arrive at a sweep point, tell the other
   * side.
   */
  virtual void sweep_helper() {
    auto now = sc_core::sc_time_stamp();

    auto q = (policy.quantum() == sc_core::SC_ZERO_TIME)
                 ? sc_core::sc_time_to_pending_activity()
                 : policy.quantum();
    do_other_async_set_window_fn({now, now + q});
  }

  /* Manage the Sync aync update  */
  void update() {
    std::lock_guard<std::mutex> lg(m_mutex);
    auto now = sc_core::sc_time_stamp();

    if (m_window.from > now) {
      m_sweep_ev.notify(m_window.from - now); // Try to move time forward.
    } else {
      m_sweep_ev.cancel(); // no need to fire event.
    }

    /* let stepper handle suspend/resume */
    m_step_ev.notify(sc_core::SC_ZERO_TIME);
    if (m_window == open_window) {
      /* The other side is signalling that they have no more events */
      sc_stop();
    }
  }

public:
  /* API call from other pair, presumably in a different thread.
   * The internal window wil be updated atomically.
   *
   * Input: window  - Window to set for sync. Sweep till the 'from' and step to
   * the 'to'.
   */
  void async_set_window(window w) {
    /* Only accept updated windows so we dont re-send redundant updates */
    std::lock_guard<std::mutex> lg(m_mutex);
    if (!(w == m_window)) {
      m_window = w;
      async_request_update();
    }
  }

  void end_of_simulation() { do_other_async_set_window_fn(open_window); }

  void start_of_simulation() { m_step_ev.notify(sc_core::SC_ZERO_TIME); }
  void bind(sc_sync_windowed *other) {
    if (m_other_async_set_window_fn) {
      SC_REPORT_WARNING("sc_sync_window",
          "m_other_async_set_window_fn was already registered or other "
          "sc_sync_windowed was already bound!");
    }
    m_other_async_set_window_fn = [other](const window &w) {
      other->async_set_window(w);
    };
  }
  void register_sync_cb(std::function<void(const window &)> fn) {
    if (m_other_async_set_window_fn) {
      SC_REPORT_WARNING("sc_sync_window",
          "m_other_async_set_window_fn was already registered or other "
          "sc_sync_windowed was already bound!");
    }
    m_other_async_set_window_fn = fn;
  }
  SC_CTOR(sc_sync_windowed)
      : m_window({sc_core::SC_ZERO_TIME, policy.quantum()}) {

    SC_METHOD(sweep_helper);
    dont_initialize();
    sensitive << m_sweep_ev;

    SC_METHOD(step_helper);
    dont_initialize();
    sensitive << m_step_ev;

    this->sc_core::sc_prim_channel::async_attach_suspending();
  }
};

} // namespace sc_core