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

// test05.cpp -- Reset and kill a thread process, including nested kills
//
//  Original Author: John Aynsley, Doulos
//
// MODIFICATION LOG - modifiers, enter your name, affiliation, date and
//
// $Log: test05.cpp,v $
// Revision 1.1  2011/02/05 21:13:26  acg
//  Andy Goodrich: move of tests John Aynsley will replace.
//
// Revision 1.2  2011/01/25 20:54:03  acg
//  Andy Goodrich: regolden for new delta counter rules.
//
// Revision 1.1  2011/01/24 12:06:10  acg
//  Andy Goodrich: changes for IEEE 1666 2011
//


#include <systemc>

using namespace sc_core;
using std::cout;
using std::endl;

struct M3: sc_module
{
  M3(sc_module_name _name)
  {
    SC_THREAD(ticker);
      k = sc_get_current_process_handle();

    SC_THREAD(calling);

    SC_THREAD(target);
      t = sc_get_current_process_handle();

    killing_over = false;
  }

  sc_process_handle t, k;
  sc_event ev;
  int count;
  bool killing_over;

  void ticker()
  {
    for (;;)
    {
      try {
        wait(10, SC_NS);
        ev.notify();
      }
      catch (const sc_unwind_exception& ex) {
        // ticker process killed by target
        cout << "sc_unwind_exception caught by ticker" << endl;
	sc_assert( sc_is_unwinding() ); 
        sc_assert( !ex.is_reset() );
        sc_assert( count == 1 );
        sc_assert( !killing_over );
        throw ex;
      }
    }
  }

  void calling()
  {
    wait(15, SC_NS);
    // Target runs at time 10 NS due to notification
    sc_assert( count == 1 );

    wait(10, SC_NS);
    // Target runs again at time 20 NS due to notification
    sc_assert( count == 2 );

    t.reset();
    // Target reset immediately at time 25 NS
    sc_assert( count == 0 );

    wait(10, SC_NS);
    // Target runs again at time 30 NS due to notification
    sc_assert( count == 1 );

    t.kill();
    sc_assert( !killing_over );
    killing_over = true;

    // Target killed immediately at time 35 NS
    sc_assert( t.terminated() ); 
    sc_assert( k.terminated() );

    sc_stop();
  }

  void target()
  {
    cout << "Target called/reset at " << sc_time_stamp() << endl;
    count = 0;
    for (;;)
    {
      try {
        wait(ev);
        cout << "Target awoke at " << sc_time_stamp() << endl;
        ++count;
      }
      catch (const sc_unwind_exception& ex) {
        cout << "sc_unwind_exception caught by target" << endl;
        if (count == 2)
          sc_assert( ex.is_reset() );
        else if (count == 1)
        {
          sc_assert( !ex.is_reset() );
          sc_assert( !killing_over );
          k.kill();
        }
        else
          sc_assert( false );
        throw ex;
      }
    }
  }

  SC_HAS_PROCESS(M3);
};

int sc_main(int argc, char* argv[])
{
  M3 m("m");

  sc_start();

  return 0;
}

