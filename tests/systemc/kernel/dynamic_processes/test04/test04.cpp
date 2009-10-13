/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  Original Author: Bishnupriya Bhattacharya, Cadence Design Systems, 
                   September 5, 2003

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
// test error message for SC_FORK-SC_JOIN on dynamic method process handles

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>


class module1 : public sc_module
{
private:
  int method_count;
public:

  SC_HAS_PROCESS(module1);

  module1(sc_module_name name) : sc_module(name), 
    method_count(0)
  {
    SC_THREAD(main);
  }

  void main()
  {
    int r;
    sc_event e1, e2, e3, e4;
    sc_spawn_options o1, o2, o3, o4;

    cout << endl << sc_time_stamp() << ", " 
    << sc_get_curr_process_handle()->name() 
    << ": main thread, Before spawning round robin methods " 
    << endl << endl;

    e1.notify(15, SC_NS);
    o1.spawn_method();
    o2.spawn_method();
    o3.spawn_method();
    o4.spawn_method();

    // Spawn several threads that co-operatively execute in round robin order

    SC_FORK
      sc_spawn(&r, 
        sc_bind(&module1::round_robin, this, "1", sc_ref(e1), sc_ref(e2), 1), "mth1", &o1),
      sc_spawn(&r, 
        sc_bind(&module1::round_robin, this, "2", sc_ref(e2), sc_ref(e3), 1), "mth2", &o2),
      sc_spawn(&r, 
        sc_bind(&module1::round_robin, this, "3", sc_ref(e3), sc_ref(e4), 1), "mth3", &o3),
      sc_spawn(&r, 
        sc_bind(&module1::round_robin, this, "4", sc_ref(e4), sc_ref(e1), 1), "mth4", &o4),
    SC_JOIN

    cout << endl << sc_time_stamp() << ", " 
	 << sc_get_curr_process_handle()->name() 
	 << ": main thread, Issuing wait(60, SC_NS)" << endl;

    wait(60, SC_NS);

    cout << endl << sc_time_stamp() << ", " 
	 << sc_get_curr_process_handle()->name() 
	 << ": Done main thread." << endl;
  }

  int round_robin(const char *str, sc_event& receive, sc_event& send, int cnt)
  {
    cout << sc_time_stamp() << ", " 
	 << sc_get_curr_process_handle()->name() 
	 << ": In Round robin method " << str; 
      
    if (method_count < 4) {
      method_count++;
      next_trigger(receive);
      cout << ". Issued next_trigger. " << endl;
    } else {
      send.notify(10, SC_NS);
      cout << ". Notified. Ending Round robin method " << str << endl;
    }

    return 0;
  }

};
int sc_main (int argc , char *argv[]) 
{
  module1 mod1("mod1");

  sc_start(100, SC_NS);

  return 0;
}
