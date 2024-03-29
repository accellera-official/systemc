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

  sc_bigint<W>_inlines.h -- sc_bigint<W> inline implementations.

  This file contains inline implementations that require the other "int"
  headers to have been included before its contents can be resolved.

  Original Author: Andy Goodrich, Cadence Design Systems

 *****************************************************************************/

#ifndef SC_BIGINT_INLINES_H
#define SC_BIGINT_INLINES_H

namespace sc_dt {

// +----------------------------------------------------------------------------
// |"sc_bigint<W>::operator ="
// |
// | These are the assignment operators for the sc_bigint<W> class.
// |
// | Arguments:
// |     from = value to be assigned this object instance.
// | Result:
// |     Constant reference to this object instance.
// +----------------------------------------------------------------------------
template<int W>
inline const sc_bigint<W>&
sc_bigint<W>::operator = (const sc_bigint<W>& from)
{
    vector_copy( DIGITS_N, from.get_digits(), get_digits() );
    return *this;
}

template<int W>
template<int WO>
inline const sc_bigint<W>&
sc_bigint<W>::operator = (const sc_bigint<WO>& from)
{
    vector_copy( sc_bigint<WO>::DIGITS_N, from.get_digits(), DIGITS_N, get_digits() );
    adjust_hod();
    return *this;
}

template<int W>
template<int WO>
inline const sc_bigint<W>&
sc_bigint<W>::operator = (const sc_biguint<WO>& from)
{
    sc_digit* to_p = get_digits();
    vector_copy( sc_biguint<WO>::DIGITS_N, from.get_digits(), DIGITS_N, to_p );
    adjust_hod();
    return *this;
}

template<int W>
inline const sc_bigint<W>&
sc_bigint<W>::operator = (const sc_unsigned& from)
{
    sc_digit* to_p = get_digits();
    vector_copy( from.get_digits_n(), from.get_digits(), DIGITS_N, to_p );
    adjust_hod();
    return *this;
}

template<int W>
inline
const sc_bigint<W>&
sc_bigint<W>::operator=(const sc_unsigned_subref_r& v)
{
    operator=(sc_unsigned(v));
    return * this;
}

template<int W>
inline const sc_bigint<W>&
sc_bigint<W>::operator = (const sc_signed& from)
{
    sc_digit* to_p = get_digits();
    vector_copy( from.get_digits_n(), from.get_digits(), DIGITS_N, to_p );
    adjust_hod();
    return *this;
}


template<int W>
inline const sc_bigint<W>&
sc_bigint<W>::operator = ( int64 from )
{
    sc_digit* to_p = get_digits();
    *to_p++ = (sc_digit)from;
    if ( HOD > 0 ) {
        *to_p++ = from >> 32;
    }
    if ( HOD > 1 ) {
        vector_fill( from < 0 ? -1 : 0, HOD-2, to_p );
    }
    adjust_hod();
    return *this;
}

template<int W>
inline const sc_bigint<W>&
sc_bigint<W>::operator = ( long from )
{
    sc_digit* to_p = get_digits();
    *to_p++ = from;
    if ( sizeof(from) > 4 ) {
	if ( HOD > 0 ) {
	    *to_p++ = (int64)from >> 32;
	}
	if ( HOD > 1 ) {
	    vector_fill( from < 0 ? -1 : 0, HOD-2, to_p );
	}
    }
    else if ( HOD > 0 ) {
	vector_fill( from < 0 ? -1 : 0, HOD-1, to_p );
    }
    adjust_hod();
    return *this;
}

template<int W>
inline const sc_bigint<W>&
sc_bigint<W>::operator = ( int from )
{
    sc_digit* to_p = get_digits();
    *to_p++ = (sc_digit)from;
    if ( HOD > 0 ) {
	vector_fill( from < 0 ? -1 : 0, HOD-1, to_p );
    }
    adjust_hod();
    return *this;
}

template<int W>
inline const sc_bigint<W>&
sc_bigint<W>::operator = ( uint64 from )
{
    sc_digit* to_p = get_digits();
    *to_p++ = (sc_digit)from;
    if ( HOD > 0 ) {
        *to_p++ = from >> 32;
    }
    if ( HOD > 1 ) {
        vector_fill( 0, HOD - 2, to_p );
    }
    adjust_hod();
    return *this;
}

template<int W>
inline const sc_bigint<W>&
sc_bigint<W>::operator = ( unsigned long from )
{
    sc_digit* to_p = get_digits();
    *to_p++ = from;
    if ( sizeof(from) > 4 ) {
        if ( HOD > 0 ) {
            *to_p++ = (int64)from >> 32;
        }
        if ( HOD > 1 ) {
            vector_fill( 0, HOD-2, to_p );
        }
    }
    else if ( HOD > 1 ) {
        vector_fill( 0, HOD-1, to_p );
    }
    adjust_hod();
    return *this;
}

template<int W>
inline const sc_bigint<W>&
sc_bigint<W>::operator = ( unsigned int from )
{
    sc_digit* to_p = get_digits();
    *to_p++ = from;
    if ( HOD > 0 ) {
	vector_fill( 0, HOD-1, to_p );
    }
    adjust_hod();
    return *this;
}

template<int W>
inline const sc_bigint<W>&
sc_bigint<W>::operator = ( const sc_int_base& from )
{
    return *this = (int64)from;
}

template<int W>
inline const sc_bigint<W>&
sc_bigint<W>::operator = ( const sc_uint_base& from )
{
    return *this = (uint64)from;
}

// Unary operators:

template<int W>
inline const sc_bigint<W>
sc_bigint<W>::operator - ()
{
    sc_carry     carry=1;
    sc_bigint<W> result;
    sc_digit* result_digits = result.get_digits();
    for ( int digit_i = 0; digit_i <= HOD; ++digit_i ) {
        carry += ~digit[digit_i];
        result_digits[digit_i] = (sc_digit)carry;
	carry >>=BITS_PER_DIGIT;
    }
    return result;
}

template<int W>
inline const sc_bigint<W>
sc_bigint<W>::operator ~ ()
{
    sc_bigint<W> result;
    sc_digit* result_digits = result.get_digits();
    for ( int digit_i = 0; digit_i <= HOD; ++digit_i ) {
        result_digits[digit_i] = ~digit[digit_i];
    }
    return result;
}

