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

  test05.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of dynamic design rule checking in signals.

#define DEBUG_SYSTEMC
#include "systemc.h"

SC_MODULE( mod_a )
{
    sc_in_clk clk;

    sc_out<int>      out_int;
    sc_out<bool>     out_bool;
    sc_out<sc_logic> out_logic;
    sc_out<int>      out_int2;
    sc_out_resolved  out_resolved;
    sc_out_rv<1>     out_rv1;

    void main_action1()
    {
        out_int = 42;
        out_bool = true;
        out_logic = SC_LOGIC_1;
        out_int2 = 1;
        out_resolved = SC_LOGIC_1;
        out_rv1 = sc_lv<1>( SC_LOGIC_1 );
    }

    void main_action2()
    {
        // out_int = 0;
        // out_bool = false;
        // out_logic = SC_LOGIC_0;
        out_int2 = 0;
        out_resolved = SC_LOGIC_0;
        out_rv1 = sc_lv<1>( SC_LOGIC_0 );
    }

    SC_CTOR( mod_a )
    {
        SC_METHOD( main_action1 );
        sensitive << clk.pos();
        dont_initialize();
        SC_METHOD( main_action2 );
        sensitive << clk.neg();
        dont_initialize();
    }
};

int
sc_main( int, char*[] )
{
    sc_clock clk;

    sc_signal<int> sig_int;
    sc_signal<bool> sig_bool;
    sc_signal<sc_logic> sig_logic;
    sc_buffer<int> buf_int;
    sc_signal_resolved sig_resolved;
    sc_signal_rv<1> sig_rv1;

    mod_a a("a");
    a << clk << sig_int << sig_bool << sig_logic << buf_int
      << sig_resolved << sig_rv1;

    sc_start( 20 );

    return 0;
}
