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

#ifdef SC_MAX_NBITS
template< int W = SC_MAX_NBITS >
#else
template< int W >
#endif
class sc_biguint
    : public sc_unsigned
{
public: // anonymous compile-type information about this type.
    enum { 
	ACTUAL_WIDTH = W+1,               // actual width.
        DIGITS_N     = SC_DIGIT_COUNT(W), // number of digits in digit vector.
        HOB          = SC_BIT_INDEX(W),   // bit index of high order bit.
        HOD          = SC_DIGIT_INDEX(W), // digit index of high order bit.
	SIGNED       = 0,                 // this type is unsigned.
	WIDTH        = W                  // width as an enum.
    };
    typedef int HOD_TYPE;                 // type of high order sc_digit.
    // @@@@#### should HOD_TYPE be unsigned int in this case???

public:

    // constructors

    sc_biguint()
	: sc_unsigned( W, true )
	{}

    sc_biguint( bool flag )
	: sc_unsigned( W, false )
	{ *this = (int)flag; }

    sc_biguint( const sc_biguint<W>& v )
	: sc_unsigned( W, false )
	{ *this = v; }

    sc_biguint( const sc_unsigned& v )
	: sc_unsigned( W, false )
	{ *this = v; }

    sc_biguint( const sc_unsigned_subref& v )
	: sc_unsigned( W, false )
	{ *this = v; }

    template< class T >
    sc_biguint( const sc_generic_base<T>& a )
	: sc_unsigned( W, false )
	{ a->to_sc_unsigned(*this); }

    sc_biguint( const sc_signed& v )
	: sc_unsigned( W, false )
	{ *this = v; }

    sc_biguint( const sc_signed_subref& v )
	: sc_unsigned( W, false )
	{ *this = v; }

    sc_biguint( const char* v )
	: sc_unsigned( W, false )
	{ *this = v; } 

    sc_biguint( int64 v )
	: sc_unsigned( W, false )
	{ *this = v; }

    sc_biguint( uint64 v )
	: sc_unsigned( W, false )
	{ *this = v; }

    sc_biguint( long v )
	: sc_unsigned( W, false )
	{ *this = v; }

    sc_biguint( unsigned long v )
	: sc_unsigned( W, false )
	{ *this = v; }

    sc_biguint( int v )
	: sc_unsigned( W, false )
	{ *this = v; } 

    sc_biguint( unsigned int v )
	: sc_unsigned( W, false )
	{ *this = v; }

    sc_biguint( double v )
	: sc_unsigned( W, false )
	{ *this = v; }
  
    sc_biguint( const sc_bv_base& v )
	: sc_unsigned( W, false )
	{ *this = v; }

    sc_biguint( const sc_lv_base& v )
	: sc_unsigned( W, false )
	{ *this = v; }

#ifdef SC_INCLUDE_FX

    explicit sc_biguint( const sc_fxval& v )
	: sc_unsigned( W, false )
	{ *this = v; }

    explicit sc_biguint( const sc_fxval_fast& v )
	: sc_unsigned( W, false )
	{ *this = v; }

    explicit sc_biguint( const sc_fxnum& v )
	: sc_unsigned( W, false )
	{ *this = v; }

    explicit sc_biguint( const sc_fxnum_fast& v )
	: sc_unsigned( W, false )
	{ *this = v; }

#endif


#ifndef SC_MAX_NBITS

    // destructor

    ~sc_biguint()
	{}

#endif
    // unary operators:

    inline const sc_bigint<W+1> operator - ();
    inline const sc_bigint<W+1> operator ~ ();
 

    // assignment operators

#if 0
    sc_biguint<W>& operator = ( const sc_biguint<W>& v )
        { sc_unsigned::operator = ( v ); return *this; }
#endif

    template<int WO>
    sc_biguint<W>& operator = ( const sc_biguint<WO>& v );

    template<int WO>
    sc_biguint<W>& operator = ( const sc_bigint<WO>& v );

    sc_biguint<W>& operator = ( const sc_unsigned& v )
	{ sc_unsigned::operator = ( v ); return *this; }

    sc_biguint<W>& operator = ( const sc_unsigned_subref& v )
	{ sc_unsigned::operator = ( v ); return *this; }

    template< class T >
    sc_biguint<W>& operator = ( const sc_generic_base<T>& a )
	{ a->to_sc_unsigned(*this); return *this; }

    sc_biguint<W>& operator = ( const sc_signed& v )
	{ sc_unsigned::operator = ( v ); return *this; }

    sc_biguint<W>& operator = ( const sc_signed_subref& v )
	{ sc_unsigned::operator = ( v ); return *this; }

    sc_biguint<W>& operator = ( const char* v ) 
	{ sc_unsigned::operator = ( v ); return *this; }

    sc_biguint<W>& operator = ( int64 v ) ; 

    sc_biguint<W>& operator = ( uint64 v ) ;

    sc_biguint<W>& operator = ( long v ) ;

    sc_biguint<W>& operator = ( unsigned long v ) ; 

    sc_biguint<W>& operator = ( int v ) ;

    sc_biguint<W>& operator = ( unsigned int v ) ; 

    sc_biguint<W>& operator = ( double v )
	{ sc_unsigned::operator = ( v ); return *this; }


    sc_biguint<W>& operator = ( const sc_bv_base& v )
	{ sc_unsigned::operator = ( v ); return *this; }

    sc_biguint<W>& operator = ( const sc_lv_base& v )
	{ sc_unsigned::operator = ( v ); return *this; }

    sc_biguint<W>& operator = ( const sc_int_base& v ) ;

    sc_biguint<W>& operator = ( const sc_uint_base& v ) ;

#ifdef SC_INCLUDE_FX

    sc_biguint<W>& operator = ( const sc_fxval& v )
	{ sc_unsigned::operator = ( v ); return *this; }

    sc_biguint<W>& operator = ( const sc_fxval_fast& v )
	{ sc_unsigned::operator = ( v ); return *this; }

    sc_biguint<W>& operator = ( const sc_fxnum& v )
	{ sc_unsigned::operator = ( v ); return *this; }

    sc_biguint<W>& operator = ( const sc_fxnum_fast& v )
	{ sc_unsigned::operator = ( v ); return *this; }

#endif
    void adjust_hod()
    {
        digit[HOD] &= ~((sc_digit)-1 << SC_BIT_INDEX(W));
    }
};

} // namespace sc_dt


#endif
