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

#include "flop.h"
#include "systemc.h"
  

int sc_main(int argc, char *argv[]) //(int ac, char** av)

{

  sc_signal<sc_uint<1> > int1 ;
  sc_signal<sc_uint<1> > int2 ;


  sc_clock clk("clk", 20, 0.5);



// instanciate Processes

  flop  FLOP("flip_flop");
  FLOP.clk(clk) ;
  FLOP.in(int1) ;
  FLOP.out(int2) ;

  sc_trace_file * tf = sc_create_wif_trace_file("test");
  sc_trace( tf, clk, "clk");
  sc_trace( tf, int1, "int1");
  sc_trace( tf, int2, "int2");

  /*
  sc_trace_file * tf2 = sc_create_vcd_trace_file("dump_vcd");
  sc_trace( tf2, clk, "clk");
  sc_trace( tf2, int1, "int1");
  sc_trace( tf2, int2, "int2");
  */

  sc_start(1000);
  sc_close_wif_trace_file( tf );
  return 0;
}
