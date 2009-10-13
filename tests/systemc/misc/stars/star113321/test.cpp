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
Nov/10/00 ulrich

The SystemC 1.0 manual says that the bit-wise AND operator is supported for
class sc_bv like here:
*/

#include "systemc.h"

int sc_main(int argc, char* arg[]) 
{
    sc_bv<10> a = "01111111";
    sc_bv<10> b = "11110000";
    sc_bv<10> c = a & b;
    cout << c << "\n";

    return 0;
}

/*
It does work fine with g++.
But SC5.0 gives this compile error:

  CC -pto  -g -I. -I/view/ccss_ulli/vobs/abc/src/scenic/scenery/include -I.   -I/view/ccss_ulli/vobs/abc/src/scenic/scenery/include -c str.cc
  "str.cc", line 7: Error: Cannot use const   sc_bv_ns::sc_bitwise_and<sc_bv_ns::sc_bv_base,sc_bv_ns::sc_bv_base> 
  to initialize sc_bv_ns::sc_bv<10>.


I used SystemC 1.0.1 on a Solaris 5.5.1 machines with SC5.0
*/

