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

// test01.cpp -- Quick Test Of Process Control On Threads For IEEE 1666-2011
//
//  Original Author: John Aynsley, Doulos
//
// MODIFICATION LOG - modifiers, enter your name, affiliation, date and
//
// $Log: test01.cpp,v $
// Revision 1.2  2011/03/07 19:32:18  acg
//  Andy Goodrich: addition to set sc_core::sc_allow_process_control_corners
//  to true so that this test avoids corner case error messages.
//
// Revision 1.1  2011/02/05 21:13:19  acg
//  Andy Goodrich: move of tests John Aynsley will replace.
//
// Revision 1.1  2011/01/14 14:23:16  acg
//  Andy Goodrich: new test.
//

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc>

using namespace sc_core;
using std::cout;
using std::endl;

SC_MODULE(Top)
{
  SC_CTOR(Top)
  {
    SC_THREAD(gen);
    SC_THREAD(T1);
      h1 = sc_get_current_process_handle();
    SC_THREAD(T2);
      h2 = sc_get_current_process_handle();
  }

  sc_event ev;

  sc_process_handle h1, h2;

  void gen()
  {
    for (;;)
    {
      wait(10, SC_NS);
      ev.notify();
    }
  }

  void T1()
  {
    wait(25, SC_NS);
    cout << "suspend: time = " << sc_time_stamp() << endl;
    h2.suspend();
    wait(20, SC_NS);
    cout << "resume: time = " << sc_time_stamp() << endl;
    h2.resume();
    wait(20, SC_NS);

    cout << "disable: time = " << sc_time_stamp() << endl;
    h2.disable();
    wait(20, SC_NS);
    cout << "enable: time = " << sc_time_stamp() << endl;
    h2.enable();
    wait(20, SC_NS);
    sc_stop();
  }

  void T2()
  {
    for (;;)
    {
      wait(ev);
      cout << "T2: time = " << sc_time_stamp() << endl;
    }
  }
};

int sc_main(int argc, char* argv[])
{
  Top top("top");
  sc_core::sc_allow_process_control_corners = true;
  sc_start();

  cout << endl << "End Of Test" << endl;
  return 0;
}
