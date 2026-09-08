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

#define COUNT 100

QTIsaac<8> rng;         // Platform independent random number generator.

#define DUMP( WHAT ) \
{ \
    cout << hex << WHAT << " " << #WHAT << " ( " << dec << WHAT.length() << " ) " << endl; \
}

#define COMPARE_UINT64_BV( LEFT, RIGHT ) \
    if ( ( LEFT != RIGHT ) ) { \
	cout << endl << "ERROR DETECTED @ " << __FILE__ << ":" << __LINE__ << ":" << endl; \
	DUMP(v_sc_unsigned); \
	DUMP(v_sc_bv_base); \
	cout << hex << v_uint64 << dec << " v_uint64 " << endl; \
	cout << endl; \
	DUMP(LEFT); \
	DUMP(RIGHT); \
	cout << endl; \
    }

#define COMPARE_UINT64_LV( LEFT, RIGHT ) \
    if ( ( LEFT != RIGHT ) ) { \
	cout << endl << "ERROR DETECTED @ " << __FILE__ << ":" << __LINE__ << ":" << endl; \
	DUMP(v_sc_unsigned); \
	DUMP(v_sc_lv_base); \
	cout << hex << v_uint64 << dec << " v_uint64 " << endl; \
	cout << endl; \
	DUMP(LEFT); \
	DUMP(RIGHT); \
	cout << endl; \
    }

#define COMPARE_UNSIGNED_BV( LEFT, RIGHT ) \
    if ( ( LEFT != RIGHT ) ) { \
	cout << endl << "ERROR DETECTED @ " << __FILE__ << ":" << __LINE__ << ":" << endl; \
	DUMP(left_sc_unsigned); \
	DUMP(right_sc_unsigned); \
	DUMP(left_sc_bv_base); \
	DUMP(right_sc_bv_base); \
	cout << endl; \
	DUMP(LEFT); \
	DUMP(RIGHT); \
	cout << endl; \
    }

#define COMPARE_UNSIGNED_LV( LEFT, RIGHT ) \
    if ( ( LEFT != RIGHT ) ) { \
	cout << endl << "ERROR DETECTED @ " << __FILE__ << ":" << __LINE__ << ":" << endl; \
	DUMP(left_sc_unsigned); \
	DUMP(right_sc_unsigned); \
	DUMP(left_sc_lv_base); \
	DUMP(right_sc_lv_base); \
	cout << endl; \
	DUMP(LEFT); \
	DUMP(RIGHT); \
	cout << endl; \
    }

template<typename T>
inline void load_rand( int bits_n, T& target )
{
    int target_n = DIV_CEIL(bits_n);

    target = rng.rand();
    for ( int target_i = 1; target_i < target_n; ++target_i ) {
        target = (target << 32) + rng.rand();
    }
}

void test_and( const sc_unsigned& left_sc_unsigned, const sc_unsigned& right_sc_unsigned,
              const sc_lv_base& left_sc_lv_base, const sc_lv_base& right_sc_lv_base )
{
    sc_unsigned left_unsigned_or_right_unsigned( left_sc_unsigned & right_sc_unsigned );
    sc_unsigned left_unsigned_or_right_lv( left_sc_unsigned & right_sc_lv_base );
    sc_unsigned left_lv_or_right_unsigned ( left_sc_lv_base & right_sc_unsigned );
    sc_unsigned left_lv_or_right_lv( left_sc_lv_base & right_sc_lv_base );
    
    COMPARE_UNSIGNED_LV( left_unsigned_or_right_unsigned, left_unsigned_or_right_lv )
    COMPARE_UNSIGNED_LV( left_unsigned_or_right_unsigned, left_lv_or_right_unsigned )
    COMPARE_UNSIGNED_LV( left_unsigned_or_right_unsigned, left_lv_or_right_lv )
}

void test_and( const sc_unsigned& left_sc_unsigned, const sc_unsigned& right_sc_unsigned,
              const sc_bv_base& left_sc_bv_base, const sc_bv_base& right_sc_bv_base )
{
    sc_unsigned left_unsigned_or_right_unsigned( left_sc_unsigned & right_sc_unsigned );
    sc_unsigned left_unsigned_or_right_bv( left_sc_unsigned & right_sc_bv_base );
    sc_unsigned left_bv_or_right_unsigned ( left_sc_bv_base & right_sc_unsigned );
    sc_unsigned left_bv_or_right_bv( left_sc_bv_base & right_sc_bv_base );
    
    COMPARE_UNSIGNED_BV( left_unsigned_or_right_unsigned, left_unsigned_or_right_bv )
    COMPARE_UNSIGNED_BV( left_unsigned_or_right_unsigned, left_bv_or_right_unsigned )
    COMPARE_UNSIGNED_BV( left_unsigned_or_right_unsigned, left_bv_or_right_bv )
}

