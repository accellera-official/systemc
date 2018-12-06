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

  in_class_initialization.cpp : Showcase for in-class initialization macros


  Original Author: Roman I. Popov, Intel

 *****************************************************************************/

#include <systemc.h>

#ifdef USE_PIMPL_ADDER
    #include "adder_int_5_pimpl.h"
#else
    #include "adder.h"
#endif

const int TEST_SIZE = 10;

template <typename T, int N_INPUTS>
struct adder_tester : sc_module {

    sc_in<bool>          SC_NAMED(clock);
    sc_in<bool>          SC_NAMED(reset);
    sc_in<T>             SC_NAMED(res);
    sc_vector<sc_out<T>> SC_NAMED(din, N_INPUTS);

    SC_CTOR(adder_tester){}

private:

    // In-class initialization of SC_CTHREAD, second parameter is clock edge,
    // third parameter is arbitrary initialization code
    SC_CTHREAD_IMP(adder_tester_cthread, clock.pos(),
                    { async_reset_signal_is(reset, true); } ) {

        wait();

        for (int ii = 0; ii < TEST_SIZE; ++ii) {
            T ref_res = 0;
            for (int jj = 0; jj < N_INPUTS; ++jj) {
                T input = ii + jj;
                ref_res += input;
                din[jj] = input;
            }

            wait();
            cout << "RES: " << res << " REFERENCE: " << ref_res << "\n";
            sc_assert(res == ref_res);

        }

        sc_stop();
    }
};

template <typename T, int N_INPUTS>
struct testbench : sc_module {

    sc_clock                SC_NAMED(clock, 10, SC_NS);
    sc_signal<bool>         SC_NAMED(reset);
    sc_signal<T>            SC_NAMED(res);
    sc_vector<sc_signal<T>> SC_NAMED(din, N_INPUTS);

    SC_CTOR(testbench) {}

private:

    // SC_NAMED_WITH_INIT allows to specify arbitrary initialization code after member declaration
    // for example you can bind module ports here
    adder_tester<T, N_INPUTS> SC_NAMED_WITH_INIT(tester_inst) {
        tester_inst.clock(clock);
        tester_inst.reset(reset);
        tester_inst.res(res);
        tester_inst.din(din);
    }

#ifdef USE_PIMPL_ADDER
    adder_int_5_pimpl SC_NAMED_WITH_INIT(adder_inst)
#else
    adder<T, N_INPUTS> SC_NAMED_WITH_INIT(adder_inst)
#endif
    {
        adder_inst.res(res);
        adder_inst.din(din);
    }

    SC_THREAD_IMP(reset_thread, sensitive << clock.posedge_event();) {
        reset = 1;
        wait();
        reset = 0;
    }
};

int sc_main(int argc, char **argv) {
    testbench<int, 5> SC_NAMED(tb_inst);
    sc_start();
    return 0;
}