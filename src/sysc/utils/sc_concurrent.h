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

  sc_concurrent.h -- Mark an SC_MODULE as safe to run concurrently with
  others.  Whether the implementation actually exploits this for parallel
  execution is implementation-defined; the API is the user's promise that
  the module's interactions with the rest of the elaboration are bounded
  to its sync_window and to thread-safe channels.

  Original Author: Mark Burton

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable
  CHANGE LOG AT END OF FILE
 *****************************************************************************/

#ifndef SC_CONCURRENT_H
#define SC_CONCURRENT_H

#include "sc_sync_window.h"
#include <systemc>

#include <condition_variable>
#include <mutex>
#include <thread>
#include <type_traits>

namespace sc_core {

// Module-author opt-out.  Default == every module is safe to wrap in
// sc_concurrent.  An author who knows their module is fundamentally
// unsafe for concurrent execution may specialise this to false_type;
// sc_concurrent's static_assert below then turns any attempt to wrap
// the module into a compile-time error.
template <typename T>
struct sc_concurrent_safe : std::true_type {};

template <typename T, class SYNC_POLICY> class sc_concurrent;
/**
 * @brief Simcontext handler, encapsulates a simcontext
 * This is required for the templated class to work, and hence is required in
 * the header. It should not be used directly
 * @tparam SYNC_POLICY
 */
template <class SYNC_POLICY = sc_core::sc_sync_policy_in_sync>
class _internal_simcontext_handler {
  template <typename T, class S> friend class sc_concurrent;
  /**
   * @brief Helper to capture start of simulation and start std::thread
   *
   */
  class control_module : public sc_core::sc_module,
                         public sc_core::sc_stage_callback_if {
    class _internal_simcontext_handler *m_simcontext_h;

    std::thread m_thread;
    std::mutex m_mutex;
    std::condition_variable m_cv;

    // Prim channel purely to accept async_request_update, to call sc_stop.
    class terminator_pc : public sc_core::sc_prim_channel {
      void update() { sc_stop(); }

    public:
      //      terminator_pc() {}
      void terminate() { async_request_update(); }
    };
    terminator_pc *m_terminator;
    sc_event *m_end_ev;
    bool started = false;
    // Captured on the parent thread in start_of_simulation(), applied on
    // the child thread in sc_context_start() so the child inherits the
    // parent's sc_report_handler configuration (handler, verbosity,
    // suppress/force masks, sev_actions, log_file_name).  Counters are
    // not in the snapshot — the child starts fresh.
    sc_core::sc_report_handler::config_snapshot m_report_config;
    void sc_context_start() {
      sc_core::sc_report_handler::apply_config(m_report_config);
      m_simcontext_h->use_simcontext();
      // Register on the child's stage_callback_registry: POST_START so the
      // parent's thread can stop blocking once we are up, and POST_END so we
      // can wake the parent if the child's sc_start was the one that ended.
      sc_core::sc_register_stage_callback(*this,
                                          sc_core::SC_POST_START_OF_SIMULATION
                                        | sc_core::SC_POST_END_OF_SIMULATION);
      sc_core::sc_start();
      m_sync_child->detach();
    }

  public:
    void start_of_simulation() {
      m_report_config = sc_core::sc_report_handler::snapshot_config();
      m_thread = std::thread(&control_module::sc_context_start, this);
      std::unique_lock<std::mutex> lg(m_mutex);
      m_cv.wait(lg, [this]() { return started; });
    }
    void end_thread() { sc_core::sc_stop(); }
    void end_of_simulation() {
      // Drive the child to stop, then wait for it to fully finish
      // BEFORE proceeding with the rest of the parent's teardown.
      // Reason: the child and the parent share an sc_object_manager
      // (see sc_simcontext::init's sub-sim handling).  The hierarchy
      // navigation stack m_object_stack is a single shared vector;
      // sc_port_base::simulation_done on every port pushes/pops it
      // via get_hierarchy_scope.  If the parent runs its
      // port_registry::simulation_done while the child is still
      // tearing down its own ports, both threads concurrently
      // mutate that stack and break its discipline.  Joining here -
      // before the parent's port simulation_done callbacks run -
      // serialises the two teardowns and avoids the race.
      m_terminator->terminate();
      if (m_thread.joinable() &&
          m_thread.get_id() != std::this_thread::get_id())
        m_thread.join();
    }
    void stage_callback(const sc_core::sc_stage &stage) {
      switch (stage) {
      case sc_core::SC_POST_END_OF_SIMULATION:
        if (sc_core::sc_get_curr_simcontext() == &m_simcontext_h->m_simcontext) {
          // Child-side POST_END: wake the parent so it stops too.
          // m_end_ev lives in the parent; notify(SC_ZERO_TIME) from the
          // child's thread is routed via sc_simcontext::run_update_async
          // to the parent's update phase, which delta-schedules the event;
          // SC_METHOD(end_thread) then fires on the parent and calls
          // sc_stop().  Use the delta variant rather than the immediate
          // notify(): the cross-context callback runs in the parent's
          // update phase, where immediate notification would error out
          // ("immediate notification outside evaluation phase").
          // This handles the child-initiated stop; the parent-initiated
          // direction is already covered by end_of_simulation() above
          // (which also joins this thread).
          m_end_ev->notify(sc_core::SC_ZERO_TIME);
        }
        // No parent-side join here any more; we joined in
        // end_of_simulation(), which fires before any port's
        // simulation_done callback.
        break;
      // Called by the child thread once it's up and running
      case sc_core::SC_POST_START_OF_SIMULATION: {
        std::lock_guard<std::mutex> lg(m_mutex);
        started = true;
        m_cv.notify_all();
        break;
      }
      default:
        break;
      }
    }

    ~control_module() {
      delete m_end_ev;
      delete m_terminator;
    }

    SC_CTOR(control_module, _internal_simcontext_handler *p)
        : m_simcontext_h(p) {
      m_simcontext_h->use_simcontext();
      SYNC_POLICY sync_policy;
      m_sync_child = new sc_sync_windowed<SYNC_POLICY>("sync_child");
      m_terminator = new terminator_pc();

      m_simcontext_h->revert_simcontext();

      sc_core::sc_register_stage_callback(*this,
                                          sc_core::SC_POST_END_OF_SIMULATION);

      m_end_ev = new sc_core::sc_event();
      SC_METHOD(end_thread);
      sensitive << *m_end_ev;
      dont_initialize();
    }
    sc_sync_windowed<SYNC_POLICY> *m_sync_child;
  };

private:
  sc_core::sc_simcontext *m_old_simcontext =
      sc_get_curr_simcontext(); // Side effect create simcontext if we are the
                                // first module
  sc_core::sc_simcontext m_simcontext;

