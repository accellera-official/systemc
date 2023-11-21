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
#if !defined(SC_VECTOR_UTILS_H)
#define SC_VECTOR_UTILS_H

#include "sc_nbdefs.h"
#include "sc_nbutils.h"

namespace sc_dt {

#define VEC_MIN(A,B) ( (A) <= (B) ? A : B )
#define VEC_MAX(A,B) ( (A) >= (B) ? A : B )

// External entities:

template<int W> class sc_bigint;
template<int W> class sc_biguint;
class sc_signed;
class sc_unsigned;

// +============================================================================
// |"sc_big_op_type<WIDTH,SIGNED>
// |
// | This templated class provides the operand type for big values with the
// | supplied width and signedness.
// +============================================================================
template<int WIDTH, bool SIGNED>
class sc_big_op_type;

template<int WIDTH>
class sc_big_op_type<WIDTH,true>
{
  public:
    typedef sc_signed        base_type;
    typedef int              hod_type;
    typedef sc_bigint<WIDTH> top_type;
};

template<int WIDTH>
class sc_big_op_type<WIDTH,false>
{
  public:
    typedef sc_unsigned       base_type;
    typedef sc_digit          hod_type;
    typedef sc_biguint<WIDTH> top_type;
};

// +============================================================================
// |"sc_big_op_info<WL,SL,WR,SR>
// |
// | This templated class provides information for operations involving two
// | sc_bigint<W> and sc_biguint<W> operands.
// |
// | Template arguments:
// |   WL = width of left operand of the operation.
// |   SL = true if left operand is signed.
// |   WR = width of right operand of the operation.
// |   SR = true if right operand is signed.
// +============================================================================
template<int WL, bool SL, int WR, bool SR>
class sc_big_op_info
{
  public: // width calculations:
    enum {
      signed_result = SL || SR,

      left_count = SC_DIGIT_COUNT(WL+!SL),
      left_extra = !SL && SR,
      left_hod   = SC_DIGIT_INDEX(WL+!SL),

      right_count = SC_DIGIT_COUNT(WR+!SR),
      right_extra = SL && !SR,
      right_hod   = SC_DIGIT_INDEX(WR+!SR),

      add_bits = VEC_MAX(WL+left_extra,WR+right_extra)+1,
      bit_bits = VEC_MAX(WL+left_extra,WR+right_extra),
      div_bits = WL+SR,
      mod_bits = VEC_MIN(WL,WR+right_extra),
      mul_bits = WL+WR,
      sub_bits = VEC_MAX(WL+left_extra,WR+right_extra)+1,

      add_hod   = SC_DIGIT_INDEX(add_bits-1),
      bit_hod   = SC_DIGIT_INDEX(bit_bits-1),
      div_hod   = SC_DIGIT_INDEX(div_bits-1),
      mod_hod   = SC_DIGIT_INDEX(mod_bits-1),
      mul_hod   = SC_DIGIT_INDEX(mul_bits-1),
      sub_hod   = SC_DIGIT_INDEX(sub_bits-1),

      add_mask  = SC_BIT_MASK1(add_bits+!signed_result),
      bit_mask  = SC_BIT_MASK1(bit_bits+!signed_result),
      div_mask  = SC_BIT_MASK1(div_bits+!signed_result),
      mod_mask  = SC_BIT_MASK1(mod_bits+!signed_result),
      mul_mask  = SC_BIT_MASK1(mul_bits+!signed_result),
      sub_mask  = SC_BIT_MASK1(sub_bits+!signed_result),

      shorter_length = SC_DIGIT_COUNT( VEC_MIN(WL+!SL,WR+!SR) )
    };

  public: // operand types:
    typedef typename sc_big_op_type<WL,SL>::base_type left_base;
    typedef typename sc_big_op_type<WL,SL>::top_type  left_op;
    typedef typename sc_big_op_type<WR,SR>::base_type right_base;
    typedef typename sc_big_op_type<WR,SR>::top_type  right_op;

  public: // result types:
   typedef typename sc_big_op_type<add_bits,signed_result>::top_type add_result;
   typedef typename sc_big_op_type<bit_bits,signed_result>::top_type bit_result;
   typedef typename sc_big_op_type<div_bits,signed_result>::top_type div_result;
   typedef typename sc_big_op_type<mod_bits,signed_result>::top_type mod_result;
   typedef typename sc_big_op_type<mul_bits,signed_result>::top_type mul_result;
   typedef typename sc_big_op_type<sub_bits,signed_result>::top_type sub_result;
};

// +============================================================================
// |"ScNativeDigits"
// |
// | This templated class contains the value of its template type as a vector
// | of sc_digit values. It is overloaded for each native C++ data type.
// +============================================================================
template<typename T> class ScNativeDigits;

template<>
class ScNativeDigits<int64>
{
  public:
    enum {
	ACTUAL_WIDTH = 8*sizeof(int64),
	DIGITS_N     = SC_DIGIT_COUNT(ACTUAL_WIDTH),
	HOD          = SC_DIGIT_INDEX(ACTUAL_WIDTH-1),
	WIDTH        = ACTUAL_WIDTH,
	SIGNED       = 1
    };

  public:
    ScNativeDigits( int64 init )
    {
        m_digits[0] = (sc_digit)init;
        for ( int digit_i = 1; digit_i < DIGITS_N; ++digit_i ) {
	    init >>= BITS_PER_DIGIT;
	    m_digits[digit_i] = (sc_digit)init;
	}
    }
    int             get_actual_length() const { return ACTUAL_WIDTH; }
    const sc_digit* get_digits() const        { return m_digits; }
    sc_digit*       get_digits()              { return m_digits; }
    int             get_digits_n() const      { return DIGITS_N; }
    int             get_hod() const           { return HOD; }
    int             is_signed() const         { return SIGNED; }
    int             length() const            { return WIDTH; }
  protected:
    sc_digit m_digits[DIGITS_N];
};

template<>
class ScNativeDigits<uint64>
{
  public:
    enum {
        ACTUAL_WIDTH = 8*sizeof(uint64)+1,
        DIGITS_N     = SC_DIGIT_COUNT(ACTUAL_WIDTH),
	HOD          = SC_DIGIT_INDEX(ACTUAL_WIDTH-1),
	WIDTH        = ACTUAL_WIDTH-1,
	SIGNED       = 0
    };

  public:
    ScNativeDigits( uint64 init )
    {
        m_digits[0] = (sc_digit)init;
        for ( int digit_i = 1; digit_i < DIGITS_N; ++digit_i ) {
	    init >>= BITS_PER_DIGIT;
	    m_digits[digit_i] = (sc_digit)init;
	}
    }
    int             get_actual_length() const { return ACTUAL_WIDTH; }
    const sc_digit* get_digits() const        { return m_digits; }
    sc_digit*       get_digits()              { return m_digits; }
    int             get_digits_n() const      { return DIGITS_N; }
    int             get_hod() const           { return HOD; }
    int             is_signed() const         { return SIGNED; }
    int             length() const            { return WIDTH; }
  protected:
    sc_digit m_digits[DIGITS_N];
};

template<>
class ScNativeDigits<long>
{
  public:
    enum {
        ACTUAL_WIDTH = 8*sizeof(long),
        DIGITS_N     = SC_DIGIT_COUNT(ACTUAL_WIDTH),
	HOD          = SC_DIGIT_INDEX(ACTUAL_WIDTH-1),
	WIDTH        = ACTUAL_WIDTH,
	SIGNED       = 1
    };

  public:
    ScNativeDigits( long init )
    {
        m_digits[0] = (sc_digit)init;
        for ( int digit_i = 1; digit_i < DIGITS_N; ++digit_i ) {
	    init >>= (ACTUAL_WIDTH > BITS_PER_DIGIT) ? BITS_PER_DIGIT : 0;
	    m_digits[digit_i] = (sc_digit)init;
	}
    }
    int             get_actual_length() const { return ACTUAL_WIDTH; }
    const sc_digit* get_digits() const        { return m_digits; }
    sc_digit*       get_digits()              { return m_digits; }
    int             get_digits_n() const      { return DIGITS_N; }
    int             get_hod() const           { return HOD; }
    int             is_signed() const         { return SIGNED; }
    int             length() const            { return WIDTH; }
  protected:
    sc_digit m_digits[DIGITS_N];
};

template<>
class ScNativeDigits<unsigned long>
{
  public:
    enum {
        ACTUAL_WIDTH = 8*sizeof(unsigned long)+1,
        DIGITS_N     = SC_DIGIT_COUNT(ACTUAL_WIDTH),
	HOD          = SC_DIGIT_INDEX(ACTUAL_WIDTH-1),
	WIDTH        = ACTUAL_WIDTH-1,
	SIGNED       = 0
    };

  public:
    ScNativeDigits( unsigned long init )
    {
        m_digits[0] = (sc_digit)init;
	unsigned long long llinit = init;
        for ( int digit_i = 1; digit_i < DIGITS_N; ++digit_i ) {
	    llinit >>= BITS_PER_DIGIT;
	    m_digits[digit_i] = (sc_digit)llinit;
	}
    }
    int             get_actual_length() const { return ACTUAL_WIDTH; }
    const sc_digit* get_digits() const        { return m_digits; }
    sc_digit*       get_digits()              { return m_digits; }
    int             get_digits_n() const      { return DIGITS_N; }
    int             get_hod() const           { return HOD; }
    int             is_signed() const         { return SIGNED; }
    int             length() const            { return WIDTH; }
  protected:
    sc_digit m_digits[DIGITS_N];
};

template<>
class ScNativeDigits<int>
{
  public:
    enum {
        ACTUAL_WIDTH = 8*sizeof(int),
        DIGITS_N     = SC_DIGIT_COUNT(ACTUAL_WIDTH),
	HOD          = SC_DIGIT_INDEX(ACTUAL_WIDTH-1),
	WIDTH        = ACTUAL_WIDTH,
	SIGNED       = 1
    };

  public:
    ScNativeDigits( int init )
    {
        m_digits[0] = (sc_digit)init;
    }
    int             get_actual_length() const { return ACTUAL_WIDTH; }
    const sc_digit* get_digits() const        { return m_digits; }
    sc_digit*       get_digits()              { return m_digits; }
    int             get_digits_n() const      { return DIGITS_N; }
    int             get_hod() const           { return HOD; }
    int             length() const            { return WIDTH; }
    int             is_signed() const         { return SIGNED; }
  protected:
    sc_digit m_digits[DIGITS_N];
};

template<>
class ScNativeDigits<unsigned int>
{
  public:
    enum {
        ACTUAL_WIDTH = 8*sizeof(unsigned int)+1,
        DIGITS_N     = SC_DIGIT_COUNT(ACTUAL_WIDTH),
	HOD          = SC_DIGIT_INDEX(ACTUAL_WIDTH-1),
	WIDTH        = ACTUAL_WIDTH-1,
	SIGNED       = 0
    };

