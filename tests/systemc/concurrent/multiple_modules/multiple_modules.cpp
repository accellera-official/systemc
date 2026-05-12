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

  multiple_modules.cpp -- Verify two independent SC_ALLOW_CONCURRENCY
  modules each feeding the same consumer in the parent simcontext.
  Each producer lives in its own child simcontext (its own std::thread).
  Both signals are owned by the parent, so each producer's write()
  crosses a simcontext boundary.

  Pass criteria:
    * Consumer observes exactly N_WRITES values on each signal.
    * Each stream is monotonic.

  Exits 0 on pass, 1 on fail.

 *****************************************************************************/

#include <systemc.h>
#include "sysc/utils/sc_concurrent.h"

#include <iostream>

constexpr int N_WRITES = 4;

SC_MODULE(producer)
{
    sc_out<int>      out;
    int              m_base;
    sc_core::sc_time m_period;

    void run()
    {
        for (int i = 1; i <= N_WRITES; ++i) {
            wait(m_period);
            out.write(m_base + i);
        }
    }

    SC_HAS_PROCESS(producer);
    producer(sc_module_name n, int base, sc_core::sc_time period)
      : sc_module(n), m_base(base), m_period(period)
    {
        SC_THREAD(run);
    }
};

SC_MODULE(consumer)
{
    sc_in<int> in_a;
    sc_in<int> in_b;

    int  m_count_a    = 0;
    int  m_count_b    = 0;
    int  m_last_a     = 0;
    int  m_last_b     = 0;
    bool m_mono_a     = true;
    bool m_mono_b     = true;

    void on_a()
    {
        int v = in_a.read();
        if (v <= m_last_a) m_mono_a = false;
        m_last_a = v;
        ++m_count_a;
        maybe_stop();
    }
    void on_b()
    {
        int v = in_b.read();
        if (v <= m_last_b) m_mono_b = false;
        m_last_b = v;
        ++m_count_b;
        maybe_stop();
    }

    void maybe_stop()
    {
        if (m_count_a >= N_WRITES && m_count_b >= N_WRITES)
            sc_stop();
    }

    SC_CTOR(consumer)
    {
        SC_METHOD(on_a); sensitive << in_a; dont_initialize();
        SC_METHOD(on_b); sensitive << in_b; dont_initialize();
    }
};

int sc_main(int, char*[])
{
    tlm_utils::tlm_quantumkeeper::set_global_quantum(
        sc_core::sc_time(1, sc_core::SC_MS));

    sc_signal<int> sig_a;
    sc_signal<int> sig_b;

    // Two producers: each in its OWN child simcontext; bases are
    // disjoint (100+ and 200+) so we can check the streams independently.
    SC_ALLOW_CONCURRENCY(producer, sc_core::sc_sync_policy_tlm_quantum)
        prod_a("prod_a", 100, sc_core::sc_time(10, sc_core::SC_MS));
    SC_ALLOW_CONCURRENCY(producer, sc_core::sc_sync_policy_tlm_quantum)
        prod_b("prod_b", 200, sc_core::sc_time(7, sc_core::SC_MS));

    consumer cons("cons");

    prod_a.out(sig_a);
    prod_b.out(sig_b);
    cons.in_a(sig_a);
    cons.in_b(sig_b);

    sc_start();

    bool ok =
        cons.m_count_a == N_WRITES &&
        cons.m_count_b == N_WRITES &&
        cons.m_last_a  == 100 + N_WRITES &&
        cons.m_last_b  == 200 + N_WRITES &&
        cons.m_mono_a  && cons.m_mono_b;

    std::cout << "a:count=" << cons.m_count_a << " last=" << cons.m_last_a
              << " mono=" << (cons.m_mono_a ? "yes" : "no")
              << "  b:count=" << cons.m_count_b << " last=" << cons.m_last_b
              << " mono=" << (cons.m_mono_b ? "yes" : "no")
              << "  result=" << (ok ? "PASS" : "FAIL")
              << std::endl;

    return ok ? 0 : 1;
}
