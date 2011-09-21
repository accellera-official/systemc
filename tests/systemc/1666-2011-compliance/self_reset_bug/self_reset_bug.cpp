
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

// self_reset_bug.cpp -- test for 
//
//  Original Author: John Aynsley, Doulus
//
// MODIFICATION LOG - modifiers, enter your name, affiliation, date and
//

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
    SC_METHOD(target);
      target_handle = sc_get_current_process_handle();

    count = 0;
    f0 = f1 = f2 = f3 = 0;
  }
  
  sc_process_handle target_handle;
  int count;
  int f0, f1, f2, f3;

  void target()
  {
    switch (count)
    {
      case 0:
        f0 = 1;
        count = 1;
        next_trigger(10, SC_NS);
        break;

      case 1:
        f1 = 1;
        count = 2;
        target_handle.reset();
        break;

      case 2:
        f2 = 1;
        count = 3;
        target_handle.reset();
        break;

      case 3:
        f3 = 1;
        count = 4;
        break;
    }
  }
  
  SC_HAS_PROCESS(Top);
};

int sc_main(int argc, char* argv[])
{
  Top top("top");
  
  sc_start();

  sc_assert( top.f0 ); 
  sc_assert( top.f1 ); 
  sc_assert( top.f2 ); 
  sc_assert( top.f3 ); 
  
  cout << endl << "Success" << endl;
  return 0;
}
  