  public:
    ScNativeDigits( unsigned long init )
    {
        m_digits[0] = (sc_digit)init;
	m_digits[1] = 0;
    }
    int             get_actual_length() const { return ACTUAL_WIDTH; }
    const sc_digit* get_digits() const        { return m_digits; }
    sc_digit*       get_digits()              { return m_digits; }
    int             get_digits_n() const      { return DIGITS_N; }
    int             get_hod()                 { return HOD; }
    int             is_signed()               { return SIGNED; }
    int             length()                  { return WIDTH; }
  protected:
    sc_digit m_digits[DIGITS_N];
};

// +----------------------------------------------------------------------------
// |"vector_skip_leading_ones"
// |
// | This function returns the index of the highest order sc_digit in the
// | supplied vector that is not all ones, or zero if all sc_digits are all 
// | ones.
// |
// | Arguments:
// |     hod      = index of high order digit in 'digits_p'.
// |     digits_p = vector of digits to find the highest non-zero entry in.
// | Result:
// |     index of the highest order non-all-ones entry in 'digits_p' or 0 if
// |     all the entries in 'digits_p' are all ones.
// +----------------------------------------------------------------------------
inline
int
vector_skip_leading_ones( const int hod, const sc_digit* digits_p )
{
    int digit_i; // digit now examining.
    for ( digit_i = hod; (digit_i > 0) && (digits_p[digit_i] == ~0u); --digit_i )
        continue;
    return digit_i;
}

// +----------------------------------------------------------------------------
// |"vector_skip_leading_zeros"
// |
// | This function returns the index of the highest order sc_digit in the
// | supplied vector that is non-zero, or zero if there is not a non-zero
// | sc_digit in the vector.
// |
// | Arguments:
// |     hod      = index of high order digit in 'digits_p'.
// |     digits_p = vector of digits to find the highest non-zero entry in.
// | Result:
// |     index of the highest order non-zero entry in 'digits_p' or 0 if
// |     all the entries in 'digits_p' are zero.
// +----------------------------------------------------------------------------
inline
int
vector_skip_leading_zeros( const int hod, const sc_digit* digits_p )
{
    int digit_i; // digit now examining.
    for ( digit_i = hod; (digit_i > 0) && (digits_p[digit_i] == 0); --digit_i )
        continue;
    return digit_i;
}

// +------------------------------------------------------------------------------------------------
// |"vector_find_significant_hod"
// | 
// | This function finds the "significant" high order digit of the supplied vector. For a
// | positive number leading zero digits are skipped, however, if the resultant high order
// | digit would be negative one digit of zeroes is added back in.
// | Similarly for a negative number leading all one digits are skipped, however, if the resultant
// | high order digit would be positive one digit of all ones is added back in.
// |
// | Arguments:
// |     hod    = high order digit of 'digits'.
// |     digits = digits to be examined.
// | Result:
// |     index of significant order digit.
// +------------------------------------------------------------------------------------------------
inline int vector_find_significant_hod( int hod, const sc_digit* digits )
{
    int result_hod;
    if ( (int)digits[hod] < 0 ) {
	result_hod = vector_skip_leading_ones( hod, digits );
	if ( (int)digits[result_hod] >= 0 ) { ++result_hod;  }
    }
    else {
	result_hod = vector_skip_leading_zeros( hod, digits );
	if ( (int)digits[result_hod] < 0 ) { ++result_hod;  }
    }
    return result_hod;
}
    
// +----------------------------------------------------------------------------
// |"vector_add"
// |
// | This function adds two signed sc_digit vectors. The second vector must be
// | shorter, or the same length, as the first one. The algorithm is from
// | grade school:
// |          AS A5 A4 A3 A2 A1
// |     +          BS B3 B2 B1
// |       --------------------
// |       R7 R6 R5 R4 R3 R2 R1
// |
// |     Let BPD represent the number of bits per digit.
// |
// |     Raw1 = A1 + B1
// |     Raw2 = (Raw1 >> BPD) + A2 + B2
// |     Raw3 = (Raw2 >> BPD) + A3 + B3
// |     Raw4 = (Raw3 >> BPD) + A4 + (signed)BS
// |     Raw5 = (Raw4 >> BPD) + A5
// |     Raw6 = (Raw5 >> BPD) + (signed)AS
// |     Raw7 = (Raw6 >> BPD)
// |
// |     R1 = Raw1 % BPD
// |     R2 = Raw2 % BPD
// |     R3 = Raw3 % BPD
// |     R4 = Raw4 % BPD
// |     R5 = Raw5 % BPD
// |     R6 = Raw6 % BPD
// |     R7 = Raw7 % BPD
// |
// | Notes:
// |   (1) Either longer_p or shorter_p may also be result_p without
// |       consequences.
// | Arguments:
// |     longer_hod   = index of high order digit in 'longer_p'
// |     longer_p     = digits in the longer operand.
// |     shorter_hod  = index of high order digit in 'shorter_p'
// |     shorter_p    = digits in the shorter operand
// |     result_hod   = index of high order digit in 'result_p'
// |     result_p     = digits to be filled in in the result.
// +----------------------------------------------------------------------------
inline
void
vector_add( const int       longer_hod,
	    const sc_digit* longer_p,
	    const int       shorter_hod,
	    const sc_digit* shorter_p,
	    const int       result_hod,
	    sc_digit*       result_p )
{

    if ( result_hod == 0 ) {
        *result_p = *longer_p + *shorter_p;
	return;
    }

    int64 carry = 0;
    int   digit_i;

    // Add the low order unsigned parts of the operands:

    for ( digit_i = 0; digit_i < shorter_hod; ++digit_i ) {
        carry += *shorter_p++;
        carry += *longer_p++;
	*result_p++ = (sc_digit)carry;
	carry >>= BITS_PER_DIGIT;
    }

    // The operands are the same length: add the high order digits as signed
    // values:

    if ( longer_hod == shorter_hod ) {
	carry += (int64)(int)*shorter_p++;
	carry += (int64)(int)*longer_p++;
	*result_p++ = (sc_digit)carry;
	carry >>= BITS_PER_DIGIT;
    }

    // The operands are different lengths:
    //   (a) Add the shorter operand's high order digit as a signed value.
    //   (b) Add the longer operand's values below its high order digit as
    //       unsigned.
    //   (c) Add the high order digit as signed.

    else {
	carry += (int64)(int)*shorter_p++;
	carry += *longer_p++;
	*result_p++ = (sc_digit)carry;
	carry >>= BITS_PER_DIGIT;

	for ( digit_i = shorter_hod+1; digit_i < longer_hod; ++digit_i ) {
	    carry += *longer_p++;
	    *result_p++ = (sc_digit)carry;
	    carry >>= BITS_PER_DIGIT;
	}

	carry += (int64)(int)*longer_p++;
	*result_p++ = (sc_digit)carry;
	carry >>= BITS_PER_DIGIT;
    }


    // If there is an additional high order digit in the result assign it the
    // carry value:

    if ( result_hod > longer_hod ) {
        *result_p = (sc_digit)carry;
    }
}

// +----------------------------------------------------------------------------
// |"vector_and"
// |
// | This function ands two signed sc_digit vectors. The second vector must be
// | shorter, or the same length, as the longer one.
// |
// | Template arguments:
// |     SL - true if longer operand is signed.
// |     SS - true if shorter operand is signed.
// | Arguments:
// |     longer_hod   = index of high order digit in 'longer_p' and 'result_p'.
// |     longer_p     = digits in the longer operand.
// |     shorter_hod  = index of high order digit in 'shorter_p'
// |     shorter_p    = digits in the shorter operand
// |     result_p     = digits to be filled in in the result, this is the same
// |                    number of digits as the longer operand.
// +----------------------------------------------------------------------------
template<bool SL, bool SS>
inline
void
vector_and( const int       longer_hod,
	    const sc_digit* longer_p,
	    const int       shorter_hod,
	    const sc_digit* shorter_p,
	    sc_digit*       result_p )
{
    if ( 0 == longer_hod ) {
        *result_p = *longer_p & *shorter_p;
	return;
    }

    int   digit_i;

    // And the two operands to the length of the shorter operand:

    for ( digit_i = 0; digit_i <= shorter_hod; ++digit_i ) {
	*result_p++ = *longer_p++ & shorter_p[digit_i];
    }

    // If the shorter operand is negative fill out the remaining entries
    // using the longer operand, otherwise fill it out with zeros.

    if ( longer_hod > shorter_hod ) {
	if ( SS && 0 > (int)shorter_p[shorter_hod] ) {
	    for ( ; digit_i <= longer_hod; ++digit_i ) {
		*result_p++ = *longer_p++;
	    }
	}
	else {
	    for ( ; digit_i <= longer_hod; ++digit_i ) {
		*result_p++ = 0;
	    }
	}
    }
}

// +----------------------------------------------------------------------------
// |"vector_and_reduce"
// |
// | This function overload performs an "and reduction" on the supplied vector,
// | returning the result.
// |
// | Arguments:
// |     bits_n   = number of bits in the vector that are valid.
// |     target_p = digits containing the bits.
// | Result:
// |     true if all bits are on in the vector, false if not.
// +----------------------------------------------------------------------------
inline
bool
vector_and_reduce( const int       bits_n,
	           const sc_digit* target_p )
{
    const int hod = SC_DIGIT_INDEX(bits_n-1);

    // First check the whole sc_digits:

    for ( int digit_i = 0; digit_i < hod; ++digit_i ) {
        if ( target_p[digit_i] != (sc_digit)-1 ) {
	    return false;
	}
    }

    // Check the high order digit:

    const sc_digit mask = ~( (std::numeric_limits<sc_digit>::max()-1)
                << SC_BIT_INDEX(bits_n-1) );
    if ( ( target_p[hod] & mask ) != mask ) {
        return false;
    }
    return true;
}

// +----------------------------------------------------------------------------
// |"vector_and_reduce"
// |
// | This function overload performs an "and reduction" on the requested range
// | of bits in the supplied vector, returning the result.
// |
// | Arguments:
// |     target_p = digits containing the bits.
// |     left_i   = left-most bit in the range to be checked.
// |     right_i  = right-most bit in the range to be checked.
// | Result:
// |     true if all bits are on in the vector, false if not.
// +----------------------------------------------------------------------------
inline
bool
vector_and_reduce( const sc_digit* target_p,
                   const int       left_i,
	           const int       right_i )
{
    const int left_hod = SC_DIGIT_INDEX(left_i);
    const int right_hod = SC_DIGIT_INDEX(right_i);

    // All the bits are in a single sc_digit:

    if ( left_hod == right_hod ) {
        const sc_digit mask =
            ~((std::numeric_limits<sc_digit>::max()-1) << (left_i-right_i) );
	return ( ( target_p[right_hod] & mask ) != mask ) ? false : true;
    }

    const int right_index = SC_BIT_INDEX(right_i);

    // The low order bit is not on an sc_digit boundary, check the low
    // order digit, then the whole sc_digits above it.

    if ( right_index ) {
        const sc_digit mask =
            (std::numeric_limits<sc_digit>::max() << (right_index-1) );
	if ( ( target_p[right_hod] & mask ) != mask ) {
	    return false;
	}
	for ( int digit_i = right_hod+1; digit_i < left_hod; ++digit_i ) {
	    if ( target_p[digit_i] != (sc_digit)-1 ) {
		return false;
	    }
	}
    }

    // The lower order bit is on an sc_digit boundary, check the whole
    // sc_digits below the high order digit.

    else {
	for ( int digit_i = right_hod; digit_i < left_hod; ++digit_i ) {
	    if ( target_p[digit_i] != (sc_digit)-1 ) {
		return false;
	    }
	}
    }

    // Check the high order sc_digit:

    const sc_digit mask =
        ~( (std::numeric_limits<sc_digit>::max()-1) << SC_BIT_INDEX(left_i) );
    if ( ( target_p[left_hod] & mask ) != mask ) {
        return false;
    }
    return true;
}

// +----------------------------------------------------------------------------
// |"vector_compare"
// |
// | This inline function compares the two supplied vectors, returing their
// | relationship.
// |
// | Template arguments:
// |     SL = true if left argument is signed, false if not.
// |     SR = true if right argument is signed, false if not.
// | Arguments:
// |     left_hod  = index of the high order 'left' digit.
// |     left_p    = vector of digits to be compared.
// |     right_hod = index of the high order 'right' digit.
// |     right_p   = second vector of digits to be compared.
// | Result:
// |     -1 if left < right.
// |      0 if left == right.
// |     +1 if left > right.
// +----------------------------------------------------------------------------
template<int SL, int SR>
inline
int
vector_compare( const int       left_hod,
	        const sc_digit* left_p,
	        const int       right_hod,
	        const sc_digit* right_p )
{
    int digit_i;          // index into left_p and right_p.
    int left_hod_value;   // left hod value as an integer.
    int right_hod_value;  // right hod value as an integer.

    // Left operand is negative, right operand is unsigned.:

    left_hod_value = (int)left_p[left_hod];
    if ( SL == true && SR == false && ( 0 > left_hod_value ) ) {
        return -1;
    }

    // Right operand is negative, left operand is unsigned.:

    right_hod_value = (int)right_p[right_hod];
    if ( SL == false && SR == true && ( 0 > right_hod_value ) ) {
        return 1;
    }

    // Both operands are signed and left operand is negative and right operand
    // is not negative:

    if ( SL == true && SR == true && (0 > left_hod_value) &&
         (0 <= right_hod_value ) ) {
        return -1;
    }

    // Both operands are signed and right operand is negative and left operand
    // is not negative:

    if ( SL == true && SR == true && (0 <= left_hod_value) &&
         (0 > right_hod_value ) ) {
        return 1;
    }

    // Both values are negative:

    else if ( SL == true && SR == true && (0 > left_hod_value) ) {
	if ( left_hod >= right_hod ) {
	    for ( digit_i = left_hod; digit_i > right_hod; --digit_i ) {
	        if ( left_p[digit_i] != (sc_digit)-1 ) {
		    return -1;
		}
	    }
	}
	else {
	    for ( digit_i = right_hod; digit_i > left_hod; --digit_i ) {
	        if ( right_p[digit_i] != (sc_digit)-1 ) {
		    return 1;
		}
	    }
	}
	for ( ; digit_i >= 0; --digit_i ) {
	    if ( left_p[digit_i] != right_p[digit_i] ) {
		return ( left_p[digit_i] < right_p[digit_i] ) ? -1 : 1;
	    }
	}
        return 0;
    }

    // Neither value is negative:

    else {
	if ( left_hod >= right_hod ) {
	    for ( digit_i = left_hod; digit_i > right_hod; --digit_i ) {
	        if ( left_p[digit_i] != 0 ) {
		    return 1;
		}
	    }
	}
	else {
	    for ( digit_i = right_hod; digit_i > left_hod; --digit_i ) {
	        if ( right_p[digit_i] != 0 ) {
		    return -1;
		}
	    }
	}
	for ( ; digit_i >= 0; --digit_i ) {
	    if ( left_p[digit_i] != right_p[digit_i] ) {
		return ( left_p[digit_i] < right_p[digit_i] ) ? -1 : 1;
	    }
	}
        return 0;
    }
}

// +----------------------------------------------------------------------------
// |"vector_copy"
// |
// | This inline function copies from one vector of sc_digits to another. The
// | from vector is treated as "signed", so if its upper bit is one any
// | extension will be ones, otherwise it will be zeros.
// |
// | Arguments:
// |     from_n = length  of 'from_p'.
// |     from_p = vector of digits to be copied from.
// |     to_n   = length of 'to_p'.
// |     to_p   = vector of digits to be copied to.
// +----------------------------------------------------------------------------
inline
void
vector_copy( const int       from_n,
	     const sc_digit* from_p,
	     const int       to_n,
	     sc_digit*       to_p )
{
    int digit_i; // digit now accessing.

    if ( from_n < to_n ) {
        for ( digit_i = 0; digit_i < from_n; ++digit_i ) {
	    to_p[digit_i] = from_p[digit_i];
	}
	sc_digit fill = 0 > (int)from_p[from_n-1] ? -1 : 0;
        for ( ; digit_i < to_n; ++digit_i ) {
	    to_p[digit_i] = fill;
	}
    }
    else {
        for ( digit_i = 0; digit_i < to_n; ++digit_i ) {
	    to_p[digit_i] = from_p[digit_i];
	}
    }
}

// +----------------------------------------------------------------------------
// |"vector_copy"
// |
// | This inline function copies from one vector of sc_digits to another of the
// | exact same size.
// |
// | Arguments:
// |     digits_n = number of digits to be copied.
// |     from_p   = vector of digits to be copied from.
// |     to_p     = vector of digits to be copied to.
// +----------------------------------------------------------------------------
inline
void
vector_copy( int             digits_n,
	     const sc_digit* from_p,
	     sc_digit*       to_p )
{
    for ( int digit_i = 0; digit_i < digits_n; ++digit_i ) {
	to_p[digit_i] = from_p[digit_i];
    }
}

// +----------------------------------------------------------------------------
// |"vector_extract"
// |
// | This inline function extracts the specified bits. 
// |   (1) high_bit must be greater than or equal to low_bit.
// |   (2) any high order bits in the high order digit of destination_p that are not part
// |       of the the value from source_p will be zeroed.
// | Arguments:
// |     source_p      -> where to extract the bits from.
// |     destination_p -> where to place the extracted bits.
// |     high_bit      =  high order bit of the extraction.
// |     low_bit       =  low order bit of the extraction.
// +----------------------------------------------------------------------------
inline
void
vector_extract( const sc_digit* source_p,
                sc_digit*       destination_p,
	        const int       high_bit,
	        const int       low_bit )
{
    int dst_hob;     // high order bit in destination_p to be moved to.
    int lod;         // low order digit containing data in source_p.
    int hod;         // high order digit containing data in source_p.
    int right_shift; // index of bit within lod of the low order bit to moved.

    dst_hob = high_bit-low_bit;
    lod = SC_DIGIT_INDEX(low_bit);
    hod = SC_DIGIT_INDEX(high_bit);
    right_shift = SC_BIT_INDEX(low_bit);

    // The extraction is on an sc_digit boundary, just move the digits.

    if ( 0 == right_shift ) {
        const sc_digit* src_p = &source_p[lod];
	sc_digit*       dst_p = destination_p;
	for ( int digit_i = lod; digit_i <= hod; ++digit_i ) {
	    *dst_p++ = *src_p++;
	}
    }

    // The extraction is within a single sc_digit, we just need to shift down.

    else if ( hod == lod ) {
        *destination_p = source_p[lod] >> right_shift;
    }

    // The extraction is not on an sc_digit boundary, we need to do shifting.

    else {
	sc_digit* dst_p = destination_p;
        const int left_shift = BITS_PER_DIGIT - right_shift;
        const sc_digit* src_p = &source_p[lod];
	sc_digit        carry = *src_p++ >> right_shift;

	for ( int digit_i = lod+1; digit_i <= hod; ++digit_i ) {
	    sc_digit value = *src_p++;
	    *dst_p++ = (value << left_shift) | carry;
	    carry = (value >> right_shift);
	}
	const int dst_hod = SC_DIGIT_INDEX(dst_hob);
	if ( dst_p == &destination_p[dst_hod] ) {
	    *dst_p = carry;
	}
    }

    // Trim the upper sc_digit:

    destination_p[SC_DIGIT_INDEX(dst_hob)] &= SC_BIT_MASK1(dst_hob);
}

// +----------------------------------------------------------------------------
// |"vector_fill"
// |
// | This inline function fills the supplied vector of digits with the
// | supplied value.
// |
// | Arguments:
// |     fill   = value to be assigned.
// |     to_hod = index of the high order 'to' digit.
// |     to_p   = vector of digits to be copied.
// +----------------------------------------------------------------------------
inline
void
vector_fill( const sc_digit  fill,
	     const int       to_hod,
	     sc_digit*       to_p )
{
    for ( int digit_i = 0; digit_i <= to_hod; ++digit_i ) {
        to_p[digit_i] = fill;
    }
}

// +----------------------------------------------------------------------------
// |"vector_fill_bits"
// |
// | This inline function sets a range of bits to a value.
// |
// | Arguments:
// |     fill     = value to set the bits to, either -1 or 0.
// |     target_p = vector to set bits in.
// |     high_bit = high order bit to be set.
// |     low_bit  = low order bit to be set.
// +----------------------------------------------------------------------------
inline
void
vector_fill_bits( sc_digit  fill,
                  sc_digit* target_p,
		  const int high_bit,
		  const int low_bit )
{
    const int hob = SC_BIT_INDEX(high_bit);
    const int hod = SC_DIGIT_INDEX(high_bit);
    const int lob = SC_BIT_INDEX(low_bit);
    const int lod = SC_DIGIT_INDEX(low_bit);

    // Fill is all within a single digit:

    if ( hod == lod ) {
        const sc_digit mask = SC_BIT_MASK1(hob-lob) << lob;
	target_p[lod] = ( target_p[lod] & ~mask ) | ( fill & mask );
    }

    // Fill is across digits perform it in three steps:
    //   (a) low order digit.
    //   (b) whole intermediate digits
    //   (c) high order digit

    else {
        const sc_digit low_mask = SC_BIT_MASK1(lob);
	target_p[lod] = (target_p[lod] & low_mask ) | (fill & low_mask);
	for ( int digit_i = lod+1; digit_i < hod; ++digit_i ) {
	    target_p[digit_i] = fill;
	}
        const sc_digit high_mask = SC_BIT_MASK1(hob);
	target_p[hod] = (target_p[lod] & ~high_mask ) | (fill & high_mask);
    }
}

// +----------------------------------------------------------------------------
// |"vector_ones_complement"
// |
// | This inline function performs a ones complement on the value in the
// | supplied vector.
// |
// | Arguments:
// |     target_hod = index of the high order 'target_p' digit.
// |     target_p   = vector of digits to be complemented.
// +----------------------------------------------------------------------------
inline
void
vector_ones_complement( const int target_hod,
	                sc_digit* target_p )
{
    for ( int digit_i = 0; digit_i <= target_hod; ++digit_i ) {
        target_p[digit_i] = ~target_p[digit_i];
    }
}

// +----------------------------------------------------------------------------
// |"vector_ones_complement"
// |
// | This inline function performs a ones complement on the value of the
// | supplied vector, placing it in the supplied destination vector.
// |
// | Arguments:
// |     source_hod = index of the high order 'source_p' digit.
// |     source_p   = vector of digits to be complemented.
// |     target_p   = vector of digits to receive the complement.
// +----------------------------------------------------------------------------
inline
void
vector_ones_complement( const int       source_hod,
	                const sc_digit* source_p,
	                sc_digit*       target_p )
{
    for ( int digit_i = 0; digit_i <= source_hod; ++digit_i ) {
	*target_p++ = ~*source_p++;
    }
}

// +----------------------------------------------------------------------------
// |"vector_reverse_bits"
// |
// | This function reverses the bits in the supplied bit range.
// |
// | Arguments:
// |     target_p = vector the bits are to be reversed in.
// |     high_i   = bit index of high order bit that is be reversed.
// |     low_i    = bit index of low order bit that is be reversed.
// +----------------------------------------------------------------------------
inline
void
vector_reverse_bits( sc_digit* target_p,
                     int       high_i,
		     int       low_i )
{
    for ( ; high_i > low_i; --high_i, ++low_i ) {
        const int hod = SC_DIGIT_INDEX(high_i);
        const int hom = 1 << SC_BIT_INDEX(high_i);
        const int lod = SC_DIGIT_INDEX(low_i);
        const int lom = 1 << SC_BIT_INDEX(low_i);

	if ( hom & target_p[hod] ) {
	    if ( !( lom & target_p[lod] ) ) {
	        target_p[hod] &= ~hom;
		target_p[lod] |= lom;
	    }
	}
	else if ( lom & target_p[lod] ) {
	    target_p[hod] |= hom;
	    target_p[lod] &= ~lom;
	}
    }
}


// +----------------------------------------------------------------------------
// |"vector_insert_bits"
// |
// | This inline function inserts the supplied value in the specified bit
// | range. It is assumed the target value has enough digits to accommodate
// | the range, and that the source data is large enough to provide data
// | for each bit in the range.
// | Arguments:
// |     from_digits_n =  number of digits in from_p.
// |     from_p        -> value to be inserted.
// |     to_p          -> vector the bits are to be inserted in.
// |     high_bit_i    =  high order bit of the insertion.
// |     low_bit_i     =  low order bit of the insertion.
// +----------------------------------------------------------------------------
inline
void
vector_insert_bits( const int       from_digits_n,
                    const sc_digit* from_p,
		    sc_digit*       to_p,
		    const int       high_bit_i,
		    const int       low_bit_i )
{
    const int debug=0;   // set to non-zero to debug.
    int       from_hod;  // digit index of high order bit in the from_p.
    bool      reverse;   // true -> reverse the bits in the insertion.
    int       to_hob;    // bit index of high order bit in the insertion.
    int       to_hod;    // digit index of high order bit in the insertion.
    int       to_lob;    // bit index of lower order bit in the insertion.
    int       to_lod;    // digit index of low order bit in the insertion.

    // Configure for big endian bit ordering:

    if ( high_bit_i >= low_bit_i ) {
        reverse = false;
	to_lob = SC_BIT_INDEX(low_bit_i);
	to_lod = SC_DIGIT_INDEX(low_bit_i);
	to_hob = SC_BIT_INDEX(high_bit_i);
	to_hod = SC_DIGIT_INDEX(high_bit_i);
    }

    // Configure for little endian bit ordering:

    else {
	reverse = true;
	to_lob = SC_BIT_INDEX(high_bit_i);
	to_lod = SC_DIGIT_INDEX(high_bit_i);
	to_hob = SC_BIT_INDEX(low_bit_i);
	to_hod = SC_DIGIT_INDEX(low_bit_i);
    }

    from_hod = from_digits_n - 1;

    if ( debug ) {
	std::cout << "vector_insert_bits(" << from_hod << ", from_p, to_p, "
	          << high_bit_i << ", " << low_bit_i << "):" << std::endl;
	std::cout << "   to_lob = " << to_lob << std::endl;
	std::cout << "   to_lod = " << to_lod << std::endl;
	std::cout << "   to_hob = " << to_hob << std::endl;
	std::cout << "   to_hod = " << to_hod << std::endl;
    }

    // ALL THE BITS ARE IN A SINGLE sc_digit:
    //
    // We are guaranteed that the low order digit of from_p has valid data
    // through all its bits even if its the only digit because of sign
    // extension.

    if ( to_hod == to_lod ) {
        sc_digit mask = SC_BIT_MASK1(to_hob-to_lob);
        to_p[to_lod] = ( to_p[to_lod] & ~(mask << to_lob) ) |
                                  ( ( *from_p & mask ) << to_lob) ;

    }

    // BITS ARE ACROSS MULTIPLE sc_digit VALUES:

    else {

        // We need to take into account that the source of the bits could be
	// smaller than the range to be filled. So we have the total amount
	// to be filled, full_hod, and the amount we can fill from the
	// source, src_hod.

        const int full_hod = to_hod - to_lod;
	const int src_hod = full_hod > from_hod ? from_hod : full_hod;
	if ( debug ) {
	    std::cout << "   full_hod = " << full_hod << std::endl;
	    std::cout << "   src_hod = " << src_hod << std::endl;
	}


	// Low order bit is on an sc_digit boundary: no shifting is necessary,
	// just a mask on the high order digit.

	if ( 0 == to_lob ) {
	    int             digit_i;
	    sc_digit        to_hob_mask = SC_BIT_MASK1(to_hob);
	    sc_digit*       dst_p = &to_p[to_lod];
	    const sc_digit* src_p = from_p;

	    // Insert the bits below the high order digit, pad with a sign
	    // extension if necessary.

	    if ( full_hod > src_hod ) {
		for ( digit_i = 0; digit_i <= src_hod; ++digit_i ) {
		    *dst_p++ = *src_p++;
		}
	        const sc_digit fill = 0 > (int)from_p[from_hod] ? -1 : 0;
		for ( ; digit_i < full_hod; ++digit_i ) {
		    *dst_p++ = fill;
		}
		*dst_p = ( *dst_p & ~to_hob_mask ) | ( fill & to_hob_mask );
	    }
	    else {
		for ( digit_i = 0; digit_i < src_hod; ++digit_i ) {
		    *dst_p++ = *src_p++;
		}
		*dst_p = ( *dst_p & ~to_hob_mask ) | ( *src_p & to_hob_mask );
	    }
	}

	// Low order bit is not on an sc_digit boundary: we will need to shift
	// pairs of digits.

	else {
	    int             digit_i;
	    sc_digit*       dst_p = &to_p[to_lod];
	    sc_digit        to_hob_mask = SC_BIT_MASK1(to_hob);
	    int             left_shift = to_lob;
	    sc_digit        low_mask = SC_BIT_MASK(to_lob);
	    int             right_shift = BITS_PER_DIGIT - left_shift;
	    const sc_digit* src_p = from_p;

	    sc_digit low_value = *dst_p & low_mask;
	    if ( full_hod > src_hod ) {
		for ( digit_i = 0; digit_i <= src_hod; ++digit_i ) {
		    sc_digit high_value = *src_p++;
		    *dst_p++ = (high_value << left_shift) | low_value;
		    low_value = high_value >> right_shift;
		}
	        const sc_digit fill = 0 > (int)from_p[from_hod] ? -1 : 0;
		for ( ; digit_i < full_hod; ++digit_i ) {
		    sc_digit high_value = fill;
		    *dst_p++ = (high_value << left_shift) | low_value;
		    low_value = high_value >> right_shift;
		}
		sc_digit highest_value = (fill << left_shift) | low_value;
		// *dst_p = (*dst_p & ~to_hob_mask) | (low_value & to_hob_mask);
		*dst_p = (*dst_p & ~to_hob_mask) | (highest_value & to_hob_mask);
	    }
	    else {
		for ( digit_i = 0; digit_i < src_hod; ++digit_i ) {
		    sc_digit high_value = *src_p++;
		    *dst_p++ = (high_value << left_shift) | low_value;
		    low_value = high_value >> right_shift;
		}
		sc_digit highest_value = *src_p++;
		highest_value = highest_value << left_shift | low_value;
		*dst_p = (*dst_p & ~to_hob_mask) | (highest_value & to_hob_mask);
	    }
	}
    }

    // REVERSE THE BIT ORDER IF NECESSARY:

    if ( reverse ) {
	vector_reverse_bits( to_p, high_bit_i, low_bit_i );
    }
}

// +================================================================================================
// |"vector_mac"
// |
// | This class implements a multiply accumulator.
// |
// | (1) Each product is the multiplication of two 32-bit numbers.
// | (2) Each product is accumulated in a pair of 64-bit numbers, m_low_bits and m_high_bits.
// | (3) m_low_bits contains the addition of the low order 32 bits of each product. 
// | (4) m_high_bits contains the addition of the high order 32 bits of each product. 
// | (5) After a series of calls to add_product():
// |      (a) m_low_bits will be a 64-bit value consisting of additions of the lower order 32 bits
// |          of each product calculated. Its low order 32 bits are the current "digit" for the
// |          additions, its high order 32 bits are the carry from those additions.
// |      (b) Similarly m_high_bits will be a 64-bit value consisting of the additions of the high 
// |          order 32 bits of each product calculated by add_product. Its two 32-bit halves
// |          are 'next digit' value and carry.
// |
// |            m_high_order_bits              m_low_order_bits
// |     +-------------+-------------+   +-------------+-------------+
// |     | next carry  | next digit  |   |current carry|current digit|
// |     +-------------+-------------+   +-------------+-------------+
// |
// | (6) The shift down operation yields the current digit and adjusts for the next digit:
// |      (a) Returns the lower order 32 bits of m_low_bits, 'digit'.
// |      (b) Calculates the current value of the next digit and carry by adding the high order 
// |          32 bits(carry) of m_low_bits to m_high_bits.
// |      (c) Sets m_low_bits to low order 32 bits of m_high_bits (this is the new 'digit'
// |          value, awaiting additional low order adds.)
// |      (d) sets m_high_bits to the upper 32 bits of m_high_bits, (this is the accumulated 
// |          high carry awaiting the addition of high-order adds.)
// |
// |     X = (m_low_order >> 32) + m_high_order_bits
// |     
// |            m_high_order_bits              m_low_order_bits
// |     +-------------+-------------+   +-------------+-------------+
// |     |     0       | high 32 of X|   |     0       | low 32 of X |
// |     +-------------+-------------+   +-------------+-------------+
// |
// +================================================================================================
class vector_mac
{
  protected:
      int64  m_high_bits; // high bits of product.
      uint64 m_low_bits;  // low bits of product.

