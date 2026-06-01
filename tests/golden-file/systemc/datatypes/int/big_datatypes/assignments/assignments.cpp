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

#define ASSIGN(to, from, value) \
{ \
    from = value; \
    to = from; \
    if ( to != from ) { \
        cout << "Error at width " << W << endl; \
        cout << "    " << #to << " = " << hex << to << dec << endl; \
        cout << "    " << #from << " = " << hex << from << dec << endl; \
	sc_assert( to == from ); \
    } \
}

#if defined(FULL_RANGE)
#   define COUNT_N 2000
#else
#   define COUNT_N 200
#endif

QTIsaac<8> rng;         // Platform independent random number generator.

template<int W, int D>
class BigAssignments : public BigAssignments<W-D, D>
{
  public:
    BigAssignments() : BigAssignments<W-D,D>() {}
    void test( const sc_signed& value )
    {
        sc_bigint<W>  v_sc_bigint;
        sc_biguint<W> v_sc_biguint;
	sc_signed     v_sc_signed(W);
	sc_unsigned   v_sc_unsigned(W);

        ASSIGN(v_sc_bigint, v_sc_signed, value);

        ASSIGN(v_sc_signed, v_sc_bigint, value);

        ASSIGN(v_sc_biguint, v_sc_unsigned, value);

        ASSIGN(v_sc_unsigned, v_sc_biguint, value);
        
	((BigAssignments<W-D,D>*)this)->test(value);
    }
    void test( const sc_unsigned& value )
    {
        sc_bigint<W>  v_sc_bigint;
        sc_biguint<W> v_sc_biguint;
	sc_signed     v_sc_signed(W);
	sc_unsigned   v_sc_unsigned(W);

        ASSIGN(v_sc_bigint, v_sc_signed, value);

        ASSIGN(v_sc_signed, v_sc_bigint, value);

        ASSIGN(v_sc_biguint, v_sc_unsigned, value);

        ASSIGN(v_sc_unsigned, v_sc_biguint, value);
        
	((BigAssignments<W-D,D>*)this)->test(value);
    }
};

template<int D>
class BigAssignments<0,D>
{
  public:
    BigAssignments() {}
    void test( const sc_unsigned& value ) { }
};

template<int W>
class LittleAssignments : public LittleAssignments<W-1>
{
  public:
    LittleAssignments() : LittleAssignments<W-1>() {}
    void test( uint64 value )
    {
        sc_bigint<W>  v_sc_bigint;
        sc_biguint<W> v_sc_biguint;
        sc_int<W>     v_sc_int;
        sc_uint<W>    v_sc_uint;
	sc_signed     v_sc_signed(W);
	sc_unsigned   v_sc_unsigned(W);

        ASSIGN(v_sc_bigint, v_sc_int, value);
        ASSIGN(v_sc_bigint, v_sc_signed, value);

        ASSIGN(v_sc_int, v_sc_bigint, value);
        ASSIGN(v_sc_int, v_sc_signed, value);

        ASSIGN(v_sc_signed, v_sc_bigint, value);
        ASSIGN(v_sc_signed, v_sc_int, value);

        ASSIGN(v_sc_biguint, v_sc_uint, value);
        ASSIGN(v_sc_biguint, v_sc_unsigned, value);

        ASSIGN(v_sc_uint, v_sc_biguint, value);
        ASSIGN(v_sc_uint, v_sc_unsigned, value);

        ASSIGN(v_sc_unsigned, v_sc_biguint, value);
        ASSIGN(v_sc_unsigned, v_sc_uint, value);
        
	((LittleAssignments<W-1>*)this)->test(value);
    }
};

template<>
class LittleAssignments<1>
{
  public:
    LittleAssignments() {}
    void test( uint64 value ) { }
};

template<typename T>
void load(T& target, int width, unsigned int value )
{
    target = value; 
    for ( int load_i = 32; load_i < width; load_i+=32 ) {
       target = (target << 32) | value;
    }
}

