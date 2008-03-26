/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

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

#ifndef SC_BIGUINT_H
#define SC_BIGUINT_H


#include "sysc/datatypes/int/sc_signed.h"
#include "sysc/datatypes/int/sc_unsigned.h"

namespace sc_dt
{

// classes defined in this module
template <int W> class sc_biguint;

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
public:

    // constructors

    sc_biguint()
	: sc_unsigned( W )
	{}

    sc_biguint( const sc_biguint<W>& v )
	: sc_unsigned( W )
	{ *this = v; }

    sc_biguint( const sc_unsigned& v )
	: sc_unsigned( W )
	{ *this = v; }

    sc_biguint( const sc_unsigned_subref& v )
	: sc_unsigned( W )
	{ *this = v; }

    template< class T >
    sc_biguint( const sc_generic_base<T>& a )
	: sc_unsigned( W )
	{ a->to_sc_unsigned(*this); }

    sc_biguint( const sc_signed& v )
	: sc_unsigned( W )
	{ *this = v; }

    sc_biguint( const sc_signed_subref& v )
	: sc_unsigned( W )
	{ *this = v; }

    sc_biguint( const char* v )
	: sc_unsigned( W )
	{ *this = v; } 

    sc_biguint( int64 v )
	: sc_unsigned( W )
	{ *this = v; }

    sc_biguint( uint64 v )
	: sc_unsigned( W )
	{ *this = v; }

    sc_biguint( long v )
	: sc_unsigned( W )
	{ *this = v; }

    sc_biguint( unsigned long v )
	: sc_unsigned( W )
	{ *this = v; }

    sc_biguint( int v )
	: sc_unsigned( W )
	{ *this = v; } 

    sc_biguint( unsigned int v )
	: sc_unsigned( W )
	{ *this = v; }

    sc_biguint( double v )
	: sc_unsigned( W )
	{ *this = v; }
  
    sc_biguint( const sc_bv_base& v )
	: sc_unsigned( W )
	{ *this = v; }

    sc_biguint( const sc_lv_base& v )
	: sc_unsigned( W )
	{ *this = v; }

#ifdef SC_INCLUDE_FX

    explicit sc_biguint( const sc_fxval& v )
	: sc_unsigned( W )
	{ *this = v; }

    explicit sc_biguint( const sc_fxval_fast& v )
	: sc_unsigned( W )
	{ *this = v; }

    explicit sc_biguint( const sc_fxnum& v )
	: sc_unsigned( W )
	{ *this = v; }

    explicit sc_biguint( const sc_fxnum_fast& v )
	: sc_unsigned( W )
	{ *this = v; }

#endif


#ifndef SC_MAX_NBITS

    // destructor

    ~sc_biguint()
	{}

#endif
 

    // assignment operators

    sc_biguint<W>& operator = ( const sc_biguint<W>& v )
	{ sc_unsigned::operator = ( v ); return *this; }

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

    sc_biguint<W>& operator = ( int64 v )
	{ sc_unsigned::operator = ( v ); return *this; }

    sc_biguint<W>& operator = ( uint64 v )
	{ sc_unsigned::operator = ( v ); return *this; }

    sc_biguint<W>& operator = ( long v )
	{ sc_unsigned::operator = ( v ); return *this; }

    sc_biguint<W>& operator = ( unsigned long v )
	{ sc_unsigned::operator = ( v ); return *this; }

    sc_biguint<W>& operator = ( int v ) 
	{ sc_unsigned::operator = ( v ); return *this; }

    sc_biguint<W>& operator = ( unsigned int v ) 
	{ sc_unsigned::operator = ( v ); return *this; }

    sc_biguint<W>& operator = ( double v )
	{ sc_unsigned::operator = ( v ); return *this; }


    sc_biguint<W>& operator = ( const sc_bv_base& v )
	{ sc_unsigned::operator = ( v ); return *this; }

    sc_biguint<W>& operator = ( const sc_lv_base& v )
	{ sc_unsigned::operator = ( v ); return *this; }

    sc_biguint<W>& operator = ( const sc_int_base& v )
	{ sc_unsigned::operator = ( v ); return *this; }

    sc_biguint<W>& operator = ( const sc_uint_base& v )
	{ sc_unsigned::operator = ( v ); return *this; }

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
};

} // namespace sc_dt


#endif
