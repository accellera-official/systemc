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

  sc_parallel.h -- Allow SC_MODULES to be nominated to run in parallel

  Original Author: Mark burton

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable
  CHANGE LOG AT END OF FILE
 *****************************************************************************/

#ifndef SC_PARALLEL_H
#define SC_PARALLEL_H

#include "sc_sync_window.h"
#include <systemc>

#include <condition_variable>
#include <mutex>
#include <thread>

namespace sc_core {

/**
 * @brief Simcontext handler, encapsulates a simcontext
 * This is required for the templated class to work, and hence is required in
 * the header. It should not be used directly
 * @tparam SYNC_POLICY
 */
template <class SYNC_POLICY = sc_core::sc_sync_policy_in_sync>
class _internal_simcontext_handler {

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
    void sc_context_start() {
      m_simcontext_h->use_simcontext();
      sc_core::sc_register_stage_callback(*this,
                                          sc_core::SC_POST_START_OF_SIMULATION);
      sc_core::sc_start();
      m_sync_child->detach();
    }

  public:
    void start_of_simulation() {
      m_thread = std::thread(&control_module::sc_context_start, this);
      std::unique_lock<std::mutex> lg(m_mutex);
      m_cv.wait(lg, [this]() { return started; });
    }
    void end_thread() { sc_core::sc_stop(); }
    void end_of_simulation() { m_terminator->terminate(); }
    void stage_callback(const sc_core::sc_stage &stage) {
      switch (stage) {
      // called by the parent thread when during shutdown.
      case sc_core::SC_POST_END_OF_SIMULATION:
        if (m_thread.get_id() != std::this_thread::get_id()) {
          m_thread.join();
        }
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

  sc_core::sc_simcontext *m_old_simcontext =
      sc_get_curr_simcontext(); // Side effect create simcontext if we are the
                                // first module
  sc_core::sc_simcontext m_simcontext;

  control_module m_ctrl_module;
  sc_sync_windowed<SYNC_POLICY> m_sync_parent;

public:
  void use_simcontext() {
    assert(sc_curr_simcontext != &m_simcontext);
    m_old_simcontext = sc_curr_simcontext;
    sc_curr_simcontext = &m_simcontext;
  }

  void revert_simcontext() { sc_curr_simcontext = m_old_simcontext; }

  _internal_simcontext_handler()
      : m_ctrl_module("ctrl_module", this), m_sync_parent("sync_parent") {
    SYNC_POLICY sync_policy;

    m_ctrl_module.m_sync_child->bind(&m_sync_parent);
    m_sync_parent.bind(m_ctrl_module.m_sync_child);

    use_simcontext();
  }
};

/**
 * @brief sc_parallel ensures _internal_simcontext_handler is constructed
 * first, the simcontext switched, then the actual module constructed, before
 * switching back the simcontext
 *
 * @tparam T
 * @tparam SYNC_POLICY
 */
template <typename T, class SYNC_POLICY = sc_sync_policy_in_sync>
class sc_parallel : public _internal_simcontext_handler<SYNC_POLICY>, public T {
public:
  template <typename... A> sc_parallel(A... a) : T(a...) {
    _internal_simcontext_handler<SYNC_POLICY>::revert_simcontext();
  }
};

#define SC_PARALLEL(mod, policy) sc_core::sc_parallel<mod, policy>

} // namespace sc_core

#endif // SC_PARALLEL_H