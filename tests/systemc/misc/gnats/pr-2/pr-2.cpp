/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  pr-2.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

SC_MODULE( foo )
{
    SC_HAS_PROCESS( foo );

    sc_in_clk clk;

    const sc_signal<bool>& a;
    const sc_signal<bool>& b;
          sc_signal<bool>& c;

    sc_signed x;
    sc_unsigned y;
//    sc_logic_vector z;
//    sc_bool_vector v;

    foo( sc_module_name name,
         sc_clock& CLK,
         const sc_signal<bool>& A,
         const sc_signal<bool>& B,
               sc_signal<bool>& C )
        : 
          a(A), b(B), c(C),
          x(13), y(15) // , z(8), v(11)
    {
      clk(CLK);
	  SC_CTHREAD( entry, clk.pos() );
    }
    void entry();
};

void
foo::entry()
{
    sc_signed x2(13);
    sc_unsigned y2(15);

    x2 = x; // should have no converts here
    y2 = y; // should have no converts here
}

int sc_main(int argc, char* argv[] )
{
  return 0;
}
