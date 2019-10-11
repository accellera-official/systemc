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

// explicit template instantiations
namespace sc_core {
template class SC_API sc_vpool<sc_dt::sc_signed_bitref>;
template class SC_API sc_vpool<sc_dt::sc_signed_subref>;
} // namespace sc_core

namespace sc_dt {

// Pool of temporary instances:

sc_core::sc_vpool<sc_signed_bitref> sc_signed_bitref::m_pool(9);
sc_core::sc_vpool<sc_signed_subref> sc_signed_subref::m_pool(9);

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


    // ALL DATA TO BE MOVED IS IN A SINGLE WORD:

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
    } catch( sc_core::sc_report ) {
        std::stringstream msg;
        msg << "character string '" << a << "' is not valid";
        SC_REPORT_ERROR( sc_core::SC_ID_CONVERSION_FAILED_, msg.str().c_str() );
    }
    return *this;
}

const sc_signed&
sc_signed::operator=(double v)
{
  is_bad_double(v);
  register int i = 0;
  while (floor(v) && (i < ndigits)) {
#ifndef WIN32
    digit[i++] = ((sc_digit)floor(remainder(v, DIGIT_RADIX))) & DIGIT_MASK;
#else
    digit[i++] = ((sc_digit)floor(fmod(v, DIGIT_RADIX))) & DIGIT_MASK;
#endif
    v /= DIGIT_RADIX;
  }
  vec_zero(i, ndigits, digit);
  return *this;  
}


// ----------------------------------------------------------------------------

const sc_signed&
sc_signed::operator = ( const sc_bv_base& v )
{
    int minlen = sc_min( nbits, v.length() );
    int i = 0;
    for( ; i < minlen; ++ i ) {
	safe_set( i, v.get_bit( i ), digit );
    }
    for( ; i < nbits; ++ i ) {
	safe_set( i, 0, digit );  // zero-extend
    }
    adjust_hod();
    return *this;
}

