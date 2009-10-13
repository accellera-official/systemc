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

  clocks.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename clocks.h */
/* This is an integrated interface & implementation file for */
/* all clock processes that react to the clock edges */

#include "systemc.h"

// First process
SC_MODULE( clk_pos )
{
  SC_HAS_PROCESS( clk_pos );

  sc_in_clk clk;

  sc_signal<bool>& out_clk_pos; 
  
  clk_pos(sc_module_name NAME,
	  sc_clock& TICK_P,
	  sc_signal<bool>& OUT_CLK_POS)
    : clk(TICK_P),
      out_clk_pos(OUT_CLK_POS)
  {
    SC_CTHREAD( entry, clk.pos() );
  }
  
  void entry();
};

void
clk_pos::entry()
{
  out_clk_pos.write(1); cout << "Clk Pos 1\n";
  wait();
  out_clk_pos.write(0); cout << "Clk Pos 0\n";
  wait();
}

// Second process
SC_MODULE( clk_neg )
{
  SC_HAS_PROCESS( clk_neg );

  sc_in_clk clk;

  sc_signal<bool>& out_clk_neg; 
  
  clk_neg(sc_module_name NAME,
	  sc_clock& TICK_N,
	  sc_signal<bool>& OUT_CLK_NEG)
    : clk(TICK_N),
      out_clk_neg(OUT_CLK_NEG)
  {
    SC_CTHREAD( entry, clk.neg() );
  }
 
  void entry();
};

void
clk_neg::entry()
{
  out_clk_neg.write(1); cout << "Clk Neg 1\n";
  wait();
  out_clk_neg.write(0); cout << "Clk Neg 0\n";
  wait();
}

// Third process
SC_MODULE( clk2_pos )
{
  SC_HAS_PROCESS( clk2_pos );

  sc_in_clk clk;

  sc_signal<bool>& out_clk2_pos; 
 
  clk2_pos(sc_module_name NAME,
	   sc_clock& TICK2_P,
	   sc_signal<bool>& OUT_CLK2_POS)
    : clk(TICK2_P),
      out_clk2_pos(OUT_CLK2_POS)
  {
    SC_CTHREAD( entry, clk.pos() );
  }
 
  void entry();
};

void
clk2_pos::entry()
{
  out_clk2_pos.write(1); cout << "Clk2 Pos 1\n";
  wait();
  out_clk2_pos.write(0); cout << "Clk2 Pos 0\n";
  wait();
}

// Fourth process
SC_MODULE( clk2_neg )
{
  SC_HAS_PROCESS( clk2_neg );

  sc_in_clk clk;

  sc_signal<bool>& out_clk2_neg; 
 
  clk2_neg(sc_module_name NAME,
	   sc_clock& TICK2_N,
	   sc_signal<bool>& OUT_CLK2_NEG)
    : clk(TICK2_N),
      out_clk2_neg (OUT_CLK2_NEG)
  {
    SC_CTHREAD( entry, clk.neg() );
  }
 
  void entry();
};

void
clk2_neg::entry()
{
  out_clk2_neg.write(1); cout << "Clk2 Neg 1\n";
  wait();
  out_clk2_neg.write(0); cout << "Clk2 Neg 0\n";
  wait();
}