template<typename T>
void load_random(T& target, int width )
{
    target = rng.rand(); 
    for ( int load_i = 32; load_i < width; load_i+=32 ) {
       target = (target << 32) | rng.rand();
    }
}

int sc_main(int argc, char* argv[])
{
    BigAssignments<128,1>    big;
    BigAssignments<1500,100> big_big;
    LittleAssignments<64>    little;

    uint64 target;
    for ( int shift = 0; shift < 64; ++shift ) {
        target = 0xffffffffffffffffull << shift;
	little.test( target );
        target = 0xaaaaaaaaaaaaaaaaull << shift;
	little.test( target );
        target = 0x9999999999999999ull << shift;
	little.test( target );
        target = 0x6666666666666666ull << shift;
	little.test( target );
        target = 0x5555555555555555ull << shift;
	little.test( target );

        target = 0xffffffffffffffffull >> shift;
	little.test( target );
        target = 0xaaaaaaaaaaaaaaaaull >> shift;
	little.test( target );
        target = 0x9999999999999999ull >> shift;
	little.test( target );
        target = 0x6666666666666666ull >> shift;
	little.test( target );
        target = 0x5555555555555555ull >> shift;
	little.test( target );

	for ( int count_i = 0; count_i < COUNT_N; ++count_i ) {
	    load_random( target, 64-shift ); 
	    little.test(target);
	}
    }

    for ( int width = 2; width < 129; ++width ) {
        sc_signed   v_signed(width);
        sc_unsigned v_unsigned(width);
	load(v_signed, width, 0xffffffffu);

	big.test(v_signed);
	load(v_signed, width, 0xaaaaaaaau);
	big.test(v_signed);
	big.test(v_signed);
	load(v_signed, width, 0x99999999u);
	big.test(v_signed);
	load(v_signed, width, 0x66666666u);
	big.test(v_signed);
	load(v_signed, width, 0x55555555u);
	big.test(v_signed);

	load(v_unsigned, width, 0xffffffffu);
	big.test(v_unsigned);
	load(v_unsigned, width, 0xaaaaaaaau);
	big.test(v_unsigned);
	load(v_unsigned, width, 0x99999999u);
	big.test(v_unsigned);
	load(v_unsigned, width, 0x66666666u);
	big.test(v_unsigned);
	load(v_unsigned, width, 0x55555555u);
	big.test(v_unsigned);
    }

    for ( int width = 100; width < 1501; width+=100 ) {
        sc_signed   v_signed(width);
        sc_unsigned v_unsigned(width);

	// Test signed assignments:

	load(v_signed, width, 0xffffffffu);
	big_big.test(v_signed);
	load(v_signed, width, 0xaaaaaaaau);
	big_big.test(v_signed);
	load(v_signed, width, 0x99999999u);
	big_big.test(v_signed);
	load(v_signed, width, 0x66666666u);
	big_big.test(v_signed);
	load(v_signed, width, 0x55555555u);
	big_big.test(v_signed);
	for ( int count_i = 0; count_i < COUNT_N; ++count_i ) {
	    load_random( v_signed, width ); 
	    big_big.test(v_signed);
	}

	// Test unsigned assignments:

	load(v_unsigned, width, 0xffffffffu);
	big_big.test(v_unsigned);
	load(v_unsigned, width, 0xaaaaaaaau);
	big_big.test(v_unsigned);
	load(v_unsigned, width, 0x99999999u);
	big_big.test(v_unsigned);
	load(v_unsigned, width, 0x66666666u);
	big_big.test(v_unsigned);
	load(v_unsigned, width, 0x55555555u);
	big_big.test(v_unsigned);

	for ( int count_i = 0; count_i < COUNT_N; ++count_i ) {
	    load_random( v_unsigned, width ); 
	    big_big.test(v_unsigned);
	}
    }



    cout << "Assignment tests passed" << endl;
    return 0;
}
