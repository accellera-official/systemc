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

  sc_biguint<W>_inlines.h -- sc_biguint<W> inline implementations.
  
  This file contains inline implementations that require the other "int" 
  headers to have been included before its contents can be resolved.

  Original Author: Andy Goodrich, Cadence Design Systems

 *****************************************************************************/

#ifndef SC_BIGUINT_INLINES_H
#define SC_BIGUINT_INLINES_H

namespace sc_dt {

// +----------------------------------------------------------------------------
// |"sc_biguint<W>::operator ="
// | 
// | These are the assignment operators for the sc_biguint<W> class.
// |
// | Arguments:
// |     from = value to be assigned this object instance.
// | Result:
// |     Constant reference to this object instance.
// +----------------------------------------------------------------------------
template<int W>
template<int WO>
inline const sc_biguint<W>&
sc_biguint<W>::operator = (const sc_biguint<WO>& from)
{
    if ( W == WO ) {
        vector_copy( DIGITS_N, from.get_digits(), get_digits() );
    }
    else {
        vector_copy(sc_biguint<WO>::HOD, from.get_digits(), HOD, get_digits() );
        adjust_hod();
    }
    return *this;
}

template<int W>
template<int WO>
inline const sc_biguint<W>&
sc_biguint<W>::operator = (const sc_bigint<WO>& from)
{
    sc_digit* to_p = get_digits();
    vector_copy( WO/BITS_PER_DIGIT, from.get_digits(), HOD, to_p );
    adjust_hod();
    return *this;
}

template<int W>
inline const sc_biguint<W>&
sc_biguint<W>::operator = (const sc_unsigned& from)
{
    sc_digit* to_p = get_digits();
    int       to_hod = get_hod();
    vector_copy( from.get_hod(), from.get_digits(), to_hod, to_p );
    adjust_hod();
    return *this;
}

#if !defined(BIGINT_CONFIG_HOLLOW)
template<int W>
inline
const sc_biguint<W>&
sc_biguint<W>::operator=(const sc_signed_subref_r& v)
{
    operator=(sc_unsigned(v));
    return * this;
}
#endif // !defined(BIGINT_CONFIG_HOLLOW)

template<int W>
inline const sc_biguint<W>&
sc_biguint<W>::operator = (const sc_signed& from)
{
    sc_digit* to_p = get_digits();
    const int to_hod = get_hod();
    const int from_hod = from.get_hod();
    vector_copy( from_hod, from.get_digits(), to_hod, to_p );
    adjust_hod();
    return *this;
}

template<int W>
inline const sc_biguint<W>&
sc_biguint<W>::operator = ( int64 from )
{
    sc_digit* to_p = get_digits();
    *to_p++ = from;
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
inline const sc_biguint<W>&
sc_biguint<W>::operator = ( long from )
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
inline const sc_biguint<W>&
sc_biguint<W>::operator = ( int from )
{
    sc_digit* to_p = get_digits();
    *to_p++ = from;
    if ( HOD > 0 ) {
	vector_fill( from < 0 ? -1 : 0, HOD-1, to_p );
    }
    adjust_hod();
    return *this;
}

template<int W>
inline const sc_biguint<W>&
sc_biguint<W>::operator = ( uint64 from )
{
    sc_digit* to_p = get_digits();
    *to_p++ = from;
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
inline const sc_biguint<W>&
sc_biguint<W>::operator = ( unsigned long from )
{
    sc_digit* to_p = get_digits();
    *to_p++ = from;
    if ( sizeof(from) > 4 ) {
        if ( HOD > 0 ) {
            *to_p++ = (uint64)from >> 32;
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
inline const sc_biguint<W>&
sc_biguint<W>::operator = ( unsigned int from )
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
inline const sc_biguint<W>&
sc_biguint<W>::operator = ( const sc_int_base& from )
{
    return *this = (int64)from; 
}

template<int W>
inline const sc_biguint<W>&
sc_biguint<W>::operator = ( const sc_uint_base& from )
{
    return *this = (uint64)from; 
}

template<int W>
inline const sc_bigint<W+1>
sc_biguint<W>::operator - ()
{
    sc_carry       carry=1;
    sc_bigint<W+1> result;
    for ( int digit_i = 0; digit_i <= HOD; ++digit_i ) {
        carry += ~digit[digit_i]; 
        result.digit[digit_i] = carry;
	carry >>=BITS_PER_DIGIT;
    }
    return result; 
}

template<int W>
inline const sc_bigint<W+1>
sc_biguint<W>::operator ~ ()
{
    sc_bigint<W+1> result;
    for ( int digit_i = 0; digit_i <= HOD; ++digit_i ) {
        result.digit[digit_i] = ~digit[digit_i]; 
    }
    return result; 
}

  // SELF-REFERENCING OPERATORS:

#if defined(BIGINT_CONFIG_HOLLOW)
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator += (const sc_signed&    v) { sc_unsigned_proxy()+= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator += (const sc_unsigned&  v) { sc_unsigned_proxy()+= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator += (int64               v) { sc_unsigned_proxy()+= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator += (uint64              v) { sc_unsigned_proxy()+= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator += (long                v) { sc_unsigned_proxy()+= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator += (unsigned long       v) { sc_unsigned_proxy()+= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator += (int                 v) { sc_unsigned_proxy()+= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator += (unsigned int        v) { sc_unsigned_proxy()+= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator += (const sc_int_base&  v) { sc_unsigned_proxy()+= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator += (const sc_uint_base& v) { sc_unsigned_proxy()+= v; return *this; }

  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator -= (const sc_signed&    v) { sc_unsigned_proxy()-= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator -= (const sc_unsigned&  v) { sc_unsigned_proxy()-= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator -= (int64               v) { sc_unsigned_proxy()-= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator -= (uint64              v) { sc_unsigned_proxy()-= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator -= (long                v) { sc_unsigned_proxy()-= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator -= (unsigned long       v) { sc_unsigned_proxy()-= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator -= (int                 v) { sc_unsigned_proxy()-= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator -= (unsigned int        v) { sc_unsigned_proxy()-= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator -= (const sc_int_base&  v) { sc_unsigned_proxy()-= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator -= (const sc_uint_base& v) { sc_unsigned_proxy()-= v; return *this; }

  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator *= (const sc_signed&    v) { sc_unsigned_proxy()*= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator *= (const sc_unsigned&  v) { sc_unsigned_proxy()*= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator *= (int64               v) { sc_unsigned_proxy()*= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator *= (uint64              v) { sc_unsigned_proxy()*= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator *= (long                v) { sc_unsigned_proxy()*= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator *= (unsigned long       v) { sc_unsigned_proxy()*= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator *= (int                 v) { sc_unsigned_proxy()*= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator *= (unsigned int        v) { sc_unsigned_proxy()*= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator *= (const sc_int_base&  v) { sc_unsigned_proxy()*= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator *= (const sc_uint_base& v) { sc_unsigned_proxy()*= v; return *this; }

  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator /= (const sc_signed&    v) { sc_unsigned_proxy()/= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator /= (const sc_unsigned&  v) { sc_unsigned_proxy()/= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator /= (int64               v) { sc_unsigned_proxy()/= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator /= (uint64              v) { sc_unsigned_proxy()/= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator /= (long                v) { sc_unsigned_proxy()/= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator /= (unsigned long       v) { sc_unsigned_proxy()/= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator /= (int                 v) { sc_unsigned_proxy()/= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator /= (unsigned int        v) { sc_unsigned_proxy()/= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator /= (const sc_int_base&  v) { sc_unsigned_proxy()/= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator /= (const sc_uint_base& v) { sc_unsigned_proxy()/= v; return *this; }

  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator %= (const sc_signed&    v) { sc_unsigned_proxy()%= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator %= (const sc_unsigned&  v) { sc_unsigned_proxy()%= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator %= (int64               v) { sc_unsigned_proxy()%= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator %= (uint64              v) { sc_unsigned_proxy()%= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator %= (long                v) { sc_unsigned_proxy()%= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator %= (unsigned long       v) { sc_unsigned_proxy()%= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator %= (int                 v) { sc_unsigned_proxy()%= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator %= (unsigned int        v) { sc_unsigned_proxy()%= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator %= (const sc_int_base&  v) { sc_unsigned_proxy()%= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator %= (const sc_uint_base& v) { sc_unsigned_proxy()%= v; return *this; }

  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator &= (const sc_signed&    v) { sc_unsigned_proxy()&= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator &= (const sc_unsigned&  v) { sc_unsigned_proxy()&= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator &= (int64               v) { sc_unsigned_proxy()&= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator &= (uint64              v) { sc_unsigned_proxy()&= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator &= (long                v) { sc_unsigned_proxy()&= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator &= (unsigned long       v) { sc_unsigned_proxy()&= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator &= (int                 v) { sc_unsigned_proxy()&= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator &= (unsigned int        v) { sc_unsigned_proxy()&= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator &= (const sc_int_base&  v) { sc_unsigned_proxy()&= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator &= (const sc_uint_base& v) { sc_unsigned_proxy()&= v; return *this; }

  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator |= (const sc_signed&    v) { sc_unsigned_proxy()|= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator |= (const sc_unsigned&  v) { sc_unsigned_proxy()|= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator |= (int64               v) { sc_unsigned_proxy()|= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator |= (uint64              v) { sc_unsigned_proxy()|= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator |= (long                v) { sc_unsigned_proxy()|= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator |= (unsigned long       v) { sc_unsigned_proxy()|= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator |= (int                 v) { sc_unsigned_proxy()|= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator |= (unsigned int        v) { sc_unsigned_proxy()|= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator |= (const sc_int_base&  v) { sc_unsigned_proxy()|= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator |= (const sc_uint_base& v) { sc_unsigned_proxy()|= v; return *this; }

  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator ^= (const sc_signed&    v) { sc_unsigned_proxy()^= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator ^= (const sc_unsigned&  v) { sc_unsigned_proxy()^= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator ^= (int64               v) { sc_unsigned_proxy()^= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator ^= (uint64              v) { sc_unsigned_proxy()^= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator ^= (long                v) { sc_unsigned_proxy()^= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator ^= (unsigned long       v) { sc_unsigned_proxy()^= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator ^= (int                 v) { sc_unsigned_proxy()^= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator ^= (unsigned int        v) { sc_unsigned_proxy()^= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator ^= (const sc_int_base&  v) { sc_unsigned_proxy()^= v; return *this; }
  template<int W>
  inline const sc_biguint<W>& sc_biguint<W>::operator ^= (const sc_uint_base& v) { sc_unsigned_proxy()^= v; return *this; }
#endif // defined(BIGINT_CONFIG_HOLLOW)

// reduce methods

template<int W>
inline bool sc_biguint<W>::and_reduce()  const { return vector_and_reduce(W,digit); }
template<int W>
inline bool sc_biguint<W>::nand_reduce() const { return !vector_and_reduce(W,digit); }
template<int W>
inline bool sc_biguint<W>::or_reduce()   const { return vector_or_reduce(W,digit); }
template<int W>
inline bool sc_biguint<W>::nor_reduce()  const { return !vector_or_reduce(W,digit); }
template<int W>
inline bool sc_biguint<W>::xor_reduce()  const { return vector_xor_reduce(W,digit); }
template<int W>
inline bool sc_biguint<W>::xnor_reduce() const { return !vector_xor_reduce(W,digit); }

// right shift methods:

template<int W>
const sc_unsigned
sc_biguint<W>::operator>>(unsigned int v) const
{
    if (v == 0) {
        return sc_unsigned(*this);
    }
    int nb = W - v;
    if ( nb <= 0 ) {
        sc_unsigned result(1, true);
	return result;
    }
    sc_unsigned result(nb, false);
    if ( W < 33 ) {
	result.digit[0] = digit[0] >> v;
    }
    else if ( W < 65 ) {
        uint64 tmp = (digit[1] << 32 | digit[0]);
	tmp = tmp >> v;
	result.digit[0] = tmp;
	if ( v > 32 ) {
	    result.digit[1] = (tmp >>32);
	}
    }
    else {
        result.digit[SC_DIGIT_INDEX(nb)] = 0;
	vector_extract(digit, result.digit, nb, v); // +1 issue?
    }
    return result;
}

template<int W>
const sc_unsigned
sc_biguint<W>::operator>>(const sc_signed &v) const
{
    return operator >> (v.to_uint());
}

template<int W>
inline
const sc_biguint<W>&
sc_biguint<W>::operator=(const sc_unsigned_subref_r& v)
{
    operator=(sc_unsigned(v));
    return * this;
}

template<int W>
template<int WO>
sc_bv<W>::sc_bv( const sc_biguint<WO>& v ) 
    : sc_bv_base(W)
{   
#if defined(BIGINT_CONFIG_HOLLOW)
    *this = v.sc_unsigned_proxy();
#else
    *this = v;
#endif // defined(BIGINT_CONFIG_HOLLOW)
}

template<int W>
template<int WO>
sc_lv<W>::sc_lv( const sc_biguint<WO>& v ) 
    : sc_lv_base(W)
{   
#if defined(BIGINT_CONFIG_HOLLOW)
    *this = v.sc_unsigned_proxy();
#else
    *this = v;
#endif // defined(BIGINT_CONFIG_HOLLOW)
}


} // namespace sc_dt

#endif // SC_BIGUINT_INLINES_H
