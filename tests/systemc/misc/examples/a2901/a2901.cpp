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

  a2901.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "a2901.h"
#include "a2901_test.h"

SC_MODULE( twosome )
{
    // signals
    sig4 Y;
    sig1 t_RAM0, t_RAM3, t_Q0, t_Q3, C4;
    sig1 Gbar, Pbar, OVR, F3, F30;
    sig9 I;
    sig4 Aadd, Badd, D;
    sig1 RAM0, RAM3, Q0, Q3, C0, OEbar;

    // modules
    a2901 SLICE;
    a2901_test TB;

    // constructor
    twosome( sc_module_name,
             const sc_clock& CLK_ )
    : SLICE( "a2901",
             CLK_,
             I, Aadd, Badd, D, RAM0, RAM3, Q0, Q3, C0, OEbar,
             Y, t_RAM0, t_RAM3, t_Q0, t_Q3, C4, Gbar, Pbar, OVR, F3, F30 ),
      TB( "a2901_test",
          CLK_,
          Y, t_RAM0, t_RAM3, t_Q0, t_Q3, C4, Gbar, Pbar, OVR, F3, F30,
          I, Aadd, Badd, D, RAM0, RAM3, Q0, Q3, C0, OEbar )
    {}
};

int
sc_main( int, char*[] )
{
    sc_clock clk;

    twosome AA( "AA", clk );

    sc_start( 410000, SC_NS );

    cout << sc_time_stamp() << endl;

    return 0;
}
