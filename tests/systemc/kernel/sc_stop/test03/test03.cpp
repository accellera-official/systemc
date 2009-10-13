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

  test03.cpp -- 

  Original Author: Andy Goodrich, Forte Design Systems

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: 
  Description of Modification: 

 *****************************************************************************/

#include "systemc.h"

SC_MODULE(X)
{
	SC_CTOR(X)
	{
		cout << "Creating able..." << endl;
		SC_THREAD(able);
		sensitive << immediate_event;
		cout << "Creating baker..." << endl;
		SC_THREAD(baker);
	}

	void able()
	{
		wait();
		cout << "able: " << sc_time_stamp() << endl;
	}
	void baker()
	{
		cout << "baker: " << sc_time_stamp()
			 << ": issuing sc_stop()" <<  endl;
		sc_stop();
		immediate_event.notify();
	}
	sc_event immediate_event;
};

int sc_main(int argc, char* argv[] )
{
	X        x("x");

	//sc_set_stop_mode(SC_STOP_IMMEDIATE);
	sc_start(100, SC_NS);
	return 0;
}


