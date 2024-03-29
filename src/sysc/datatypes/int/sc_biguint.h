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

  sc_biguint.h -- Template version of sc_unsigned. This class
                  enables compile-time bit widths for sc_unsigned numbers.

  Original Author: Ali Dasdan, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Gene Bushayev, Synopsys, Inc.
  Description of Modification: - Interface between sc_bigint and sc_bv/sc_lv.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_biguint.h,v $
// Revision 1.2  2011/02/18 20:19:14  acg
//  Andy Goodrich: updating Copyright notice.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:49:31  acg
// Added $Log command so that CVS check in comments are reproduced in the
// source.
//

#ifndef SC_BIGUINT_H
#define SC_BIGUINT_H


#include "sysc/datatypes/int/sc_signed.h"
#include "sysc/datatypes/int/sc_unsigned.h"

namespace sc_dt
{

// classes defined in this module
template <int W> class sc_biguint;

// classes referenced by this module
template <int W> class sc_bigint;

// forward class declarations
class sc_bv_base;
class sc_lv_base;
class sc_fxval;
class sc_fxval_fast;
class sc_fxnum;
class sc_fxnum_fast;


// ----------------------------------------------------------------------------
//  CLASS TEMPLATE : sc_biguint<W>
//
//  Arbitrary size unsigned integer type.
// ----------------------------------------------------------------------------

template< int W >
class sc_biguint
#if !defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_NO_BASE_CLASS)
    : public sc_unsigned
#endif // SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_NO_BASE_CLASS
{
public: // anonymous compile-type information about this type.
    enum {
	ACTUAL_WIDTH = W+1,                 // actual width.
        DIGITS_N     = SC_DIGIT_COUNT(W+1), // number of digits in digit vector.
        HOB          = SC_BIT_INDEX(W),     // bit index of high order bit.
        HOD          = SC_DIGIT_INDEX(W),   // digit index of high order bit.
	SIGNED       = 0,                   // this type is unsigned.
	WIDTH        = W                    // width as an enum.
    };
    typedef int HOD_TYPE;                 // type of high order sc_digit.

public:

    // constructors

    sc_biguint()
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = 0; }

    sc_biguint(int, int)
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{  }

