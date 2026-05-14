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

  child_stop.cpp -- Verify that sc_stop called from inside a
  SC_ALLOW_CONCURRENCY module (running on the child simcontext's
  thread) cleanly stops the whole simulation including the parent.

  Without the child->parent stop propagation, the parent would keep
  running until its own events run out.  We set up a parent module
  that would otherwise run indefinitely (ticker that keeps arming
  its own event), so if the child's sc_stop does not propagate, the
  test hangs and the outer timeout kills it.

  Pass criteria:
    * sc_start returns within the test timeout.
    * The child's stop intent is observed (child_stopped flag).
    * The parent's own ticker ran at least once but did NOT exhaust
      itself, confirming the parent was stopped by the child rather
      than by starvation.

  Exits 0 on pass, 1 on fail.

 *****************************************************************************/

#include <systemc.h>
#include "sysc/utils/sc_concurrent.h"

#include <iostream>

SC_MODULE(child_mod)
{
    bool* m_child_stopped;

    void run()
    {
        // Run for a few ms, then call sc_stop from the child's thread.
        wait(5, sc_core::SC_MS);
        *m_child_stopped = true;
        sc_stop();
    }

    SC_HAS_PROCESS(child_mod);
    child_mod(sc_module_name n, bool* stop_flag)
      : sc_module(n), m_child_stopped(stop_flag)
    {
        SC_THREAD(run);
    }
};

SC_MODULE(parent_ticker)
{
    int* m_tick_count;
    sc_event m_tick;

    void tick()
    {
        ++(*m_tick_count);
        // re-arm: this module would run forever if never stopped
        m_tick.notify(sc_core::sc_time(1, sc_core::SC_MS));
    }

    SC_HAS_PROCESS(parent_ticker);
    parent_ticker(sc_module_name n, int* tick_count)
      : sc_module(n), m_tick_count(tick_count)
    {
        SC_METHOD(tick);
        sensitive << m_tick;
        // kick off
        m_tick.notify(sc_core::sc_time(1, sc_core::SC_MS));
    }
};

int sc_main(int, char*[])
{
    tlm_utils::tlm_quantumkeeper::set_global_quantum(
        sc_core::sc_time(1, sc_core::SC_MS));

    bool child_stopped = false;
    int  parent_ticks  = 0;

    SC_ALLOW_CONCURRENCY(child_mod, sc_core::sc_sync_policy_tlm_quantum)
        child("child", &child_stopped);
    parent_ticker parent("parent", &parent_ticks);

    sc_start();

    // Parent's ticker re-arms itself every 1 ms.  If child's sc_stop
    // did not propagate, the parent would tick forever.  A bounded
    // tick count + the outer timeout together catch both the "did
    // not stop" case and the "stopped too early" case.
    bool ok =
        child_stopped &&
        parent_ticks >= 1 &&
        parent_ticks < 1000;

    // parent_ticks is timing-dependent (depends on how quickly the
    // child schedules sc_stop), so it's deliberately omitted from the
    // printed verdict to keep the golden log deterministic.  The
    // bounds check above is the real pass condition.
    std::cout << "child_stopped=" << (child_stopped ? "yes" : "no")
              << " result=" << (ok ? "PASS" : "FAIL")
              << std::endl;

    return ok ? 0 : 1;
}