void test_or( const sc_unsigned& left_sc_unsigned, const sc_unsigned& right_sc_unsigned,
              const sc_lv_base& left_sc_lv_base, const sc_lv_base& right_sc_lv_base )
{
    sc_unsigned left_unsigned_or_right_unsigned( left_sc_unsigned | right_sc_unsigned );
    sc_unsigned left_unsigned_or_right_lv( left_sc_unsigned | right_sc_lv_base );
    sc_unsigned left_lv_or_right_unsigned ( left_sc_lv_base | right_sc_unsigned );
    sc_unsigned left_lv_or_right_lv( left_sc_lv_base | right_sc_lv_base );
    

    COMPARE_UNSIGNED_LV( left_unsigned_or_right_unsigned, left_unsigned_or_right_lv )
    COMPARE_UNSIGNED_LV( left_unsigned_or_right_unsigned, left_lv_or_right_unsigned )
    COMPARE_UNSIGNED_LV( left_unsigned_or_right_unsigned, left_lv_or_right_lv )
}

void test_or( const sc_unsigned& left_sc_unsigned, const sc_unsigned& right_sc_unsigned,
              const sc_bv_base& left_sc_bv_base, const sc_bv_base& right_sc_bv_base )
{
    sc_unsigned left_unsigned_or_right_unsigned( left_sc_unsigned | right_sc_unsigned );
    sc_unsigned left_unsigned_or_right_bv( left_sc_unsigned | right_sc_bv_base );
    sc_unsigned left_bv_or_right_unsigned ( left_sc_bv_base | right_sc_unsigned );
    sc_unsigned left_bv_or_right_bv( left_sc_bv_base | right_sc_bv_base );
    

    COMPARE_UNSIGNED_BV( left_unsigned_or_right_unsigned, left_unsigned_or_right_bv )
    COMPARE_UNSIGNED_BV( left_unsigned_or_right_unsigned, left_bv_or_right_unsigned )
    COMPARE_UNSIGNED_BV( left_unsigned_or_right_unsigned, left_bv_or_right_bv )
}

void test_xor( const sc_unsigned& left_sc_unsigned, const sc_unsigned& right_sc_unsigned,
              const sc_lv_base& left_sc_lv_base, const sc_lv_base& right_sc_lv_base )
{
    sc_unsigned left_unsigned_or_right_unsigned( left_sc_unsigned ^ right_sc_unsigned );
    sc_unsigned left_unsigned_or_right_lv( left_sc_unsigned ^ right_sc_lv_base );
    sc_unsigned left_lv_or_right_unsigned ( left_sc_lv_base ^ right_sc_unsigned );
    sc_unsigned left_lv_or_right_lv( left_sc_lv_base ^ right_sc_lv_base );
    
    COMPARE_UNSIGNED_LV( left_unsigned_or_right_unsigned, left_unsigned_or_right_lv )
    COMPARE_UNSIGNED_LV( left_unsigned_or_right_unsigned, left_lv_or_right_unsigned )
    COMPARE_UNSIGNED_LV( left_unsigned_or_right_unsigned, left_lv_or_right_lv )
}

void test_xor( const sc_unsigned& left_sc_unsigned, const sc_unsigned& right_sc_unsigned,
              const sc_bv_base& left_sc_bv_base, const sc_bv_base& right_sc_bv_base )
{
    sc_unsigned left_unsigned_or_right_unsigned( left_sc_unsigned ^ right_sc_unsigned );
    sc_unsigned left_unsigned_or_right_bv( left_sc_unsigned ^ right_sc_bv_base );
    sc_unsigned left_bv_or_right_unsigned ( left_sc_bv_base ^ right_sc_unsigned );
    sc_unsigned left_bv_or_right_bv( left_sc_bv_base ^ right_sc_bv_base );
    
    COMPARE_UNSIGNED_BV( left_unsigned_or_right_unsigned, left_unsigned_or_right_bv )
    COMPARE_UNSIGNED_BV( left_unsigned_or_right_unsigned, left_bv_or_right_unsigned )
    COMPARE_UNSIGNED_BV( left_unsigned_or_right_unsigned, left_bv_or_right_bv )
}

void test1()
{
    for ( int left_i = 1; left_i < 128; ++left_i ) {
	for ( int right_i = left_i; right_i < 128; ++right_i ) {
	    sc_unsigned left_sc_unsigned(left_i);
	    sc_unsigned right_sc_unsigned(right_i);
	    sc_bv_base  left_sc_bv_base(left_i);
	    sc_bv_base  right_sc_bv_base(right_i);
	    sc_lv_base  left_sc_lv_base(left_i);
	    sc_lv_base  right_sc_lv_base(right_i);

	    for ( size_t count_i = 0; count_i < COUNT; ++count_i ) {
		load_rand( left_i, left_sc_unsigned );
		load_rand( right_i, right_sc_unsigned );

		left_sc_lv_base = left_sc_unsigned;
		right_sc_lv_base = right_sc_unsigned;
		test_and( left_sc_unsigned, right_sc_unsigned, left_sc_lv_base, right_sc_lv_base );
		test_or( left_sc_unsigned, right_sc_unsigned, left_sc_lv_base, right_sc_lv_base );
		test_xor( left_sc_unsigned, right_sc_unsigned, left_sc_lv_base, right_sc_lv_base );

		left_sc_bv_base = left_sc_unsigned;
		right_sc_bv_base = right_sc_unsigned;
		test_and( left_sc_unsigned, right_sc_unsigned, left_sc_bv_base, right_sc_bv_base );
		test_or( left_sc_unsigned, right_sc_unsigned, left_sc_bv_base, right_sc_bv_base );
		test_xor( left_sc_unsigned, right_sc_unsigned, left_sc_bv_base, right_sc_bv_base );
	    }
        }
    }
}