  public:
    vector_mac() : m_high_bits(0), m_low_bits(0) {}

    inline void add_product( int left, int right )
    {
	int64 product = (int64)left * (int64)right;
	m_low_bits += (unsigned int)product;
	m_high_bits += (product >> BITS_PER_DIGIT);
    }

    inline void add_product( unsigned int left, int right )
    {
	int64 product = (uint64)left * (int64)right;
	m_low_bits += (unsigned int)product;
	m_high_bits += (product >> BITS_PER_DIGIT);
    }

    inline void add_product( int left, unsigned int right )
    {
	int64 product = (int64)left * (uint64)right;
	m_low_bits += (unsigned int)product;
	m_high_bits += (product >> BITS_PER_DIGIT);
    }

    inline void add_product( unsigned int left, unsigned int right )
    {
	uint64 product = (uint64)left * (uint64)right;
	m_low_bits += (unsigned int)product;
	m_high_bits += (product >> BITS_PER_DIGIT);
    }

    void dump()
    {
        std::cout << "    m_high_bits " << m_high_bits << " [0x" << std::hex
	          << m_high_bits << "]" << std::endl;
        std::cout << "    m_low_bits " << m_low_bits << " [0x" << std::hex
	          << m_low_bits << "]" << std::endl;
    }

    sc_digit low_bits() { return (sc_digit)m_low_bits; }

