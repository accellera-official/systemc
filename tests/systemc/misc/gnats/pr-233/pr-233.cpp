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

  pr-233.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

int*
bar(int* x, int* y, int q)
{
    return q ? x : y;
}

SC_MODULE( pr233 )
{
    SC_HAS_PROCESS( pr233 );

    sc_in_clk clk;

    // const sc_signal<int*>& x;
    // const sc_signal<int*>& y;
    // const sc_signal<int>&  q;
    //       sc_signal<int*>& z;
    sc_in<int*>  x;
    sc_in<int*>  y;
    sc_in<int>   q;
    sc_out<int*> z;

    pr233( sc_module_name         NAME,
           sc_clock&              CLK,
           const sc_signal<int*>& X,
           const sc_signal<int*>& Y,
           const sc_signal<int>&  Q,
           sc_signal<int*>&       Z )
        : clk( CLK ),
          x(X), y(Y), q(Q), z(Z)
    {
      SC_CTHREAD( entry, clk.pos() );
    }
    void entry();

};

void
pr233::entry()
{
    z.write( bar(x, y, q) );
    wait();
}

int sc_main(int,char**) { return 0; }
