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
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <list>
#include <vector>
using std::cout;
using std::endl;
using std::hex;
using std::dec;
#include "systemc.h"

#if !defined(SC_VECTOR_UTILS_H)
  
int sc_main( int argc, char* argv[] )
{
    cout << endl << "The vector utiliies package is not implemented in this systemc library" 
         << endl << endl;
    return 0;
}

#else

using namespace sc_dt;

void dump( size_t n, const sc_digit* digits )
{
    for ( int digit_i = n-1; digit_i >= 0; --digit_i ) {
        cout << hex << std::setfill('0') << std::setw(8) << digits[digit_i] << " ";
    }
    cout << endl;
}

bool equal( size_t n, const sc_digit* left_p, const sc_digit* right_p )
{
    for ( size_t digit_i = 0; digit_i < n; ++digit_i ) {     
        if ( left_p[digit_i] != right_p[digit_i] ) { return false; }
    }
    return true;
}

void fill( size_t n, sc_digit* to_p, sc_digit value )
{
    for ( size_t digit_i = 0; digit_i < n; ++digit_i ) { to_p[digit_i] = value; }
}

template<typename T>
void fill( size_t n, T& to, sc_digit value )
{
    to = value;
    for ( size_t digit_i = 1; digit_i < n; ++digit_i ) { 
        to = (to << 32) | value;
    }
}

void load( size_t n, sc_digit* from_p, sc_digit* to_p )
{
    for ( size_t digit_i = 0; digit_i < n; ++digit_i ) { to_p[digit_i] = from_p[digit_i]; }
}

void load( size_t n, sc_digit* from_p, sc_unsigned& to )
{
    to = from_p[0]; 
    for ( size_t digit_i = 1; digit_i < n; ++digit_i ) { 
        to = (to << 32) | from_p[digit_i]; 
    }
}


// #define EXTRACT_DEBUG
void extract( size_t bits_n, sc_digit fill_value )
{
    sc_unsigned     expected(bits_n);
    static sc_digit from_digits[5000];
    sc_unsigned     mask(bits_n);
    static sc_digit to_digits[5000];

    cout << endl << "extract from sc_digit(" << bits_n << ") filled with " << hex << fill_value
         << dec << endl;
    size_t  from_digits_n = SC_DIGIT_COUNT(bits_n);
    fill( from_digits_n, from_digits, fill_value );

    for ( size_t low_i = 0; low_i < bits_n; ++low_i ) {
	for ( size_t high_i = low_i; high_i < bits_n; ++high_i ) {
	    size_t from_bits_n = (high_i - low_i) + 1;
	    size_t to_digits_n = SC_DIGIT_COUNT(from_bits_n);
	    fill( to_digits_n, to_digits, 0u );
	    to_digits[to_digits_n-1] = ~0u; 
	    vector_extract( from_digits, to_digits, high_i, low_i );

	    fill( bits_n, mask, ~0u );
	    mask = ~( mask << ( (high_i - low_i) +1 ) );
	    fill( bits_n, expected, fill_value );
	    expected = (expected >> low_i) & mask;
	    const sc_digit* expected_p = expected.get_raw();
	    if ( !equal( to_digits_n, to_digits, expected_p ) ) {
	        cout << endl << "ERROR: extract( " << high_i << "," << low_i << "):" << endl;
		cout << "    to_digits_n " << to_digits_n << endl;
		cout << "    expected    "; dump( to_digits_n, expected_p );
		cout << "    actual      "; dump( to_digits_n, to_digits );
	    }

#	    if defined(EXTRACT_DEBUG)
		cout << endl << dec << "abc(" << high_i << "," << low_i << ") width:" << from_bits_n
		     << " fill:0x" << hex << fill_value << dec << endl;
		dump(to_digits_n, to_digits);
#	    endif

	}
    }
}


int sc_main( int argc, char* argv[] )
{
    extract( 20, ~0u );

    extract( 12*32, ~0u );

    extract( 4*32, 0x66666666u );

    extract( 4*32, 0xaaaaaaaau );

    extract( 4*32, 0x87654321u );

    cout << "Program completed" << endl;
    return 0;
}
#endif // #if !defined(SC_VECTOR_UTILS_H)
