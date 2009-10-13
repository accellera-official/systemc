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

  blast3.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

SC_MODULE( array )
{
    SC_HAS_PROCESS( array );

    sc_in_clk clk;

    const sc_signal<bool>& reset;
          sc_signal<bool>& ready;
    const sc_signal<char>& a;
    const sc_signal<char>& b;
          sc_signal<short>& c;

    char mem[17];
    sc_unsigned i, j;

    array( sc_module_name NAME,
           sc_clock& CLK,
           const sc_signal<bool>& RESET,
                 sc_signal<bool>& READY,
           const sc_signal<char>& A,
           const sc_signal<char>& B,
                 sc_signal<short>& C )
        : clk(CLK),
          reset(RESET), ready(READY),
          a(A), b(B), c(C),
          i(2), j(2)
    {
        SC_CTHREAD( entry, clk );
        watching( reset.delayed() == 1 );
    }
    void entry();
};

void
array::entry()
{
    i = 2;
    j = 1;
    ready = 0;
    mem[i.to_uint()] = a.read();
    mem[j.to_uint()] = b.read();
    mem[14] = i.to_uint();
    mem[12] = j.to_uint();
    wait();
    mem[mem[14]] = a + 1;
    mem[mem[12]] = b - 2;
    c = mem[i.to_uint()] * mem[j.to_uint()];
    ready = 1;
    wait();
}

int sc_main(int argc, char* argv[] )
{
  return 0;
}

