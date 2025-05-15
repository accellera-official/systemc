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
#include <systemc>
#include "sc_sync_window.h"

#include <thread>

namespace sc_core {


/**
 * @brief Simcontext handler, encapsulates a simcontext
 * This is required for the templated class to work, and hence is required in the header.
 * It should not be used directly
 * @tparam SYNC_POLICY
 */
template <class SYNC_POLICY = sc_core::sc_sync_policy_sc_zero_time>
class _internal_simcontext_handler
{

    /**
     * @brief Helper to capture start of simulation and start std::thread
     *
     */
    SC_MODULE (control_module) {
        class _internal_simcontext_handler* m_simcontext_h;

        std::thread m_thread;
        void sc_context_start()
        {
            m_simcontext_h->use_simcontext();

            sc_core::sc_start();
        }

    public:
        void start_of_simulation() { m_thread = std::thread(&control_module::sc_context_start, this); }
        void end_of_simulation()
        {
            m_thread.join();
        }

        SC_CTOR (control_module, _internal_simcontext_handler * p): m_simcontext_h(p)
            {
                m_simcontext_h->use_simcontext();
                SYNC_POLICY sync_policy;
                m_sync_child = new sc_sync_windowed<SYNC_POLICY>("sync_child");
                m_simcontext_h->reset_simcontext();
            }
        sc_sync_windowed<SYNC_POLICY>* m_sync_child;
    };

    sc_core::sc_simcontext m_simcontext;
    sc_core::sc_simcontext* m_old_simcontext;

    control_module m_ctrl_module;
    sc_sync_windowed<SYNC_POLICY>* m_sync_parent;

public:
    void use_simcontext()
    {
        assert(sc_curr_simcontext != &m_simcontext);
        m_old_simcontext = sc_curr_simcontext;
        sc_curr_simcontext = &m_simcontext;
    }

    void reset_simcontext()
    {
        sc_curr_simcontext = m_old_simcontext;
    }

    _internal_simcontext_handler(): m_ctrl_module("ctrl_module", this)
    {
        SYNC_POLICY sync_policy;
        m_sync_parent = new sc_sync_windowed<SYNC_POLICY>("sync_parent");

        m_ctrl_module.m_sync_child->bind(m_sync_parent);
        m_sync_parent->bind(m_ctrl_module.m_sync_child);

        use_simcontext();
    }
};

/**
 * @brief sc_parallel ensures _internal_simcontext_handler is constructed first, the simcontext switched, then the actual module
 * constructed, before switching back the simcontext
 *
 * @tparam T
 * @tparam SYNC_POLICY
 */
template <typename T, class SYNC_POLICY = sc_sync_policy_sc_zero_time>
class sc_parallel : public _internal_simcontext_handler<SYNC_POLICY>, public T
{
public:
    template <typename... A>
    sc_parallel(A... a): T(a...)
    {
        _internal_simcontext_handler<SYNC_POLICY>::reset_simcontext();
    }
};

#define SC_PARALLEL(mod, policy) sc_core::sc_parallel<mod, policy>

} // namespace sc_core