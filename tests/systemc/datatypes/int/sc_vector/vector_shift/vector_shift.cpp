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
#include <cmath>
using std::cout;
using std::endl;
using std::hex;
using std::dec;
#include "systemc.h"

using namespace sc_dt;

#define MAX_SIZE 50

#if !defined(SC_VECTOR_UTILS_H)

int sc_main( int argc, char* argv[] )
{
    cout << endl << "The vector utiliies package is not implemented in this systemc library"
         << endl << endl;
    return 0;
}

#else

// +----------------------------------------------------------------------------------------------
// |"old_vec_shift_left"
// | 
// | This is the original vec_shift_left() used in systemc, we use it here to verify that the
// | new vector_shift_left matches when used with the << operator.
// |
// | Arguments:
// |     ulen = length of 'u'.
// |     u    = digits to be shifted.
// |     nsl  = number of bits to shift.
// +----------------------------------------------------------------------------------------------
// Set u <<= nsl.
// If nsl is negative, it is ignored.
void 
old_vec_shift_left(int ulen, sc_digit *u, int nsl)
{

  if (nsl <= 0)
    return;

  // Shift left whole digits if nsl is large enough.
  if (nsl >= (int) BITS_PER_DIGIT) {

    int nd;

    if ( SC_BIT_INDEX(nsl) == 0 ) {
      nd = SC_DIGIT_INDEX(nsl);
      nsl = 0;
    }
    else {
      nd = DIV_CEIL(nsl) - 1;
      nsl -= nd * BITS_PER_DIGIT;
    }

    if (nd) {

      // Shift left for nd digits.
      for (int j = ulen - 1; j >= nd; --j)
        u[j] = u[j - nd];
      
      vector_zero( 0, sc_min( nd, ulen ), u );
      
    }

    if (nsl == 0)
      return;

  }

  // Shift left if nsl < BITS_PER_DIGIT.
  sc_digit *uiter = u;
  sc_digit *uend = uiter + ulen;

  int nsr = BITS_PER_DIGIT - nsl;
  sc_digit mask = one_and_ones(nsr);

  sc_carry carry = 0;

  while (uiter < uend) {
    sc_digit uval = (*uiter);
    (*uiter++) = (((uval & mask) << nsl) | carry);
    carry = uval >> nsr;
  }

  if (uiter < uend)
    (*uiter) = carry;

}

// +------------------------------------------------------------------------------------------------
// |"old_vec_shift_right"
// | 
// | This is the original vec_shift_right() used in systemc, we use it here to verify that the
// | new vector_shift_right matches when used with the << operator.
// |
// | Arguments:
// |     ulen = length of 'u'.
// |     u    = digits to be shifted.
// |     nsr  = number of bits to shift.
// |     fill = value to fill upper bits with.
// +------------------------------------------------------------------------------------------------
void 
old_vec_shift_right(int ulen, sc_digit *u, int nsr, sc_digit fill)
{

  // fill is usually either 0 or DIGIT_MASK; it can be any value.

  if (nsr <= 0)
    return;

  // Shift right whole digits if nsr is large enough.
  if (nsr >= (int) BITS_PER_DIGIT) {

    int nd;

    if (SC_BIT_INDEX(nsr) == 0) {
      nd = SC_DIGIT_INDEX(nsr);
      nsr = 0;
    }
    else {
      nd = DIV_CEIL(nsr) - 1;
      nsr -= nd * BITS_PER_DIGIT;
    }

    if (nd) {

      // Shift right for nd digits.
      for (int j = 0; j < (ulen - nd); ++j)
        u[j] = u[j + nd];

      if (fill) {
        for (int j = ulen - sc_min( nd, ulen ); j < ulen; ++j)
          u[j] = fill;
      }
      else
        vector_zero( ulen - sc_min( nd, ulen ), ulen, u);
    }

    if (nsr == 0)
      return;

  }

  // Shift right if nsr < BITS_PER_DIGIT.
  sc_digit *ubegin = u;
  sc_digit *uiter = (ubegin + ulen);

  int nsl = BITS_PER_DIGIT - nsr;
  sc_digit mask = one_and_ones(nsr);

  sc_carry carry = (fill & mask) << nsl;

  while (ubegin < uiter) {
    sc_digit uval = (*--uiter);
    (*uiter) = (uval >> nsr) | carry;
    carry = (uval & mask) << nsl;
  }

}

using namespace sc_dt;

