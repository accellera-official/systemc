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

  test01.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test string conversion of sc_[u]int

#include "systemc.h"

void
test_string_conversions()
{
    cout << "*** test_string_conversions" << endl;
    {
	cout << "sc_int" << endl;

        sc_int<8> a = -1;
        sc_int<8> b;
	sc_string s;

	s = a.to_string();
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_BIN );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_BIN_US );
	cout << s << endl;
	// b = s.c_str();
	// assert( b == a );

	s = a.to_string( SC_BIN_SM );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_OCT );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_OCT_US );
	cout << s << endl;
	// b = s.c_str();
	// assert( b == a );

	s = a.to_string( SC_OCT_SM );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_HEX );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_HEX_US );
	cout << s << endl;
	// b = s.c_str();
	// assert( b == a );

	s = a.to_string( SC_HEX_SM );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_DEC );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_CSD );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	sc_int<8> c( a.to_string().c_str() );
	cout << c.to_string() << endl;

	c.range( 7, 0 ) = a.to_string().c_str();
	cout << c.range( 7, 0 ).to_string() << endl;
    }
    {
	cout << "sc_uint" << endl;

        sc_uint<8> a = -1;
        sc_uint<8> b;
	sc_string s;

	s = a.to_string();
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_BIN );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_BIN_US );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_BIN_SM );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_OCT );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_OCT_US );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_OCT_SM );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_HEX );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_HEX_US );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_HEX_SM );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_DEC );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	s = a.to_string( SC_CSD );
	cout << s << endl;
	b = s.c_str();
	assert( b == a );

	sc_uint<8> c( a.to_string().c_str() );
	cout << c.to_string() << endl;

	c.range( 7, 0 ) = a.to_string().c_str();
	cout << c.range( 7, 0 ).to_string() << endl;
    }
}

int
sc_main( int, char*[] )
{
    test_string_conversions();

    return 0;
}
