/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  test.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of sc_stop() inside end_of_elaboration()

#include "systemc.h"

SC_MODULE( mod_a )
{
    sc_in_clk clk;

    void main_action()
    {
        cout << "main_action\n";
    }

    SC_CTOR( mod_a )
    {
        SC_METHOD( main_action );
        sensitive << clk.pos();
    }

    virtual void end_of_elaboration()
    {
        sc_stop();
    }
};

int
sc_main( int, char*[] )
{
    sc_clock clk;
    mod_a a("a");
    a.clk( clk );
    sc_start( 100 );
    return 0;
}