void adjust_signed_hod( size_t width, sc_dt::sc_digit* digits )
{
  int hob = SC_BIT_INDEX(width-1);
  int hod = SC_DIGIT_COUNT(width) - 1;
  sc_digit high_digit = digits[hod];
  if ( (1u << hob) & high_digit ) 
    high_digit = high_digit | (~0u << hob) ;
  else
    high_digit = high_digit & ~(~0u << hob);
  digits[hod] = high_digit;
}

void adjust_unsigned_hod( size_t width, sc_dt::sc_digit* digits )
{
  sc_digit tmp = 0;
  tmp = ~tmp;
  int hod = SC_DIGIT_COUNT(width+1) - 1;
  unsigned shift = (width & (std::numeric_limits<sc_digit>::digits-1));
  digits[hod] &= ~(tmp << shift);
}

void dump( size_t n, const sc_dt::sc_digit* digits )
{
  for ( int digit_i = n-1; digit_i >= 0; --digit_i )
  {
    cout << hex << std::setfill('0') << std::setw(8) << digits[digit_i] << " ";
  }
  cout << " digits_n " << n << endl;
}

bool equal( size_t n, const sc_dt::sc_digit* left_p, const sc_dt::sc_digit* right_p )
{
  for ( size_t digit_i = 0; digit_i < n; ++digit_i ) { 
    if ( left_p[digit_i] != right_p[digit_i] ) return false;
  }
  return true;
}

void fill( size_t n, sc_dt::sc_digit* to_p, unsigned int fill_value )
{
  for ( size_t digit_i = 0; digit_i < n; ++digit_i ) { to_p[digit_i] = fill_value; }
}

template<typename T>
void fill( size_t n, T& to, unsigned int fill_value )
{
  to = fill_value;
  for ( size_t digit_i = 1; digit_i < n; ++digit_i ) { to = (to << 32) | fill_value; }
}

void load( size_t n, sc_dt::sc_digit* from, sc_dt::sc_digit* to )
{
  for ( size_t digit_i = 0; digit_i < n; ++digit_i ) { to[digit_i] = from[digit_i]; }
}

template<int W, int D=1>
class ShiftLeftSigned : public ShiftLeftSigned<W-D,D>
{
  public:
    ShiftLeftSigned( unsigned int fill_value ) : ShiftLeftSigned<W-D,D>(fill_value)
    {
      sc_bigint<W>    source;
      size_t          source_digits_n = SC_DIGIT_COUNT(W);

      fill( source_digits_n, source, fill_value );

      for ( size_t shift_i = 1; shift_i < W; ++shift_i ) {
        size_t        result_width = W + shift_i;
        size_t        result_digits_n = SC_DIGIT_COUNT(result_width);
        sc_signed     actual(result_width);
#if !defined(_MSC_VER)
#  if defined(SC_VECTOR_UTILS_H)
        sc_digit      actual1[result_digits_n];
#  endif
        sc_digit      expected[result_digits_n];
#else
#  if defined(SC_VECTOR_UTILS_H)
        sc_digit* actual1= new sc_digit[result_digits_n];
#  endif
        sc_digit* expected = new sc_digit[result_digits_n];
#endif
        load( source_digits_n, source.get_raw(), expected );
        old_vec_shift_left(result_digits_n, expected, shift_i);
        adjust_signed_hod( result_width, expected );

        // Test vector_shift_left( int target_n, sc_digit* target_p, int shift_n ) 

        actual = source << shift_i;
        if ( !equal( result_digits_n, actual.get_raw(), expected ) ) {
          cout << "ERROR: actual != expected in " << __FILE__ << " at line " << __LINE__ 
          << endl;
          cout << "    source   " << hex << source << dec << " width " << W << endl;
          cout << "    shift    " << shift_i << endl;
          cout << "    expected "; dump( result_digits_n, expected );
          cout << "    actual   "; dump( result_digits_n, actual.get_raw() );
          assert(0);
        }

        // Test vector_shift_left( int source_n, const sc_digit* source_p, int target_n,
        //                         sc_digit* target_p, int shift_n )

#if defined(SC_VECTOR_UTILS_H)
        vector_shift_left( source_digits_n, source.get_raw(), result_digits_n, actual1, 
                           shift_i );
        // @@@@@@@@ adjust_signed_hod( result_width, actual1 );
        if ( !equal( result_digits_n, actual1, expected ) ) {
          cout << "ERROR: actual != expected in " << __FILE__ << " at line " << __LINE__ 
           << endl;
          cout << "    source   " << hex << source << dec << " width " << W << endl;
          cout << "    shift    " << shift_i << endl;
          cout << "    expected "; dump( result_digits_n, expected );
          cout << "    actual   "; dump( result_digits_n, actual1 );
          assert(0);
        }
#endif
#if defined(_MSC_VER)
        delete actual1;
        delete expected;
#endif
      }
    }
};

