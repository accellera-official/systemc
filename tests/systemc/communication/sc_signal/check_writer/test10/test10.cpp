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

  test08.cpp -- 

  Original Author: Andy Goodrich, Forte Design Systems, 15 Oct 2003

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test register port for more than one writer

#define DEBUG_SYSTEMC
#include "systemc.h"
typedef sc_biguint<5> target;

SC_MODULE( mod_a )
{
    sc_in_clk clk;

    sc_out<target>      out_target;
    sc_out<target>      out_target2;

    SC_CTOR( mod_a )
    {
    }
};

int
sc_main( int, char*[] )
{
    sc_clock clk;

    sc_signal<target> sig_target;

    mod_a a("a");
    a << clk << sig_target << sig_target;

    sc_start(1);

    return 0;
}