  // SELF-REFERENCING OPERATORS:

  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator += (const sc_signed&    v) { sc_signed_proxy()+= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator += (const sc_unsigned&  v) { sc_signed_proxy()+= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator += (int64               v) { sc_signed_proxy()+= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator += (uint64              v) { sc_signed_proxy()+= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator += (long                v) { sc_signed_proxy()+= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator += (unsigned long       v) { sc_signed_proxy()+= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator += (int                 v) { sc_signed_proxy()+= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator += (unsigned int        v) { sc_signed_proxy()+= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator += (const sc_int_base&  v) { sc_signed_proxy()+= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator += (const sc_uint_base& v) { sc_signed_proxy()+= v; return *this; }

  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator -= (const sc_signed&    v) { sc_signed_proxy()-= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator -= (const sc_unsigned&  v) { sc_signed_proxy()-= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator -= (int64               v) { sc_signed_proxy()-= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator -= (uint64              v) { sc_signed_proxy()-= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator -= (long                v) { sc_signed_proxy()-= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator -= (unsigned long       v) { sc_signed_proxy()-= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator -= (int                 v) { sc_signed_proxy()-= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator -= (unsigned int        v) { sc_signed_proxy()-= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator -= (const sc_int_base&  v) { sc_signed_proxy()-= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator -= (const sc_uint_base& v) { sc_signed_proxy()-= v; return *this; }

  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator *= (const sc_signed&    v) { sc_signed_proxy()*= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator *= (const sc_unsigned&  v) { sc_signed_proxy()*= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator *= (int64               v) { sc_signed_proxy()*= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator *= (uint64              v) { sc_signed_proxy()*= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator *= (long                v) { sc_signed_proxy()*= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator *= (unsigned long       v) { sc_signed_proxy()*= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator *= (int                 v) { sc_signed_proxy()*= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator *= (unsigned int        v) { sc_signed_proxy()*= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator *= (const sc_int_base&  v) { sc_signed_proxy()*= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator *= (const sc_uint_base& v) { sc_signed_proxy()*= v; return *this; }

  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator /= (const sc_signed&    v) { sc_signed_proxy()/= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator /= (const sc_unsigned&  v) { sc_signed_proxy()/= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator /= (int64               v) { sc_signed_proxy()/= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator /= (uint64              v) { sc_signed_proxy()/= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator /= (long                v) { sc_signed_proxy()/= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator /= (unsigned long       v) { sc_signed_proxy()/= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator /= (int                 v) { sc_signed_proxy()/= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator /= (unsigned int        v) { sc_signed_proxy()/= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator /= (const sc_int_base&  v) { sc_signed_proxy()/= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator /= (const sc_uint_base& v) { sc_signed_proxy()/= v; return *this; }

  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator %= (const sc_signed&    v) { sc_signed_proxy()%= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator %= (const sc_unsigned&  v) { sc_signed_proxy()%= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator %= (int64               v) { sc_signed_proxy()%= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator %= (uint64              v) { sc_signed_proxy()%= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator %= (long                v) { sc_signed_proxy()%= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator %= (unsigned long       v) { sc_signed_proxy()%= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator %= (int                 v) { sc_signed_proxy()%= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator %= (unsigned int        v) { sc_signed_proxy()%= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator %= (const sc_int_base&  v) { sc_signed_proxy()%= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator %= (const sc_uint_base& v) { sc_signed_proxy()%= v; return *this; }

  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator &= (const sc_signed&    v) { sc_signed_proxy()&= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator &= (const sc_unsigned&  v) { sc_signed_proxy()&= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator &= (int64               v) { sc_signed_proxy()&= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator &= (uint64              v) { sc_signed_proxy()&= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator &= (long                v) { sc_signed_proxy()&= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator &= (unsigned long       v) { sc_signed_proxy()&= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator &= (int                 v) { sc_signed_proxy()&= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator &= (unsigned int        v) { sc_signed_proxy()&= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator &= (const sc_int_base&  v) { sc_signed_proxy()&= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator &= (const sc_uint_base& v) { sc_signed_proxy()&= v; return *this; }

  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator |= (const sc_signed&    v) { sc_signed_proxy()|= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator |= (const sc_unsigned&  v) { sc_signed_proxy()|= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator |= (int64               v) { sc_signed_proxy()|= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator |= (uint64              v) { sc_signed_proxy()|= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator |= (long                v) { sc_signed_proxy()|= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator |= (unsigned long       v) { sc_signed_proxy()|= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator |= (int                 v) { sc_signed_proxy()|= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator |= (unsigned int        v) { sc_signed_proxy()|= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator |= (const sc_int_base&  v) { sc_signed_proxy()|= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator |= (const sc_uint_base& v) { sc_signed_proxy()|= v; return *this; }

  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator ^= (const sc_signed&    v) { sc_signed_proxy()^= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator ^= (const sc_unsigned&  v) { sc_signed_proxy()^= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator ^= (int64               v) { sc_signed_proxy()^= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator ^= (uint64              v) { sc_signed_proxy()^= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator ^= (long                v) { sc_signed_proxy()^= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator ^= (unsigned long       v) { sc_signed_proxy()^= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator ^= (int                 v) { sc_signed_proxy()^= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator ^= (unsigned int        v) { sc_signed_proxy()^= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator ^= (const sc_int_base&  v) { sc_signed_proxy()^= v; return *this; }
  template<int W>
  inline sc_bigint<W>& sc_bigint<W>::operator ^= (const sc_uint_base& v) { sc_signed_proxy()^= v; return *this; }

// reduce methods

template<int W>
inline bool sc_bigint<W>::and_reduce()  const { return vector_and_reduce(W,digit); }
template<int W>
inline bool sc_bigint<W>::nand_reduce() const { return !vector_and_reduce(W,digit); }
template<int W>
inline bool sc_bigint<W>::or_reduce()   const { return vector_or_reduce(W,digit); }
template<int W>
inline bool sc_bigint<W>::nor_reduce()  const { return !vector_or_reduce(W,digit); }
template<int W>
inline bool sc_bigint<W>::xor_reduce()  const { return vector_xor_reduce(W,digit); }
template<int W>
inline bool sc_bigint<W>::xnor_reduce() const { return !vector_xor_reduce(W,digit); }

// left shift methods:

template<int W>
const sc_signed
sc_bigint<W>::operator<<(int v) const
{
  if (v <= 0)
    return sc_signed(*this);

  int nb = W + v;
  int nd = DIV_CEIL(nb);
  sc_signed result(nb, false);

  vector_shift_left( DIV_CEIL(W), digit, nd, result.digit, v );
  result.adjust_hod();

  return result;
}

template<int W>
const sc_bigint<W>&
sc_bigint<W>::operator<<=(int v)
{
  if (v <= 0)
    return *this;

  vector_shift_left( DIV_CEIL(W+1), digit, v );
  adjust_hod();

  return *this;
}


template<int W>
const sc_bigint<W>&
sc_bigint<W>::operator>>=(int v)
{

    if (v <= 0)
        return *this;

    int nd = DIV_CEIL(W);

    vector_shift_right(nd, digit, v, (int)digit[nd-1]<0 ? DIGIT_MASK:0);

  return *this;
}

// right shift methods:

template<int W>
const sc_signed
sc_bigint<W>::operator>>(int v) const
{
    if (v <= 0) {
        return sc_signed(*this);
    }
    int nb = W - v;

    // If we shift off the end return the sign bit.

    if ( 0 >= nb ) {
	sc_signed result(1, false);
        result.digit[0] = 0 > (int)digit[HOD] ? -1 : 0;
	return result;
    }

    // Return a value that is the width of the shifted value:

    sc_signed result(nb, false);
    if ( W < 33 ) {
	result.digit[0] = (int)digit[0] >> v;
    }
    else if ( W < 65 ) {
        int64 tmp = digit[DIV_CEIL(W)-1];
        tmp = (tmp << 32) | digit[0];
	tmp = tmp >> v;
	result.digit[0] = tmp;
	if ( nb > 32 ) {
	    result.digit[1] = (tmp >>32);
	}
    }
    else {
	vector_extract(digit, result.digit, W-1, v);
    }
    result.adjust_hod();
    return result;
}

// sc_bv<W> and sc_lv<W> constructors using an sc_bigint<WO> value:

template<int W>
template<int WO>
sc_bv<W>::sc_bv( const sc_bigint<WO>& v )
    : sc_bv_base(W)
{
    *this = v.sc_signed_proxy();
}

template<int W>
template<int WO>
sc_lv<W>::sc_lv( const sc_bigint<WO>& v )
    : sc_lv_base(W)
{
    *this = v.sc_signed_proxy();
}

} // namespace sc_dt

#endif // SC_BIGINT_INLINES_H
