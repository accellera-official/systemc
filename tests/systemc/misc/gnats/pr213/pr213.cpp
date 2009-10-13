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

  pr213.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

SC_MODULE( abc )
{
    SC_HAS_PROCESS( abc );

    sc_in_clk clk;

    const sc_signal<bool>& a;
    const sc_signal<bool>& b;
          sc_signal<bool>& c;

    abc( sc_module_name NAME,
         sc_clock& CLK,

         const sc_signal<bool>& A,
         const sc_signal<bool>& B,
               sc_signal<bool>& C )

        : clk(CLK),
          a(A), b(B), c(C)
    {
      SC_CTHREAD( entry, clk.pos() );
    }
    void entry();
};

void
abc::entry()
{
    int i = 0, j = 0;
    do {
        i++;
        if (i == 5) continue;
        if (i == 7) break;
        j = j + 2;
        wait();
    } while (a.read() && b.read());
}

int sc_main( int, char** ) { return 0; }
