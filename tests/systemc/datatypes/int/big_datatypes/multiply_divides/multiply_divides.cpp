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
	cout << "ERROR: " <<  #ACTUAL << " != " << #EXPECTED << " in " << __FILE__ \
             << " at line " << __LINE__ << endl; \
	cout << "  left     " << left << " width " << left_width << endl; \
	cout << "  right    " << right << " width " << right_width << endl; \
	cout << "  product  " << product << endl; \
	cout << "  quotient " << quotient << endl; \
	assert( ACTUAL == EXPECTED ); \
    }

template<typename T>
inline void load( int bits_n, T& target )
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
		load(left_width, left);
		load(right_width, right);

		left |= 1; // zero is bad!!!
		sc_signed product = left*right;
		sc_signed quotient = product / left;
		CHECK( quotient , right ) 
	    }

	}
    }
}

void test_mixed( int max_width, int delta_width )
{
    for ( int left_width = max_width; left_width > 1; left_width -= delta_width ) {
	for ( int right_width = max_width; right_width > 1; right_width -= delta_width ) {
	    {
		sc_signed left(left_width);
		sc_unsigned right(right_width);

		for ( int count = 0; count < COUNT_N; ++count ) {
		    load(left_width, left);
		    load(right_width+1, right);

		    left |= 1; // zero is bad!!!
		    sc_signed product = left*right;
		    sc_signed quotient = product / left;
		    CHECK( quotient , right ) 
		}
	    }

	    {
		sc_unsigned left(left_width);
		sc_signed right(right_width);

		for ( int count = 0; count < COUNT_N; ++count ) {
		    load(left_width+1, left);
		    load(right_width, right);

		    left |= 1; // zero is bad!!!
		    sc_signed product = left*right;
		    sc_signed quotient = product / left;
		    CHECK( quotient , right ) 
		}
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
		load(left_width+1, left);
		load(right_width+1, right);

		left |= 1; // zero is bad!!!
		sc_signed product = left*right;
		sc_signed quotient = product / left;
		CHECK( quotient , right ) 
	    }

	}
    }
}

template<int W, int D>
class MultiplyDivide : public MultiplyDivide<W-D,D>
{
  public:
    MultiplyDivide() : MultiplyDivide<W-D,D>() {}
    void test_signed_signed()
    {
        sc_bigint<W>   v_sc_bigint_a;
        sc_bigint<W>   v_sc_bigint_b;
        sc_bigint<W+W> v_product;
        sc_bigint<W+W> v_quotient;

	for ( size_t count = 0; count < COUNT_N; ++count ) {
	    v_sc_bigint_a = rng.rand();
	    v_sc_bigint_b = rng.rand();
	    for ( int digit_i = 1; digit_i < DIV_CEIL(W); ++digit_i ) {
		v_sc_bigint_a = (v_sc_bigint_a << 32) + rng.rand();
		v_sc_bigint_b = (v_sc_bigint_b << 32) + rng.rand();
	    }

	    // Make sure no divide by zero:

	    v_sc_bigint_a = v_sc_bigint_a | 1;
	    v_sc_bigint_b = v_sc_bigint_b | 1;

	    // a * b

	    v_product = v_sc_bigint_a * v_sc_bigint_b;
	    v_quotient = v_product / v_sc_bigint_b;
	    if ( v_quotient != v_sc_bigint_a ) {
		cout << "ERROR: quotient != a in " << __FILE__ << " at line " 
		     << __LINE__ << endl;
		cout << "  width    " << W << endl;
		cout << "  a        " << v_sc_bigint_a << endl;
		cout << "  b        " << v_sc_bigint_b << endl;
		cout << " product " << v_product << endl;
		cout << " quotient " << v_quotient << endl;
		assert( v_quotient == v_sc_bigint_a );
	    }

	    // b * a

	    v_product = v_sc_bigint_b * v_sc_bigint_a;
	    v_quotient = v_product / v_sc_bigint_a;
	    if ( v_quotient != v_sc_bigint_b ) {
		cout << "ERROR: quotient != b in " << __FILE__ << " at line " 
		     << __LINE__ << endl;
		cout << "  width    " << W << endl;
		cout << "  a        " << v_sc_bigint_a << endl;
		cout << "  b        " << v_sc_bigint_b << endl;
		cout << " product " << v_product << endl;
		cout << " quotient " << v_quotient << endl;
		assert( v_quotient == v_sc_bigint_b );
	    }
	}

	((MultiplyDivide<W-D,D>*)this)->test_signed_signed();
    }

