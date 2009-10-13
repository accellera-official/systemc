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

  T_2_3_2_5_neg.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

#define MYNAME T_2_3_1_5

const int WIDTH = 8;
typedef sc_lv<WIDTH>         my_vector;
typedef sc_signal<my_vector> signal_vector;

#include "T_2_3_1.h"

void
MYNAME::entry()
{
    my_vector a;
    my_vector b;

    a = x;
    b = y;
    z = a | b;
    wait();
    a = x.read() | y.read();
    b = x.read() ^ y.read();
    z = a & b;
    wait();
    a = x.read() & y.read();
    b = x.read() | y.read();
    z = a ^ b;
    wait();
    a = ~ x.read();
    b = ~ y.read();
    z = a | b;
    wait();
}

int
sc_main( int, char*[] )
{
    return 0;
}
