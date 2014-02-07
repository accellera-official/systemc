
//******************************************************************************
//
//  The following code is derived, directly or indirectly, from the SystemC
//  source code Copyright (c) 1996-2014 by all Contributors.
//  All Rights reserved.
//
//  The contents of this file are subject to the restrictions and limitations
//  set forth in the SystemC Open Source License (the "License");
//  You may not use this file except in compliance with such restrictions and
//  limitations. You may obtain instructions on how to receive a copy of the
//  License at http://www.accellera.org/. Software distributed by Contributors
//  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
//  ANY KIND, either express or implied. See the License for the specific
//  language governing rights and limitations under the License.
//******************************************************************************

// living_dead_bug.cpp -- test for 
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
    SC_THREAD(control);
    
    SC_THREAD(target);
      dont_initialize();
      target_handle = sc_get_current_process_handle();

    f0 = f1 = 0;
  }
  
  sc_process_handle target_handle;
  int f0, f1;
  
  void control()
  {
    // create another (orphaned) instance of "target"
    {
      sc_spawn_options opt;
      opt.dont_initialize();
      sc_spawn( sc_bind( &Top::target, this ), "dyn_target", &opt );
    }

    wait(10, SC_NS);
    f0 = 1;
    target_handle.kill();
    f1 = 1;
  }

  void target()
  {
    sc_assert( false );  // FAILS!!!!!!
  }
  
  SC_HAS_PROCESS(Top);
};

int sc_main(int argc, char* argv[])
{
  Top top("top");
  
  sc_start();

  sc_assert( top.f0 ); 
  sc_assert( top.f1 ); 
  
  cout << endl << "Success" << endl;
  return 0;
}
  
