/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  test.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/*
Dec/20/00 ulrich

I am using SystemC 1.0.1

This example causes some really ugly warnings when compiled with Solaris SC5.0.
Please note that there are actually 3 time more warnings than the one below. 
Although the simulation result is correct,  all these warnings make my hair stand up
and I wonder what I did wrong in my application code.

It works fine with gcc.
*/

#include "systemc.h"

int sc_main(int argc, char* arg[]) 
{
  sc_bv<8> bv8 = 3;
  int i;

  // gcc: OK
  // SC5.0:  works but strange warnings:
  //  "/home/pumba0/systemc-1.0.1/include/sc_bit_proxies.h", line 169: 
  //  Warning: bv hides sc_bv_ns::sc_range<sc_bv_ns::sc_bv_base>::bv.
  //   "/home/pumba0/systemc-1.0.1/include/sc_proxy.h", line 487:     
  //  Where: While instantiating "sc_bv_ns::sc_range<sc_bv_ns::sc_bv_base>
  //   ::sc_range(sc_bv_ns::sc_bv_base&, unsigned, unsigned)".
  //  "/home/pumba0/systemc-1.0.1/include/sc_proxy.h", line 487:
  //  Where: Instantiated from non-template code.
  i = (bv8.range(4, 1)).to_uint();

  cout << i << "\n";

  return 0;
}