    inline sc_digit shift_down()
    {
        sc_digit result = (sc_digit)m_low_bits;
        m_high_bits += (m_low_bits >> BITS_PER_DIGIT);
	m_low_bits = (sc_digit)m_high_bits;
	m_high_bits >>= BITS_PER_DIGIT;
	return result;
    }
};

// +----------------------------------------------------------------------------
// |"vector_multiply"
// |
// | This inline function will multiply two vectors of sc_digits representing
// | signed numbers. Some sign extension is left to the caller.
// |
// | Notes:
// |   (1) We use standard long multiplication to generate the result, using
// |
// |       Given two numbers, A and B:
// |
// |         A:   aS a5 a4 a3 a2 a1
// |         B:         bS b3 b2 b1
// |
// |       Let BPD represent the number of bits per digit.
// |
// |       The high-order digit variables for this example:
// |           * shorter_hod is 4 
// |           * longer_hod is 6
// |
// |       Long multiplication consists of calculating R from A and B:
// |
// |         R: rS r6 r5 r4 r3 r2 r1
// |
// |       The raw terms are:
// |
// |         raw1 =               a1 * b1
// |         raw2 = (raw1 >> BPD) + a2 * b1 + a1 * b2
// |         raw3 = (raw2 >> BPD) + a3 * b1 + a2 * b2 + a1 * b3
// |         raw4 = (raw3 >> BPD) + a4 * b1 + a3 * b2 + a2 * b3 + a1 * bS
// |         raw5 = (raw4 >> BPD) + a5 * b1 + a4 * b2 + a3 * b3 + a2 * bS
// |         raw6 = (raw5 >> BPD) + aS * b1 + a5 * b2 + a4 * b3 + a3 * bS
// |         raw7 = (raw6 >> BPD)           + aS * b2 + a5 * b3 + a4 * bS
// |         raw8 = (raw7 >> BPD)           + aS * b3 + a5 * bS
// |         raw9 = (raw8 >> BPD)           + aS * bS
// |         rawS = (raw9 >> BPD)
// |
// |       The resultant digits are:
// |         r1 = raw1 % BPD
// |         r2 = raw2 % BPD
// |         r3 = raw3 % BPD
// |         r4 = raw4 % BPD
// |         r5 = raw5 % BPD
// |         r6 = raw6 % BPD
// |         r7 = raw7 % BPD
// |         r8 = raw8 % BPD
// |         r9 = raw9 % BPD
// |         rS = rawS % BPD
// |
// |   (2) It is assumed that result_hod >= longer_hod+shorter_hod.
// |
// | Arguments:
// |     left_hod   =  number of sc_digits in the left operand.
// |     left_p     -> vector of sc_digits representing the left operand.
// |     right_hod  =  number of sc_digits in the right operand.
// |     right_p    -> vector of sc_digits representing the right operand.
// |     result_hod =  number of sc_digits in the result.
// |     result_p   -> vector of sc_digits representing the result.
// +----------------------------------------------------------------------------
inline
void
vector_multiply( int             left_hod,
                 const sc_digit* left_p,
                 int             right_hod,
                 const sc_digit* right_p,
                 int             result_hod,
                 sc_digit*       result_p)
{
    int             longer_hod;
    const sc_digit* longer_p;
    vector_mac      mac;
    int             result_i;
    int             shorter_hod;
    const sc_digit* shorter_p;

    // Trim leading zeros and all ones sc_digits to speed up the operation, and use the 
    // shorter result operand is as the multiplier

    if ( true ) {
	left_hod = vector_find_significant_hod( left_hod, left_p ); 
	right_hod = vector_find_significant_hod( right_hod, right_p ); 
    }
    if ( left_hod >= right_hod ) { 
	longer_hod = left_hod; 
	longer_p = left_p; 
	shorter_hod = right_hod; 
	shorter_p = right_p; 
    } 
    else { 
	longer_hod = right_hod; 
	longer_p = right_p; 
	shorter_hod = left_hod; 
	shorter_p = left_p; 
    } 


    // Set the low order digits in the result that are the product of unsigned digits from both 
    // operands. For the example in note (1) this is:
    //    raw1 = a1 * b1
    //    r1 = raw1 % BPD
    //    raw2 = a2 * b1 + a1 * b2 + (raw1 >> BPD)
    //    r2 = raw2 % BPD
    //    raw3 = a3 * b1 + a2 * b2 + a1 * b3 + (raw2 >> BPD)
    //    r3 = raw3 % BPD

    for (result_i=0; result_i < shorter_hod; ++result_i ) {
	for (int other_i=0; other_i <= result_i; ++other_i ) {
	    mac.add_product( longer_p[result_i-other_i], shorter_p[other_i]);
	}
	result_p[result_i] = mac.shift_down();
    }

    // Set the next higher order digits that are the result of unsigned
    // digits in the longer operand times all the digits in the shorter
    // operand. For example 1 at this point the mac contains raw3 shifted,
    // and the digits set will be:
    //    raw4 = (raw3 >> BPD) + a4 * b1 + a3 * b2 + a2 * b3 + a1 * bS
    //    r4 = raw4 % BPD
    //    raw5 = (raw4 >> BPD) + a5 * b1 + a4 * b2 + a3 * b3 + a2 * bS
    //    r5 = raw5 % BPD
    //    raw6 = (raw5 >> BPD) + aS * b1 + a5 * b2 + a4 * b3 + a3 * bS
    //    r6 = raw6 % BPD
    //    raw7 = (raw6 >> BPD)           + aS * b2 + a5 * b3 + a4 * bS
    //    r7 = raw7 % BPD
    //    raw8 = (raw7 >> BPD)           + aS * b3 + a5 * bS
    //    r8 = raw8 % BPD

    for ( result_i = shorter_hod; result_i < longer_hod; ++result_i ) {
	mac.add_product( longer_p[result_i-shorter_hod],
	                 (int)shorter_p[shorter_hod] );
	for(int other_i=0; other_i < shorter_hod; ++other_i ) {
            mac.add_product( longer_p[result_i-other_i], shorter_p[other_i] );
	}
	result_p[result_i] = mac.shift_down();
    }

    // Set the digits in the result that are the product of all the
    // longer operand digits times all the digits in the shorter operand.
    // In example 1 at this point the mac contains raw8 shifted and the
    // digit to be set will be r9:
    //     raw9 = (raw8 >> BPD)           + aS * bS
    //     r9 = raw9 % BPD

    for ( result_i=longer_hod; result_i < longer_hod+shorter_hod;
          ++result_i ) {
	mac.add_product( longer_p[result_i-shorter_hod],
			 (int)shorter_p[shorter_hod] );
	for ( int other_i=result_i-(longer_hod-1); other_i < shorter_hod;
	      other_i++ ) {
            mac.add_product( longer_p[result_i-other_i],
	                  shorter_p[other_i] );
	}
        mac.add_product( (int)longer_p[longer_hod],
	                 shorter_p[result_i-longer_hod] );
	result_p[result_i] = mac.shift_down();
    }
        mac.add_product((int)longer_p[longer_hod], (int)shorter_p[shorter_hod]);
	result_p[longer_hod+shorter_hod] = mac.shift_down();

    // Set the next higher order digit to the signed product of the
    // operand's high order digits:
    //
    // If there are further digits in the result fill the first with the
    // remaining accumulation. Then fill any remaining digits based on the
    // on the sign of the accumulation.
    //     rawS = (raw9 >> BPD)
    //     rS = rawS % BPD

    if ( (result_hod+1) >= longer_hod+shorter_hod+1) {
        if ( (result_hod) >= longer_hod+shorter_hod+1 ) {
	    result_p[longer_hod+shorter_hod+1] = mac.low_bits(); // mac.shift_down();
	    sc_digit fill = 0 > (int)result_p[longer_hod+shorter_hod+1] ? -1:0;
	    for ( result_i = longer_hod+shorter_hod+2; result_i <= result_hod;
	          ++result_i ) {
	        result_p[result_i] = fill;
	    }
        }
    }
}

// +----------------------------------------------------------------------------
// |"vector_or"
// |
// | This function ors two signed sc_digit vectors. The second vector must be
// | shorter, or the same length, as the longer one.
// |
// | Template arguments:
// |     SL - true if longer operand is signed.
// |     SS - true if shorter operand is signed.
// | Arguments:
// |     longer_hod   = index of high order digit in 'longer_p' and 'result_p'.
// |     longer_p     = digits in the longer operand.
// |     shorter_hod  = index of high order digit in 'shorter_p'
// |     shorter_p    = digits in the shorter operand
// |     result_p     = digits to be filled in in the result, this is the same
// |                    number of digits as the longer operand.
// +----------------------------------------------------------------------------
template<bool SL, bool SS>
inline
void
vector_or( const int       longer_hod,
	   const sc_digit* longer_p,
	   const int       shorter_hod,
	   const sc_digit* shorter_p,
	   sc_digit*       result_p )
{
    if ( 0 == longer_hod ) {
        *result_p = *longer_p | *shorter_p;
	return;
    }

    int       digit_i;

    // And the two operands to the length of the shorter operand:

    for ( digit_i = 0; digit_i <= shorter_hod; ++digit_i ) {
	*result_p++ = *longer_p++ | shorter_p[digit_i];
    }

    // If the shorter operand is negative fill out the remaining entries
    // with ones, otherwise with the longer operand.

    if ( longer_hod > shorter_hod ) {
        if ( SS && 0 > (int)shorter_p[shorter_hod] ) {
	    for ( ; digit_i <= longer_hod; ++digit_i ) {
		*result_p++ = (sc_digit)-1;
	    }
	}
	else {
	    for ( ; digit_i <= longer_hod; ++digit_i ) {
		*result_p++ = *longer_p++;
	    }
	}
    }
}

// +----------------------------------------------------------------------------
// |"vector_or_reduce"
// |
// | This function overload performs an "or reduction" on the supplied vector,
// | returning the result.
// |
// | Arguments:
// |     bits_n   = number of bits in the vector that are valid.
// |     target_p = digits containing the bits.
// | Result:
// |     true if any bit is on in the vector, false if not.
// +----------------------------------------------------------------------------
inline
bool
vector_or_reduce( const int       bits_n,
	          const sc_digit* target_p )
{
    const int hod = SC_DIGIT_INDEX(bits_n-1);

    // First check the whole sc_digits:

    for ( int digit_i = 0; digit_i < hod; ++digit_i ) {
        if ( target_p[digit_i] != (sc_digit)0 ) {
	    return true;
	}
    }

    // Check the high order digit:
    const sc_digit mask =
        ~( (std::numeric_limits<sc_digit>::max()-1) << SC_BIT_INDEX(bits_n-1) );
    if ( ( target_p[hod] & mask ) != 0 ) {
        return true;
    }
    return false;
}

// +----------------------------------------------------------------------------
// |"vector_or_reduce"
// |
// | This function overload performs an "or reduction" on the requested range
// | of bits in the supplied vector, returning the result.
// |
// | Arguments:
// |     target_p = digits containing the bits.
// |     left_i   = left-most bit in the range to be checked.
// |     right_i  = right-most bit in the range to be checked.
// | Result:
// |     true if any bit is on in the vector, false if not.
// +----------------------------------------------------------------------------
inline
bool
vector_or_reduce( const sc_digit* target_p,
                  const int       left_i,
	          const int       right_i )
{
    const int left_hod = SC_DIGIT_INDEX(left_i);
    const int right_hod = SC_DIGIT_INDEX(right_i);

    // All the bits are in a single sc_digit:

    if ( left_hod == right_hod ) {
        const sc_digit mask =
            ~( (std::numeric_limits<sc_digit>::max()-1) << (left_i-right_i) );
	return ( ( target_p[right_hod] & mask ) != 0 ) ? true : false;
    }

    const int right_index = SC_BIT_INDEX(right_i);

    // The low order bit is not on an sc_digit boundary, check the low
    // order digit, then the whole sc_digits above it.

    if ( right_index ) {
        const sc_digit mask =
            (std::numeric_limits<sc_digit>::max() << (right_index-1) );
        if ( ( target_p[right_hod] & mask ) != (sc_digit)0 ) {
            return true;
        }
        for ( int digit_i = right_hod+1; digit_i < left_hod; ++digit_i ) {
            if ( target_p[digit_i] != (sc_digit)0 ) {
                return true;
            }
        }
    }

    // The lower order bit is on an sc_digit boundary, check the whole
    // sc_digits below the high order digit.

    else {
	for ( int digit_i = right_hod; digit_i < left_hod; ++digit_i ) {
	    if ( target_p[digit_i] != (sc_digit)0 ) {
		return true;
	    }
        }
    }

    // Check the high order sc_digit:

    const sc_digit mask =
        ~( (std::numeric_limits<sc_digit>::max()-1) << SC_BIT_INDEX(left_i) );
    if ( ( target_p[left_hod] & mask ) != 0 ) {
        return true;
    }
    return false;
}

// +----------------------------------------------------------------------------
// |"vector_shift_left"
// |
// | This function overload shifts the supplied vector left by the supplied
// | number of bits placing them at the appropriate offset  in the supplied
// | target vector. The low order bits will be zeroed. The source will be
// | sign extended if extra bits are required for the target.
// |
// | Arguments:
// |     from_digits_n =  number of digits in 'from_p'
// |     from_p        -> digits of source.
// |     to_digits_n   =  number of highest digits in 'to_p'
// |     to_p          -> digits in the target.
// |     shift_n       =  number of bits to shift.
// +----------------------------------------------------------------------------
inline
void
vector_shift_left( const int       from_digits_n,
                   const sc_digit* from_p,
                   const int       to_digits_n,
		   sc_digit*       to_p,
		   const int       shift_n )
{
    // If the shift is not positive we are done:

    if ( shift_n < 0 ) {
        return;
    }

    int      carry_shift_n; // amount to shift a carry down by when composing a to_p digit.
    int      from_hod;      // index of high order digit in 'from_p'.
    int      from_i;        // digit in from_p now accessing.
    int      from_shift_n;  // amount to shift a from_p digit down by when composing a to_p digit.
    sc_digit fill;          // high order fill value, either zero or all ones.
    int      to_hod;        // index of high order digit in 'to_p'.
    int      to_i;          // digit in to_p now accessing.
    int      to_start_hod;  // first digit in to_p to receive a from_p digit.
    int      to_end_hod;    // last digit in to_p to recieve a from_p digit.
    
    from_hod = from_digits_n-1;
    from_shift_n = SC_BIT_INDEX(shift_n);
    carry_shift_n = 32-from_shift_n;
    to_hod = to_digits_n-1;
    to_start_hod = VEC_MIN( SC_DIGIT_INDEX(shift_n), to_hod );
    to_end_hod = VEC_MIN( to_hod, to_start_hod+from_hod );
    fill = 0 > (int)from_p[from_hod] ? ~0u : 0;

    if ( false ) {
	std::cout << "from_hod      = " << std::dec << from_hod << std::endl;
	std::cout << "to_hod        = " << to_hod << std::endl;
	std::cout << "shift_n       = " << shift_n << std::endl;
	std::cout << "carry_shift_n = " << carry_shift_n << std::endl;
	std::cout << "from_shift_n  = " << from_shift_n << std::endl;
	std::cout << "to_start_hod  = " << to_start_hod << std::endl;
	std::cout << "to_end_hod    = " << to_end_hod << std::endl;
	std::cout << "fill          = " << std::hex << fill << std::dec << std::endl;
    }

    // Zero any whole low order words below our transfer point:

    for ( to_i = 0; to_i < to_start_hod; ++to_i ) {
        to_p[to_i] = 0;
    }

    // If the shift is larger than the our target we are done:

    if ( to_i > to_hod ) {
        std::cerr << "vector_shift_left: shift larger than target, word " << to_i << " > "
	          << to_hod << std::endl;
        return;
    }

    // Shift point is on an sc_digit boundary. 

    if ( 0 == from_shift_n ) {
        for ( from_i=0; to_i <= to_end_hod; ++from_i, ++to_i ) {
	    to_p[to_i] = from_p[from_i];
        }
	if ( to_i <= to_hod ) {
	    fill = 0 > (int)from_p[from_hod] ? -1 : 0;
	    for ( ; to_i <= to_hod; ++to_i ) {
	        to_p[to_i] = fill;
	    }
	}
    }

    // The messy case we need to shift within a digit:

    else {
        from_i = 0;
        sc_digit carry = 0; 
        for ( ; to_i <= to_end_hod; ++from_i, ++to_i ) {
	    sc_digit from_digit = from_p[from_i];
            to_p[to_i] = (from_digit << from_shift_n) | carry;
            carry = from_digit >> carry_shift_n;
        }
	if ( to_i <= to_hod ) { to_p[to_i] = (fill << from_shift_n) | carry; }
	for ( to_i = to_i+1; to_i <= to_hod; ++to_i ) {
	    to_p[to_i] = fill;
	}
    }
}

// +----------------------------------------------------------------------------
// |"vector_shift_left"
// |
// | This function shifts left the supplied vector by the supplied number of
// | bits.
// | This function shifts left the supplied vector by the supplied number of
// | bits. Because the order of the digits in the array have the low order bits
// | at index 0 and the high order bits at index N, the actual shifting is to 
// | the right from lower index to higher index.
// |
// |
// | Arguments:
// |     target_n   =  number of digits in 'target_p'
// |     target_p   -> digits in the target.
// |     shift_n    =  number of bits to shift.
// +----------------------------------------------------------------------------
inline
void
vector_shift_left( const int target_n,
		   sc_digit* target_p,
		   const int shift_n )
{
    if (shift_n <= 0) {
	if (shift_n < 0) {
            std::cerr << "vector_shift_left: negative shift encountered " << shift_n << std::endl;
            sc_assert(false);
	}
        return;
    }
    int shift_remaining = shift_n;

    // Shift left whole digits if shift_n is large enough.

    if (shift_n >= (int) BITS_PER_DIGIT) {

      int digits_n;

      if ( SC_BIT_INDEX(shift_n) == 0 ) {
        digits_n = SC_DIGIT_INDEX(shift_n);
        shift_remaining = 0;
      }
      else {
        digits_n = ( (shift_n+BITS_PER_DIGIT-1)/BITS_PER_DIGIT ) - 1;
        shift_remaining -= digits_n * BITS_PER_DIGIT;
      }

      // Shift left for digits_n digits.

      if (digits_n) {

        for (int digit_i = target_n - 1; digit_i >= digits_n; --digit_i)
          target_p[digit_i] = target_p[digit_i - digits_n];

        const int clear_n = VEC_MIN( digits_n, target_n );
        for (int digit_i = 0; digit_i < clear_n; ++digit_i ) {
            target_p[digit_i] = 0;
        }

      }

      if (shift_remaining == 0)
        return;

    }

    // Shift left if shift_remaining < BITS_PER_DIGIT.

    sc_digit* target_iter_p = target_p;
    sc_digit* target_end_p = target_iter_p + target_n;

    int carry_shift = BITS_PER_DIGIT - shift_remaining;
    sc_digit mask = (sc_digit)one_and_ones(carry_shift);

    sc_carry carry = 0;

    while (target_iter_p < target_end_p) {
      sc_digit target_value = (*target_iter_p);
      (*target_iter_p++) = (sc_digit)(((target_value & mask) << shift_remaining) | carry);
      carry = target_value >> carry_shift;
    }
}

// +----------------------------------------------------------------------------
// |"vector_shift_right"
// |
// | This function shifts right the supplied vector by the supplied number of
// | bits. Because the order of the digits in the array have the low order bits
// | at index 0 and the high order bits at index N, the actual shifting is to 
// | the left from higher index to lower index.
// |
// | Arguments:
// |     target_n   =  number of digits in 'target_p'
// |     target_p   -> digits in the target.
// |     bits_n     =  number of bits to shift.
// |     fill       =  fill for high order bits.
// +----------------------------------------------------------------------------
inline
void
vector_shift_right( const int       target_n,
                    sc_digit*       target_p,
		    int             bits_n,
		    const sc_digit  fill )
{
    if (bits_n <= 0) {
        return;
    }

    // PERFORM INTER-DIGIT SHIFTING:
    //
    // (1) If the low order digit (target_p[0]) does not contain the intra-digit shift point move 
    //     all the digits in target_p down so that it does contain the shift point. 
    // (2) In the degenerate case, where the shift is a multiple of the number of bits in a
    //     digit we are done.

    if (bits_n >= (int) BITS_PER_DIGIT) {

        int nd;

        if (SC_BIT_INDEX(bits_n) == 0) {
            nd = SC_DIGIT_INDEX(bits_n);
            bits_n = 0;
        }
        else {
            nd = DIV_CEIL(bits_n) - 1;
            bits_n -= nd * BITS_PER_DIGIT;
        }

        if (nd) {

          // Shift right for nd digits.

          for (int digit_i = 0; digit_i < (target_n - nd); ++digit_i) {
              target_p[digit_i] = target_p[digit_i + nd];
	  }

          for (int digit_i = target_n - VEC_MIN( nd, target_n );
             digit_i < target_n; ++digit_i) {
              target_p[digit_i] = fill;
          }

          if (bits_n == 0)
              return;
        }
    }

    // PERFORM INTRA-DIGIT SHIFTING:
    //
    // (1) At this point target_p[0] contains the shift point. Roll through the digits in target_p
    //     from top to bottom performing shifts smaller than BITS_PER_DIGIT bits.
    // (2) Each digit will contain it high order bits as low order bits, and the next digit's
    //     low order bits as its high order bits.

    int      other_shift_n = BITS_PER_DIGIT - bits_n;
    sc_digit carry = fill << other_shift_n;
    sc_digit *target_iter_p = (target_p + target_n );

    while (target_p < target_iter_p) {
        sc_digit target_val = *--target_iter_p;
        (*target_iter_p) = (target_val >> bits_n) | carry;
        carry = target_val << other_shift_n;
    }
}

// +----------------------------------------------------------------------------
// |"vector_subtract_longer"
// |
// | This function subtracts the longer of two operands from the shorter. The
// | algorithm is basically that from grade school:
// |
// |                BS B3 B2 B1
// |     -    AS A5 A4 A3 A2 A1
// |       --------------------
// |       R7 R6 R5 R4 R3 R2 R1
// |
// |     Let BPD represent the number of bits per digit.
// |
// |     Raw1 = B1 - A1
// |     Raw2 = (Raw1 >> BPD) - A2 + B2
// |     Raw3 = (Raw2 >> BPD) - A3 + B3
// |     Raw4 = (Raw3 >> BPD) - A4 + (signed)BS
// |     Raw5 = (Raw4 >> BPD) - A5
// |     Raw6 = (Raw5 >> BPD) - (signed)AS
// |     Raw7 = (Raw6 >> BPD)
// |
// |     R1 = Raw1 % BPD
// |     R2 = Raw2 % BPD
// |     R3 = Raw3 % BPD
// |     R4 = Raw4 % BPD
// |     R5 = Raw5 % BPD
// |     R6 = Raw6 % BPD
// |     R7 = Raw7 % BPD
// |
// | Arguments:
// |     longer_hod   = index of high order digit in 'longer_p'
// |     longer_p     = digits in the longer operand.
// |     shorter_hod  = index of high order digit in 'shorter_p'
// |     shorter_p    = digits in the shorter operand
// |     result_hod   = index of high order digit in 'result_p'
// |     result_p     = digits to be filled in in the result.
// +----------------------------------------------------------------------------
inline
void
vector_subtract_longer( const int       longer_hod,
			const sc_digit* longer_p,
			const int       shorter_hod,
			const sc_digit* shorter_p,
			const int       result_hod,
			sc_digit*       result_p )
{
    if ( result_hod == 0 ) {
        *result_p = *shorter_p - *longer_p;
	return;
    }

    int64 borrow = 0;
    int   digit_i;

    // Subtract the low-order unsigned parts of the operands:

    for ( digit_i = 0; digit_i < shorter_hod; ++digit_i ) {
        borrow += *shorter_p++;
        borrow -= *longer_p++;
	*result_p++ = (sc_digit)borrow;
	borrow >>= BITS_PER_DIGIT;
    }

    // The operands are the same length: process the high order digits as
    // signed values:

    if ( longer_hod == shorter_hod ) {
	borrow += (int64)(int)*shorter_p++;
	borrow -= (int64)(int)*longer_p++;
	*result_p++ = (sc_digit)borrow;
	borrow >>= BITS_PER_DIGIT;
    }

    // The operands are different lengths:
    //   (a) Add the shorter operand's high order digit as a signed value.
    //   (b) Subtract the longer operand's values below its high order digit as
    //       unsigned.
    //   (c) Subtract the high order digit as signed.

    else {
	borrow += (int64)(int)*shorter_p++;
	borrow -= *longer_p++;
	*result_p++ = (sc_digit)borrow;
	borrow >>= BITS_PER_DIGIT;

	for ( ++digit_i; digit_i < longer_hod; ++digit_i ) {
	    borrow -= *longer_p++;
	    *result_p++ = (sc_digit)borrow;
	    borrow >>= BITS_PER_DIGIT;
	}

	borrow -= (int64)(int)*longer_p++;
	*result_p++ = (sc_digit)borrow;
	borrow >>= BITS_PER_DIGIT;
    }

    // If there is an additional high order digit in the result assign it the
    // borrow value:

    if ( result_hod > longer_hod ) {
        *result_p = (sc_digit)borrow;
    }
}

// +----------------------------------------------------------------------------
// |"vector_subtract_shorter"
// |
// | This function subtracts the shorter of two operands from the longer. The
// | algorithm is basically that from grade school:
// |
// |          AS A5 A4 A3 A2 A1
// |     -          BS B3 B2 B1
// |       --------------------
// |       R7 R6 R5 R4 R3 R2 R1
// |
// |     Let BPD represent the number of bits per digit.
// |
// |     Raw1 = A1 - B1
// |     Raw2 = (Raw1 >> BPD) + A2 - B2
// |     Raw3 = (Raw2 >> BPD) + A3 - B3
// |     Raw4 = (Raw3 >> BPD) + A4 - (signed)BS
// |     Raw5 = (Raw4 >> BPD) + A5
// |     Raw6 = (Raw5 >> BPD) + (signed)AS
// |     Raw7 = (Raw6 >> BPD)
// |
// |     R1 = Raw1 % BPD
// |     R2 = Raw2 % BPD
// |     R3 = Raw3 % BPD
// |     R4 = Raw4 % BPD
// |     R5 = Raw5 % BPD
// |     R6 = Raw6 % BPD
// |     R7 = Raw7 % BPD
// |
// |
// | Arguments:
// |     longer_hod   = index of high order digit in 'longer_p'
// |     longer_p     = digits in the longer operand.
// |     shorter_hod  = index of high order digit in 'shorter_p'
// |     shorter_p    = digits in the shorter operand
// |     result_hod   = index of high order digit in 'result_p'
// |     result_p     = digits to be filled in in the result.
// +----------------------------------------------------------------------------
inline
void
vector_subtract_shorter( const int       longer_hod,
			 const sc_digit* longer_p,
			 const int       shorter_hod,
			 const sc_digit* shorter_p,
			 const int       result_hod,
			 sc_digit*       result_p )
{
    if ( result_hod == 0 ) {
        *result_p = *longer_p - *shorter_p;
	return;
    }

    int64 borrow = 0;
    int   digit_i;

    // Subtract the low-order unsigned parts of the operands:

    for ( digit_i = 0; digit_i < shorter_hod; ++digit_i ) {
        borrow -= *shorter_p++;
        borrow += *longer_p++;
	*result_p++ = (sc_digit)borrow;
	borrow >>= BITS_PER_DIGIT;
    }

    // The operands are the same length: treat the high order digits as
    // signed values:

    if ( longer_hod == shorter_hod ) {
	borrow -= (int64)(int)*shorter_p++;
	borrow += (int64)(int)*longer_p++;
	*result_p++ = (sc_digit)borrow;
	borrow >>= BITS_PER_DIGIT;
    }

    // The operands are different lengths:
    //   (a) Subtract the shorter operand's high order digit as a signed value.
    //   (b) Add the longer operand's values below its high order digit as
    //       unsigned.
    //   (c) Add the high order digit as signed.

    else {
	borrow -= (int64)(int)*shorter_p++;
	borrow += *longer_p++;
	*result_p++ = (sc_digit)borrow;
	borrow >>= BITS_PER_DIGIT;

	for ( ++digit_i; digit_i < longer_hod; ++digit_i ) {
	    borrow += *longer_p++;
	    *result_p++ = (sc_digit)borrow;
	    borrow >>= BITS_PER_DIGIT;
	}

	borrow += (int64)(int)*longer_p++;
	*result_p++ = (sc_digit)borrow;
	borrow >>= BITS_PER_DIGIT;
    }

    // If there is an additional high order digit in the result assign it the
    // borrow value:

    if ( result_hod > longer_hod ) {
        *result_p = (sc_digit)borrow;
    }
}

// +----------------------------------------------------------------------------
// |"vector_twos_complement"
// |
// | This inline function complements the value in the supplied vector.
// |
// | Arguments:
// |     target_n = number of digits in 'target_p'
// |     target_p = vector of digits to be complemented.
// +----------------------------------------------------------------------------
inline
void
vector_twos_complement( const int target_n,
	                sc_digit* target_p )
{
    long long carry = 0;
    for ( int digit_i = 0; digit_i < target_n; ++digit_i ) {
        carry -= target_p[digit_i];
	target_p[digit_i] = (sc_digit)carry;
	carry >>= BITS_PER_DIGIT;
    }
}

// +----------------------------------------------------------------------------
// |"vector_twos_complement"
// |
// | This inline function complements the value of the supplied vector,
// | placing it in the supplied destination vector.
// |
// | Arguments:
// |     source_hod = index of the high order 'source_p' digit.
// |     source_p   = vector of digits to be complemented.
// |     target_p   = vector of digits to receive the complement.
// +----------------------------------------------------------------------------
inline
void
vector_twos_complement( const int       source_hod,
	                const sc_digit* source_p,
	                sc_digit*       target_p )
{
    long long carry = 0;
    for ( int digit_i = 0; digit_i <= source_hod; ++digit_i ) {
        carry -= *source_p++;
	*target_p++ = (sc_digit)carry;
	carry >>= BITS_PER_DIGIT;
    }
}

// +----------------------------------------------------------------------------
// |"vector_divide"
// |
// | This function implements division of the supplied numerator by the
// | supplied denominator. Both are vectors of sc_digit (radix32) values, but
// | are accessed at times as if they were vectors of radix16 values. Either
// | the quotient of the division, or the remainder, or both may be returned
// | depending upon the caller's arguments.
// |
// |
// | Notes:
// |   (1) The division is always performed using non-negative values.
// |       Conversion of negative values, as well as conversion to negative
// |       results is performed to allow the use of non-negative values.
// |
// |   (2) The quotient and remainder values are treated as radix-16 values
// |       at times.
// |
// |   (3) The algorithm is basically "long division" from grade school:
// |
// |                     X   Y   Z
// |         +--------------------
// |     abc |   A   B   C   D   E
// |       -   X*a X*b X*c
// |           -----------
// |           r1A r1B r1C   D
// |       -       Y*a Y*b Y*c
// |           ---------------
// |	           r2B r2C r2D   E
// |       -           Z*a Z*b Z*c
// |	           ---------------
// |	               r3C r3D r3E
// |
// |       The algorithm consists of:
// |         (a) Creating a remainder vector that initially is the numerator
// |             for the division operation.
// |         (b) Calculating a potential quotient radix16 digit using 64-bit
// |             native division hardware. So divide the high-order 4 radix16
// |             digits of the current remainder by the the high-order 3
// |             radix16 digits of the denominator.
// |         (c) The potential quotient digit is then multiplied by the
// |             denominator and that value is subtracted from the remainder.
// |         (d) If the result is negative then the quotient digit was too
// |             large by one, so adjust the remainder by adding the
// |             denominator, and decrement the quotient digit by one.
// |         (e) Store the quotient digit into the quotient vector.
// |         (f) Steps (b) through (e) are repeated until all the digits in
// |             the quotient have been calculated.
// |
// | Template arguments:
// |     SN = true if numerator is signed, false if not.
// |     SD = true if denominator is signed, false if not.
// | Arguments:
// |     numerator_n   = number of digits in numerator_p.
// |     numerator_p   = vector of digits containing the numerator.
// |     denominator_n = number of digits in denominator_p.
// |     denominator_p = vector of digits containing the denominator.
// |     quotient_n    = number of digits in 'quotient_p' or zero.
// |     quotient_p    = vector of digits to receive quotient of the divide.
// |     remainder_n   = number of digits in remainder_p or zero if no remainder is desired.
// |     remainder_p   = vector of digits to receive remainder of the divide, (NULL - none desired).
// | Result:
// |   true if the operation was successful, false if a divide by zero was
// |   detected.
// +----------------------------------------------------------------------------
template<bool SN, bool SD>
inline
bool
vector_divide( const int       numerator_n,
               const sc_digit* numerator_p,
	       const int       denominator_n,
	       const sc_digit* denominator_p,
	       const int       quotient_n,
	       sc_digit*       quotient_p,
	       const int       remainder_n,
	       sc_digit*       remainder_p )
{
    // Initialize the quotient to zero and the remainder to the numerator,
    // if their return is requested:
    if ( 0 != quotient_n ) {
	for( int quot_i=0; quot_i < quotient_n; ++quot_i) {
	    quotient_p[quot_i] = 0;
	}
    }
    if ( 0 != remainder_n ) {
	for( int remain_i=0; remain_i < remainder_n; ++remain_i) {
	    remainder_p[remain_i] = numerator_p[remain_i];
	}
    }

    // CONVERT THE OPERANDS TO NON-NEGATIVE VALUES:
    //
    // If a complement is necessary use a temporary buffer, otherwise use
    // the vector that was passed in from the caller.

    bool     negative_numerator = false;
    bool     negative = false;

    if ( SN && ( 0 > (int)numerator_p[numerator_n-1]  ) ) {
	sc_digit* numerator_complement_p = sc_temporary_digits.allocate(numerator_n+1);
        vector_twos_complement( numerator_n-1, numerator_p,
	                        numerator_complement_p );
	numerator_p = numerator_complement_p;
	negative = true;
	negative_numerator = true;
    }

    if ( SD && (0 > (int)denominator_p[denominator_n-1]) ) {
	sc_digit* denominator_complement_p = sc_temporary_digits.allocate(denominator_n+1);
        vector_twos_complement( denominator_n-1, denominator_p,
	                        denominator_complement_p );
	denominator_p = denominator_complement_p;
	negative = !negative;
    }

    // SET UP FOR THE CALCULATION:
    //
    // (1) Trim the numerator and denominator to highest order non-zero value.
    // (2) Create the denominator value to use in guesses at the quotient
    //     digit.
    // (3) Initialize the remainder array to the value of the numerator.

    int       denom_32_hod;     // high order radix32 denominator digit.
    int       numer_32_hod;     // most significant radix32 numerator digit.

    denom_32_hod = vector_skip_leading_zeros( denominator_n-1, denominator_p );
    bool denom_16_hod_odd = (bool)(denominator_p[denom_32_hod] >> 16);
    int  denom_16_hod= 2*denom_32_hod + denom_16_hod_odd;

    for ( numer_32_hod = numerator_n-1;
          numer_32_hod > 0 && !numerator_p[numer_32_hod];
          --numer_32_hod ) {
        continue;
    }
    int  numer_16_hod = 2*numer_32_hod + (bool)(numerator_p[numer_32_hod]>>16);

    // If the denom is zero we are in trouble...

    if ( !denom_32_hod && !denominator_p[0]) {
        return false;
    }

    // If the numerator was smaller than the denominator return a zero
    // quotient and the numerator as the remainder:

    if ( numer_16_hod < denom_16_hod ) {
	return true;
    }


    // We will estimate the quotient digits by dividing the the first four
    // radix-16 digits of the current "remainder" value by the first three
    // radix-16 digits of the denominator, so assemble those three denominator
    // digits as a value we can use over and over. Use zeros for any missing
    // low order digits so that we have three properly aligned radix-16 digits.

    uint64 denom_value = (uint64)denominator_p[denom_32_hod] <<
                  (16 << (int)!denom_16_hod_odd);
    if ( denom_32_hod ) {
        denom_value |= denominator_p[denom_32_hod-1] >>
	               (denom_16_hod_odd ? 16 : 0);
    }

    // Copy the numerator into the remainder to start off the division process
    // and zero the extra high order radix32 digit of the remainder:

    sc_digit* remain_work_p = sc_temporary_digits.allocate( numerator_n+10 ); 

    for (int remain_32_i = 0; remain_32_i <= numer_32_hod; ++remain_32_i ) {
	remain_work_p[remain_32_i] = numerator_p[remain_32_i];
    }
    remain_work_p[numer_32_hod+1] = 0;

    // CALCULATE THE QUOTIENT AND REMAINDER:
    //
    // This is done with a pair of nested loops that are applied to the radix-16
    // representation of the numerator and denominator:
    //   (a) The outer loop runs downwards from the most significant radix-16
    //       digit in the numerator through the most significant radix-16 in
    //       the denominator, effectively the values that will contribute to an
    //       integer result (the quotient). It calculates the next guess at
    //       a quotient radix16 digit.
    //   (b) The inner loop runs upwards from the least significant radix32
    //       digit in the denominator up through the most significant radix32
    //       digit in the denominator. It subtracts the product of the
    //       quotient digit times the denominator from the remainder.

    for ( int quot_16_i = numer_16_hod - denom_16_hod; quot_16_i >= 0;
	  --quot_16_i ) {

	// Calculate some values we need to navigate our vectors:

	bool quot_16_odd   = quot_16_i & 1;
	int  quot_32_i     = quot_16_i >> 1;
	int  remain_16_i   = quot_16_i + denom_16_hod;
	int  remain_32_i   = remain_16_i >> 1;
	bool remain_16_odd = remain_16_i & 1;

	// Make an educated guess at the next quotient digit by dividing
	// the current high-order 4 radix-16 digits of the remainder by the
	// 3 high-order radix-16 digits of the denominator. Potentially the
	// result can be larger than the radix, so adjust it down by 1
	// if necessary.

	sc_digit low_order_bits = remain_32_i > 0 ? remain_work_p[remain_32_i-1] :
						    (sc_digit) 0;
	uint64 numer_value;
	if ( remain_16_odd ) {
            numer_value = ( (uint64)remain_work_p[remain_32_i+1] << 48 ) |
	                  ( (uint64)remain_work_p[remain_32_i] << 16 ) |
			  ( low_order_bits >> 16 );
	}
	else {
	    numer_value = ( (uint64)remain_work_p[remain_32_i] << 32 ) |
	                    low_order_bits;
	}

	sc_digit quot_guess = (sc_digit)(numer_value / denom_value);
	if ( quot_guess >> 16 ) {
	    quot_guess--;
	}

	// Subtract off the product of the current guess at the quotient
	// times the denominator vector from the remainder working vector.

	int64  carry = 0;
	int64  product;
	uint64 quot_term = quot_guess << (quot_16_odd ? 16:0);

	for ( int denom_32_i = 0; denom_32_i <= denom_32_hod; ++denom_32_i) {
	    carry += remain_work_p[quot_32_i + denom_32_i];
	    bool carry_was_minus = carry < 0;
	    product = denominator_p[denom_32_i] * quot_term;
	    bool product_hob_one = (product < 0);
	    carry -= product;
	    remain_work_p[quot_32_i + denom_32_i] = (sc_digit)carry;
	    bool carry_is_minus = carry < 0;
	    carry >>= 32;

	    // Because we are subtracting two 64-bit numbers to generate the
	    // remainder values an overflow is possible. If an overflow does
	    // occur the upper bits of the carry value will not be correct
	    // after it was shifted down by 32 bits. There are two cases
	    // requiring correction:
	    //   (1) If the carry is non-negative, and either it was negative
	    //       before the addition of the quotient-denominator product,
	    //       or the high order bit of the quotient-denominator product
	    //       is one, an overflow occurred and the high order bits of
	    //       the carry should have become ones when it was shifted down
	    //       by 32 bits, so set those bits to one.
	    //   (2) If the carry is negative, and it was negative before
	    //       the addition of the quotient-denominator product and the
	    //       high order bit of the quotient-denominator product is one,
	    //       then the low order bit of the shifted carry has the wrong
	    //       sense, so flip it.

	    if ( !carry_is_minus && (product_hob_one || carry_was_minus ) ) {
		carry |= (std::numeric_limits<uint64_t>::max() << 32);
	    }
	    else if ( carry_is_minus && product_hob_one && carry_was_minus ) {
		carry ^= (1LL << 32);
	    }
	}

	// If there are an odd number of digits in either the quotient or
	// denominator there is an extra digit to be processed, so add the
	// current carry to the remainder for that digit.

	if ( quot_16_odd || denom_16_hod_odd ) {
	    carry += remain_work_p[quot_32_i + denom_32_hod + 1];
	    remain_work_p[quot_32_i + denom_32_hod + 1] = (sc_digit)carry;
	}

	// If the carry is negative we overshot by one in our approximation
	// of the quotient digit, so add back in the denominator once and
	// decrement the quotient guess by one:

	if(carry < 0) {
	    carry = 0;
	    for(int restore_i = 0; restore_i <= denom_32_hod; ++restore_i) {
		carry += (int64)denominator_p[restore_i] <<
		                 (quot_16_odd ? 16:0);
		carry += remain_work_p[quot_32_i + restore_i];
		remain_work_p[quot_32_i + restore_i] = (sc_digit)carry;
		carry >>= BITS_PER_DIGIT;
	    }
	    if ( quot_16_odd | denom_16_hod_odd ) {
		remain_work_p[quot_32_i+denom_32_hod+1] += (sc_digit)carry;
	    }
	    quot_guess--;
	}

	// Assign the next quotient digit, if we are returning it:

	if( quotient_n && quot_32_i < quotient_n ) { 
	    if(quot_16_odd) {
		quotient_p[quot_32_i] = quot_guess << 16;
	    }
	    else {
		quotient_p[quot_32_i] |= quot_guess;
	    }
	}
    }

    // RETURN THE QUOTIENT AND REMAINDER IF REQUESTED:
    //
    // (1) If the result is negative complement the quotient.
    // (2) If the numerator was negative complement the remainder.

    if ( (SN || SD) && negative && quotient_n ) { 
        vector_twos_complement( quotient_n, quotient_p );
    }

    if ( remainder_n) {
        int remain_n = VEC_MIN(remainder_n-1, numer_32_hod);
        for ( int remain_i = 0; remain_i <= remain_n; ++remain_i) {
	    remainder_p[remain_i] = remain_work_p[remain_i];
	}
        for (int remain_i = remain_n+1; remain_i < remainder_n; ++remain_i) {
	    remainder_p[remain_i] = 0;
	}
	if ( SN && negative_numerator ) {
	    vector_twos_complement( remainder_n, remainder_p );
	}
    }
    return true;
}

// +----------------------------------------------------------------------------
// |"vector_xor"
// |
// | This function xors two signed sc_digit vectors. The second vector must be
// | shorter, or the same length, as the longer one.
// |
// | Template arguments:
// |     SL - true if longer operand is signed.
// |     SS - true if shorter operand is signed.
// | Arguments:
// |     longer_hod   = index of high order digit in 'longer_p' and 'result_p'.
// |     longer_p     = digits in the longer operand.
// |     shorter_hod  = index of high order digit in 'shorter_p'
// |     shorter_p    = digits in the shorter operand
// |     result_p     = digits to be filled in in the result, this is the same
// |                    number of digits as the longer operand.
// +----------------------------------------------------------------------------
template<int SL, int SS>
inline
void
vector_xor( const int       longer_hod,
	    const sc_digit* longer_p,
	    const int       shorter_hod,
	    const sc_digit* shorter_p,
	    sc_digit*       result_p )
{
    if ( 0 == longer_hod ) {
        *result_p = *longer_p ^ *shorter_p;
	return;
    }

    int       digit_i;

    // Xor the two operands to the length of the shorter operand:

    for ( digit_i = 0; digit_i <= shorter_hod; ++digit_i ) {
	*result_p++ = *longer_p++ ^ shorter_p[digit_i];
    }

    // If the shorter operand is negative fill out the remaining entries
    // with by xoring, otherwise with the longer operand.

    if ( longer_hod > shorter_hod ) {
        if ( SS && 0 > (int)shorter_p[shorter_hod] ) {
	    for ( ; digit_i <= longer_hod; ++digit_i ) {
		*result_p++ = *longer_p++ ^ (sc_digit)-1; // could use ~
	    }
	}
	else {
	    for ( ; digit_i <= longer_hod; ++digit_i ) {
		*result_p++ = *longer_p++;
	    }
	}
    }
}

// +----------------------------------------------------------------------------
// |"vector_xor_reduce"
// |
// | This function overload performs an "xor reduction" on the supplied vector,
// | returning the result.
// |
// | Arguments:
// |     bits_n   = number of bits in the vector that are valid.
// |     target_p = digits containing the bits.
// | Result:
// |     true if an odd number of bits are on in the vector, false if not.
// +----------------------------------------------------------------------------
inline
bool
vector_xor_reduce( const int       bits_n,
	           const sc_digit* target_p )
{
    const unsigned char* byte_p = (const unsigned char*)target_p;
    int                  count = 0;
    const int            hod = SC_DIGIT_INDEX(bits_n-1);

    // First count the bits in the whole sc_digits:

    for ( int byte_i = 0; byte_i < 4*hod; ++byte_i ) {
        count += byte_one_bits[ byte_p[byte_i] ];
    }

    // Check the high order digit:

    const sc_digit mask =
        ~((std::numeric_limits<sc_digit>::max()-1) << SC_BIT_INDEX(bits_n-1));
    const sc_digit high_order_bits = target_p[hod] & mask;
    byte_p = (const unsigned char*)&high_order_bits;
    count += byte_one_bits[ byte_p[0] ];
    count += byte_one_bits[ byte_p[1] ];
    count += byte_one_bits[ byte_p[2] ];
    count += byte_one_bits[ byte_p[3] ];
    return count & 1;
}

// +----------------------------------------------------------------------------
// |"vector_xor_reduce"
// |
// | This function overload performs an "xor reduction" on the requested range
// | of bits in the supplied vector, returning the result.
// |
// | Arguments:
// |     target_p = digits containing the bits.
// |     left_i   = left-most bit in the range to be checked.
// |     right_i  = right-most bit in the range to be checked.
// | Result:
// |     true if an odd number of bits are on in the vector, false if not.
// +----------------------------------------------------------------------------
inline
bool
vector_xor_reduce( const sc_digit* target_p,
                   const int       left_i,
	           const int       right_i )
{
    const unsigned char* byte_p = (const unsigned char*)target_p;
    int                  count = 0;
    const int            left_hod = SC_DIGIT_INDEX(left_i);
    const int            right_hod = SC_DIGIT_INDEX(right_i);

    // All the bits are in a single sc_digit:

    if ( left_hod == right_hod ) {
        const sc_digit mask =
            ~((std::numeric_limits<sc_digit>::max()-1) << (left_i-right_i) );
	const sc_digit digit_bits = target_p[right_hod] & mask;
	byte_p = (const unsigned char*)&digit_bits;
	count += byte_one_bits[ byte_p[0] ];
	count += byte_one_bits[ byte_p[1] ];
	count += byte_one_bits[ byte_p[2] ];
	count += byte_one_bits[ byte_p[3] ];
	return count & 1;
    }

    const int right_index = SC_BIT_INDEX(right_i);

    // The low order bit is not on an sc_digit boundary, check the low
    // order digit, then the whole sc_digits above it.

    if ( right_index ) {
        const sc_digit mask =
            ( std::numeric_limits<sc_digit>::max() << (right_index-1) );
	const sc_digit digit_bits = target_p[right_hod] & mask;
	byte_p = (const unsigned char*)&digit_bits;
	count += byte_one_bits[ byte_p[0] ];
	count += byte_one_bits[ byte_p[1] ];
	count += byte_one_bits[ byte_p[2] ];
	count += byte_one_bits[ byte_p[3] ];
	byte_p = (const unsigned char*)target_p;
	for ( int byte_i = 4*(right_hod+1); byte_i < 4*left_hod; ++byte_i ) {
	    count += byte_one_bits[ byte_p[byte_i] ];
	}
    }

    // The lower order bit is on an sc_digit boundary, check the whole
    // sc_digits below the high order digit.

    else {
	byte_p = (const unsigned char*)target_p;
	for ( int byte_i = 0; byte_i < 4*left_hod; ++byte_i ) {
	    count += byte_one_bits[ byte_p[byte_i] ];
	}
    }

    // Check the high order sc_digit:

    const sc_digit mask =
        ~( (std::numeric_limits<sc_digit>::max()-1) << SC_BIT_INDEX(left_i) );

    const sc_digit high_order_bits = target_p[left_hod] & mask;
    byte_p = (const unsigned char*)&high_order_bits;
    count += byte_one_bits[ byte_p[0] ];
    count += byte_one_bits[ byte_p[1] ];
    count += byte_one_bits[ byte_p[2] ];
    count += byte_one_bits[ byte_p[3] ];

    return count & 1;
}

// +------------------------------------------------------------------------------------------------
// |"vector_zero"
// |
// | This function sets a specified range of digits in the supplied vector to zero.
// |
// | Arguments:
// |     from_i   = first digit to be zeroed.
// |     to_i     = first digit NOT to be zeroed.
// |     target_p = digit vector containing the digits to be zeroed..
// +------------------------------------------------------------------------------------------------
inline
void
vector_zero( int from_i, int to_i, sc_digit* target_p )
{
    for ( int digit_i = from_i; digit_i < to_i; ++digit_i ) { target_p[digit_i] = 0; }
}

#undef VEC_MIN
#undef VEC_MAX

} //namespace sc_dt

#endif // !defined(SC_VECTOR_UTILS_H)
