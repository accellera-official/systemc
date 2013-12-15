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

#include <systemc.h>

int
sc_main(int, char**)
{
  { // sc_int works fine
    sc_int<8> a;
    sc_int<16> x;
    a=sc_bigint<8>("0b11101011");
    
    x=(a,a); 
    cout << "sc_int     (1) " << sc_bigint<16>(x).to_string(SC_BIN) << endl;
    
    x=(a.range(7,0), a.range(3,0), a.range(3,0));
    cout << "sc_int     (2)                 "
         << sc_bigint<16>(x).to_string(SC_BIN) << endl;
  }
  { // sc_uint works fine
    sc_uint<8> a;
    sc_uint<16> x;
    a=sc_biguint<8>("0b11101011");
    
    x=(a,a); 
    cout << "sc_uint    (1) " << sc_biguint<16>(x).to_string(SC_BIN) << endl;
    
    x=(a.range(7,0), a.range(3,0), a.range(3,0));
    cout << "sc_uint    (2)                 "
         << sc_biguint<16>(x).to_string(SC_BIN) << endl;
  }
  {
    sc_bigint<8> a;
    sc_bigint<16> x;
    a="0b11101011";

    // compile error (SC6.1): 
    // Error: Overloading ambiguity between "operator,(const sc_uint_base&, const sc_uint_base&)" 
    //   and "operator,(const sc_int_base&, const sc_int_base&)".
    // Error: Overloading ambiguity between "sc_bigint<16>::operator=(const sc_uint_base&)" 
    //   and "sc_bigint<16>::operator=(long long)".
    // ----
    // x=(a,a);
    // cout << "sc_bigint  (1) " << x.to_string(SC_BIN) << endl;

    // runtime error: concat yields wrong result
    // returned value is 1111111111111011
    // but should be     1110101110111011
    x=(a.range(7,0), a.range(3,0), a.range(3,0));
    cout << "sc_bigint  (2)                 "<< x.to_string(SC_BIN) << endl;
  }
  {
    sc_biguint<8> a;
    sc_biguint<16> x;
    a="0b11101011";

    // compile error (SC6.1): 
    // Error: Overloading ambiguity between "operator,(const sc_uint_base&, const sc_uint_base&)" 
    //   and "operator,(const sc_int_base&, const sc_int_base&)".
    // Error: Overloading ambiguity between "sc_bigint<16>::operator=(const sc_uint_base&)" 
    //   and "sc_bigint<16>::operator=(long long)".
    // ----
    // x=(a,a);
    // cout << "sc_biguint (1) " << x.to_string(SC_BIN) << endl;

    // runtime error: concat yields wrong result
    // returned value is 0000000000001011
    // but should be     1110101110111011
    x=(a.range(7,0), a.range(3,0), a.range(3,0));
    cout << "sc_biguint (2)                 " << x.to_string(SC_BIN) << endl;
  }
  { // sc_bv works fine
    sc_bv<8> a;
    sc_bv<16> x;
    a="11101011";
    
    x=(a,a); 
    cout << "sc_bv      (1) " << x.to_string() << endl;
    
    x=(a.range(7,0), a.range(3,0), a.range(3,0));
    cout << "sc_bv      (2)                 " << x.to_string() << endl;
  }
  { // sc_lv works fine
    sc_lv<8> a;
    sc_lv<16> x;
    a="11101011";
    
    x=(a,a); 
    cout << "sc_lv      (1) " << x.to_string() << endl;
    
    x=(a.range(7,0), a.range(3,0), a.range(3,0));
    cout << "sc_lv      (2)                 " << x.to_string() << endl;
  }
  return 0;
}
