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

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of SC_METHOD and SC_THREAD (but not SC_CTHREAD) initialization

#include "systemc.h"

SC_MODULE( mod_a ) {

    sc_in_clk clk;

    void main_action_method()
    {
        cout << sc_delta_count()
             << " main_action_method called" << endl;
    }

    void main_action_thread()
    {
        while( true ) {
            cout << sc_delta_count()
                 << " main_action_thread called" << endl;
            wait(); // for what?
        }
    }

    void main_action_cthread()
    {
        while( true ) {
            cout << sc_delta_count()
                 << " main_action_cthread called" << endl;
            wait();
        }
    }

    SC_CTOR( mod_a )
    {
        SC_METHOD( main_action_method );
        SC_THREAD( main_action_thread );
        SC_CTHREAD( main_action_cthread, clk.pos() );
    }
};

int
sc_main( int, char*[] )
{
    sc_clock clk;

    mod_a a( "a" );
    a.clk( clk );

    sc_start(1, SC_NS);

    return 0;
}
