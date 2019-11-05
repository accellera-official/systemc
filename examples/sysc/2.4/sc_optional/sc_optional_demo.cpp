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

  sc_optional_demo.cpp -- Demonstrates sc_optional class

 *****************************************************************************/

#include <systemc.h>

struct demo_module : sc_module
{
    sc_in<bool> SC_NAMED(clk);

    demo_module(sc_module_name) {}

    void attach_debug_output(sc_signal<int> &signal)
    {
        debug_output.init("debug_output");
        debug_output->bind(signal);
    }

private:

    SC_METHOD_IMP(clocked_method, sensitive << clk.pos(); dont_initialize(); )
    {
        if (debug_output) {
            debug_output->write(debug_output->read() + 1);
        }
    }

    // optional output that tracks how many times clocked_method was triggered
    sc_optional<sc_out<int>>   debug_output;
};


int sc_main(int argc, char **argv)
{

    demo_module    SC_NAMED(demo);
    sc_signal<int> SC_NAMED(debug_out);
    sc_clock       SC_NAMED(clk, {10, SC_NS});

    demo.clk(clk);
    demo.attach_debug_output(debug_out);

    sc_start({40, SC_NS});

    cout << debug_out << endl;

    // expected 3 children: clk, debug_output and clocked_method
    sc_assert(demo.get_child_objects().size() == 3);
    sc_assert(debug_out == 4);

    return 0;
}