const sc_signed&
sc_signed::operator = ( const sc_lv_base& v )
{
    int minlen = sc_min( nbits, v.length() );
    int i = 0;
    for( ; i < minlen; ++ i ) {
	safe_set( i, sc_logic( v.get_bit( i ) ).to_bool(), digit );
    }
    for( ; i < nbits; ++ i ) {
	safe_set( i, 0, digit );  // zero-extend
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
//  SECTION: Interfacing with sc_int_base
// ----------------------------------------------------------------------------

sc_signed 
operator << (const sc_signed& u, const sc_int_base& v)
{ return operator<<(u, (int64) v); }

const sc_signed& 
sc_signed::operator <<= (const sc_int_base& v)
{ return operator<<=((int64) v); }


sc_signed 
operator >> (const sc_signed&    u, const sc_int_base&  v)
{ return operator>>(u, (int64) v); }

const sc_signed& 
sc_signed::operator >>= (const sc_int_base&  v)
{ return operator>>=((int64) v); }

// ----------------------------------------------------------------------------
//  SECTION: Interfacing with sc_uint_base
// ----------------------------------------------------------------------------

const sc_signed& 
sc_signed::operator <<= (const sc_uint_base& v)
{ return operator<<=((uint64) v); }


sc_signed
operator >> (const sc_signed&    u, const sc_uint_base&  v)
{ return operator>>(u, (uint64) v); }

const sc_signed& 
sc_signed::operator >>= (const sc_uint_base&  v)
{ return operator>>=((uint64) v); }

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
  return operator<<(u, v.to_ulong());
}

// The rest of the operators in this section are included from
// sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: RIGHT SHIFT operators: >>, >>=
// ----------------------------------------------------------------------------

sc_signed
operator>>(const sc_signed& u, const sc_unsigned& v)
{

  return operator>>(u, v.to_ulong());

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
    return check_for_zero(ndigits, digit);
}


bool
sc_signed::sign() const
{
    //@@@@#### CHECK THIS
    return ((digit[ndigits - 1] & one_and_zeros(bit_ord(nbits - 1))) != 0);
}

// The rest of the utils in this section are included from sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: Private members.
// ----------------------------------------------------------------------------

// The private members in this section are included from sc_nbcommon.cpp.

#define CLASS_TYPE sc_signed
#define CLASS_TYPE_STR "sc_signed"

#define ADD_HELPER add_signed_friend
#define SUB_HELPER sub_signed_friend
#define MUL_HELPER mul_signed_friend
#define DIV_HELPER div_signed_friend
#define MOD_HELPER mod_signed_friend
#define AND_HELPER and_signed_friend
#define  OR_HELPER  or_signed_friend
#define XOR_HELPER xor_signed_friend 

// REMOVE #include "sc_nbfriends.inc"

#undef  SC_UNSIGNED
#define SC_SIGNED
#define IF_SC_SIGNED              1  // 1 = sc_signed
#define CLASS_TYPE_SUBREF         sc_signed_subref_r
#define OTHER_CLASS_TYPE          sc_unsigned
#define OTHER_CLASS_TYPE_SUBREF   sc_unsigned_subref_r

#define MUL_ON_HELPER mul_on_help_signed
#define DIV_ON_HELPER div_on_help_signed
#define MOD_ON_HELPER mod_on_help_signed

#undef MOD_ON_HELPER
#undef DIV_ON_HELPER
#undef MUL_ON_HELPER

#undef OTHER_CLASS_TYPE_SUBREF
#undef OTHER_CLASS_TYPE
#undef CLASS_TYPE_SUBREF
#undef IF_SC_SIGNED
#undef SC_SIGNED

#undef XOR_HELPER
#undef  OR_HELPER
#undef AND_HELPER
#undef MOD_HELPER
#undef DIV_HELPER
#undef MUL_HELPER
#undef SUB_HELPER
#undef ADD_HELPER

#undef CLASS_TYPE
#undef CLASS_TYPE_STR


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
//  SECTION: LEFT SHIFT operators: <<, <<=
// ----------------------------------------------------------------------------

sc_signed
operator<<(const sc_signed& u, const sc_signed& v)
{
  return operator<<(u, v.to_ulong());
}


const sc_signed&
sc_signed::operator<<=(const sc_signed& v)
{
  return operator<<=(v.to_ulong());
}


sc_signed
operator<<(const sc_signed& u, int64 v)
{
  if (v <= 0)
    return sc_signed(u);

  return operator<<(u, (unsigned long) v);
}


sc_signed
operator<<(const sc_signed& u, uint64 v)
{
  if (v == 0)
    return sc_signed(u);

  return operator<<(u, (unsigned long) v);
}


const sc_signed&
sc_signed::operator<<=(int64 v)
{
  if (v <= 0)
    return *this;

  return operator<<=((unsigned long) v);
}


const sc_signed&
sc_signed::operator<<=(uint64 v)
{
  if (v == 0)
    return *this;

  return operator<<=((unsigned long) v);
}


sc_signed
operator<<(const sc_signed& u, long v)
{
  if (v <= 0)
    return sc_signed(u);

  return operator<<(u, (unsigned long) v);
}

sc_signed
operator<<(const sc_signed& u, unsigned long v)
{
  if (v == 0)
    return sc_signed(u);

  int nb = u.nbits + v;
  int nd = DIV_CEIL(nb);
  sc_signed result(nb, false);

  vector_copy( nd, u.digit, result.digit ); 

  vector_shift_left( nd, result.digit, v );

  return result;
}


const sc_signed&
sc_signed::operator<<=(long v)
{
  if (v <= 0)
    return *this;

  return operator<<=((unsigned long) v);
}


const sc_signed&
sc_signed::operator<<=(unsigned long v)
{
  if (v == 0)
    return *this;

  vec_shift_left(ndigits, digit, v);
  adjust_hod();
  return *this;
}


// ----------------------------------------------------------------------------
//  SECTION: RIGHT SHIFT operators: >>, >>=
// ----------------------------------------------------------------------------

sc_signed
operator>>(const sc_signed& u, const sc_signed& v)
{
  return operator>>(u, v.to_long());
}


const sc_signed&
sc_signed::operator>>=(const sc_signed& v)
{
  return operator>>=(v.to_long());
}


sc_signed
operator>>(const sc_signed& u, int64 v)
{
  if (v <= 0)
    return sc_signed(u);

  return operator>>(u, (unsigned long) v);
}


sc_signed
operator>>(const sc_signed& u, uint64 v)
{
  if (v == 0)
    return sc_signed(u);

  return operator>>(u, (unsigned long) v);
}

const sc_signed&
sc_signed::operator>>=(int64 v)
{
  if (v <= 0)
    return *this;

  return operator>>=((unsigned long) v);
}


const sc_signed&
sc_signed::operator>>=(uint64 v)
{
  if (v == 0)
    return *this;

  return operator>>=((unsigned long) v);
}


sc_signed
operator>>(const sc_signed& u, long v)
{
  if (v <= 0)
    return sc_signed(u);

  return operator>>(u, (unsigned long) v);
}


sc_signed
operator>>(const sc_signed& u, unsigned long v)
{
  if (v == 0)
    return sc_signed(u);

  int nb = u.nbits;
  int nd = u.ndigits;

  sc_signed result(nb, false);
  vector_copy(nd, u.digit, result.digit);
  vector_shift_right( nd, result.digit, v, 
                      sc_signed::SIGNED&&(int)u.digit[nd-1]<0 ? DIGIT_MASK:0);
    
  return result;
}


const sc_signed&
sc_signed::operator>>=(long v)
{
  if (v <= 0)
    return *this;

  return operator>>=((unsigned long) v);
}


const sc_signed&
sc_signed::operator>>=(unsigned long v)
{
    if (v == 0)
        return *this;

    vec_shift_right(ndigits, digit, v, sc_signed::SIGNED&&(int)digit[ndigits-1]<0 ? DIGIT_MASK:0);

  return *this;
}



// ----------------------------------------------------------------------------
//  SECTION: Private members.
// ----------------------------------------------------------------------------


// Create a signed number with (s, nb, nd, d) as its attributes (as
// defined in class sc_signed). If alloc is set, delete d.
sc_signed::sc_signed(int nb, int nd, sc_digit *d, 
                       bool alloc) :
    sc_value_base(), nbits(num_bits(nb)), ndigits(), digit()
{
  ndigits = DIV_CEIL(nbits);

    if ( ndigits > ( (int)(sizeof(small_vec)/sizeof(sc_digit)) ) ) {
	digit = new sc_digit[ndigits];
    } else {
	digit = small_vec;
    }

  if (ndigits <= nd)
    vec_copy(ndigits, digit, d);
  else
    vec_copy_and_zero(ndigits-1, digit, nd-1, d);
    //vec_copy_and_zero(ndigits, digit, nd, d);

}

// This constructor is mainly used in finding a "range" of bits from a
// number of type sc_signed. The function range(l, r) can have
// arbitrary precedence between l and r. If l is smaller than r, then
// the output is the reverse of range(r, l). 
sc_signed::sc_signed(const sc_signed* u, int l, int r) :
    sc_value_base(), nbits(), ndigits(), digit()
{
  bool reversed = false;

  if( l < r ) {
    reversed = true;
    int tmp = l;
    l = r;
    r = tmp;
  }

  // at this point, l >= r

  // make sure that l and r point to the bits of u
  r = sc_max( r, 0 );
  l = sc_min( l, u->nbits - 1 );
    
  nbits = num_bits( l - r + 1 );

  // nbits can still be <= 0 because l and r have just been updated
  // with the bounds of u.

  // if u == 0 or the range is out of bounds, return 0
  if( nbits <= num_bits( 0 ) ) {
    if( nbits <= num_bits( 0 ) ) {
      nbits = 1;
    }
    ndigits = DIV_CEIL( nbits );
    if ( ndigits > ( (int)(sizeof(small_vec)/sizeof(sc_digit)) ) ) {
	digit = new sc_digit[ndigits];
    } else {
	digit = small_vec;
    }
    vec_zero( ndigits, digit );
    return;
  }

  // The rest will be executed if u is not zero.

  ndigits = DIV_CEIL(nbits);
  
  // The number of bits up to and including l and r, respectively.
  int nl = l + 1; 
  int nr = r + 1; 
  
  // The indices of the digits that have lth and rth bits, respectively.
  int left_digit = DIV_CEIL(nl) - 1;
  int right_digit = DIV_CEIL(nr) - 1;
  
  int nd;

    // The range is performed on the 2's complement representation, so
    // first get the indices for that.

    nd = left_digit - right_digit + 1;
    if ( ndigits > ( (int)(sizeof(small_vec)/sizeof(sc_digit)) ) ) {
	digit = new sc_digit[ndigits];
    } else {
	digit = small_vec;
    }
    ScBigTemp d; // sc_digit *d = sc_get_big_temp();
  
    for (int i = right_digit; i <= left_digit; ++i)
        d[i - right_digit] = u->digit[i];
    
    vec_shift_right(nd, d, r - right_digit * BITS_PER_DIGIT, sc_signed::SIGNED&&(int)d[nd-1]<0 ? DIGIT_MASK:0);
    
    if (! reversed) {
      vec_copy(sc_min(nd, ndigits), digit, d);
  
    }
    else {

    // If l < r, i.e., reversed is set, reverse the bits of digit.  d
    // will be used as a temporary store. The following code tries to
    // minimize the use of bit_ord and digit_ord, which use mod and
    // div operators. Since these operators are function calls to
    // standard library routines, they are slow. The main idea in
    // reversing is "read bits out of d from left to right and push
    // them into digit using right shifting."

    // Take care of the last digit.
    int nd_less_1 = nd - 1;

    // Deletions will start from the left end and move one position
    // after each deletion.
    sc_digit del_mask = one_and_zeros(bit_ord(l - r));
      
    while (del_mask) {
      vec_shift_right(ndigits, digit, 1, ((d[nd_less_1] & del_mask) != 0));
      del_mask >>= 1;
    }

    // Take care of the other digits if any.

    // Insertion to digit will always occur at the left end.
    sc_digit ins_mask = one_and_zeros(BITS_PER_DIGIT - 1);

    for (int j = nd - 2; j >= 0; --j) { // j = nd - 2

      // Deletions will start from the left end and move one position
      // after each deletion.
      del_mask = ins_mask;

      while (del_mask) {
        vec_shift_right(ndigits, digit, 1, ((d[j] & del_mask) != 0));
        del_mask >>= 1;
      }
    }

      vec_shift_right(ndigits, digit, 
                      ndigits * BITS_PER_DIGIT - length(), 0);


  }  // if reversed.

  
}

// This constructor is mainly used in finding a "range" of bits from a
// number of type sc_unsigned. The function range(l, r) can have
// arbitrary precedence between l and r. If l is smaller than r, then
// the output is the reverse of range(r, l). 
sc_signed::sc_signed(const sc_unsigned* u, int l, int r) :
    sc_value_base(), nbits(), ndigits(), digit()
{
  bool reversed = false;

  if( l < r ) {
    reversed = true;
    int tmp = l;
    l = r;
    r = tmp;
  }

  // at this point, l >= r

  // make sure that l and r point to the bits of u
  r = sc_max( r, 0 );
  l = sc_min( l, u->nbits - 1 );
    
  nbits = num_bits( l - r + 1 );

  // nbits can still be <= 0 because l and r have just been updated
  // with the bounds of u.

  // if u == 0 or the range is out of bounds, return 0
  if( nbits <= num_bits( 0 ) ) {
    if( nbits <= num_bits( 0 ) ) {
      nbits = 1;
    }
    ndigits = DIV_CEIL( nbits );
    if ( ndigits > ( (int)(sizeof(small_vec)/sizeof(sc_digit)) ) ) {
	digit = new sc_digit[ndigits];
    } else {
	digit = small_vec;
    }
    vec_zero( ndigits, digit );
    return;
  }

  // The rest will be executed if u is not zero.

  ndigits = DIV_CEIL(nbits);
  
  // The number of bits up to and including l and r, respectively.
  int nl = l + 1; 
  int nr = r + 1; 
  
  // The indices of the digits that have lth and rth bits, respectively.
  int left_digit = DIV_CEIL(nl) - 1;
  int right_digit = DIV_CEIL(nr) - 1;
  
  int nd;

  // The range is performed on the 2's complement representation, so
  // first get the indices for that.
    nd = left_digit - right_digit + 1;

  // Allocate memory for the range.
    if ( ndigits > ( (int)(sizeof(small_vec)/sizeof(sc_digit)) ) ) {
	digit = new sc_digit[ndigits];
    } else {
	digit = small_vec;
    }
  ScBigTemp d; // sc_digit *d = sc_get_big_temp();
  
  // Getting the range on the 2's complement representation.
  {
    
    for (int i = right_digit; i <= left_digit; ++i)
      d[i - right_digit] = u->digit[i];
    
    vec_shift_right(nd, d, r - right_digit * BITS_PER_DIGIT, sc_signed::SIGNED&&(int)d[nd-1]<0 ? DIGIT_MASK:0);
    
  }
  
  vec_zero(ndigits, digit);

  if (! reversed)
    vec_copy(sc_min(nd, ndigits), digit, d);
  
  else {

    // If l < r, i.e., reversed is set, reverse the bits of digit.  d
    // will be used as a temporary store. The following code tries to
    // minimize the use of bit_ord and digit_ord, which use mod and
    // div operators. Since these operators are function calls to
    // standard library routines, they are slow. The main idea in
    // reversing is "read bits out of d from left to right and push
    // them into digit using right shifting."

    // Take care of the last digit.
    int nd_less_1 = nd - 1;

    // Deletions will start from the left end and move one position
    // after each deletion.
    sc_digit del_mask = one_and_zeros(bit_ord(l - r));
      
    while (del_mask) {
      vec_shift_right(ndigits, digit, 1, ((d[nd_less_1] & del_mask) != 0));
      del_mask >>= 1;
    }

    // Take care of the other digits if any.

    // Insertion to digit will always occur at the left end.
    sc_digit ins_mask = one_and_zeros(BITS_PER_DIGIT - 1);

    for (int j = nd - 2; j >= 0; --j) { // j = nd - 2

      // Deletions will start from the left end and move one position
      // after each deletion.
      del_mask = ins_mask;

      while (del_mask) {
        vec_shift_right(ndigits, digit, 1, ((d[j] & del_mask) != 0));
        del_mask >>= 1;
      }
    }

      vec_shift_right(ndigits, digit, 
                      ndigits * BITS_PER_DIGIT - length(), sc_signed::SIGNED&&(int)d[nd-1]<0 ? DIGIT_MASK:0);


  }  // if reversed.

  
}


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
	  std::sprintf( msg, "%s::check_if_outside( int bit_num ) : "
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
    m_obj_p->set(low_i, value);
}

void sc_signed_bitref::concat_set(const sc_signed& src, int low_i)
{
    if ( low_i < src.length() )
        m_obj_p->set(low_i, src.test(low_i));
    else
        m_obj_p->set(low_i, src<0);
}       

void sc_signed_bitref::concat_set(const sc_unsigned& src, int low_i)
{
    if ( low_i < src.length() )
        m_obj_p->set(low_i, src.test(low_i));
    else
        m_obj_p->set(low_i, 0);
}

void sc_signed_bitref::concat_set(uint64 src, int low_i)
{
	bool value = 1 & ((low_i < 64) ? (src >> low_i) : 0);
    m_obj_p->set(low_i, value);
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
    vector_insert_bits( v.get_hod(), v.get_digits(), m_obj_p->get_digits(), 
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
    vector_insert_bits( v.get_hod(), v.get_digits(), m_obj_p->get_digits(), 
                          m_left, m_right );
    m_obj_p->adjust_hod();
    return *this;
}

const sc_signed_subref& 
sc_signed_subref::operator = ( unsigned long v )
{
    ScNativeDigits<unsigned long> source(v);

    vector_insert_bits( source.get_hod(), source.get_digits(), 
                        m_obj_p->get_digits(), m_left, m_right );
    m_obj_p->adjust_hod();
    return *this;
}

const sc_signed_subref& 
sc_signed_subref::operator = ( long v ) 
{
    ScNativeDigits<long> source(v);

    vector_insert_bits( source.get_hod(), source.get_digits(), 
                        m_obj_p->get_digits(), m_left, m_right );
    m_obj_p->adjust_hod();
    return *this;
}

const sc_signed_subref& 
sc_signed_subref::operator = ( uint64 v )
{
    ScNativeDigits<uint64> source(v);

    vector_insert_bits( source.get_hod(), source.get_digits(), 
                        m_obj_p->get_digits(), m_left, m_right );
    m_obj_p->adjust_hod();
    return *this;
}

const sc_signed_subref& 
sc_signed_subref::operator = ( int64 v ) 
{
    ScNativeDigits<int64> source(v);

    vector_insert_bits( source.get_hod(), source.get_digits(), 
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

    ScBigTemp d; // sc_digit *d = sc_get_big_temp(); 

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

    vec_zero(i, nd, d);

    sc_digit val = 1;  // Bit value.
    int j = 0;   // Current digit in d.

    i = 0;  // Current bit in d.

    while (i < nb) {

	m_obj_p->set(i + m_right, (bool) (d[j] & val));

	++i;

	if (SC_BIT_INDEX(i) == 0) {
	    val = 1;
	    ++j;
	}
	else
	    val <<= 1;
    }

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


} // namespace sc_dt

// End of file.
