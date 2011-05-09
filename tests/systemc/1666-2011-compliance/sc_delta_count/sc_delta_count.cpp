
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

// sc_delta_count.cpp -- test for 
//
//  Original Author: John Aynsley, Doulos, Inc.
//
// MODIFICATION LOG - modifiers, enter your name, affiliation, date and
//
// $Log: sc_delta_count.cpp,v $
// Revision 1.2  2011/05/08 19:18:46  acg
//  Andy Goodrich: remove extraneous + prefixes from git diff.
//

// sc_delta_count, including notifications across pauses

#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <systemc>
using namespace sc_core;
using std::cout;
using std::endl;

struct Top: sc_module
{
  Top(sc_module_name _name)
  {
    
    SC_THREAD(T);

    SC_METHOD(M);
      sensitive << ev << ev2 << ev3;
      dont_initialize();
      m = sc_get_current_process_handle();
      
    sc_assert( sc_delta_count() == 0 );
    count = 0;
    reached_end = false;
  }
  
  void start_of_simulation()
  {
    sc_assert( sc_delta_count() == 0 );
  }

  sc_process_handle m;  
  sc_event ev, ev2, ev3;
  int count;
  bool reached_end;
  
  void T()
  {
    sc_assert( sc_delta_count() == 0 );
    
    count = 1;
    ev.notify();
    wait(SC_ZERO_TIME);
    sc_assert( sc_delta_count() == 1 );
    
    count = 2;
    ev.notify();
    wait(SC_ZERO_TIME);
    sc_assert( sc_delta_count() == 2 );
    
    count = 3;
    ev.notify(SC_ZERO_TIME);
    wait(SC_ZERO_TIME);
    sc_assert( sc_delta_count() == 3 );
    
    count = 4;
    ev.notify(1, SC_NS);
    wait(SC_ZERO_TIME);
    sc_assert( sc_delta_count() == 4 );

    count = 5;
    wait(1, SC_NS);
    ev.notify(1, SC_NS);
    sc_assert( sc_delta_count() == 5 );
    
    count = 6;
    wait(2, SC_NS);
    sc_assert( sc_delta_count() == 7 );

    count = 7;
    m.disable();
    ev.notify(1, SC_NS);
    wait(2, SC_NS);
    sc_assert( sc_delta_count() == 8 );

    count = 8;
    m.enable();
    ev .notify();
    ev2.notify(1, SC_NS);
    ev3.notify(2, SC_NS);
    wait(3, SC_NS);
    sc_assert( sc_delta_count() == 11 );

    count = 9;
    sc_pause(); // 1st pause
    wait(ev);
    sc_assert( sc_delta_count() == 12 );

    count = 10;
    sc_pause(); // 2nd pause
    wait(ev);
    sc_assert( sc_delta_count() == 13 );

    count = 11;
    sc_pause(); // 3rd pause
    wait(ev);
    sc_assert( sc_delta_count() == 14 );

    count = 12;
    sc_pause(); // 4th pause
    wait(ev);
    sc_assert( sc_delta_count() == 15 );
    
    count = 13;
    sc_pause(); // 5th pause
    wait(ev);
    sc_assert( sc_delta_count() == 16 );
    
    reached_end = true;
  }
  
  void M()
  {
    cout << "M() awoke at " << sc_time_stamp() << endl;
    switch (count)
    {
      case  0: sc_assert( false ); break;
      case  1: sc_assert( sc_delta_count() == 0 ); break;
      case  2: sc_assert( sc_delta_count() == 1 ); break;
      case  3: sc_assert( sc_delta_count() == 3 ); break;
      case  4: sc_assert( false ); break;
      case  5: sc_assert( sc_delta_count() == 5 ); break;
      case  6: sc_assert( sc_delta_count() == 6 ); break;
      case  7: sc_assert( false ); break;
      case  8: sc_assert( sc_delta_count() == 8 ||
                         sc_delta_count() == 9 ||
                         sc_delta_count() == 10 ); break;
      case  9: sc_assert( sc_delta_count() == 12 ); break;
      case 10: sc_assert( sc_delta_count() == 13 ); break;
      case 11: sc_assert( sc_delta_count() == 14 ); break;
      case 12: sc_assert( sc_delta_count() == 15 ); break;
      case 13: sc_assert( sc_delta_count() == 16 ); break;
    }
  }
    
  SC_HAS_PROCESS(Top);
};

int sc_main(int argc, char* argv[])
{
  sc_assert( sc_delta_count() == 0 );

  Top top("top");
  
  sc_start();
  
  sc_assert( sc_delta_count() == 12 );
  sc_assert( sc_get_status() == SC_PAUSED );
  top.ev.notify();  // Wake from 1st pause on immed notification
  
  sc_start();
  
  sc_assert( sc_delta_count() == 13 );
  sc_assert( sc_get_status() == SC_PAUSED );
  sc_assert( sc_pending_activity_at_current_time() == false );
  sc_assert( sc_pending_activity_at_future_time() == false );

  sc_start(SC_ZERO_TIME);
  sc_assert( sc_delta_count() == 13 );
  
  sc_assert( sc_get_status() == SC_PAUSED );
  sc_start(SC_ZERO_TIME);
  sc_assert( sc_delta_count() == 13 );
  
  sc_start(1, SC_NS);
  sc_assert( sc_delta_count() == 13 );
  sc_assert( top.count == 10 );

  top.ev.notify(SC_ZERO_TIME); // Wake from 2nd pause on delta notification
  sc_start(1, SC_NS);
  sc_assert( sc_delta_count() == 14 );
  
  top.ev.notify(1, SC_NS);   // Wake from 3rd pause on timed notification
  sc_start(2, SC_NS);
  sc_assert( sc_delta_count() == 15 );
  
  top.ev.notify(2, SC_NS);   // Future notification beyond the subsequent start
  sc_assert( sc_pending_activity_at_current_time() == false );
  sc_assert( sc_pending_activity_at_future_time() == true );
  sc_start(1, SC_NS);
  sc_assert( sc_delta_count() == 15 );

  sc_start();
  sc_assert( sc_delta_count() == 16 );

  top.ev.notify();  // Wake from 5th pause on immed notification
  sc_start(SC_ZERO_TIME);
  sc_assert( sc_delta_count() == 17 );
  
  sc_assert( sc_get_status() == SC_PAUSED );
  sc_stop();

  sc_assert( sc_get_status() == SC_STOPPED );

  sc_assert( top.reached_end );    
  cout << endl << "Success" << endl;
  return 0;
}
