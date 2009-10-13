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

  test.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of sc_signal<sc_logic> posedge and negedge

#include "systemc.h"

SC_MODULE( pro )
{
    sc_out<sc_logic> out;

    void main_action()
    {
        int i = 0;
        while( true ) {
            sc_logic tmp( i );
            out.write( tmp );
            cout << sc_simulation_time() << ": " << tmp.to_char() << endl;
            wait( 3, SC_NS );
            i = (i + 1) % 4;
        }
    }

    SC_CTOR( pro )
    {
        SC_THREAD( main_action );
    }
};

SC_MODULE( con )
{
    sc_in<sc_logic> in;

    void pos_action()
    {
        cout << sc_simulation_time() << ": posedge" << endl;
    }

    void neg_action()
    {
        cout << sc_simulation_time() << ": negedge" << endl;
    }

    SC_CTOR( con )
    {
        SC_METHOD( pos_action );
        sensitive_pos << in;
        dont_initialize();
        SC_METHOD( neg_action );
        sensitive_neg << in;
        dont_initialize();
    }
};

int
sc_main( int, char*[] )
{
    sc_signal<sc_logic> sig;
    pro p( "p" );
    con c( "c" );
    p.out( sig );
    c.in( sig );
    sc_start( 100 );
    return 0;
}
