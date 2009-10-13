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

  pr-134.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

SC_MODULE( arst )
{
    SC_HAS_PROCESS( arst );

    sc_in_clk clk;

    const sc_signal<char>& a;
          sc_signal<char>& b;

    arst( sc_module_name NAME,
          sc_clock& CLK,
          
          const sc_signal<char>& A,
                sc_signal<char>& B )
        : clk(CLK), a(A), b(B)
    {
      SC_CTHREAD( entry, clk.pos() );
    }
    void entry();
};

sc_signal<char> yikes; /* instantiation bug with gcc 2.95 */

struct xyz {
    char x;
    char y;
};

void
arst::entry()
{
    xyz xyz_array[8];
    for (signed char i = 0; i < 8; ++i) {
        xyz_array[i].x = a;
        wait();
    }
    for (signed char i = 0; i < 8; ++i) {
        b = xyz_array[i].x;
        wait();
    }
}

int sc_main(int argc, char* argv[] )
{ 
  return 0;
}