    sc_biguint( bool flag )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = (int)flag; }

    sc_biguint( const sc_biguint<W>& v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    template<int WO>
    sc_biguint( const sc_biguint<WO>& v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    template<int WO>
    sc_biguint( const sc_bigint<WO>& v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    sc_biguint( const sc_unsigned& v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    sc_biguint( const sc_unsigned_subref& v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    template< class T >
    sc_biguint( const sc_generic_base<T>& a )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ a->to_sc_unsigned(*this); }

    sc_biguint( const sc_signed& v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    sc_biguint( const sc_signed_subref& v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    sc_biguint( const char* v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    sc_biguint( int64 v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    sc_biguint( uint64 v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    sc_biguint( long v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    sc_biguint( unsigned long v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    sc_biguint( int v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    sc_biguint( unsigned int v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    sc_biguint( double v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    sc_biguint( const sc_bv_base& v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    sc_biguint( const sc_lv_base& v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

#ifdef SC_INCLUDE_FX

    explicit sc_biguint( const sc_fxval& v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    explicit sc_biguint( const sc_fxval_fast& v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    explicit sc_biguint( const sc_fxnum& v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

    explicit sc_biguint( const sc_fxnum_fast& v )
    #if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, compile_time_digits )
    #elif defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
        : sc_unsigned( W, false )
    #endif
	{ *this = v; }

#endif


    // destructor

    ~sc_biguint()
	{}

    // unary operators:

    inline const sc_bigint<W+1> operator - ();
    inline const sc_bigint<W+1> operator ~ ();


    // assignment operators

    const sc_biguint<W>& operator = ( const sc_biguint<W>& v );

    template<int WO>
    const sc_biguint<W>& operator = ( const sc_biguint<WO>& v );

    template<int WO>
    const sc_biguint<W>& operator = ( const sc_bigint<WO>& v );

    const sc_biguint<W>& operator = ( const sc_unsigned& v );

    const sc_biguint<W>& operator = ( const sc_unsigned_subref& v )
	{ sc_unsigned_proxy() = ( v ); return *this; }

    template< class T >
    const sc_biguint<W>& operator = ( const sc_generic_base<T>& a )
	{ a->to_sc_unsigned(*this); return *this; }

    const sc_biguint<W>& operator = ( const sc_signed& v );

    const sc_biguint<W>& operator = ( const sc_signed_subref& v )
	{ sc_unsigned_proxy() = ( v ); return *this; }

    const sc_biguint<W>& operator = ( const char* v )
	{ sc_unsigned_proxy() = ( v ); return *this; }

    const sc_biguint<W>& operator = ( int64 v ) ;

    const sc_biguint<W>& operator = ( uint64 v ) ;

    const sc_biguint<W>& operator = ( long v ) ;

    const sc_biguint<W>& operator = ( unsigned long v ) ;

    const sc_biguint<W>& operator = ( int v ) ;

    const sc_biguint<W>& operator = ( unsigned int v ) ;

    const sc_biguint<W>& operator = ( double v )
	{ sc_unsigned_proxy() = ( v ); return *this; }


    const sc_biguint<W>& operator = ( const sc_bv_base& v )
	{ sc_unsigned_proxy() = ( v ); return *this; }

    const sc_biguint<W>& operator = ( const sc_lv_base& v )
	{ sc_unsigned_proxy() = ( v ); return *this; }

    const sc_biguint<W>& operator = ( const sc_int_base& v ) ;

    const sc_biguint<W>& operator = ( const sc_uint_base& v ) ;

#ifdef SC_INCLUDE_FX

    const sc_biguint<W>& operator = ( const sc_fxval& v )
	{ sc_unsigned_proxy() = ( v ); return *this; }

    const sc_biguint<W>& operator = ( const sc_fxval_fast& v )
	{ sc_unsigned_proxy() = ( v ); return *this; }

    const sc_biguint<W>& operator = ( const sc_fxnum& v )
	{ sc_unsigned_proxy() = ( v ); return *this; }

    const sc_biguint<W>& operator = ( const sc_fxnum_fast& v )
	{ sc_unsigned_proxy() = ( v ); return *this; }

#endif

// +----------------------------------------------------------------------------
// |"sc_bigint<W>::to_XXXX"
// |
// | These functions return an object instance's value as the requested
// | native C++ type.
// |
// | Notes:
// |   (1) These are set up for BITS_PER_DIGIT == 32.
// | Result:
// |     Native C++ type containing the object instance's value.
// +----------------------------------------------------------------------------
inline
double
to_double() const
{
    return sc_unsigned_proxy().to_double();
}

inline
int
to_int() const
{
    int result;

    result =  (int)digit[0];
    return result;
}

inline
unsigned int
to_uint() const
{
    unsigned int result;

    result =  (unsigned int)digit[0];
    if ( W < 32 ) { result &= ~((~0u)<<W); }
    return result;
}

inline
int64
to_int64() const
{
    int64 result;

    if ( W < 33 ) {
        result =  to_int();
    }
    else {
        result = ( (int64)digit[1] << BITS_PER_DIGIT ) | digit[0];
    }
    return result;
}

inline
uint64
to_uint64() const
{
    uint64 result;

    if ( W < 33 ) {
        result = to_uint();
    }
    else {
        result = ( (uint64)digit[1] << BITS_PER_DIGIT ) | digit[0];
	if ( W < 64 ) { result &= ~(~UINT64_ZERO << W); }
    }
    return result;
}

inline
long
to_long() const
{
    long result =  ( sizeof(long) < 5 ) ? to_int() : to_int64();
    return result;
}


inline
unsigned long
to_ulong() const
{
    unsigned long result = ( sizeof(unsigned long) < 5 ) ? to_uint() : to_uint64();
    return result;
}

// SELF-REFERENCING OPERATORS:

inline const sc_biguint<W>& operator += (const sc_signed&    v);
inline const sc_biguint<W>& operator += (const sc_unsigned&  v);
inline const sc_biguint<W>& operator += (int64               v);
inline const sc_biguint<W>& operator += (uint64              v);
inline const sc_biguint<W>& operator += (long                v);
inline const sc_biguint<W>& operator += (unsigned long       v);
inline const sc_biguint<W>& operator += (int                 v);
inline const sc_biguint<W>& operator += (unsigned int        v);
inline const sc_biguint<W>& operator += (const sc_int_base&  v);
inline const sc_biguint<W>& operator += (const sc_uint_base& v);

inline const sc_biguint<W>& operator -= (const sc_signed&    v);
inline const sc_biguint<W>& operator -= (const sc_unsigned&  v);
inline const sc_biguint<W>& operator -= (int64               v);
inline const sc_biguint<W>& operator -= (uint64              v);
inline const sc_biguint<W>& operator -= (long                v);
inline const sc_biguint<W>& operator -= (unsigned long       v);
inline const sc_biguint<W>& operator -= (int                 v);
inline const sc_biguint<W>& operator -= (unsigned int        v);
inline const sc_biguint<W>& operator -= (const sc_int_base&  v);
inline const sc_biguint<W>& operator -= (const sc_uint_base& v);

inline const sc_biguint<W>& operator *= (const sc_signed&    v);
inline const sc_biguint<W>& operator *= (const sc_unsigned&  v);
inline const sc_biguint<W>& operator *= (int64               v);
inline const sc_biguint<W>& operator *= (uint64              v);
inline const sc_biguint<W>& operator *= (long                v);
inline const sc_biguint<W>& operator *= (unsigned long       v);
inline const sc_biguint<W>& operator *= (int                 v);
inline const sc_biguint<W>& operator *= (unsigned int        v);
inline const sc_biguint<W>& operator *= (const sc_int_base&  v);
inline const sc_biguint<W>& operator *= (const sc_uint_base& v);

inline const sc_biguint<W>& operator /= (const sc_signed&    v);
inline const sc_biguint<W>& operator /= (const sc_unsigned&  v);
inline const sc_biguint<W>& operator /= (int64               v);
inline const sc_biguint<W>& operator /= (uint64              v);
inline const sc_biguint<W>& operator /= (long                v);
inline const sc_biguint<W>& operator /= (unsigned long       v);
inline const sc_biguint<W>& operator /= (int                 v);
inline const sc_biguint<W>& operator /= (unsigned int        v);
inline const sc_biguint<W>& operator /= (const sc_int_base&  v);
inline const sc_biguint<W>& operator /= (const sc_uint_base& v);

inline const sc_biguint<W>& operator %= (const sc_signed&    v);
inline const sc_biguint<W>& operator %= (const sc_unsigned&  v);
inline const sc_biguint<W>& operator %= (int64               v);
inline const sc_biguint<W>& operator %= (uint64              v);
inline const sc_biguint<W>& operator %= (long                v);
inline const sc_biguint<W>& operator %= (unsigned long       v);
inline const sc_biguint<W>& operator %= (int                 v);
inline const sc_biguint<W>& operator %= (unsigned int        v);
inline const sc_biguint<W>& operator %= (const sc_int_base&  v);
inline const sc_biguint<W>& operator %= (const sc_uint_base& v);

inline const sc_biguint<W>& operator &= (const sc_signed&    v);
inline const sc_biguint<W>& operator &= (const sc_unsigned&  v);
inline const sc_biguint<W>& operator &= (int64               v);
inline const sc_biguint<W>& operator &= (uint64              v);
inline const sc_biguint<W>& operator &= (long                v);
inline const sc_biguint<W>& operator &= (unsigned long       v);
inline const sc_biguint<W>& operator &= (int                 v);
inline const sc_biguint<W>& operator &= (unsigned int        v);
inline const sc_biguint<W>& operator &= (const sc_int_base&  v);
inline const sc_biguint<W>& operator &= (const sc_uint_base& v);

inline const sc_biguint<W>& operator |= (const sc_signed&    v);
inline const sc_biguint<W>& operator |= (const sc_unsigned&  v);
inline const sc_biguint<W>& operator |= (int64               v);
inline const sc_biguint<W>& operator |= (uint64              v);
inline const sc_biguint<W>& operator |= (long                v);
inline const sc_biguint<W>& operator |= (unsigned long       v);
inline const sc_biguint<W>& operator |= (int                 v);
inline const sc_biguint<W>& operator |= (unsigned int        v);
inline const sc_biguint<W>& operator |= (const sc_int_base&  v);
inline const sc_biguint<W>& operator |= (const sc_uint_base& v);

inline const sc_biguint<W>& operator ^= (const sc_signed&    v);
inline const sc_biguint<W>& operator ^= (const sc_unsigned&  v);
inline const sc_biguint<W>& operator ^= (int64               v);
inline const sc_biguint<W>& operator ^= (uint64              v);
inline const sc_biguint<W>& operator ^= (long                v);
inline const sc_biguint<W>& operator ^= (unsigned long       v);
inline const sc_biguint<W>& operator ^= (int                 v);
inline const sc_biguint<W>& operator ^= (unsigned int        v);
inline const sc_biguint<W>& operator ^= (const sc_int_base&  v);
inline const sc_biguint<W>& operator ^= (const sc_uint_base& v);

// Left shift operators:

const sc_unsigned operator<<(int v) const;
const sc_unsigned operator<<(const sc_signed& v) const { return operator << (v.to_int()); } 
const sc_unsigned operator<<(const sc_unsigned& v) const { return operator << (v.to_int()); }

const sc_biguint<W>& operator<<=(int v);
const sc_biguint<W>& operator<<=(const sc_unsigned& v) { return operator <<=( v.to_int() ); }
const sc_biguint<W>& operator<<=(const sc_signed& v) { return operator <<=( v.to_int() ); }

// Right shift operators:

const sc_unsigned operator>>(int v) const;
const sc_unsigned operator>>(const sc_signed& v) const { return operator >> (v.to_int()); } 
const sc_unsigned operator>>(const sc_unsigned& v) const { return operator >> (v.to_int()); }

const sc_biguint<W>& operator>>=(int v);
const sc_biguint<W>& operator>>=(const sc_unsigned& v) { return operator >>=( v.to_int() ); }
const sc_biguint<W>& operator>>=(const sc_signed& v) { return operator >>=( v.to_int() ); }

// Range operators:

sc_unsigned_bitref& bit( int i ) { return sc_unsigned_proxy().bit(i); }
const sc_unsigned_bitref_r& bit( int i ) const { return sc_unsigned_proxy().bit(i); }
sc_unsigned_bitref& operator [] ( int i ) { return bit(i); }
const sc_unsigned_bitref_r& operator [] ( int i ) const { return bit(i); }

sc_unsigned_subref& range( int i, int j ) { return sc_unsigned_proxy().range(i,j); }
const sc_unsigned_subref_r& range( int i, int j ) const { return sc_unsigned_proxy().range(i,j); }
sc_unsigned_subref& operator () ( int i, int j ) { return range(i,j); }
const sc_unsigned_subref_r& operator () ( int i, int j ) const { return range(i,j); }

// reduce methods

inline bool and_reduce() const;
inline bool nand_reduce() const;
inline bool or_reduce() const;
inline bool nor_reduce() const;
inline bool xor_reduce() const ;
inline bool xnor_reduce() const;

// Increment operators:

inline sc_biguint<W>& operator ++ () // prefix
{
    *this = *this + 1;
    return *this;
}

inline sc_biguint<W> operator ++ (int) // postfix
{
    sc_biguint<W> result(*this);
    *this = *this + 1;
    return result;
}

// Decrement operators:

inline sc_biguint<W>& operator -- () // prefix
{
    *this = *this - 1;
    return *this;
}

inline sc_biguint<W> operator -- (int) // postfix
{
    sc_biguint<W> result(*this);
    *this = *this - 1;
    return result;
}

public:
#if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE)
   // If the number of digits is sufficient to fit in sc_signed::base_vec then just allocate
   // a single word here to save storage. Otherwise we allocate enough storage to accomodate
   // our value.
   sc_digit compile_time_digits[DIV_CEIL(W+1)>SC_BASE_VEC_DIGITS?DIV_CEIL(W+1):1];
#elif defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_NO_BASE_CLASS)
   sc_digit digit[DIV_CEIL(W+1)];
#endif // SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_NO_BASE_CLASS

public:
    void adjust_hod()
    {
        digit[HOD] &= ~(~0U << SC_BIT_INDEX(W));
    }

#if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_NO_BASE_CLASS)
    inline sc_unsigned& sc_unsigned_proxy()
    {
        sc_dt::sc_unsigned& result = sc_unsigned::allocate_temporary(W,(sc_digit*)digit);
	return result;
    }

    inline const sc_unsigned& sc_unsigned_proxy() const
    {
        const sc_dt::sc_unsigned& result = sc_unsigned::allocate_temporary(W,(sc_digit*)digit);
	return result;
    }

    inline operator sc_dt::sc_unsigned& ()
    {
        return sc_unsigned_proxy();
    }

    inline operator const sc_dt::sc_unsigned& () const
    {
        return sc_unsigned_proxy();
    }
#else

    inline sc_unsigned& sc_unsigned_proxy()
    {
        return *this;
    }

    inline const sc_unsigned& sc_unsigned_proxy() const
    {
        return *this;
    }

#endif // SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_NO_BASE_CLASS

    // explicit conversion to character string:

    const std::string to_string( sc_numrep numrep = SC_DEC ) const {
        return sc_unsigned_proxy().to_string( numrep );
    }

    const std::string to_string( sc_numrep numrep, bool w_prefix ) const {
        return sc_unsigned_proxy().to_string( numrep, w_prefix );
    }

#if defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_NO_BASE_CLASS)
public: // "mirror" for sc_value_base concatenation support:
  int              concat_length(bool xzp) const    { return W; }
#endif // SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_NO_BASE_CLASS

public: // field and template value accesses:

  inline int              get_actual_length() const { return W+1; }
  inline const sc_digit*  get_digits() const        { return digit; }
  inline sc_digit*        get_digits()              { return digit; }
  inline int              get_digits_n() const      { return DIV_CEIL(W+1); }
  inline int              get_hod() const           { return SC_DIGIT_INDEX(W); }
  inline sc_digit*        get_raw()                 { return digit; }
  inline sc_digit*        get_raw() const           { return (sc_digit*)digit; }

  inline int              length() const            { return W; }

};

} // namespace sc_dt


#endif