template<int D>
class ShiftLeftSigned<0,D>
{
  public:
    ShiftLeftSigned(unsigned int fill_value ) {}
};

template<int W, int D=1>
class ShiftLeftUnsigned : public ShiftLeftUnsigned<W-D,D>
{
  public:
    ShiftLeftUnsigned( unsigned int fill_value ) : ShiftLeftUnsigned<W-D,D>(fill_value)
    {
      sc_biguint<W>   source;
      size_t          source_digits_n = SC_DIGIT_COUNT(W+1);

      fill( source_digits_n, source, fill_value );

      for ( size_t shift_i = 1; shift_i < W; ++shift_i ) {
        const size_t          result_width = (W+1) + shift_i;
        const size_t          result_digits_n = SC_DIGIT_COUNT(result_width);
        sc_unsigned     actual(result_width);
#if !defined(_MSC_VER)
#  if defined(SC_VECTOR_UTILS_H)
        sc_digit        actual1[result_digits_n];
#  endif
        sc_digit        expected[result_digits_n];
#else
#  if defined(SC_VECTOR_UTILS_H)
        sc_digit* actual1= new sc_digit[result_digits_n];
#  endif
        sc_digit* expected = new sc_digit[result_digits_n];
#endif
        load( source_digits_n, source.get_raw(), expected );
        old_vec_shift_left(result_digits_n, expected, shift_i);
        adjust_signed_hod( result_width, expected );

        // Test vector_shift_left( int target_n, sc_digit* target_p, int shift_n ) 

        actual = source << shift_i;
        if ( !equal( result_digits_n, actual.get_raw(), expected ) ) {
          cout << "ERROR: actual != expected in " << __FILE__ << " at line " << __LINE__ << endl;
          cout << "    source       " << hex << source << dec << " width " << W << endl;
          cout << "    shift        " << shift_i << endl;
          cout << "    result_width " << result_width << endl;
          cout << "    result_n     " << result_digits_n << endl;
          cout << "    expected     "; dump( result_digits_n, expected );
          cout << "    actual       "; dump( result_digits_n, actual.get_raw() );
          assert(0);
        }

        // Test vector_shift_left( int source_hod, const sc_digit* source_p, int target_hod,
        //                         sc_digit* target_p, int shift_n )

#if defined(SC_VECTOR_UTILS_H)
        vector_shift_left( source_digits_n, source.get_raw(), result_digits_n, actual1, shift_i );
        // adjust_unsigned_hod( W, actual1 );
        if ( !equal( result_digits_n, actual1, expected ) ) {
          cout << "ERROR: actual != expected in " << __FILE__ << " at line " << __LINE__  << endl;
          cout << "    source       " << hex << source << dec << " width " << W << endl;
          cout << "    shift    " << shift_i << endl;
          cout << "    expected "; dump( result_digits_n, expected );
          cout << "    actual   "; dump( result_digits_n, actual1 );
          assert(0);
        }
#endif
#if defined(_MSC_VER)
        delete actual1;
        delete expected;
#endif
      }
    }
};

template<int D>
class ShiftLeftUnsigned<0,D>
{
  public:
    ShiftLeftUnsigned(unsigned int fill_value ) {}
};


template<int W, int D=1>
class ShiftRightSigned : public ShiftRightSigned<W-D,D>
{
  public:
    ShiftRightSigned( unsigned int fill_value ) : ShiftRightSigned<W-D,D>(fill_value)
    {
      sc_bigint<W>    source;
      size_t          digits_n = SC_DIGIT_COUNT(W);
      sc_bigint<W>    actual;
#if !defined(_MSC_VER)
      sc_digit        actual_bits[digits_n];
      sc_digit        expected[digits_n];
#else
      sc_digit* actual_bits = new sc_digit[digits_n];
      sc_digit* expected = new sc_digit[digits_n];
#endif
      fill( digits_n, source, fill_value );

      for ( size_t shift_i = 1; shift_i < W; ++shift_i ) {
        size_t result_width = W - shift_i;
        size_t result_digits_n = SC_DIGIT_COUNT(result_width);
        load( digits_n, source.get_raw(), expected );
        old_vec_shift_right(digits_n, expected, shift_i, source < 0 ? -1:0);
        actual = source >> shift_i;
        if ( !equal( result_digits_n, actual.get_raw(), expected ) ) {
          cout << "ERROR: actual != expected in " << __FILE__ << " at line " << __LINE__ 
          << endl;
          cout << "    source       " << hex << source << dec << " width " << W << endl;
          cout << "    shift    " << shift_i << endl;
          cout << "    expected "; dump( result_digits_n, expected );
          cout << "    actual   "; dump( result_digits_n, actual_bits );
          assert(0);
        }
      }
#if defined(_MSC_VER)
      delete actual_bits;
      delete expected;
#endif
    }
};

