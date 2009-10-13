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

  test01.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of the initialize() method in output signal ports

#include "systemc.h"

SC_MODULE( mod_a )
{
    sc_out<int>      out_int;
    sc_out<bool>     out_bool;
    sc_out<sc_logic> out_logic;
    sc_out_rv<4>     out_rv4;

    SC_CTOR( mod_a )
    {
        out_int.initialize( 1 );
        out_int.initialize( 2 );
        out_bool.initialize( true );
        out_logic.initialize( sc_dt::Log_Z );
        out_logic.initialize( sc_dt::Log_1 );
        out_rv4.initialize( sc_lv<4>( "ZZZZ" ) );
        out_rv4.initialize( sc_lv<4>( "1111" ) );
    }
};

int
sc_main( int, char*[] )
{
    sc_signal<int> sig_int;
    sc_signal<bool> sig_bool;
    sc_signal<sc_logic> sig_logic;
    sc_signal_rv<4> sig_rv4;

    mod_a a( "a" );

    a.out_int( sig_int );
    a.out_bool( sig_bool );
    a.out_logic( sig_logic );
    a.out_rv4( sig_rv4 );

    cout << sig_int << endl;
    cout << sig_bool << endl;
    cout << sig_logic << endl;
    cout << sig_rv4 << endl;

    sc_initialize();

    cout << sig_int << endl;
    cout << sig_bool << endl;
    cout << sig_logic << endl;
    cout << sig_rv4 << endl;

    return 0;
}
