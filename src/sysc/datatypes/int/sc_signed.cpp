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

/*****************************************************************************

  sc_signed.cpp -- Arbitrary precision signed arithmetic.

                   This file includes the definitions of sc_signed_bitref,
                   sc_signed_subref, and sc_signed classes. The first two
                   classes are proxy classes to reference one bit and a range
                   of bits of a sc_signed number, respectively. This file also
                   includes sc_nbcommon.cpp and sc_nbfriends.cpp, which
                   contain the definitions shared by sc_unsigned.

  Original Author: Ali Dasdan, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_signed.cpp,v $
// Revision 1.6  2011/02/18 20:19:15  acg
//  Andy Goodrich: updating Copyright notice.
//
// Revision 1.5  2008/12/10 20:38:45  acg
//  Andy Goodrich: fixed conversion of double values to the digits vector.
//  The bits above the radix were not being masked off.
//
// Revision 1.4  2008/06/19 17:47:56  acg
//  Andy Goodrich: fixes for bugs. See 2.2.1 RELEASENOTES.
//
// Revision 1.3  2008/04/29 21:20:41  acg
//  Andy Goodrich: added mask to first word transferred when processing
//  a negative sc_signed value in sc_signed::concat_get_data().
//
// Revision 1.2  2007/11/04 21:27:00  acg
//  Andy Goodrich: changes to make sure the proper value is returned from
//  concat_get_data().
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.5  2006/10/23 19:32:47  acg
//  Andy Goodrich: further fix for incorrect value being returned from
//  concat_get_data. This one is in the non-aligned value code.
//
// Revision 1.3  2006/01/13 18:49:32  acg
// Added $Log command so that CVS check in comments are reproduced in the
// source.
//

#include <cctype>
#include <cmath>

#include "sysc/kernel/sc_cmnhdr.h"
#include "sysc/kernel/sc_macros.h"
#include "sysc/datatypes/int/sc_signed.h"
#include "sysc/datatypes/int/sc_unsigned.h"
#include "sysc/datatypes/int/sc_int_base.h"
#include "sysc/datatypes/int/sc_uint_base.h"
#include "sysc/datatypes/int/sc_int_ids.h"
#include "sysc/datatypes/bit/sc_bv_base.h"
#include "sysc/datatypes/bit/sc_lv_base.h"
#include "sysc/datatypes/int/sc_vector_utils.h"
#include "sysc/datatypes/int/sc_signed_ops.h"
#include "sysc/datatypes/int/sc_signed_inlines.h"
#include "sysc/datatypes/int/sc_unsigned_inlines.h"
#include "sysc/datatypes/int/sc_int_ids.h"
#include "sysc/datatypes/misc/sc_concatref.h"
#include "sysc/datatypes/fx/sc_fix.h"
#include "sysc/datatypes/fx/scfx_other_defs.h"

#include <sstream>

namespace sc_dt {

void sc_signed::invalid_init( const char* type_name, int nb ) const
{
    std::stringstream msg;
    msg << "sc_signed( "<< type_name << " ) : nb = " << nb << " is not valid";
    SC_REPORT_ERROR( sc_core::SC_ID_INIT_FAILED_, msg.str().c_str() );
}


// -----------------------------------------------------------------------------
// SECTION: Public members - Invalid selections.
// -----------------------------------------------------------------------------

void
sc_signed::invalid_index( int i ) const
{
    std::stringstream msg;
    msg << "sc_bigint bit selection: index = " << i << " violates "
           "0 <= index <= " << (nbits-1);
    SC_REPORT_ERROR( sc_core::SC_ID_OUT_OF_BOUNDS_, msg.str().c_str() );
    sc_core::sc_abort(); // can't recover from here
}

void
sc_signed::invalid_range( int l, int r ) const
{
    std::stringstream msg;
    msg << "sc_bigint part selection: left = " << l << ", right = " << r << "\n"
           "  violates either (" << (nbits-1) << " >= left >= 0) or "
           "(" << (nbits-1) << " >= right >= 0)";
    SC_REPORT_ERROR( sc_core::SC_ID_OUT_OF_BOUNDS_, msg.str().c_str() );
    sc_core::sc_abort(); // can't recover from here
}


// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//  SECTION: Public members.
// ----------------------------------------------------------------------------

// Insert this object's value at the specified place in a vector of biguint
// style values.

bool sc_signed::concat_get_ctrl( sc_digit* dst_p, int low_i ) const
{
    int      dst_i;        // Index to next word to set in dst_p.
    int      end_i;        // Index of high order word to set.
    int      left_shift;   // Amount to shift value left.
    sc_digit mask;         // Mask for partial word sets.


    // CALCULATE METRICS FOR DATA MOVEMENT:

    dst_i = SC_DIGIT_INDEX(low_i);
    end_i = SC_DIGIT_INDEX((low_i + nbits - 1));
    left_shift = SC_BIT_INDEX(low_i);


    // MOVE FIRST WORD (IT MAY BE PARTIAL) AND THEN ANY OTHERS:
    //
    // We may "clobber" upper bits, but they will be written at some point
    // anyway.

    mask = ~(~0U << left_shift);
    dst_p[dst_i] = ( dst_p[dst_i] & ~mask );
    dst_i++;

    for ( ; dst_i <= end_i; dst_i++ ) dst_p[dst_i] = 0;

    return false;
}

bool sc_signed::concat_get_data( sc_digit* dst_p, int low_i ) const
{
    int      dst_i;        // Index to next word to set in dst_p.
    int      end_i;        // Index of high order word to set.
    int      high_i;       // Index w/in word of high order bit.
    int      left_shift;   // Amount to shift value left.
    sc_carry left_word;    // High word component for set.
    sc_carry  mask;         // Mask for partial word sets.
    bool     result;	   // True if inserted non-zero data.
    int      right_shift;  // Amount to shift value right.
    sc_carry right_word;   // Low word component for set.
    int      src_i;        // Index to next word to get from digit.



    // CALCULATE METRICS FOR DATA MOVEMENT:

    dst_i = SC_DIGIT_INDEX(low_i);
    high_i = low_i + nbits - 1;
    end_i = SC_DIGIT_INDEX(high_i);
    left_shift = SC_BIT_INDEX(low_i);

    result = true;

    // ALL DATA TO BE MOVED IS IN A SINGLE WORD:

    if ( dst_i == end_i )
    {
	mask = ~((sc_carry)-1 << left_shift);
	dst_p[dst_i] = ( ( dst_p[dst_i] & mask ) |
	    (digit[0] << left_shift) ) & DIGIT_MASK;
    }


    // DATA IS IN MORE THAN ONE WORD, BUT IS WORD ALIGNED:

    else if ( left_shift == 0 )
    {
	for ( src_i = 0; dst_i < end_i; dst_i++, src_i++ )
	{
	    dst_p[dst_i] = digit[src_i];
	}
	high_i = SC_BIT_INDEX(high_i);
	mask = ~((sc_carry)-2 << high_i) & DIGIT_MASK;
	dst_p[dst_i] = digit[src_i] & mask;
    }


    // DATA IS IN MORE THAN ONE WORD, AND NOT WORD ALIGNED:

    else
    {
	high_i = SC_BIT_INDEX(high_i);
	right_shift = BITS_PER_DIGIT - left_shift;
	mask = ~((sc_carry)-1 << left_shift);
	right_word = digit[0];
	dst_p[dst_i] = (dst_p[dst_i] & mask) |
	    ((right_word << left_shift) & DIGIT_MASK);
	for ( src_i = 1, dst_i++; dst_i < end_i; dst_i++, src_i++ )
	{
	    left_word = digit[src_i];
	    dst_p[dst_i] = ((left_word << left_shift)&DIGIT_MASK) |
		(right_word >> right_shift);
	    right_word = left_word;
	}
	left_word = (src_i < ndigits) ? digit[src_i] : 0;
	mask = ~((sc_carry)-2 << high_i) & DIGIT_MASK;
	dst_p[dst_i] = ((left_word << left_shift) |
	    (right_word >> right_shift)) & mask;
    }
    return result;
}

// Return this object instance's bits as a uint64 without sign extension.

uint64 sc_signed::concat_get_uint64() const
{
    uint64        result;

    result = 0;
    if ( ndigits > 2 )
	result = digit[2];
    if ( ndigits > 1 )
	result = (result << BITS_PER_DIGIT) | digit[1];
    result = (result << BITS_PER_DIGIT) | digit[0];
    return result;
}

// #### OPTIMIZE
void sc_signed::concat_set(int64 src, int low_i)
{
    *this = (low_i < 64) ? src >> low_i : src >> 63;
}

void sc_signed::concat_set(const sc_signed& src, int low_i)
{
    if ( low_i < src.length() )
        *this = src >> low_i;
    else
        *this = (src<0) ? (int_type)-1 : 0;
}

void sc_signed::concat_set(const sc_unsigned& src, int low_i)
{
    if ( low_i < src.length() )
        *this = src >> low_i;
    else
        *this = 0;
}

void sc_signed::concat_set(uint64 src, int low_i)
{
    *this = (low_i < 64) ? src >> low_i : 0;
}

// ----------------------------------------------------------------------------
//  SECTION: Public members - Reduction methods.
// ----------------------------------------------------------------------------

bool sc_signed::and_reduce() const
{
    for ( int digit_i = ndigits-1; digit_i >= 0; --digit_i ) {
        if ( digit[digit_i] != (sc_digit)-1 ) {
	    return false;
	}
    }
    return true;
}

bool sc_signed::or_reduce() const
{
    for ( int digit_i = ndigits-1; digit_i >= 0; --digit_i ) {
        if ( digit[digit_i] != 0 ) {
	    return true;
	}
    }
    return false;
}

bool sc_signed::xor_reduce() const
{
    int i;   // Digit examining.
    int odd; // Flag for odd number of digits.

    odd = 0;
    for ( i = 0; i < nbits; i++ )
        if ( test(i) ) odd = ~odd;
    return odd ? true : false;
}



// ----------------------------------------------------------------------------
//  SECTION: Public members - Assignment operators.
// ----------------------------------------------------------------------------

// assignment operators

const sc_signed&
sc_signed::operator = ( const char* a )
{
    if( a == 0 ) {
        SC_REPORT_ERROR( sc_core::SC_ID_CONVERSION_FAILED_,
                         "character string is zero" );
    }
    else if( *a == 0 ) {
        SC_REPORT_ERROR( sc_core::SC_ID_CONVERSION_FAILED_,
                         "character string is empty" );
    }
    else try {
        int len = length();
        sc_fix aa( a, len, len, SC_TRN, SC_WRAP, 0, SC_ON );
        this->operator = ( aa );
	this->adjust_hod();
        return *this;
    } catch( sc_core::sc_report& ) {
        std::stringstream msg;
        msg << "character string '" << a << "' is not valid";
        SC_REPORT_ERROR( sc_core::SC_ID_CONVERSION_FAILED_, msg.str().c_str() );
	// never reached
    }
    return *this;
}

const sc_signed&
sc_signed::operator=(double v)
{
  is_bad_double(v);
  int i = 0;
  while (floor(v) && (i < ndigits)) {
#ifndef WIN32
    digit[i++] = ((sc_digit)floor(remainder(v, DIGIT_RADIX))) & DIGIT_MASK;
#else
    digit[i++] = ((sc_digit)floor(fmod(v, DIGIT_RADIX))) & DIGIT_MASK;
#endif
    v /= DIGIT_RADIX;
  }
  vector_zero(i, ndigits, digit);
  return *this;
}


// ----------------------------------------------------------------------------

const sc_signed&
sc_signed::operator = ( const sc_bv_base& v )
{
    int minlen = sc_min( nbits, v.length() );
    int digits_n = DIV_CEIL(minlen);
    for ( int digit_i = 0; digit_i < digits_n; ++digit_i ) {
        digit[digit_i] = v.get_word(digit_i);
    }
    adjust_hod();
    return *this;
}

const sc_signed&
sc_signed::operator = ( const sc_lv_base& v )
{
    int minlen = sc_min( nbits, v.length() );
    int digits_n = DIV_CEIL(minlen);
    for ( int digit_i = 0; digit_i < digits_n; ++digit_i ) {
        if ( v.get_cword(digit_i) != 0 ) { // warn about offending 4-state bits:
            int bit_max = sc_min( (digit_i+1)*std::numeric_limits<sc_digit>::digits-1, minlen );
            for( int i = digit_i*std::numeric_limits<sc_digit>::digits; i < bit_max; ++i ) {
                sc_logic( v.get_bit( i ) ).to_bool();
            }
        }
        digit[digit_i] = v.get_word(digit_i);
    }

    adjust_hod();
    return *this;
}


// explicit conversion to character string

const std::string
sc_signed::to_string( sc_numrep numrep ) const
{
    int len = length();
    sc_fix aa( *this, len, len, SC_TRN, SC_WRAP, 0, SC_ON );
    return aa.to_string( numrep );
}

const std::string
sc_signed::to_string( sc_numrep numrep, bool w_prefix ) const
{
    int len = length();
    sc_fix aa( *this, len, len, SC_TRN, SC_WRAP, 0, SC_ON );
    return aa.to_string( numrep, w_prefix );
}


// ----------------------------------------------------------------------------
//  SECTION: Input and output operators
// ----------------------------------------------------------------------------

// Operators in this section are included from sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: Bitwise NOT operator: ~
// ----------------------------------------------------------------------------

// Operators in this section are included from sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: LEFT SHIFT operators: <<, <<=
// ----------------------------------------------------------------------------

sc_signed
operator<<(const sc_signed& u, const sc_unsigned& v)
{
  return u << v.to_uint(); // operator<<(u, v.to_uint());
}

// The rest of the operators in this section are included from
// sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: RIGHT SHIFT operators: >>, >>=
// ----------------------------------------------------------------------------

sc_signed
operator>>(const sc_signed& u, const sc_unsigned& v)
{

  return u >> v.to_uint(); // operator>>(u, v.to_ulong());

}

// The rest of the operators in this section are included from
// sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: Unary arithmetic operators.
// ----------------------------------------------------------------------------

sc_signed
operator+(const sc_signed& u)
{
  return sc_signed(u);
}

sc_signed
operator-(const sc_signed& u)
{
  sc_carry  carry = 1;
  sc_signed result(u.nbits,false);

  for ( int digit_i = 0; digit_i < u.ndigits; ++digit_i ) {
      carry += (sc_digit)~u.digit[digit_i];
      result.digit[digit_i] = carry;
      carry >>= BITS_PER_DIGIT;
  }
  return result;
}

sc_signed
operator-(const sc_unsigned& u)
{
  sc_carry  carry = 1;
  sc_signed result(u.nbits,false);
  sc_digit* result_p = result.get_digits();

  for ( int digit_i = 0; digit_i < u.ndigits; ++digit_i ) {
      carry += (sc_digit)~u.digit[digit_i];
      result_p[digit_i] = carry;
      carry >>= BITS_PER_DIGIT;
  }
  return result;
}

// Get a packed bit representation of the number.
void
sc_signed::get_packed_rep(sc_digit *buf) const
{
  for (int digit_i =0; digit_i < ndigits; ++digit_i) {
      digit[digit_i] = *buf++;
  }
}


// Set a packed bit representation of the number.
void
sc_signed::set_packed_rep(sc_digit *buf)
{
  for (int digit_i =0; digit_i < ndigits; ++digit_i) {
      *buf++ = digit[digit_i];
   }
}


// ---------------------------------------------------------------------------
//  SECTION: Public members - Other utils.
// ---------------------------------------------------------------------------

bool
sc_signed::iszero() const
{
    for ( int digit_i = 0; digit_i < ndigits; ++digit_i ) {
        if ( digit[digit_i] != 0 ) return false;
    }
    return true;
}


bool
sc_signed::sign() const
{
    //@@@@#### CHECK THIS
    return ((digit[ndigits - 1] & one_and_zeros(SC_BIT_INDEX(nbits - 1))) != 0);
}

// The rest of the utils in this section are included from sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: Input and output operators
// ----------------------------------------------------------------------------

void
sc_signed::scan( ::std::istream& is )
{
    std::string s;
    is >> s;
    *this = s.c_str();
}


// ----------------------------------------------------------------------------
//  SECTION: Private members.
// ----------------------------------------------------------------------------

// Print out all the physical attributes.
void
sc_signed::dump(::std::ostream& os) const
{
  // Save the current setting, and set the base to decimal.
#if defined(__MINGW32__)
  std::_Ios_Fmtflags old_flags = os.setf(::std::ios::dec,::std::ios::basefield);
#else
  fmtflags old_flags = os.setf(::std::ios::dec, ::std::ios::basefield);
#endif

  os << "width = " << length() << ::std::endl;
  os << "value = " << *this << ::std::endl;
  os << "bits  = ";

  int len = length();

  for (int i = len - 1; i >= 0; --i) {

    os << "01"[test(i)];
    if (--len % 4 == 0)
      os << " ";

  }

  os << ::std::endl;

  // Restore old_flags.
  os.setf(old_flags, ::std::ios::basefield);
}


// Checks to see if bit_num is out of bounds.
bool
sc_signed::check_if_outside(int bit_num) const
{
  if ((bit_num < 0) || (num_bits(bit_num) >= nbits)) {

#ifdef DEBUG_SYSTEMC
      if( bit_num < 0 || bit_num >= nbits ) {
	  char msg[BUFSIZ];
	  std::snprintf(msg, sizeof(msg), "%s::check_if_outside( int bit_num ) : "
		   "bit_num = %d is out of bounds",
		   "sc_signed", bit_num );
	  SC_REPORT_WARNING( sc_core::SC_ID_OUT_OF_BOUNDS_, msg );
      }
#endif

    return true;
  }

  return false;
}

// ----------------------------------------------------------------------------
//  CLASS : sc_signed_bitref_r
//
//  Proxy class for sc_signed bit selection (r-value only).
// ----------------------------------------------------------------------------

// implicit conversion to uint64

sc_signed_bitref_r::operator uint64 () const
{
    return m_obj_p->test( m_index );
}

bool
sc_signed_bitref_r::operator ! () const
{
    return ( ! m_obj_p->test( m_index ) );
}

bool
sc_signed_bitref_r::operator ~ () const
{
    return ( ! m_obj_p->test( m_index ) );
}


// ----------------------------------------------------------------------------
//  CLASS : sc_signed_bitref
//
//  Proxy class for sc_signed bit selection (r-value and l-value).
// ----------------------------------------------------------------------------

// assignment operators

const sc_signed_bitref&
sc_signed_bitref::operator = ( const sc_signed_bitref_r& b )
{
    m_obj_p->set( m_index, (bool) b );
    return *this;
}

const sc_signed_bitref&
sc_signed_bitref::operator = ( const sc_signed_bitref& b )
{
    m_obj_p->set( m_index, (bool) b );
    return *this;
}

const sc_signed_bitref&
sc_signed_bitref::operator = ( bool b )
{
    m_obj_p->set( m_index, b );
    return *this;
}


const sc_signed_bitref&
sc_signed_bitref::operator &= ( bool b )
{
    if( ! b ) {
	m_obj_p->clear( m_index );
    }
    return *this;
}

const sc_signed_bitref&
sc_signed_bitref::operator |= ( bool b )
{
    if( b ) {
	m_obj_p->set( m_index );
    }
    return *this;
}

const sc_signed_bitref&
sc_signed_bitref::operator ^= ( bool b )
{
    if( b ) {
	m_obj_p->invert( m_index );
    }
    return *this;
}

// #### OPTIMIZE
void sc_signed_bitref::concat_set(int64 src, int low_i)
{
	bool value = 1 & ((low_i < 64) ? (src >> low_i) : (src >> 63));
    m_obj_p->set(m_index, value);
}

void sc_signed_bitref::concat_set(const sc_signed& src, int low_i)
{
    if ( low_i < src.length() )
        m_obj_p->set(m_index, src.test(low_i));
    else
        m_obj_p->set(m_index, src<0);
}

void sc_signed_bitref::concat_set(const sc_unsigned& src, int low_i)
{
    if ( low_i < src.length() )
        m_obj_p->set(m_index, src.test(low_i));
    else
        m_obj_p->set(m_index, 0);
}

void sc_signed_bitref::concat_set(uint64 src, int low_i)
{
	bool value = 1 & ((low_i < 64) ? (src >> low_i) : 0);
    m_obj_p->set(m_index, value);
}


// other methods

void
sc_signed_bitref::scan( ::std::istream& is )
{
    bool b;
    is >> b;
    *this = b;
}

// ----------------------------------------------------------------------------
//  CLASS : sc_signed_subref_r
//
//  Proxy class for sc_signed part selection (r-value only).
// ----------------------------------------------------------------------------

// concatenation support

uint64 sc_signed_subref_r::concat_get_uint64() const
{
    return to_uint64();
}


bool sc_signed_subref_r::concat_get_ctrl(sc_digit* dst_p, int low_i ) const
{
    sc_unsigned a( *this );
    return a.concat_get_ctrl( dst_p, low_i );
}


bool sc_signed_subref_r::concat_get_data(sc_digit* dst_p, int low_i ) const
{
    sc_unsigned a( *this );
    return a.concat_get_data( dst_p, low_i );
}

// reverse bit extraction:

uint64 sc_signed_subref_r::to_uint64_reversed() const
{
    uint64 result;
    result = 0;
    // for ( int i = m_right; i >= m_left; --i ) {
    for ( int i = m_left; i <= m_right; ++i ) {
	result = result << 1;
        if ( m_obj_p->test(i) ) { result |= 1; }
    }
    return result;
}

// implicit conversion to sc_signed

sc_signed_subref_r::operator sc_unsigned () const
{
    return sc_unsigned( *this );
}


double
sc_signed_subref_r::to_double() const
{
    sc_unsigned a( *this );
    return a.to_double();
}


// explicit conversion to character string

const std::string
sc_signed_subref_r::to_string( sc_numrep numrep ) const
{
    sc_unsigned a( length() );
    a = *this;
    return a.to_string( numrep );
}

const std::string
sc_signed_subref_r::to_string( sc_numrep numrep, bool w_prefix ) const
{
    sc_unsigned a( length() );
    a = *this;
    return a.to_string( numrep, w_prefix );
}


// ----------------------------------------------------------------------------
//  CLASS : sc_signed_subref
//
//  Proxy class for sc_signed part selection (r-value and l-value).
// ----------------------------------------------------------------------------

// assignment operators

const sc_signed_subref&
sc_signed_subref::operator = ( const sc_signed_subref_r& a )
{
    return operator = ( (sc_unsigned)( a ) );
}

const sc_signed_subref&
sc_signed_subref::operator = ( const sc_signed_subref& v )
{
    if( this == &v ) {
	return *this;
    }
    return operator = ( (sc_unsigned)( v ) );
}

// +----------------------------------------------------------------------------
// |"sc_signed_subref::operator ="
// |
// | These operators assign a value to an sc_signed part selection.
// |
// | Arguments:
// |     v = value to be assigned.
// | Result:
// |     const reference to this object instance.
// +----------------------------------------------------------------------------
const sc_signed_subref&
sc_signed_subref::operator = ( const sc_signed& v )
{
    vector_insert_bits( v.get_digits_n(), v.get_digits(), m_obj_p->get_digits(),
                        m_left, m_right );
    m_obj_p->adjust_hod();
    return *this;
}

const sc_signed_subref&
sc_signed_subref::operator = ( const sc_unsigned_subref_r& v )
{
    return operator = ( (sc_unsigned)( v ) );
}

const sc_signed_subref&
sc_signed_subref::operator = ( const sc_unsigned& v )
{
    vector_insert_bits( v.get_digits_n(), v.get_digits(), m_obj_p->get_digits(),
                          m_left, m_right );
    m_obj_p->adjust_hod();
    return *this;
}

const sc_signed_subref&
sc_signed_subref::operator = ( unsigned long v )
{
    ScNativeDigits<unsigned long> source(v);

    vector_insert_bits( source.get_digits_n(), source.get_digits(),
                        m_obj_p->get_digits(), m_left, m_right );
    m_obj_p->adjust_hod();
    return *this;
}

const sc_signed_subref&
sc_signed_subref::operator = ( long v )
{
    ScNativeDigits<long> source(v);

    vector_insert_bits( source.get_digits_n(), source.get_digits(),
                        m_obj_p->get_digits(), m_left, m_right );
    m_obj_p->adjust_hod();
    return *this;
}

const sc_signed_subref&
sc_signed_subref::operator = ( uint64 v )
{
    ScNativeDigits<uint64> source(v);

    vector_insert_bits( source.get_digits_n(), source.get_digits(),
                        m_obj_p->get_digits(), m_left, m_right );
    m_obj_p->adjust_hod();
    return *this;
}

const sc_signed_subref&
sc_signed_subref::operator = ( int64 v )
{
    ScNativeDigits<int64> source(v);

    vector_insert_bits( source.get_digits_n(), source.get_digits(),
                        m_obj_p->get_digits(), m_left, m_right );
    m_obj_p->adjust_hod();
    return *this;
}

const sc_signed_subref&
sc_signed_subref::operator = ( double v )
{
    is_bad_double(v);

    int nb = m_left - m_right + 1;
    int nd = DIV_CEIL(nb);

    sc_digit* d = sc_temporary_digits.allocate(nd);

    if (v < 0)
	v = -v;

    int i = 0;

    while (floor(v) && (i < nd)) {
#ifndef WIN32
	d[i++] = (sc_digit) floor(remainder(v, DIGIT_RADIX));
#else
	d[i++] = (sc_digit) floor(fmod(v, DIGIT_RADIX));
#endif
	v /= DIGIT_RADIX;
    }

    vector_zero(i, nd, d);

    vector_insert_bits( nd, d, m_obj_p->get_digits(), m_left, m_right );
    m_obj_p->adjust_hod();

    return *this;
}

const sc_signed_subref&
sc_signed_subref::operator = ( const sc_int_base& a )
{
    return operator = ( (int64) a );
}

const sc_signed_subref&
sc_signed_subref::operator = ( const sc_uint_base& a )
{
    return operator = ( (uint64) a );
}


const sc_signed_subref&
sc_signed_subref::operator = ( const sc_bv_base& a )
{
    sc_signed aa( a );
    return operator = ( aa );
}


const sc_signed_subref&
sc_signed_subref::operator = ( const sc_lv_base& a )
{
    sc_signed aa( a );
    return operator = ( aa );
}


// concatenation methods


void sc_signed_subref::concat_set( int64 src, int low_i )
{
    int  i;
    int  l;
    bool sign = src < 0;

    if ( low_i < 64 )
    {
	src = src >> low_i;
	l = sc_min( m_left, (63-low_i) + m_right );
	for( i = m_right; i <= l; ++ i ) {
		m_obj_p->set( i, src & 1 );
		src = src >> 1;
	}
	for ( ; i <= m_left; i++ ) m_obj_p->set(i, sign);
    }
    else
    {
	for( i = m_right; i <= m_left; ++ i ) m_obj_p->set(i, sign);
    }
}

void sc_signed_subref::concat_set( const sc_signed& src, int low_i )
{
    int i;
    int l;
    int src_i;
    bool sign = src.test(src.nbits-1);
    l = src.nbits - (low_i+1);
    if ( l >= 0 )
    {
	l = sc_min( m_left, l + m_right );
	src_i = low_i;
	for( i = m_right; i <= l; ++ i, src_i++ ) {
	    m_obj_p->set( i, src.test( src_i ) );
	}
	for ( ; i <= m_left; i++ ) m_obj_p->set(i, sign);
    }
    else
    {
	for( i = m_right; i <= m_left; ++ i ) m_obj_p->set(i, sign);
    }
}

void sc_signed_subref::concat_set( const sc_unsigned& src, int low_i )
{
    int i;
    int l;
    int src_i;
    l = src.nbits - (low_i+2);
    if ( l >= 0 )
    {
	l = sc_min( m_left, l + m_right );
	src_i = low_i;
	for( i = m_right; i <= l; ++ i, src_i++ ) {
	    m_obj_p->set( i, src.test( src_i ) );
	}
	for ( ; i <= m_left; i++ ) m_obj_p->set(false);
    }
    else
    {
	for( i = m_right; i <= m_left; ++ i ) m_obj_p->set(false);
    }
}

void sc_signed_subref::concat_set( uint64 src, int low_i )
{
    int  i;
    int  l;

    if ( low_i < 64 )
    {
	src = src >> low_i;
	l = sc_min( m_left, (63-low_i) + m_right );
	for( i = m_right; i <= l; ++ i ) {
		m_obj_p->set( i, src & 1 );
		src = src >> 1;
	}
	for ( ; i <= m_left; i++ ) m_obj_p->set(false);
    }
    else
    {
	for( i = m_right; i <= m_left; ++ i ) m_obj_p->set(false);
    }
}
// other methods

void
sc_signed_subref::scan( ::std::istream& is )
{
    std::string s;
    is >> s;
    *this = s.c_str();
}

#if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_NO_BASE_CLASS)
// Temporary values:

sc_signed sc_signed::m_temporaries[SC_SIGNED_TEMPS_N];
size_t    sc_signed::m_temporaries_i = 0;
#endif // defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_NO_BASE_CLASS)

} // namespace sc_dt

// End of file.
