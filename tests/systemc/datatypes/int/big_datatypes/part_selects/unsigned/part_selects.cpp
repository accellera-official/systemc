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
#include <string>
#include <list>
#include <vector>
using std::cout;
using std::endl;
using std::hex;
using std::dec;
#include "systemc.h"

#if !defined(FULL_RANGE) || !defined(SC_VECTOR_UTILS_H)
#   define COUNT_N 4
#else
#   define COUNT_N 1
#endif


template<typename T>
void dump( const char* prefix, const T& value ) 
{
    cout << prefix << hex << value << dec 
#if defined(SC_VECTOR_UTILS_H)
         << " hod " << value.get_hod() << " bits "
	 << value.get_actual_length()
#endif                       
	 << endl;
}

template<typename T>
inline void load( int bits_n,  T& target, unsigned int fill )
{
    int target_n = DIV_CEIL(bits_n);

    target = fill;
    for ( int target_i = 1; target_i < target_n; ++target_i ) {
        target = (target << 32) + fill;
    }
}

template<int W, int D=1>
class Selection : public Selection<W-D,D>
{
  public:
    Selection( unsigned int fill ) : Selection<W-D,D>(fill)
    {
        sc_biguint<W> biguint_source;
        sc_unsigned   unsigned_source(W);

        load( W, biguint_source, fill );
        load( W, unsigned_source, fill );
	
	for ( int low = 0; low < W; low+=COUNT_N ) {
	    for ( int high = low; high < W; high+=COUNT_N ) {
	        int width = (high-low) + 1;
		sc_biguint<W> mask = -1; 
		sc_biguint<W> pre_mask = mask << width;
		mask = ~(mask << width);

		// test sc_biguint<W> selection:

                if ( 1 )
		{
		    sc_unsigned actual = biguint_source(high,low);
		    sc_unsigned shifted_source = (biguint_source >> low);

		    sc_unsigned expected = shifted_source & mask;
		    if (0) {
		        std::cout << std::endl << "sc_biguint<" << W << ">(" << high << "," 
			          << low << "):" << endl;
		        std::cout << "shift & mask = expected" << std::endl;
		        std::cout << std::hex << shifted_source << " & " << mask << " = " 
                                  << expected << std::dec << std::endl;
                    }
		    if ( actual != expected ) {
			cout << "ERROR: actual != expected in " << __FILE__ << " at line " 
			     << __LINE__ << endl;
			cout << "  range    )" << high << "," << low << "):" << endl;
			cout << "  width    " << width << endl;
			dump( "  expected ", expected );
			dump( "  actual   " ,actual );
			dump( "  pre_mask ", pre_mask );
			dump( "  mask     ", mask );
			dump( "  shift    ", shifted_source );
			dump( "  source   ", unsigned_source );
			assert(0);
		    }
		}

		// test sc_unsigned selection:

		{
		    sc_unsigned actual = unsigned_source(high,low);
		    sc_unsigned shifted_source = (unsigned_source >> low);
		    sc_unsigned expected = shifted_source & mask;
		    if (0) {
		        std::cout << std::endl << "sc_unsigned(" << W << ")(" << high << "," 
			          << low << "):" << endl;
		        std::cout << "shift & mask = expected" << std::endl;
		        std::cout << std::hex << shifted_source << " & " << mask << " = " 
                                  << expected << std::dec << std::endl;
                    }
		    if ( actual != expected ) {
			cout << "ERROR: actual != expected in " << __FILE__ << " at line " 
			     << __LINE__ << endl;
			cout << "  range    )" << high << "," << low << "):" << endl;
			cout << "  width    " << width << endl;
			dump( "  expected ", expected );
			dump( "  actual   " ,actual );
			dump( "  pre_mask ", pre_mask );
			dump( "  mask     ", mask );
			dump( "  shift    ", shifted_source );
			dump( "  source   ", unsigned_source );
                        assert(0);
		    } 
		}

	    }
        }
    }
};

template<int D>
class Selection<0,D>
{
  public:
    Selection(unsigned int fill ) {}
};

