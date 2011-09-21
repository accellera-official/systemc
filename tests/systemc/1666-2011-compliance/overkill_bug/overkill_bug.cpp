
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

// overkill_bug.cpp -- test for 
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
    SC_THREAD(target);
      target_handle = sc_get_current_process_handle();
    f0 = 0;
  }
  
  struct bomb
  {
    sc_process_handle h;
    
    bomb(sc_process_handle _h)
    {
      h = _h;
    }
    
    ~bomb()
    {
      h.kill();  // Aborts !!!!!!
    }
  };

  sc_process_handle target_handle;
  int f0;
  
  void target()
  {
    bomb local_obj(target_handle);

    wait(10, SC_NS);
    
    f0 = 1;
    target_handle.kill();
  }
  
  SC_HAS_PROCESS(Top);
};

int sc_main(int argc, char* argv[])
{
  Top top("top");
  
  sc_start();
  
  sc_assert( top.f0 );

  cout << endl << "Success" << endl;
  return 0;
}
  
