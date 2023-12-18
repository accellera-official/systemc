/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/
#include "systemc.h"

#include "isaac.h"

#if defined(FULL_RANGE)
#   define COUNT_N 2000
#else
#   define COUNT_N 200
#endif

QTIsaac<8> rng;         // Platform independent random number generator.

template<typename T>
inline void load_rand( int bits_n, T& target )
{
    int target_n = DIV_CEIL(bits_n);

    target = rng.rand();
    for ( int target_i = 1; target_i < target_n; ++target_i ) {
        target = (target << 32) + rng.rand();
    }
}

struct SignedComponent
{
    SignedComponent( int width ) : 
        m_digits_n( DIV_CEIL(width) ), m_target(width), m_width(width)
    {
        m_target = rng.rand();
	for ( int digit_i = 0; digit_i < m_digits_n; ++digit_i ) { 
	    m_target = (m_target << 32) | rng.rand(); 
	}
    }
    
    operator const sc_signed& () const { return m_target; }
    
    int              m_digits_n;
    sc_signed        m_target;
    int              m_width;
};

struct UnsignedComponent
{
    UnsignedComponent( int width ) : 
        m_digits_n( DIV_CEIL(width+1) ), m_target(width), m_width(width)
    {
        m_target = rng.rand();
	for ( int digit_i = 0; digit_i < m_digits_n; ++digit_i ) { 
	    m_target = (m_target << 32) | rng.rand(); 
	}
    }
    
    UnsignedComponent( const SignedComponent& other ) : 
        m_digits_n( DIV_CEIL(other.m_width+1) ), m_target(other.m_width), m_width(other.m_width)
    {
        m_target = other.m_target;
    }

    operator const sc_unsigned& () const { return m_target; }

    int                m_digits_n;
    sc_unsigned        m_target;
    int                m_width;
};



void test_concat( size_t widths_n, int* widths )
{
    int total_width = 0;
    for ( size_t width_i = 0; width_i < widths_n; ++width_i ) { total_width += widths[width_i]; }

    sc_signed                      signed_actual(total_width);
    std::vector<SignedComponent>   signed_components;
    sc_signed                      signed_expected(total_width);

    sc_unsigned                    unsigned_actual(total_width);
    std::vector<UnsignedComponent> unsigned_components;
    sc_unsigned                    unsigned_expected(total_width);

    // Generate signed components and unsigned components to be used to generate concatenations:

    for ( size_t width_i = 0; width_i < widths_n; ++width_i ) { 
        SignedComponent x( widths[width_i] );
	signed_components.push_back(x);
    }

    for ( size_t width_i = 0; width_i < widths_n; ++width_i ) { 
        UnsignedComponent x(signed_components[width_i]); 
	unsigned_components.push_back(x);
    }

    // Test signed concatenation target from unsigned components:

    signed_actual = unsigned_components[0];
    signed_expected = unsigned_components[0];
    for ( size_t width_i = 1; width_i < widths_n; ++width_i ) { 
        signed_actual = (signed_actual, signed_components[width_i]);
        signed_expected = (signed_expected << widths[width_i]) | unsigned_components[width_i];
    }

    if ( signed_actual != signed_expected ) {
	cout << "ERROR: actual != expected in " << __FILE__ << " at line " << __LINE__ << endl;
        cout << "  actual   " << hex << signed_actual << dec  << endl;
        cout << "  expected " << hex << signed_expected << dec  << endl;
        assert( signed_actual == signed_expected );
    }

    // Test unsigned concatenation target from unsigned components:

    unsigned_actual = unsigned_components[0];
    unsigned_expected = unsigned_components[0];
    for ( size_t width_i = 1; width_i < widths_n; ++width_i ) { 
        unsigned_actual = (unsigned_actual, unsigned_components[width_i]);
        unsigned_expected = (unsigned_expected << widths[width_i]) | unsigned_components[width_i];
    }

    if ( unsigned_actual != unsigned_expected ) {
        cout << "ERROR: actual != expected in " << __FILE__ << " at line " << __LINE__ << endl;
        cout << "  actual   " << hex << unsigned_actual << dec  << endl;
        cout << "  expected " << hex << unsigned_expected << dec  << endl;
        assert( unsigned_actual == unsigned_expected );
    }

    // Test a mixture to an unsigned concatenation target:

    unsigned_actual = unsigned_components[0];
    unsigned_expected = unsigned_components[0];
    for ( size_t width_i = 1; width_i < widths_n; ++width_i ) { 
	if ( width_i & 1 ) {
	    unsigned_actual = (unsigned_actual, signed_components[width_i]);
	}
	else {
	    unsigned_actual = (unsigned_actual, unsigned_components[width_i]);
	}
        unsigned_expected = (unsigned_expected << widths[width_i]) | unsigned_components[width_i];
    }

    if ( unsigned_actual != unsigned_expected ) {
        cout << "ERROR: actual != expected in " << __FILE__ << " at line " << __LINE__ << endl;
        cout << "  actual   " << hex << unsigned_actual << dec  << endl;
        cout << "  expected " << hex << unsigned_expected << dec  << endl;
        assert( unsigned_actual == unsigned_expected );
    }

    // Test unsigned concatenation target from part selections:

    {
	int width_i = widths_n-1;
	int high = total_width;
	int low = high - widths[width_i];
	unsigned_actual = unsigned_expected( high-1, low );
	for ( width_i = width_i-1; width_i >= 0; --width_i ) { 
	    high = low;
	    low = high - widths[width_i];
	    unsigned_actual = ( unsigned_actual,unsigned_expected( high-1, low ) );
	}
	if ( unsigned_actual != unsigned_expected ) {
	    cout << "ERROR: actual != expected in " << __FILE__ << " at line " << __LINE__ << endl;
	    cout << "  actual   " << hex << unsigned_actual << dec  << endl;
	    cout << "  expected " << hex << unsigned_expected << dec  << endl;
	    assert( unsigned_actual == unsigned_expected );
	}
    }
}

int sc_main(int argc, char* argv[])
{

    int test1[] =
    {
        5,  7, 2, 71, 80, 6
    };

    int test2[] =
    {
        400, 171, 900
    };

    int test3[] =
    {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 
    };

    int test4[] =
    {
        1280, 171, 900
    };

    int test5[] =
    {
        1100, 100, 79
    };

    test_concat( sizeof(test1)/sizeof(int), test1 );
    test_concat( sizeof(test2)/sizeof(int), test2 );
    test_concat( sizeof(test3)/sizeof(int), test3 );
    test_concat( sizeof(test4)/sizeof(int), test4 );
    test_concat( sizeof(test5)/sizeof(int), test5 );

    cout << "Concatenation test complete" << endl;
    return 0;
}
