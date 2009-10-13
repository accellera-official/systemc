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

  activa.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

SC_MODULE( activa )
{
    SC_HAS_PROCESS( activa );

    sc_in_clk clk;

    const sc_signal<unsigned>& a;
    const sc_signal<unsigned>& b;
          sc_signal<unsigned>& c;

    activa( sc_module_name NAME,
            sc_clock& CLK,
            const sc_signal<unsigned>& A,
            const sc_signal<unsigned>& B,
                  sc_signal<unsigned>& C )
        : clk( CLK ),
          a(A), b(B), c(C)
    {
      SC_CTHREAD( entry, clk.pos() );
    }
    void entry();
};

void activa::entry()
{
    bool cond1 = false, cond2 = false;

    while (1) {
        c.write( a + b );
        if (cond1) {
            if (cond2) {
                c.write( a + b );
            }
            c.write( a - b );
        }
        c.write( a * b );
    }
}

int
sc_main( int, char** )
{
  sc_clock clk;

  return 0;
}
