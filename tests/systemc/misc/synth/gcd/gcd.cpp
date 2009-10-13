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

  gcd.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

SC_MODULE( gcd_cc )
{
    SC_HAS_PROCESS( gcd_cc );

    sc_in_clk clk;

    const sc_signal<bool>& reset;
    const sc_signal<unsigned>& a;
    const sc_signal<unsigned>& b;
          sc_signal<unsigned>& c;
          sc_signal<bool>&     ready;

    gcd_cc( sc_module_name NAME,
            sc_clock& CLK,
            const sc_signal<bool>& RESET,
            const sc_signal<unsigned>& A,
            const sc_signal<unsigned>& B,
            sc_signal<unsigned>& C,
            sc_signal<bool>&     READY )
        : 
          reset(RESET),
          a(A),
          b(B),
          c(C),
          ready(READY)
    {
        clk( CLK );
		SC_CTHREAD( entry, clk.pos() );
        reset_signal_is(reset,true);
    }

    void entry();
};

void
gcd_cc::entry()
{
    unsigned tmp_a;
    wait();         // Note that this wait() is required, otherwise,
                    // the reset is wrong!  This is a problem with BC,
                    // not our frontend.

    while (true) {
        unsigned tmp_b;

        c = tmp_a;
        ready = true;
        wait();

        tmp_a = a;
        tmp_b = b;
        ready = false;
        wait();

        while (tmp_b != 0) {

            unsigned tmp_c = tmp_a;
            tmp_a = tmp_b;
            wait();

            while (tmp_c >= tmp_b) {
                tmp_c = tmp_c - tmp_b;
                wait();
            }

            tmp_b = tmp_c;
            wait();
        }
    }
}

int sc_main(int argc, char* argv[] )
{
  return 0;
}
