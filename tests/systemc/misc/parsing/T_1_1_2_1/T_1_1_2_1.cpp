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

  T_1_1_2_1.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

int sc_main(int argc, char* argv[] )
{
    sc_clock clock1("CLK1");
    sc_clock clock2("CLK2", 2);
    sc_clock clock3("CLK3", 3, 0.25);
    sc_clock clock4("CLK4", 4, 0.5, 0.5);
    sc_clock clock5("CLK5", 5, 0.75, 1.0, false);
    const char *name = clock1.name();
    // double period = clock2.period();
    sc_time period = clock2.period();
    double duty_cycle = clock3.duty_cycle();

    // clock4.pos();
    // clock5.neg();

    return 0;
}
