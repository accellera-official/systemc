/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  test01.cpp -- Demo "new" dynamic method support.


  See the README file for a description of these capabilities. This demo
  excercises all of the major capabilities.

  Original Author: Stuart Swan, Cadence Design Systems, Inc., 2002-10-22

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Andy Goodrich, Forte Design Systems, 30 July 03
  Description of Modification: Converted thread demo to method demo.

 *****************************************************************************/


#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>


int test_function(double d)
{
  cout << endl << "Test_function sees " << d << endl;
  return int(d);
}

void void_function(double d)
{
  cout << endl << "void_function sees " << d << endl;
}

int ref_function(const double& d)
{
  cout << endl << "ref_function sees " << d << endl;
  return int(d);
}


class top : public sc_module
{
public:
  SC_HAS_PROCESS(top);

  top(sc_module_name name) : sc_module(name) 
  {
     SC_THREAD(main);
  }

  void main()
  {
    sc_event e1, e2, e3, e4;
	sc_spawn_options options1, options2, options3, options4;
    int r;

    cout << endl;

    e1.notify(100, SC_NS);

    // Spawn several methods that co-operatively execute in round robin order

	options1.spawn_method();
	options1.dont_initialize();
	options1.set_sensitivity(&e1);
    sc_spawn(&r,
        sc_bind(&top::round_robin, this, "1", sc_ref(e1), sc_ref(e2), 3), 
		"1", &options1
	  );

	options2.spawn_method();
	options2.dont_initialize();
	options2.set_sensitivity(&e2);
    sc_spawn(&r,
        sc_bind(&top::round_robin, this, "2", sc_ref(e2), sc_ref(e3), 3), 
		"2", &options2
	);

	options3.spawn_method();
	options3.dont_initialize();
	options3.set_sensitivity(&e3);
    sc_spawn(&r,
        sc_bind(&top::round_robin, this, "3", sc_ref(e3), sc_ref(e4), 3), 
		"3", &options3
	);

	options4.spawn_method();
	options4.dont_initialize();
	options4.set_sensitivity(&e4);
    sc_spawn(&r,
        sc_bind(&top::round_robin, this, "4", sc_ref(e4), sc_ref(e1), 3), 
		"4", &options4
	);
	wait(295, SC_NS);
    cout << endl << "Done." << endl;
	sc_stop();
  }

  int round_robin(const char *str, sc_event& receive, sc_event& send, int cnt)
  {
      cout << "Round robin method " << str <<
			" at time " << sc_time_stamp() << endl;
      next_trigger(receive);
      send.notify(10, SC_NS);
      return 0;
  }
};

int sc_main (int argc , char *argv[]) 
{
  top top1("Top1");
  sc_start();

  return 0;
}
