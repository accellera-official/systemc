/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
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

  sc_bigint.h -- Template version of sc_signed. This class enables
                 compile-time bit widths for sc_signed numbers.

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

// $Log: sc_bigint.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:36  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:49:31  acg
// Added $Log command so that CVS check in comments are reproduced in the
// source.
//

#ifndef SC_BIGINT_H
#define SC_BIGINT_H


#include "sysc/datatypes/int/sc_signed.h"
#include "sysc/datatypes/int/sc_unsigned.h"

namespace sc_dt
{

// classes defined in this module
template <int W> class sc_bigint;

// forward class declarations
class sc_bv_base;
class sc_lv_base;
class sc_fxval;
class sc_fxval_fast;
class sc_fxnum;
class sc_fxnum_fast;


// ----------------------------------------------------------------------------
//  CLASS TEMPLATE : sc_bigint<W>
//
//  Arbitrary size signed integer type.
// ----------------------------------------------------------------------------

#ifdef SC_MAX_NBITS
template< int W = SC_MAX_NBITS >
#else
template< int W >
#endif
class sc_bigint
    : public sc_signed
{
public:

    // constructors

    sc_bigint()
	: sc_signed( W )
	{}

    sc_bigint( const sc_bigint<W>& v )
	: sc_signed( W )
	{ *this = v; }

    sc_bigint( const sc_signed& v )
	: sc_signed( W )
	{ *this = v; }

    sc_bigint( const sc_signed_subref& v )
	: sc_signed( W )
	{ *this = v; }

    template< class T >
    sc_bigint( const sc_generic_base<T>& a )
	: sc_signed( W )
	{ a->to_sc_signed(*this); }

    sc_bigint( const sc_unsigned& v )
	: sc_signed( W )
	{ *this = v; }

    sc_bigint( const sc_unsigned_subref& v )
	: sc_signed( W )
	{ *this = v; }

    sc_bigint( const char* v )
	: sc_signed( W )
	{ *this = v; }

    sc_bigint( int64 v )
	: sc_signed( W )
	{ *this = v; }

    sc_bigint( uint64 v )
	: sc_signed( W )
	{ *this = v; }

    sc_bigint( long v )
	: sc_signed( W )
	{ *this = v; }

    sc_bigint( unsigned long v )
	: sc_signed( W )
	{ *this = v; }

    sc_bigint( int v )
	: sc_signed( W )
	{ *this = v; }

    sc_bigint( unsigned int v )
	: sc_signed( W )
	{ *this = v; }

    sc_bigint( double v )
	: sc_signed( W )
	{ *this = v; }
  
    sc_bigint( const sc_bv_base& v )
	: sc_signed( W )
	{ *this = v; }

    sc_bigint( const sc_lv_base& v )
	: sc_signed( W )
	{ *this = v; }

#ifdef SC_INCLUDE_FX

    explicit sc_bigint( const sc_fxval& v )
	: sc_signed( W )
	{ *this = v; }

    explicit sc_bigint( const sc_fxval_fast& v )
	: sc_signed( W )
	{ *this = v; }

    explicit sc_bigint( const sc_fxnum& v )
	: sc_signed( W )
	{ *this = v; }

    explicit sc_bigint( const sc_fxnum_fast& v )
	: sc_signed( W )
	{ *this = v; }

#endif


#ifndef SC_MAX_NBITS

    // destructor

    ~sc_bigint()
	{}

#endif
 
    // assignment operators

    sc_bigint<W>& operator = ( const sc_bigint<W>& v )
	{ sc_signed::operator = ( v ); return *this; }

    sc_bigint<W>& operator = ( const sc_signed& v )
	{ sc_signed::operator = ( v ); return *this; }

    sc_bigint<W>& operator = (const sc_signed_subref& v )
	{ sc_signed::operator = ( v ); return *this; }

    template< class T >
    sc_bigint<W>& operator = ( const sc_generic_base<T>& a )
	{ a->to_sc_signed(*this); return *this;}

    sc_bigint<W>& operator = ( const sc_unsigned& v )
	{ sc_signed::operator = ( v ); return *this; }

    sc_bigint<W>& operator = ( const sc_unsigned_subref& v )
	{ sc_signed::operator = ( v ); return *this; }

    sc_bigint<W>& operator = ( const char* v )
	{ sc_signed::operator = ( v ); return *this; }

    sc_bigint<W>& operator = ( int64 v )
	{ sc_signed::operator = ( v ); return *this; }

    sc_bigint<W>& operator = ( uint64 v )
	{ sc_signed::operator = ( v ); return *this; }

    sc_bigint<W>& operator = ( long v )
	{ sc_signed::operator = ( v ); return *this; }

    sc_bigint<W>& operator = ( unsigned long v )
	{ sc_signed::operator = ( v ); return *this; }

    sc_bigint<W>& operator = ( int v )
	{ sc_signed::operator = ( v ); return *this; }

    sc_bigint<W>& operator = ( unsigned int v )
	{ sc_signed::operator = ( v ); return *this; }

    sc_bigint<W>& operator = ( double v )
	{ sc_signed::operator = ( v ); return *this; }


    sc_bigint<W>& operator = ( const sc_bv_base& v )
	{ sc_signed::operator = ( v ); return *this; }

    sc_bigint<W>& operator = ( const sc_lv_base& v )
	{ sc_signed::operator = ( v ); return *this; }

    sc_bigint<W>& operator = ( const sc_int_base& v )
	{ sc_signed::operator = ( v ); return *this; }

    sc_bigint<W>& operator = ( const sc_uint_base& v )
	{ sc_signed::operator = ( v ); return *this; }

#ifdef SC_INCLUDE_FX

    sc_bigint<W>& operator = ( const sc_fxval& v )
	{ sc_signed::operator = ( v ); return *this; }

    sc_bigint<W>& operator = ( const sc_fxval_fast& v )
	{ sc_signed::operator = ( v ); return *this; }

    sc_bigint<W>& operator = ( const sc_fxnum& v )
	{ sc_signed::operator = ( v ); return *this; }

    sc_bigint<W>& operator = ( const sc_fxnum_fast& v )
	{ sc_signed::operator = ( v ); return *this; }

#endif
};

} // namespace sc_dt


#endif
