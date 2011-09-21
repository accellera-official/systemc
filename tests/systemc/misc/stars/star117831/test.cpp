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
Mar/9/01 ulrich

This is SystemC version 1.0.1

The concat or comma operator "(,)" doesn't work for sc_bv on Solaris 
SC5.0. It works fine with g++ though.
*/

#include <systemc.h>

int sc_main(int argc, char* arg[]) 
{
    sc_bv<4> a("1101"), b("0001");
    sc_bv<8> c;

    c = (a,b);

    // OK with g++
    //
    // Error with SC5.0:
    //  Cannot assign sc_bv_ns::sc_concat<sc_bv_ns::sc_bv_base, sc_bv_ns::sc_bv_base> 
    //  to sc_bv_ns::sc_bv<8> without 
    //  "sc_bv_ns::sc_bv<8>::operator=(const sc_bv_ns::sc_bv<8>&)";.

    cout << c << "\n";

    return 0;
}
