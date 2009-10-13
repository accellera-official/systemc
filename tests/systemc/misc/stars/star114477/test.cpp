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

#ifdef RTL
#include "design_rtl.h"
#else
#include "design.h"
#endif

#define NS * 1e-9

int sc_main(int ac, char *av[])
{
  // Signals
  sc_signal<bool>        clk;
  sc_signal<sc_bv<9> >  out_a0, out_a1;
  sc_signal<int>         count;

  fun c_block("fun_block");
  c_block.clk(clk);
  c_block.count(count);
  c_block.out_a0(out_a0);
  c_block.out_a1(out_a1);

  sc_start(0);

  count = 0;

  for(int i = 0; i < 10; i++){
      clk.write(1);
      sc_start( 5 NS );
      clk.write(0);
      sc_start( 5 NS );      
      count = count + 1;
      cout << "constants " << out_a0 << "\t" << out_a1 << endl;
  }
 
  return 0;
}
