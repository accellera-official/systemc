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

  cross_context_write.cpp -- Verify sc_signal::write from a foreign
  simcontext (the writer's process belongs to a SC_ALLOW_CONCURRENCY
  module) lands correctly on the signal's owning simcontext, and the
  reader (in the parent simcontext) observes every write.

  Producer is wrapped in SC_ALLOW_CONCURRENCY -- it lives in a child
  simcontext.  Consumer is in the parent.  The signal that connects
  them is constructed at sc_main scope (parent).  Producer's
  SC_THREAD writes a strictly-increasing sequence; Consumer's
  SC_METHOD is sensitive to the signal and records every value seen.
  After the expected number of values arrive, Consumer calls sc_stop.

  Pass criteria:
    * Consumer method fires at least N times (one per write).
    * Each successive observed value is strictly greater than the
      previous (no torn read, no out-of-order values).
    * The final observed value equals N.

  Exits 0 on pass, 1 on fail.

 *****************************************************************************/

#include <systemc.h>
#include "sysc/utils/sc_concurrent.h"

#include <iostream>

constexpr int N_WRITES = 5;
static const sc_core::sc_time WRITE_PERIOD(10, sc_core::SC_MS);

SC_MODULE(producer)
{
    sc_out<int> out;

    void run()
    {
        for (int i = 1; i <= N_WRITES; ++i) {
            wait(WRITE_PERIOD);
            out.write(i);
        }
    }

    SC_CTOR(producer) { SC_THREAD(run); }
};

SC_MODULE(consumer)
{
    sc_in<int> in;

    int   m_last_seen = 0;
    int   m_count     = 0;
    bool  m_monotonic = true;

    void on_change()
    {
        int v = in.read();
        if (v <= m_last_seen) m_monotonic = false;
        m_last_seen = v;
        ++m_count;
        if (v >= N_WRITES) sc_stop();
    }

    SC_CTOR(consumer)
    {
        SC_METHOD(on_change);
        sensitive << in;
        dont_initialize();
    }
};

int sc_main(int, char*[])
{
    tlm_utils::tlm_quantumkeeper::set_global_quantum(
        sc_core::sc_time(1, sc_core::SC_MS));

    sc_signal<int> sig;

    SC_ALLOW_CONCURRENCY(producer, sc_core::sc_sync_policy_tlm_quantum)
        prod("prod");
    consumer cons("cons");

    prod.out(sig);
    cons.in(sig);

    sc_start();

    bool ok =
        cons.m_count >= N_WRITES &&
        cons.m_monotonic &&
        cons.m_last_seen == N_WRITES;

    std::cout << "count=" << cons.m_count
              << " last_seen=" << cons.m_last_seen
              << " monotonic=" << (cons.m_monotonic ? "yes" : "no")
              << " result=" << (ok ? "PASS" : "FAIL")
              << std::endl;

    return ok ? 0 : 1;
}