    void test_unsigned_unsigned()
    {
        sc_biguint<W>   v_sc_biguint_a;
        sc_biguint<W>   v_sc_biguint_b;
        sc_biguint<W+W> v_product;
        sc_biguint<W+W> v_quotient;

	for ( size_t count = 0; count < COUNT_N; ++count ) {
	    v_sc_biguint_a = rng.rand();
	    v_sc_biguint_b = rng.rand();
	    for ( int digit_i = 1; digit_i < DIV_CEIL(W); ++digit_i ) {
		v_sc_biguint_a = (v_sc_biguint_a << 32) + rng.rand();
		v_sc_biguint_b = (v_sc_biguint_b << 32) + rng.rand();
	    }

	    // Make sure no divide by zero:

	    v_sc_biguint_a = v_sc_biguint_a | 1;
	    v_sc_biguint_b = v_sc_biguint_b | 1;

	    // a * b
	    v_product = v_sc_biguint_a * v_sc_biguint_b;
	    v_quotient = v_product / v_sc_biguint_b;
	    if ( v_quotient != v_sc_biguint_a ) {
		cout << "ERROR: quotient != a in " << __FILE__ << " at line " 
		     << __LINE__ << endl;
		cout << "  width    " << W << endl;
		cout << "  a        " << v_sc_biguint_a << endl;
		cout << "  b        " << v_sc_biguint_b << endl;
		cout << " product " << v_product << endl;
		cout << " quotient " << v_quotient << endl;
		assert( v_quotient == v_sc_biguint_a );
	    }

	    // b * a

	    v_product = v_sc_biguint_b * v_sc_biguint_a;
	    v_quotient = v_product / v_sc_biguint_a;
	    if ( v_quotient != v_sc_biguint_b ) {
		cout << "ERROR: quotient != b in " << __FILE__ << " at line " 
		     << __LINE__ << endl;
		cout << "  width    " << W << endl;
		cout << "  a        " << v_sc_biguint_a << endl;
		cout << "  b        " << v_sc_biguint_b << endl;
		cout << " product " << v_product << endl;
		cout << " quotient " << v_quotient << endl;
		assert( v_quotient == v_sc_biguint_b );
	    }
	}

	((MultiplyDivide<W-D,D>*)this)->test_signed_signed();
    }
};

template<int D>
class MultiplyDivide<0,D>
{
  public:
    MultiplyDivide() {}
    void test_signed_signed() {}
};

template<typename X, typename Y>
void test_trim_pair( X& x, Y& y )
{
    x = ~0u;
    x = (x << 32) | ~0u;
    x = (x << 32) | 8;
    x = (x << 32) | 0;
    y = 2;
    cout << hex << (x*y) << " = " << x << " * " << y << endl;

    x = ~0u;
    x = (x << 32) | ~0u;
    x = (x << 32) | 0x80000000;
    x = (x << 32) | 16;
    y = 2;
    cout << hex << (x*y) << " = " << x << " * " << y << endl;

    x = ~0u;
    x = (x << 32) | ~0u;
    x = (x << 32) | 0x80000000;
    x = (x << 32) | 16;
    y = -2;
    cout << hex << (x*y) << " = " << x << " * " << y << endl;

    x = 0u;
    x = (x << 32) | 0u;
    x = (x << 32) | 0x80000000;
    x = (x << 32) | 16;
    y = -2;
    cout << hex << (x*y) << " = " << x << " * " << y << endl;

    x = 0u;
    x = (x << 32) | 0u;
    x = (x << 32) | 0x80000000;
    x = (x << 32) | 16;
    y = 8;
    cout << hex << (x*y) << " = " << x << " * " << y << endl;
}

void test_trim_pairs()
{
    sc_bigint<128>  bigint_128_a;
    sc_bigint<128>  bigint_128_b;

    sc_biguint<128> biguint_128_a;
    sc_biguint<128> biguint_128_b;

    sc_signed       signed_128_a(128);
    sc_signed       signed_128_b(128);

    sc_unsigned     unsigned_128_a(128);
    sc_unsigned     unsigned_128_b(128);

    test_trim_pair( bigint_128_a, bigint_128_b);
    cout << endl;

    test_trim_pair( unsigned_128_a, bigint_128_b);
    cout << endl;

    test_trim_pair( bigint_128_a, unsigned_128_b);
    cout << endl;

    test_trim_pair( unsigned_128_a, unsigned_128_b);
    cout << endl;

    test_trim_pair( signed_128_a, signed_128_b);
    cout << endl;

    test_trim_pair( unsigned_128_a, signed_128_b);
    cout << endl;

    test_trim_pair( signed_128_a, unsigned_128_b);
    cout << endl;

    test_trim_pair( unsigned_128_a, unsigned_128_b);
    cout << endl;
}

int sc_main(int argc, char* argv[])
{
    MultiplyDivide<128,1> x;
    MultiplyDivide<3100,31> y;

    x.test_signed_signed();
    x.test_unsigned_unsigned();
    y.test_signed_signed();
    y.test_unsigned_unsigned();

    test_signed(128,1);
    test_mixed(128,1);
    test_unsigned(128,1);

    test_trim_pairs();

    cout << "Big multiply/divide tests completed" << endl;
    return 0;
}