template<int W, int D=1>
class SelectionWrite : public SelectionWrite<W-D,D>
{
  public:
    SelectionWrite( unsigned int fill ) : SelectionWrite<W-D,D>(fill)
    {
        sc_biguint<W> biguint_actual;
        sc_biguint<W> biguint_expected;
        sc_biguint<W> biguint_source;
        sc_unsigned   unsigned_actual(W);
        sc_unsigned   unsigned_expected(W);
        sc_unsigned   unsigned_source(W);

        load( W, biguint_source, fill );
        load( W, unsigned_source, fill );

	for ( int low = 0; low < W; low+=COUNT_N ) {
	    for ( int high = low; high < W; high+=COUNT_N ) {
	        int width = (high-low) + 1;
		sc_biguint<W> mask = -1; 
		mask = ~(mask << width);

		// test sc_biguint<W> selection:

                if ( 1 )
		{
		    biguint_actual = 0;
		    biguint_actual(high,low) = biguint_source;
		    biguint_expected = biguint_source & mask;
		    biguint_expected = biguint_expected << low;
		    if ( biguint_actual != biguint_expected ) {
			cout << "ERROR: actual != expected in " << __FILE__ << " at line " 
			     << __LINE__ << endl;
			cout << "  range    )" << high << "," << low << "):" << endl;
			cout << "  width    " << width << endl;
			dump( "  expected ", biguint_expected );
			dump( "  actual   " , biguint_actual );
			dump( "  mask     ", mask );
			dump( "  source   ", biguint_source );
			assert(0);
		    }
		}

		// test sc_unsigned selection:

		{
		    unsigned_actual = 0;
		    unsigned_actual(high,low) = unsigned_source;
		    unsigned_expected = unsigned_source & mask;
		    unsigned_expected = unsigned_expected << low;
		    if ( biguint_actual != biguint_expected ) {
			cout << "ERROR: actual != expected in " << __FILE__ << " at line " 
			     << __LINE__ << endl;
			cout << "  range    )" << high << "," << low << "):" << endl;
			cout << "  width    " << width << endl;
			dump( "  expected ", unsigned_expected );
			dump( "  actual   " , unsigned_actual );
			dump( "  mask     ", mask );
			dump( "  source   ", unsigned_source );
			assert(0);
		    }
		}

	    }
        }
    }
};

template<int D>
class SelectionWrite<0,D>
{
  public:
    SelectionWrite(unsigned int fill ) {}
};

int sc_main( int argc, char* argv[] )
{
    if ( true )
    {
	cout << "Selection(0x" << std::hex << ~0u << std::dec << ")" << std::endl;
	Selection<128> x(~0u);
	Selection<1550,31> y(~0u);

	cout << "SelectionWrite(0x" << std::hex << ~0u << std::dec << ")" << std::endl;
	SelectionWrite<128> x_write(~0u);
	SelectionWrite<1550,31> y_write(~0u);
    }
    if ( true )
    {
	cout << "Selection(0x" << std::hex << 0x55555555u << std::dec << ")" << std::endl;
	Selection<128> x(0x55555555u);
	Selection<1550,31> y(0x55555555u);

	cout << "SelectionWrite(0x" << std::hex << 0x55555555u << std::dec << ")" << std::endl;
	SelectionWrite<128> x_write1(0x55555555u);
	SelectionWrite<1550,31> y_write(0x55555555u);
    }
    if ( true )
    {
	cout << "Selection(0x" << std::hex << 0xaaaaaaaau << std::dec << ")" << std::endl;
	Selection<128> x(0xaaaaaaaau);

	cout << "Selection(0x" << std::hex << 0xaaaaaaaau << std::dec << ")" << std::endl;
	SelectionWrite<128> x_write(0xaaaaaaaau);
    }
    if ( true )
    {
	cout << "Selection(0x" << std::hex << 0x99999999u << std::dec << ")" << std::endl;
	Selection<128> x(0x99999999u);

	cout << "SelectionWrite(0x" << std::hex << 0x99999999u << std::dec << ")" << std::endl;
	SelectionWrite<128> x_write(0x99999999u);
    }
    if ( true )
    {
	cout << "Selection(0x" << std::hex << 0x66666666u << std::dec << ")" << std::endl;
	Selection<128> x(0x66666666u);

	cout << "SelectionWrite(0x" << std::hex << 0x66666666u << std::dec << ")" << std::endl;
	SelectionWrite<128> x_write(0x66666666u);
    }
    cout << "Big unsigned selection completed" << endl;

    return 0;
}
