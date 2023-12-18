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

  empty_wif.cpp -- Test empty trace files (no updates are triggered)

  Original Author: Philipp A Hartmann, Intel Corporation, 2018-05-10

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <systemc>

SC_MODULE( module )
{
  sc_core::sc_in<bool> clk;

  SC_CTOR(module)
    : clk("clk")
  {}
};

int sc_main(int ac, char *av[])
{
  using namespace sc_core;

  sc_signal<bool> clock("clock");

  module top("top");
    top.clk(clock);

  sc_trace_file *tf = sc_create_wif_trace_file("empty");

  sc_trace(tf, clock, clock.name());
  sc_trace(tf, top.clk, top.clk.name());

  // toggle something, but without advancing the time
  clock.write(1);
  sc_start(0, SC_NS);
  clock.write(0);
  sc_start(0, SC_NS);

  sc_close_wif_trace_file( tf );

  std::cout << "\nProgram completed." << std::endl;
  return 0;
}
