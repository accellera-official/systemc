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

#define CHECK(ACTUAL,EXPECTED) \
    if ( ACTUAL != EXPECTED ) { \
	cout << "ERROR: " << #ACTUAL << " != " << #EXPECTED " in " << __FILE__ << " at line " \
	     << __LINE__ << endl; \
	cout << "  left       " << left << " width " << left_width << endl; \
	cout << "  right      " << right << " width " << right_width << endl; \
	cout << "  sum        " << sum << endl; \
	cout << "  difference " << difference << endl; \
	assert( difference == right ); \
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

void test_signed( int max_width, int delta_width )
{
    for ( int left_width = max_width; left_width > 1; left_width -= delta_width ) {
	for ( int right_width = max_width; right_width > 1; right_width -= delta_width ) {
	    sc_signed left(left_width);
	    sc_signed right(right_width);

	    for ( int count = 0; count < COUNT_N; ++count ) {
		load_rand(left_width, left);
		load_rand(right_width, right);

		sc_signed sum = left+right;
		sc_signed difference = sum - left;
		CHECK(difference,right)
	    }

	    for ( int count = 0; count < COUNT_N; ++count ) {
		load_rand(left_width, left);
		load_rand(right_width, right);

		sc_signed sum = -left+right;
		sc_signed difference = sum + left;
		CHECK( difference, right )
	    }

	    for ( int count = 0; count < COUNT_N; ++count ) {
		load_rand(left_width, left);
		load_rand(right_width, right);

		sc_signed sum = left-right;
		sc_signed difference = sum - left;
		CHECK( difference, -right ) 

		if ( -left != ( ~left + 1 ) ) {
		    cout << "ERROR: actual != expected in " << __FILE__ << " at line " << __LINE__ 
		         << endl;
		    cout << "  -left      " << -left << endl;
		    cout << "  ~left+1    " << (~left+1) << endl;
		    assert( -left == ( ~left + 1 ) );
		}
	    }

	}
    }
}

void test_mixed( int max_width, int delta_width )
{
    for ( int left_width = max_width; left_width > 1; left_width -= delta_width ) {
	for ( int right_width = max_width; right_width > 1; right_width -= delta_width ) {
	    sc_signed   left(left_width);
	    sc_unsigned right(right_width);
	    
	    for ( int count = 0; count < COUNT_N; ++count ) {
		load_rand(left_width, left);
		load_rand(right_width, right);

		sc_signed sum = left+right;
		sc_signed difference = sum - left;
		CHECK( difference , right ) 
	    }

	    for ( int count = 0; count < COUNT_N; ++count ) {
		load_rand(left_width, left);
		load_rand(right_width, right);

		sc_signed sum = left-right;
		sc_signed difference = sum - left;
		CHECK( difference , -right )
	    }

	    for ( int count = 0; count < COUNT_N; ++count ) {
		load_rand(left_width, left);
		load_rand(right_width, right);

		sc_signed sum = -left+right;
		sc_signed difference = sum + left;
		CHECK( difference , right ) 
	    }

	}
    }
}

void test_unsigned( int max_width, int delta_width )
{
    for ( int left_width = max_width; left_width > 1; left_width -= delta_width ) {
	for ( int right_width = max_width; right_width > 1; right_width -= delta_width ) {
	    sc_unsigned left(left_width);
	    sc_unsigned right(right_width);

	    for ( int count = 0; count < COUNT_N; ++count ) {
		load_rand(left_width, left);
		load_rand(right_width, right);

		sc_signed sum = left+right;
		sc_signed difference = sum - left;
		CHECK( difference , right )
	    }

	    for ( int count = 0; count < COUNT_N; ++count ) {
		load_rand(left_width, left);
		load_rand(right_width, right);

		sc_signed sum = -left+right;
		sc_signed difference = sum + left;
		CHECK( difference , right ) 
	    }

	    for ( int count = 0; count < COUNT_N; ++count ) {
		load_rand(left_width, left);
		load_rand(right_width, right);

		sc_signed sum = left-right;
		sc_signed difference = sum - left;
		CHECK( difference , -right )
	    }

	}
    }
}

template<int W, int D>
class AddSubtract : public AddSubtract<W-D,D>
{
  public:
    AddSubtract() : AddSubtract<W-D,D>() {}
    void test_signed()
    {
        sc_bigint<W>     v_sc_bigint_a;
        sc_bigint<W>     v_sc_bigint_b;
        sc_bigint<W+1>   v_sum;
        sc_bigint<W+1+1> v_difference;

	for ( size_t count = 0; count < COUNT_N; ++count ) {
	    load_rand( W, v_sc_bigint_a );
	    load_rand( W, v_sc_bigint_b );

	    // a + b

	    v_sum = v_sc_bigint_a + v_sc_bigint_b;
	    v_difference = v_sum - v_sc_bigint_b;
	    if ( v_difference != v_sc_bigint_a ) {
		cout << "ERROR: a != difference in " << __FILE__ << " at line " << __LINE__ << endl;
		cout << "  W        " << W << endl;
		cout << "  a        " << v_sc_bigint_a << endl;
		cout << "  b        " << v_sc_bigint_b << endl;
		cout << " product " << v_sum << endl;
		cout << " difference " << v_difference << endl;
		assert( v_difference == v_sc_bigint_a );
	    }

	    if ( -v_sc_bigint_a != ( ~v_sc_bigint_a + 1 ) ) {
		cout << "ERROR: -a != ~a+1 in " << __FILE__ << " at line " << __LINE__ << endl;
		cout << "ERROR: sc_biguint<" << W << "> complement failure:" << endl;
		cout << "  -v_sc_bigint_a   " << -v_sc_bigint_a << endl;
		cout << "  ~v_sc_bigint_a+1 " << (~v_sc_bigint_a+1) << endl;
		assert( -v_sc_bigint_a == ( ~v_sc_bigint_a + 1 ) );
	    }

	    // b + a

	    v_sum = v_sc_bigint_b + v_sc_bigint_a;
	    v_difference = v_sum - v_sc_bigint_a;
	    if ( v_difference != v_sc_bigint_b ) {
		cout << "ERROR: b != difference in " << __FILE__ << " at line " << __LINE__ << endl;
		cout << "  a        " << v_sc_bigint_a << endl;
		cout << "  b        " << v_sc_bigint_b << endl;
		cout << " product " << v_sum << endl;
		cout << " difference " << v_difference << endl;
		assert( v_difference == v_sc_bigint_b );
	    }
	}

	((AddSubtract<W-D,D>*)this)->test_signed();
    }

    void test_mixed()
    {
        sc_bigint<W>       v_sc_bigint_a;
        sc_biguint<W>      v_sc_biguint_b;
        sc_bigint<W+1+1>   v_sum;
        sc_bigint<W+1+1+1> v_difference;

	for ( size_t count = 0; count < COUNT_N; ++count ) {
	    load_rand(W, v_sc_bigint_a);
	    load_rand(W+1, v_sc_biguint_b);

	    // a + b

	    v_sum = v_sc_bigint_a + v_sc_biguint_b;
	    v_difference = v_sum - v_sc_biguint_b;
	    if ( v_difference != v_sc_bigint_a ) {
		cout << "ERROR: a != difference in " << __FILE__ << " at line " << __LINE__ << endl;
		cout << "  W        " << W << endl;
		cout << "  a        " << v_sc_bigint_a << endl;
		cout << "  b        " << v_sc_biguint_b << endl;
		cout << " product " << v_sum << endl;
		cout << " difference " << v_difference << endl;
		assert( v_difference == v_sc_bigint_a );
	    }

	    // b + a

	    v_sum = v_sc_biguint_b + v_sc_bigint_a;
	    v_difference = v_sum - v_sc_bigint_a;
	    if ( v_difference != v_sc_biguint_b ) {
		cout << "ERROR: b != difference in " << __FILE__ << " at line " << __LINE__ << endl;
		cout << "  W         " << W << endl;
		cout << "  a         " << v_sc_bigint_a << endl;
		cout << "  b         " << v_sc_biguint_b << endl;
		cout << " product    " << v_sum << endl;
		cout << " difference " << v_difference << endl;
		assert( v_difference == v_sc_biguint_b );
	    }
	}

	((AddSubtract<W-D,D>*)this)->test_signed();
    }

    void test_unsigned()
    {
        sc_biguint<W>      v_sc_biguint_a;
        sc_biguint<W>      v_sc_biguint_b;
        sc_bigint<W+2+1>   v_sum;
        sc_bigint<W+2+1+1> v_difference;

	for ( size_t count = 0; count < COUNT_N; ++count ) {
	    load_rand(W+1, v_sc_biguint_a);
	    load_rand(W+1, v_sc_biguint_b);

	    // a + b

	    v_sum = v_sc_biguint_a + v_sc_biguint_b;
	    v_difference = v_sum - v_sc_biguint_b;
	    if ( v_difference != v_sc_biguint_a ) {
		cout << "ERROR: a != difference in " << __FILE__ << " at line " << __LINE__ << endl;
		cout << "  W         " << W << endl;
		cout << "  a         " << v_sc_biguint_a << endl;
		cout << "  b         " << v_sc_biguint_b << endl;
		cout << " sum        " << v_sum << endl;
		cout << " difference " << v_difference << endl;
		assert( v_difference == v_sc_biguint_a );
	    }

	    if ( -v_sc_biguint_a != ( ~v_sc_biguint_a + 1 ) ) {
		cout << "ERROR: -a != ~a+1 in " << __FILE__ << " at line " << __LINE__ << endl;
		cout << "  W                 " << W << endl;
		cout << "  -v_sc_biguint_a   " << -v_sc_biguint_a << endl;
		cout << "  ~v_sc_biguint_a+1 " << (~v_sc_biguint_a+1) << endl;
		assert( -v_sc_biguint_a == ( ~v_sc_biguint_a + 1 ) );
	    }

	    // b + a

	    v_sum = v_sc_biguint_b + v_sc_biguint_a;
	    v_difference = v_sum - v_sc_biguint_a;
	    if ( v_difference != v_sc_biguint_b ) {
		cout << "ERROR: b != difference in " << __FILE__ << " at line " << __LINE__ << endl;
		cout << "  W         " << W << endl;
		cout << "  a         " << v_sc_biguint_a << endl;
		cout << "  b         " << v_sc_biguint_b << endl;
		cout << " sum        " << v_sum << endl;
		cout << " difference " << v_difference << endl;
		assert( v_difference == v_sc_biguint_b );
	    }

	}

	((AddSubtract<W-D,D>*)this)->test_signed();
    }
};

template<int D>
class AddSubtract<0,D>
{
  public:
    AddSubtract() {}
    void test_signed() {}
};

int sc_main(int argc, char* argv[])
{
    AddSubtract<128,1>      x;
    AddSubtract<3100,31>    y;

    x.test_signed();
    x.test_mixed();
    x.test_unsigned();
    y.test_signed();
    y.test_mixed();
    y.test_unsigned();

    test_signed(128,1);
    test_mixed(128,1);
    test_unsigned(128,1);

    cout << "Big Addition/Subtraction tests completed" << endl;
    return 0;
}
