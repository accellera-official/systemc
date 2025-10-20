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

// +------------------------------------------------------------------------------------------------
// |"sc_unsigned_subref_r shift operators"
// | 
// | These are the shift operators for part selections.
// |
// | Notes:
// |   (1) There are definitions for shift types of int, sc_signed, and sc_unsigned. All other
// |       native C++ integer types will be matched to the int shift type by the compiler.
// | Arguments:
// |     shift = amount to shift by.
// | Result:
// |     sc_unsigned instance representing the shifted value.
// +------------------------------------------------------------------------------------------------
inline
sc_unsigned sc_unsigned_subref_r::operator<<( int shift ) const
{ 
    return ((sc_unsigned)*this)<<(shift); 
}

inline 
sc_unsigned sc_unsigned_subref_r::operator<<(const sc_signed&    v) const 
{
    return this->operator<<( v.to_int() ); 
}

inline 
sc_unsigned sc_unsigned_subref_r::operator<<(const sc_unsigned&  v) const
{
    return this->operator<<( v.to_int() ); 
}

inline
sc_unsigned sc_unsigned_subref_r::operator>>( int shift ) const
{ 
    return ((sc_unsigned)*this)>>(shift); 
}

inline 
sc_unsigned sc_unsigned_subref_r::operator>>(const sc_signed&    v) const 
{
    return this->operator>>( v.to_int() ); 
}

inline 
sc_unsigned sc_unsigned_subref_r::operator>>(const sc_unsigned&  v) const
{
    return this->operator>>( v.to_int() ); 
}

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
    if ( ndigits > SC_BASE_VEC_DIGITS ) {
	digit = new sc_digit[ndigits];
	SC_FREE_DIGIT(true)
    }
    else  {
	digit = base_vec;
	SC_FREE_DIGIT(false)
    }

  vector_copy( ndigits, v.digit, digit);
}


inline
sc_unsigned::sc_unsigned(const sc_signed& v) :
    sc_value_base(v), nbits(v.nbits+1), 
    ndigits( SC_DIGIT_COUNT(nbits) ), digit()
{
    if ( ndigits > SC_BASE_VEC_DIGITS ) {
	digit = new sc_digit[ndigits];
	SC_FREE_DIGIT(true)
    } else {
	digit = base_vec;
	SC_FREE_DIGIT(false)
    }

  vector_copy( v.ndigits, v.digit, digit );
  adjust_hod();
}

inline
sc_unsigned::sc_unsigned(const sc_int_subref_r& v) :
    sc_value_base(v), nbits(), ndigits(), digit()
{
    nbits = v.length()+1;
    ndigits = SC_DIGIT_COUNT(nbits);
    digit = base_vec;
    SC_FREE_DIGIT(false)
    *this = v.to_uint64();
}

inline
sc_unsigned::sc_unsigned(const sc_uint_subref_r& v) :
    sc_value_base(v), nbits(), ndigits(), digit()
{
    nbits = v.length() + 1;
    ndigits = SC_DIGIT_COUNT(nbits);
    digit = base_vec;
    SC_FREE_DIGIT(false)
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
    if ( ndigits > SC_BASE_VEC_DIGITS ) {
	digit = new sc_digit[ndigits];
	SC_FREE_DIGIT(true)
    } else {
	digit = base_vec;
	SC_FREE_DIGIT(false)
    }
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
    adjust_hod();
    if (0) std::cout << "@@@@ " << nbits << " " << ndigits << " " << low_bit << " " << high_bit << std::endl;
}

inline
sc_unsigned::sc_unsigned(const sc_unsigned_subref_r& v) :
    sc_value_base(v), nbits(), ndigits(), digit()
{
    nbits = v.length() + 1;
    ndigits = SC_DIGIT_COUNT(nbits);
    if ( ndigits > SC_BASE_VEC_DIGITS ) {
	digit = new sc_digit[ndigits];
	SC_FREE_DIGIT(true)
    } else {
	digit = base_vec;
	SC_FREE_DIGIT(false)
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
        std::snprintf( msg, sizeof(msg), "%s::%s( sc_bv_base ) : nb = %d is not valid",
                 "sc_unsigned", "sc_unsigned", nb );
        SC_REPORT_ERROR( sc_core::SC_ID_INIT_FAILED_, msg );
    }
    ndigits = DIV_CEIL(nbits);
    if ( ndigits > SC_BASE_VEC_DIGITS ) {
	digit = new sc_digit[ndigits];
	SC_FREE_DIGIT(true)
    } else {
	digit = base_vec;
	SC_FREE_DIGIT(false)
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
        std::snprintf( msg, sizeof(msg), "%s::%s( sc_lv_base ) : nb = %d is not valid",
                 "sc_unsigned", "sc_unsigned", nb );
        SC_REPORT_ERROR( sc_core::SC_ID_INIT_FAILED_, msg );
    }
    ndigits = DIV_CEIL(nbits);
    if ( ndigits > SC_BASE_VEC_DIGITS ) {
	digit = new sc_digit[ndigits];
	SC_FREE_DIGIT(true)
    } else {
	digit = base_vec;
	SC_FREE_DIGIT(false)
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
    vector_copy( from.get_digits_n(), from.get_digits(), get_digits_n(), get_digits() );
    adjust_hod();
    return *this;
}

inline const sc_unsigned&
sc_unsigned::operator = (const sc_signed& from)
{
    vector_copy( from.get_digits_n(), from.get_digits(), get_digits_n(), get_digits() );
    adjust_hod();
    return *this;
}

inline const sc_unsigned&
sc_unsigned::operator = ( int64 from )
{
    int to_hod = get_hod();
    sc_digit* to_p = get_digits();
    *to_p++ = (sc_digit)from;
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

inline const sc_unsigned&
sc_unsigned::operator = ( uint64 from )
{
    int to_hod = get_hod();
    sc_digit* to_p = get_digits();
    *to_p++ = (sc_digit)from;
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

inline sc_unsigned
sc_unsigned::operator>>(const sc_signed& v) const
{
  return operator>>(v.to_int());
}

inline const sc_unsigned&
sc_unsigned::operator>>=(const sc_signed& v)
{
  return operator>>=(v.to_int());
}

inline sc_unsigned
sc_unsigned::operator<<(const sc_signed& v) const
{
  return operator<<(v.to_int());
}

inline const sc_unsigned&
sc_unsigned::operator<<=(const sc_signed& v)
{
  return operator<<=(v.to_int());
}


} // namespace sc_dt

#endif // SC_UNSIGNED_INLINES_H
