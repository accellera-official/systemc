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
  Assignment of an integer to a bit-slice of a bit-vector
  works fine for sc_[u]int, sc_bug[u]int but not sc_bv
  and sc_lv. That's very strange, especially since it
  it allowed to assign an integer to the entire sc_bv/sc_lv
  vector. The error message from SC6.1 is

    no match for `sc_bv_ns::sc_range<sc_bv_ns::sc_lv_base> = int &'

  Example:
*/

#include <systemc.h>

int sc_main(int, char**)
{
  sc_int<32> i;
  sc_uint<32> u;
  sc_bigint<32> bi;
  sc_biguint<32> bu;
  sc_lv<32> lv;
  sc_bv<32> bv;
  int I = 42;

  i.range(14,1) = I;
  u.range(14,1) = I;
  bi.range(14,1) = I;
  bu.range(14,1) = I;
  lv.range(14,1) = I; // ERROR
  bv.range(14,1) = I; // ERROR

  return 0;
}
