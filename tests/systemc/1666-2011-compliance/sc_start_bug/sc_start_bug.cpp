//******************************************************************************
//
//  The following code is derived, directly or indirectly, from the SystemC
//  source code Copyright (c) 1996-2011 by all Contributors.
//  All Rights reserved.
//
//  The contents of this file are subject to the restrictions and limitations
//  set forth in the SystemC Open Source License Version 2.4 (the "License");
//  You may not use this file except in compliance with such restrictions and
//  limitations. You may obtain instructions on how to receive a copy of the
//  License at http://www.systemc.org/. Software distributed by Contributors
//  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
//  ANY KIND, either express or implied. See the License for the specific
//  language governing rights and limitations under the License.
//******************************************************************************

// sc_start_bug.cpp
//
//  Original Author: John Aynsley, Doulos, Inc.
//
// MODIFICATION LOG - modifiers, enter your name, affiliation, date and
//

#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <systemc>
using namespace sc_core;
using std::cout;
using std::endl;

struct Top: sc_module
{
  Top(sc_module_name _name)
  {
    SC_THREAD(T1);
    SC_THREAD(T2);
  }
  
  void T1()
  {
    sc_assert( sc_time_stamp() == sc_time(0, SC_NS) );
    wait(15, SC_NS);
    sc_assert( sc_time_stamp() == sc_time(15, SC_NS) );
    sc_pause();
  }

  void T2()
  {
    sc_assert( sc_time_stamp() == sc_time(0, SC_NS) );
    wait(30, SC_NS);
    sc_assert( sc_time_stamp() == sc_time(30, SC_NS) );
    sc_pause();
  }

  SC_HAS_PROCESS(Top);
};

int sc_main(int argc, char* argv[])
{
  Top top("top");
  
  sc_start(10, SC_NS, SC_RUN_TO_TIME);
  sc_assert( sc_time_stamp() == sc_time(10, SC_NS) );

  sc_start(10, SC_NS, SC_RUN_TO_TIME);
  sc_assert( sc_time_stamp() == sc_time(15, SC_NS) );
  
  sc_start(10, SC_NS, SC_RUN_TO_TIME);
  sc_assert( sc_time_stamp() == sc_time(25, SC_NS) );
  
  sc_start(10, SC_NS, SC_RUN_TO_TIME);
  sc_assert( sc_time_stamp() == sc_time(30, SC_NS) );
  
  cout << endl << "Success" << endl;
  return 0;
}