template<int D>
class ShiftRightSigned<0,D>
{
  public:
    ShiftRightSigned(unsigned int fill_value ) {}
};

template<int W, int D=1>
class ShiftRightUnsigned : public ShiftRightUnsigned<W-D,D>
{
  public:
    ShiftRightUnsigned( unsigned int fill_value ) : ShiftRightUnsigned<W-D,D>(fill_value)
    {
      sc_biguint<W>   source;
      size_t          digits_n = SC_DIGIT_COUNT(W);
      sc_biguint<W>   actual;
#if !defined(_MSC_VER)
      sc_digit        expected[digits_n]; // expected[digits_n];
#else
      sc_digit* expected = new sc_digit[digits_n];
#endif
      fill( digits_n, source, fill_value );

      for ( size_t shift_i = 1; shift_i < W; ++shift_i ) {
        size_t result_width = W - shift_i;
        size_t result_digits_n = SC_DIGIT_COUNT(result_width);
        load( digits_n, source.get_raw(), expected );
        old_vec_shift_right(digits_n, expected, shift_i, 0);
        actual = source >> shift_i;
        if ( !equal( result_digits_n, actual.get_raw(), expected ) ) {
           cout << "ERROR: actual != expected in " << __FILE__ << " at line " << __LINE__ 
                << endl;
           cout << "    source       " << hex << source << dec << " width " << W << endl;
           cout << "    shift    " << shift_i << endl;
           cout << "    expected "; dump( result_digits_n, expected );
           cout << "    actual   "; dump( result_digits_n, actual.get_raw() );
           assert(0);
        }
      }
#if defined(_MSC_VER)
      delete expected;
#endif
    }
};

template<int D>
class ShiftRightUnsigned<0,D>
{
  public:
    ShiftRightUnsigned(unsigned int fill_value ) {}
};

int sc_main( int argc, char* argv[] )
{
  {
    std::cout << std::endl << "---------- shift right signed ----------" << std::endl;
    ShiftRightSigned<128>      a(~0u);
    ShiftRightSigned<128>      b(0x55555555u);
    ShiftRightSigned<128>      c(0xaaaaaaaau);
    ShiftRightSigned<128>      d(0x66666666u);
    ShiftRightSigned<128>      e(0x99999999u);
    ShiftRightSigned<1550,31>  f(~0u);
  }
  {
    std::cout << std::endl << "---------- shift right unsigned ----------" << std::endl;
    ShiftRightUnsigned<128>     a(~0u);
    ShiftRightUnsigned<128>     b(0x55555555u);
    ShiftRightUnsigned<128>     c(0xaaaaaaaau);
    ShiftRightUnsigned<128>     d(0x66666666u);
    ShiftRightUnsigned<128>     e(0x99999999u);
    ShiftRightUnsigned<1550,31> f(~0u);
  }
  {
    std::cout << std::endl << "---------- shift left signed ----------" << std::endl;
    ShiftLeftSigned<128>      a(~0u);
    ShiftLeftSigned<128>      b(0x55555555u);
    ShiftLeftSigned<128>      c(0xaaaaaaaau);
    ShiftLeftSigned<128>      d(0x66666666u);
    ShiftLeftSigned<128>      e(0x99999999u);
    ShiftLeftSigned<1550,31>  f(~0u);
    ShiftLeftSigned<1550,31>  g(0x7fffffffu);
  }
  {
    std::cout << std::endl << "---------- shift left unsigned ----------" << std::endl;
    ShiftLeftUnsigned<128>      a(~0u);
    ShiftLeftUnsigned<128>      b(0x55555555u);
    ShiftLeftUnsigned<128>      c(0xaaaaaaaau);
    ShiftLeftUnsigned<128>      d(0x66666666u);
    ShiftLeftUnsigned<128>      e(0x99999999u);
    ShiftLeftUnsigned<1550,31>  f(~0u);
  }
  cout << "Vector shift test completed" << endl;
  return 0;
}
#endif
