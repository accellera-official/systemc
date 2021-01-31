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

  sc_unsigned_inlines.h -- sc_unsigned and sc_biguint<W> inline implementations.
  
  This file contains inline implementations that require the other "int" 
  headers to have been included before its contents can be resolved.

  Original Author: Andy Goodrich, Cadence Design Systems

 *****************************************************************************/

#ifndef SC_UNSIGNED_INLINES_H
#define SC_UNSIGNED_INLINES_H

#include "sc_vector_utils.h"

namespace sc_dt {

// +----------------------------------------------------------------------------
// |"sc_unsigned::sc_unsigned"
// | 
// | These are the object constructors the sc_unsigned class that are
// | take another object as a constructor.
// |
// | Arguments:
// |     v = other object.
// +----------------------------------------------------------------------------

inline
sc_unsigned::sc_unsigned(const sc_unsigned& v) :
    sc_value_base(v), nbits(v.nbits), ndigits(v.ndigits), digit()
{
    if ( ndigits > ( (int)(sizeof(small_vec)/sizeof(sc_digit)) ) ) {
	digit = new sc_digit[ndigits];
	m_free = true;
    }
    else  {
	digit = small_vec;
	m_free = false;
    }

  vec_copy(ndigits, digit, v.digit);
}


inline
sc_unsigned::sc_unsigned(const sc_signed& v) :
    sc_value_base(v), nbits(v.nbits+1), 
    ndigits( SC_DIGIT_COUNT(nbits) ), digit()
{
    if ( ndigits > ( (int)(sizeof(small_vec)/sizeof(sc_digit)) ) ) {
	digit = new sc_digit[ndigits];
	m_free = true;
    } else {
	digit = small_vec;
	m_free = false;
    }

  copy_digits(v.nbits, v.ndigits, v.digit);
}

inline
sc_unsigned::sc_unsigned(const sc_int_subref_r& v) :
    sc_value_base(v), nbits(), ndigits(), digit()
{
    nbits = v.length()+1;
    ndigits = SC_DIGIT_COUNT(nbits);
    if ( ndigits > ( (int)(sizeof(small_vec)/sizeof(sc_digit)) ) ) {
	digit = new sc_digit[ndigits];
	m_free = true;
    } else {
	digit = small_vec;
	m_free = false;
    }
    *this = v.to_uint64();
}

inline
sc_unsigned::sc_unsigned(const sc_uint_subref_r& v) :
    sc_value_base(v), nbits(), ndigits(), digit()
{
    nbits = v.length() + 1;
    ndigits = SC_DIGIT_COUNT(nbits);
    if ( ndigits > ( (int)(sizeof(small_vec)/sizeof(sc_digit)) ) ) {
	digit = new sc_digit[ndigits];
	m_free = true;
    } else {
	digit = small_vec;
	m_free = false;
    }
    makezero();
    *this = v.to_uint64();
}

inline
sc_unsigned::sc_unsigned(const sc_signed_subref_r& v) :
    sc_value_base(v), nbits(), ndigits(), digit()
{
    int  low_bit;
    bool reversed = false;
    int  high_bit;

    nbits = v.length() + 1;
    ndigits = SC_DIGIT_COUNT(nbits);
    if ( ndigits > ( (int)(sizeof(small_vec)/sizeof(sc_digit)) ) ) {
	digit = new sc_digit[ndigits];
	m_free = true;
    } else {
	digit = small_vec;
	m_free = false;
    }
    digit[ndigits-1] = 0; 
    if ( v.m_left >= v.m_right ) {
        high_bit = v.m_left;
        low_bit = v.m_right;
    }
    else {
        reversed = true;
        low_bit = v.m_left;
        high_bit = v.m_right;
    }
    vector_extract( v.m_obj_p->digit, digit, high_bit, low_bit );
    if ( reversed ) {
        vector_reverse_bits( digit, high_bit-low_bit, 0 );
    }
}

inline
sc_unsigned::sc_unsigned(const sc_unsigned_subref_r& v) :
    sc_value_base(v), nbits(), ndigits(), digit()
{
    nbits = v.length() + 1;
    ndigits = SC_DIGIT_COUNT(nbits);
    if ( ndigits > ( (int)(sizeof(small_vec)/sizeof(sc_digit)) ) ) {
	digit = new sc_digit[ndigits];
	m_free = true;
    } else {
	digit = small_vec;
	m_free = false;
    }
    digit[ndigits-1] = 0; 
    int  low_bit;
    bool reversed = false;
    int  high_bit;
    if ( v.m_left >= v.m_right ) {
        high_bit = v.m_left;
        low_bit = v.m_right;
    }
    else {
        reversed = true;
        low_bit = v.m_left;
        high_bit = v.m_right;
    }
    vector_extract( v.m_obj_p->digit, digit, high_bit, low_bit );

    if ( reversed ) {
        vector_reverse_bits( digit, high_bit-low_bit, 0 );
    }
}

inline
sc_unsigned::sc_unsigned(const sc_bv_base& v) :
    sc_value_base(), nbits(), ndigits(), digit()
{
    int nb = v.length();
    if( nb > 0 ) {
        nbits = num_bits( nb );
    } else {
        char msg[BUFSIZ];
        std::sprintf( msg, "%s::%s( sc_bv_base ) : nb = %d is not valid",
                 "sc_unsigned", "sc_unsigned", nb );
        SC_REPORT_ERROR( sc_core::SC_ID_INIT_FAILED_, msg );
    }
    ndigits = DIV_CEIL(nbits);
    if ( ndigits > ( (int)(sizeof(small_vec)/sizeof(sc_digit)) ) ) {
	digit = new sc_digit[ndigits];
	m_free = true;
    } else {
	digit = small_vec;
	m_free = false;
    }
    *this = v;
}

inline
sc_unsigned::sc_unsigned(const sc_lv_base& v) : 
    sc_value_base(), nbits(), ndigits(), digit()
{
    int nb = v.length();
    if( nb > 0 ) {
        nbits = num_bits( nb );
    } else {
        char msg[BUFSIZ];
        std::sprintf( msg, "%s::%s( sc_lv_base ) : nb = %d is not valid",
                 "sc_unsigned", "sc_unsigned", nb );
        SC_REPORT_ERROR( sc_core::SC_ID_INIT_FAILED_, msg );
    }
    ndigits = DIV_CEIL(nbits);
    if ( ndigits > ( (int)(sizeof(small_vec)/sizeof(sc_digit)) ) ) {
	digit = new sc_digit[ndigits];
	m_free = true;
    } else {
	digit = small_vec;
	m_free = false;
    }
    *this = v;
}

// +----------------------------------------------------------------------------
// |"sc_unsigned::operator ="
// | 
// | These are the assignment operators for the sc_unsigned class.
// |
// | Arguments:
// |     from = value to be assigned this object instance.
// | Result:
// |     Constant reference to this object instance.
// +----------------------------------------------------------------------------
// extern bool print_it;
inline const sc_unsigned&
sc_unsigned::operator = (const sc_unsigned& from)
{
    sc_digit* to_p = get_digits();
    int       to_hod = get_hod();
    vector_copy( from.get_hod(), from.get_digits(), to_hod, to_p );
    adjust_hod();
    return *this;
}

inline const sc_unsigned&
sc_unsigned::operator = (const sc_signed& from)
{
    sc_digit* to_p = get_digits();
    const int to_hod = get_hod();
    vector_copy( from.get_hod(), from.get_digits(), to_hod, to_p );
    adjust_hod();
    return *this;
}

inline const sc_unsigned&
sc_unsigned::operator = ( int64 from )
{
    int to_hod = get_hod();
    sc_digit* to_p = get_digits();
    *to_p++ = from;
    if ( to_hod > 0 ) {
        *to_p++ = from >> BITS_PER_DIGIT;
    }
    if ( to_hod > 1 ) {
        vector_fill( from < 0 ? -1 : 0, to_hod-2, to_p );
    }
    adjust_hod();
    return *this;
}

inline const sc_unsigned&
sc_unsigned::operator = ( long from )
{
    int to_hod = get_hod();
    sc_digit* to_p = get_digits();
    *to_p++ = from;
    if ( sizeof(long) > 4 ) { // 64-bit system.
	if ( to_hod > 0 ) {
	    *to_p++ = ((int64)from) >> 32; // eliminate 32-bit warnings.
	}
	if ( to_hod > 1 ) {
	    vector_fill( from < 0 ? -1 : 0, to_hod-2, to_p );
	}
    }
    else if ( to_hod > 0 ) { // 32-bit system.
	vector_fill( from < 0 ? -1 : 0, to_hod-1, to_p );
    }
    adjust_hod();
    return *this;
}

#if 0
inline const sc_unsigned&
sc_unsigned::operator = ( int from )
{
    int to_hod = get_hod();
    sc_digit* to_p = get_digits();
    *to_p++ = from;
    if ( to_hod > 0 ) {
	vector_fill( from < 0 ? -1 : 0, to_hod-1, to_p );
    }
    adjust_hod();
    return *this;
}
#endif

inline const sc_unsigned&
sc_unsigned::operator = ( uint64 from )
{
    int to_hod = get_hod();
    sc_digit* to_p = get_digits();
    *to_p++ = from;
    if ( to_hod > 0 ) {
        *to_p++ = from >> 32;
    }
    if ( to_hod > 1 ) {
        vector_fill( 0, to_hod - 2, to_p );
    }
    adjust_hod();
    return *this;
}

inline const sc_unsigned&
sc_unsigned::operator = ( unsigned long from )
{
    int to_hod = get_hod();
    sc_digit* to_p = get_digits();
    *to_p++ = from;
    if ( sizeof(long) > 4 ) { // 64-bit system.
	if ( to_hod > 0 ) {
	    *to_p++ = ((uint64)from) >> 32; // eliminate 32-bit warnings.
	}
	if ( to_hod > 1 ) {
	    vector_fill( 0, to_hod-2, to_p );
	}
    } 
    else if ( to_hod > 0 ) { // 32-bit system.
	vector_fill( 0, to_hod-1, to_p );
    }
    adjust_hod();
    return *this;
}

#if 0
inline const sc_unsigned&
sc_unsigned::operator = ( unsigned int from )
{
    int to_hod = get_hod();
    sc_digit* to_p = get_digits();
    *to_p++ = from;
    if ( to_hod > 0 ) {
	vector_fill( 0, to_hod-1, to_p );
    }
    adjust_hod();
    return *this;
}
#endif

inline const sc_unsigned&
sc_unsigned::operator = ( const sc_int_base& from )
{
    return *this = (int64)from; 
}

inline const sc_unsigned&
sc_unsigned::operator = ( const sc_uint_base& from )
{
    return *this = (uint64)from; 
}

inline
const sc_unsigned&
sc_unsigned::operator=(const sc_unsigned_subref_r& v)
{
  return operator=(sc_unsigned(v));
}


inline
const sc_unsigned&
sc_unsigned::operator=(const sc_signed_subref_r& v)
{
  return operator=(sc_unsigned(v));
}

// +----------------------------------------------------------------------------
// |"sc_unsigned::reverse"
// | 
// | This method reverses the bits in this object instance.
// +----------------------------------------------------------------------------
inline
void
sc_unsigned::reverse()
{   
    vector_reverse_bits( digit, length()-1, 0 );
}

// +----------------------------------------------------------------------------
// |"sc_unsigned::to_double"
// | 
// | This method returns this object instance's value as a double.
// |
// | Result:
// |     double value representing this object instance.
// +----------------------------------------------------------------------------
inline
double
sc_unsigned::to_double() const
{
    int hod = ndigits-1;

    double v = 0.0;
    for ( ; hod >= 0; --hod ) {
	v = v * DIGIT_RADIX + digit[hod];
    }

    return v;
}

#if 0
// Set the ith bit with 1.
inline
void
sc_unsigned::set(int i)
{
  if (check_if_outside(i))
    return;

  int bit_num = bit_ord(i);
  int digit_num = digit_ord(i);

  digit[digit_num] |= one_and_zeros(bit_num);
  digit[digit_num] = SC_MASK_DIGIT(digit[digit_num]);
}


// Set the ith bit with 0, i.e., clear the ith bit.
inline
void
sc_unsigned::clear(int i)
{
  if (check_if_outside(i))
    return;

  int bit_num = bit_ord(i);
  int digit_num = digit_ord(i);

  digit[digit_num] &= ~(one_and_zeros(bit_num));
  digit[digit_num] = SC_MASK_DIGIT(digit[digit_num]);
}

// Return true if the bit i is 1, false otherwise. If i is outside the
// bounds, return 1/0 according to the sign of the number by assuming
// that the number has infinite length.

inline
bool
sc_unsigned::test(int i) const
{
  if (check_if_outside(i))
    return 0;

  int bit_num = bit_ord(i);
  int digit_num = digit_ord(i);

    return ((digit[digit_num] & one_and_zeros(bit_num)) != 0);
}
#endif


} // namespace sc_dt

#endif // SC_UNSIGNED_INLINES_H
