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
template<int WO>
inline const sc_bigint<W>&
sc_bigint<W>::operator = (const sc_bigint<WO>& from)
{
    if ( W == WO ) {
        vector_copy( DIGITS_N, from.get_digits(), get_digits() );
    }
    else {
	vector_copy( sc_bigint<WO>::HOD, from.get_digits(), HOD, get_digits() );
	adjust_hod();
    }
    return *this;
}

template<int W>
template<int WO>
inline const sc_bigint<W>&
sc_bigint<W>::operator = (const sc_biguint<WO>& from)
{
    sc_digit* to_p = get_digits();
    vector_copy( sc_biguint<WO>::HOD, from.get_digits(), HOD, to_p );
    adjust_hod();
    return *this;
}

template<int W>
inline  sc_bigint<W>&
sc_bigint<W>::operator = ( int64 from )
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
inline  sc_bigint<W>&
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
inline  sc_bigint<W>&
sc_bigint<W>::operator = ( int from )
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
inline  sc_bigint<W>&
sc_bigint<W>::operator = ( uint64 from )
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
inline  sc_bigint<W>&
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
inline  sc_bigint<W>&
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
inline  sc_bigint<W>&
sc_bigint<W>::operator = ( const sc_int_base& from )
{
    return *this = (int64)from; 
}

template<int W>
inline  sc_bigint<W>&
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
    for ( int digit_i = 0; digit_i <= HOD; ++digit_i ) {
        carry += ~digit[digit_i]; 
        result.digit[digit_i] = carry;
	carry >>=BITS_PER_DIGIT;
    }
    return result; 
}

template<int W>
inline const sc_bigint<W>
sc_bigint<W>::operator ~ ()
{
    sc_bigint<W> result;
    for ( int digit_i = 0; digit_i <= HOD; ++digit_i ) {
        result.digit[digit_i] = ~digit[digit_i]; 
    }
    return result;
}

} // namespace sc_dt

#endif // SC_BIGINT_INLINES_H
