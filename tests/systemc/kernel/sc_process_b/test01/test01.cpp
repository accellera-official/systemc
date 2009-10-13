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

  test01.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-03-22
                   Ucar Aziz, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of sc_process_b::kind()

#include "systemc.h"

SC_MODULE( mod_a )
{
    void main_action()
    {
	cout << "main action ";
	sc_process_b* proc;
	proc = sc_get_curr_process_handle();
	cout << proc->kind() << endl;
    }

    SC_CTOR( mod_a )
    {
	SC_METHOD( main_action );
    }
};

int
sc_main( int, char*[] )
{
    mod_a a( "a" );

    sc_start( 5, SC_NS );   

    return 0;
}
