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

  test09.cpp -- 

  Original Author: Ucar Aziz, Synopsys, Inc., 2002-02-15
                   Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of at_negedge(const sc_signal_in_if<bool>&,..)

#include "systemc.h"

SC_MODULE( mod_a )
{
    sc_in<bool> clk;
    sc_in<bool> clk3;
        
    void main_action()
    {
     const sc_signal_in_if<bool>* iface;
     iface = (DCAST<const sc_signal_in_if<bool>*>(clk3.get_interface()));
     cout << "main_action\n";
     int a = 0;
     while( true ) {
       at_negedge(*iface);
       cout<<"a = " << ( ++ a ) << endl;
       }
    }

    SC_CTOR( mod_a )
    {
        SC_CTHREAD( main_action, clk.neg() );
    }
};

int
sc_main( int, char*[] )
{
    mod_a a( "a" );
    sc_clock clk( "clk", 0.1 );
    sc_clock clk3("clk3", 0.5);
    a.clk( clk );
    a.clk3( clk3 );

    sc_start( 5 );

    return 0;
}
