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

#include <systemc>

#if SC_CPLUSPLUS >= 201703L
#include <optional>
#else
#error "This test requires C++17 features.";
#endif

struct demo_module : sc_core::sc_module
{
  sc_core::sc_in<bool> clk;

  demo_module(sc_core::sc_module_name) : clk("clk")
  {
    SC_METHOD(clocked_method);
    sensitive << clk.pos();
    dont_initialize();
  }

  void attach_debug_output(sc_core::sc_signal<int> &signal)
  {
    std::cout << "attach debug output.." << std::endl;

    if (!debug_output.has_value())
    {
      sc_core::sc_hierarchy_scope scope{ get_hierarchy_scope() };
      debug_output.emplace("debug_output");
      debug_output->bind(signal);
    }
    else
    {
      SC_REPORT_ERROR("optional", "Cannot attach debug output more than once.");
    }
  }

private:

  void clocked_method()
  {
    if (debug_output) {
      debug_output->write(debug_output->read() + 1);
    }
  }

  // optional output that tracks how many times clocked_method was triggered
  std::optional<sc_core::sc_out<int> > debug_output;

};


int sc_main(int argc, char **argv)
{
  demo_module demo("demo");
  sc_core::sc_signal<int> debug_out("debug_out");
  sc_core::sc_clock clk("clk", {10, sc_core::SC_NS});

  demo.clk(clk);

  demo.attach_debug_output(debug_out);

  sc_core::sc_start(40.0, sc_core::SC_NS);

  std::vector<sc_core::sc_object*> children = demo.get_child_objects();

  // Print out names and kinds of top-level objects
  for (unsigned i = 0; i < children.size(); i++)
  {
    std::cout << " name: "
              << children[i]->name()
              << " kind: " << children[i]->kind()
              << std::endl;
  }

  // expected 3 children: clk, debug_output and clocked_method
  sc_assert(demo.get_child_objects().size() == 3);
  sc_assert(debug_out == 4);

  std::cout << "program complete" << std::endl;
  return 0;
}
