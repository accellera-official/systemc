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

  test03.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-03-19

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of sc_length_param and sc_length_context and their use in the datatype
// base classes.

#include "systemc.h"

void
test_sc_length_param()
{
    cout << "\n*** test_sc_length_param ***" << endl;

    sc_length_param a;
    cout << a << endl;

    sc_length_param b( 5 );
    cout << b << endl;

    try {
        sc_length_param c( -1 );
        cout << c << endl;
    } catch( sc_exception x ) {
        cout << "\nException caught" << endl;
        cout << x.what() << endl;
    }

    sc_length_param d( b );
    cout << d << endl;

    sc_dt::sc_without_context foo;
    sc_length_param e( foo );
    cout << e << endl;

    e = b;
    cout << e << endl;

    cout << (e == b) << endl;
    cout << (e != b) << endl;

    cout << e.len() << endl;
    e.len( 42 );
    cout << e.len() << endl;

    cout << e.to_string() << endl;

    e.print();
    cout << endl;
    e.dump();
}

#define TEST_DEFAULT_CTOR(tp)                                                 \
{                                                                             \
    cout << "\n" << #tp << endl;                                              \
                                                                              \
    tp a;                                                                     \
    cout << a.length() << endl;                                               \
                                                                              \
    sc_length_context con1( sc_length_param( 5 ) );                           \
    tp b;                                                                     \
    cout << b.length() << endl;                                               \
                                                                              \
    sc_length_context con2( sc_length_param( 42 ) );                          \
    tp c;                                                                     \
    cout << c.length() << endl;                                               \
                                                                              \
    con2.end();                                                               \
    tp d;                                                                     \
    cout << d.length() << endl;                                               \
                                                                              \
    con1.end();                                                               \
    tp e;                                                                     \
    cout << e.length() << endl;                                               \
}

void
test_default_ctors()
{
    cout << "\n*** test_default_ctors ***" << endl;

    TEST_DEFAULT_CTOR(sc_bv_base);
    TEST_DEFAULT_CTOR(sc_lv_base);

    TEST_DEFAULT_CTOR(sc_int_base);
    TEST_DEFAULT_CTOR(sc_uint_base);
    TEST_DEFAULT_CTOR(sc_signed);
    TEST_DEFAULT_CTOR(sc_unsigned);
}

int
sc_main( int, char*[] )
{
    test_sc_length_param();
    test_default_ctors();

    return 0;
}