void test2()
{
    for ( int width_i = 1; width_i < 128; ++width_i ) {
        sc_bv_base  v_sc_bv_base(width_i);
        sc_lv_base  v_sc_lv_base(width_i);
        sc_unsigned v_sc_unsigned(width_i);
	uint64      v_uint64;

	for ( size_t count_i = 0; count_i < COUNT; ++count_i ) {
	    load_rand( width_i, v_sc_unsigned );
	    v_sc_lv_base = v_sc_unsigned;
	    v_sc_bv_base = v_sc_unsigned;
	    v_uint64 = rng.rand();
	    v_uint64 = (v_uint64 << 32) | rng.rand();

	    // Test &

	    sc_unsigned v_uint64_and_sc_unsigned( v_uint64 & v_sc_unsigned );
	    sc_unsigned v_uint64_and_sc_bv_base( v_uint64 & v_sc_bv_base );
	    sc_unsigned v_uint64_and_sc_lv_base( v_uint64 & v_sc_lv_base ); 
	    COMPARE_UINT64_BV( v_uint64_and_sc_unsigned, v_uint64_and_sc_bv_base ) 
	    COMPARE_UINT64_LV( v_uint64_and_sc_unsigned, v_uint64_and_sc_lv_base ) 

	    sc_unsigned v_sc_unsigned_and_uint64( v_sc_unsigned & v_uint64 );
	    sc_unsigned v_sc_bv_base_and_uint64( v_sc_bv_base & v_uint64 );
	    sc_unsigned v_sc_lv_base_and_uint64( v_sc_lv_base & v_uint64 );
	    COMPARE_UINT64_BV( v_sc_unsigned_and_uint64, v_sc_bv_base_and_uint64 ) 
	    COMPARE_UINT64_LV( v_sc_unsigned_and_uint64, v_sc_lv_base_and_uint64 ) 

	    // Test |

	    sc_unsigned v_uint64_or_sc_unsigned( v_uint64 | v_sc_unsigned );
	    sc_unsigned v_uint64_or_sc_bv_base( v_uint64 | v_sc_bv_base );
	    sc_unsigned v_uint64_or_sc_lv_base( v_uint64 | v_sc_lv_base ); 
	    COMPARE_UINT64_BV( v_uint64_or_sc_unsigned, v_uint64_or_sc_bv_base ) 
	    COMPARE_UINT64_LV( v_uint64_or_sc_unsigned, v_uint64_or_sc_lv_base ) 

	    sc_unsigned v_sc_unsigned_or_uint64( v_sc_unsigned | v_uint64 );
	    sc_unsigned v_sc_bv_base_or_uint64( v_sc_bv_base | v_uint64 );
	    sc_unsigned v_sc_lv_base_or_uint64( v_sc_lv_base | v_uint64 );
	    COMPARE_UINT64_BV( v_sc_unsigned_or_uint64, v_sc_bv_base_or_uint64 ) 
	    COMPARE_UINT64_LV( v_sc_unsigned_or_uint64, v_sc_lv_base_or_uint64 ) 

	    // Test ^

	    sc_unsigned v_uint64_xor_sc_unsigned( v_uint64 ^ v_sc_unsigned );
	    sc_unsigned v_uint64_xor_sc_bv_base( v_uint64 ^ v_sc_bv_base );
	    sc_unsigned v_uint64_xor_sc_lv_base( v_uint64 ^ v_sc_lv_base ); 
	    COMPARE_UINT64_BV( v_uint64_xor_sc_unsigned, v_uint64_xor_sc_bv_base ) 
	    COMPARE_UINT64_LV( v_uint64_xor_sc_unsigned, v_uint64_xor_sc_lv_base ) 

	    sc_unsigned v_sc_unsigned_xor_uint64( v_sc_unsigned ^ v_uint64 );
	    sc_unsigned v_sc_bv_base_xor_uint64( v_sc_bv_base ^ v_uint64 );
	    sc_unsigned v_sc_lv_base_xor_uint64( v_sc_lv_base ^ v_uint64 );
	    COMPARE_UINT64_BV( v_sc_unsigned_xor_uint64, v_sc_bv_base_xor_uint64 ) 
	    COMPARE_UINT64_LV( v_sc_unsigned_xor_uint64, v_sc_lv_base_xor_uint64 ) 
        }
    }
	
}

int sc_main(int argc, char* argv[])
{
    cout << std::numeric_limits<sc_digit>::digits << endl;
    // test1();
    // test2();

    cout << "Bit logic operators test completed" << endl;
    return 0;
}