  control_module m_ctrl_module;
  sc_sync_windowed<SYNC_POLICY> m_sync_parent;

  void use_simcontext() {
    assert(sc_curr_simcontext != &m_simcontext);
    m_old_simcontext = sc_curr_simcontext;
    sc_curr_simcontext = &m_simcontext;
  }

  void revert_simcontext() { sc_curr_simcontext = m_old_simcontext; }

public:
  _internal_simcontext_handler()
      // sc_gen_unique_name keeps each wrapper's internal modules
      // distinct in the shared sc_object_manager.  Hardcoded names
      // would collide across multiple SC_ALLOW_CONCURRENCY wrappers
      // and trigger an "object already exists" rename warning.
      : m_ctrl_module(sc_core::sc_gen_unique_name("ctrl_module"), this),
        m_sync_parent(sc_core::sc_gen_unique_name("sync_parent")) {
    SYNC_POLICY sync_policy;

    m_ctrl_module.m_sync_child->bind(&m_sync_parent);
    m_sync_parent.bind(m_ctrl_module.m_sync_child);

    use_simcontext();
  }
};

/**
 * @brief sc_concurrent marks a module as safe to run concurrently with
 * other modules.  It ensures _internal_simcontext_handler is constructed
 * first (creating the child simcontext), the simcontext is switched,
 * the wrapped module T is then constructed in that child simcontext,
 * and the simcontext is restored afterwards.
 *
 * Whether the runtime actually spins a separate thread for the child
 * simcontext is implementation-defined.  The user's contract is only
 * that this module's interactions with the rest of the design are
 * bounded to its sync_window and thread-safe channels.
 *
 * @tparam T
 * @tparam SYNC_POLICY
 */
template <typename T, class SYNC_POLICY = sc_sync_policy_in_sync>
class sc_concurrent : public _internal_simcontext_handler<SYNC_POLICY>, public T {
  static_assert(sc_concurrent_safe<T>::value,
                "module type is not declared sc_concurrent_safe "
                "(specialise sc_concurrent_safe<T> to std::false_type "
                "to forbid wrapping; default is true)");
public:
  template <typename... A> sc_concurrent(A... a) : T(a...) {
    _internal_simcontext_handler<SYNC_POLICY>::revert_simcontext();
  }
};

#define SC_ALLOW_CONCURRENCY(mod, policy) sc_core::sc_concurrent<mod, policy>

} // namespace sc_core

#endif // SC_CONCURRENT_H