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

  sc_bv.h -- Arbitrary size bit vector class.

  Original Author: Gene Bushuyev, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_BV_H
#define SC_BV_H


#include "sysc/datatypes/bit/sc_bv_base.h"


namespace sc_dt
{

// classes defined in this module
template <int W> class sc_bv;


// ----------------------------------------------------------------------------
//  CLASS TEMPLATE : sc_bv<W>
//
//  Arbitrary size bit vector class.
// ----------------------------------------------------------------------------

template <int W>
class sc_bv
    : public sc_bv_base
{
public:

    // constructors

    sc_bv()
	:sc_bv_base( W )
	{}

    explicit sc_bv( bool init_value )
	: sc_bv_base( init_value, W )
	{}

    explicit sc_bv( char init_value )
	: sc_bv_base( (init_value != '0'), W )
	{}

    sc_bv( const char* a )
	: sc_bv_base( W )
	{ sc_bv_base::operator = ( a ); }

    sc_bv( const bool* a )
	: sc_bv_base( W )
	{ sc_bv_base::operator = ( a ); }

    sc_bv( const sc_logic* a )
	: sc_bv_base( W )
	{ sc_bv_base::operator = ( a ); }

    sc_bv( const sc_unsigned& a )
	: sc_bv_base( W )
	{ sc_bv_base::operator = ( a ); }

    sc_bv( const sc_signed& a )
	: sc_bv_base( W )
	{ sc_bv_base::operator = ( a ); }

    sc_bv( const sc_uint_base& a )
	: sc_bv_base( W )
	{ sc_bv_base::operator = ( a ); }

    sc_bv( const sc_int_base& a )
	: sc_bv_base( W )
	{ sc_bv_base::operator = ( a ); }

    sc_bv( unsigned long a )
	: sc_bv_base( W )
	{ sc_bv_base::operator = ( a ); }

    sc_bv( long a )
	: sc_bv_base( W )
	{ sc_bv_base::operator = ( a ); }

    sc_bv( unsigned int a )
	: sc_bv_base( W )
	{ sc_bv_base::operator = ( a ); }

    sc_bv( int a )
	: sc_bv_base( W )
	{ sc_bv_base::operator = ( a ); }

    sc_bv( uint64 a )
	: sc_bv_base( W )
	{ sc_bv_base::operator = ( a ); }

    sc_bv( int64 a )
	: sc_bv_base( W )
	{ sc_bv_base::operator = ( a ); }

    template <class X>
    sc_bv( const sc_proxy<X>& a )
	: sc_bv_base( W )
	{ sc_bv_base::operator = ( a ); }

    sc_bv( const sc_bv<W>& a )
	: sc_bv_base( a )
	{}


    // assignment operators

    template <class X>
    sc_bv<W>& operator = ( const sc_proxy<X>& a )
	{ sc_bv_base::operator = ( a ); return *this; }

    sc_bv<W>& operator = ( const sc_bv<W>& a )
	{ sc_bv_base::operator = ( a ); return *this; }

    sc_bv<W>& operator = ( const char* a )
	{ sc_bv_base::operator = ( a ); return *this; }

    sc_bv<W>& operator = ( const bool* a )
	{ sc_bv_base::operator = ( a ); return *this; }

    sc_bv<W>& operator = ( const sc_logic* a )
	{ sc_bv_base::operator = ( a ); return *this; }

    sc_bv<W>& operator = ( const sc_unsigned& a )
	{ sc_bv_base::operator = ( a ); return *this; }

    sc_bv<W>& operator = ( const sc_signed& a )
	{ sc_bv_base::operator = ( a ); return *this; }

    sc_bv<W>& operator = ( const sc_uint_base& a )
	{ sc_bv_base::operator = ( a ); return *this; }

    sc_bv<W>& operator = ( const sc_int_base& a )
	{ sc_bv_base::operator = ( a ); return *this; }

    sc_bv<W>& operator = ( unsigned long a )
	{ sc_bv_base::operator = ( a ); return *this; }

    sc_bv<W>& operator = ( long a )
	{ sc_bv_base::operator = ( a ); return *this; }

    sc_bv<W>& operator = ( unsigned int a )
	{ sc_bv_base::operator = ( a ); return *this; }

    sc_bv<W>& operator = ( int a )
	{ sc_bv_base::operator = ( a ); return *this; }

    sc_bv<W>& operator = ( uint64 a )
	{ sc_bv_base::operator = ( a ); return *this; }

    sc_bv<W>& operator = ( int64 a )
	{ sc_bv_base::operator = ( a ); return *this; }
};

} // namespace sc_dt


#endif
