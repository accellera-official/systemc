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

  test10.cpp -- 

  Original Author: Ucar Aziz, Synopsys, Inc., 2002-02-15
                   Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of at_negedge(const sc_signal_in_if<sc_logic>&,..) and at_posedge(....)

#include "systemc.h"

SC_MODULE( mod_a )
{
    sc_in<bool> clk;
    sc_in<sc_logic> input;

    void main_action_1()
    {
     cout << "main_action_1 increments a\n";
     int a = 0;
     const sc_signal_in_if<sc_logic>* iface;
     iface = (DCAST<const sc_signal_in_if<sc_logic>*>(input.get_interface()));
       while( true ) {
         at_negedge(*iface);
         cout<<"a = " << ( ++ a ) << endl;
         }
    };

    void main_action_2()
    {
     cout << "main_action_2 increments b\n";
     int b = 0;
     const sc_signal_in_if<sc_logic>* iface1;
     iface1 = (DCAST<const sc_signal_in_if<sc_logic>*>(input.get_interface()));
       while( true ) {
         at_posedge(*iface1);
         cout<<"b = " << ( ++ b ) << endl;
         }
    }

    SC_CTOR( mod_a )
    {
        SC_CTHREAD( main_action_1, clk.pos() );
        SC_CTHREAD( main_action_2, clk.neg() );

    }
};


// sc_logic source model
SC_MODULE( mod_b )
{
  sc_in<bool> clk;
  sc_out<sc_logic> out;

  void main_action()
  {
    static sc_logic output;
    output = '0';
    while( true ){
      wait( );
      out->write(output);
      output = ~output;
    }
  }
    SC_CTOR( mod_b )
    {
        SC_THREAD( main_action );
        sensitive_pos << clk;
    }
};


int
sc_main( int, char*[] )
{
    mod_a a( "a" );
    mod_b b( "b" );
    sc_signal<sc_logic> chanel;
    sc_clock clk( "clk", 0.1 );
    sc_clock clk3("clk3", 0.5);
    a.clk( clk );
    b.clk( clk3 );
    b.out(chanel);
    a.input(chanel);

    sc_start( 5 );

    return 0;
}
