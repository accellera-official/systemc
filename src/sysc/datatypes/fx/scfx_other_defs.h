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

  scfx_other_defs.h - 

  Original Author: Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SCFX_OTHER_DEFS_H
#define SCFX_OTHER_DEFS_H


#include "sysc/datatypes/fx/sc_fx_ids.h"
#include "sysc/datatypes/int/sc_signed.h"
#include "sysc/datatypes/int/sc_unsigned.h"
#include "sysc/datatypes/int/sc_int_base.h"
#include "sysc/datatypes/int/sc_uint_base.h"
#include "sysc/tracing/sc_trace.h"


namespace sc_dt
{

#ifdef SC_INCLUDE_FX

// ----------------------------------------------------------------------------
//  CLASS : sc_signed
// ----------------------------------------------------------------------------

// assignment operators

inline
const sc_signed&
sc_signed::operator = ( const sc_fxval& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	SC_REPORT_ERROR( sc_core::SC_ID_INVALID_FX_VALUE_,
			 "sc_signed::operator = ( const sc_fxval& )" );
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}

inline
const sc_signed&
sc_signed::operator = ( const sc_fxval_fast& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	SC_REPORT_ERROR( sc_core::SC_ID_INVALID_FX_VALUE_,
			 "sc_signed::operator = ( const sc_fxval_fast& )" );
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}

inline
const sc_signed&
sc_signed::operator = ( const sc_fxnum& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	SC_REPORT_ERROR( sc_core::SC_ID_INVALID_FX_VALUE_,
			 "sc_signed::operator = ( const sc_fxnum& )" );
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}

inline
const sc_signed&
sc_signed::operator = ( const sc_fxnum_fast& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	SC_REPORT_ERROR( sc_core::SC_ID_INVALID_FX_VALUE_,
			 "sc_signed::operator = ( const sc_fxnum_fast& )" );
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned
// ----------------------------------------------------------------------------

// assignment operators

inline
const sc_unsigned&
sc_unsigned::operator = ( const sc_fxval& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	SC_REPORT_ERROR( sc_core::SC_ID_INVALID_FX_VALUE_,
			 "sc_unsigned::operator = ( const sc_fxval& )" );
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}

inline
const sc_unsigned&
sc_unsigned::operator = ( const sc_fxval_fast& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	SC_REPORT_ERROR( sc_core::SC_ID_INVALID_FX_VALUE_,
			 "sc_unsigned::operator = ( const sc_fxval_fast& )" );
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}

inline
const sc_unsigned&
sc_unsigned::operator = ( const sc_fxnum& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	SC_REPORT_ERROR( sc_core::SC_ID_INVALID_FX_VALUE_,
			 "sc_unsigned::operator = ( const sc_fxnum& )" );
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}

inline
const sc_unsigned&
sc_unsigned::operator = ( const sc_fxnum_fast& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	SC_REPORT_ERROR( sc_core::SC_ID_INVALID_FX_VALUE_,
			 "sc_unsigned::operator = ( const sc_fxnum_fast& )" );
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_int_base
// ----------------------------------------------------------------------------

#ifndef _32BIT_
#define NUM_WIDTH LLWIDTH
#else
#define NUM_WIDTH INTWIDTH
#endif


// assignment operators

inline
sc_int_base&
sc_int_base::operator = ( const sc_fxval& v )
{
    if( ! v.is_normal() ) { /* also triggers OBSERVER_READ call */
	SC_REPORT_ERROR( sc_core::SC_ID_INVALID_FX_VALUE_,
			 "sc_int_base::operator = ( const sc_fxval& )" );
    }
    for( int i = 0; i < m_len; ++ i ) {
	set( i, v.get_bit( i ) );
    }
    extend_sign();
    return *this;
}

inline
sc_int_base&
sc_int_base::operator = ( const sc_fxval_fast& v )
{
    if( ! v.is_normal() ) { /* also triggers OBSERVER_READ call */
	SC_REPORT_ERROR( sc_core::SC_ID_INVALID_FX_VALUE_,
			 "sc_int_base::operator = ( const sc_fxval_fast& )" );
    }
    for( int i = 0; i < m_len; ++ i ) {
	set( i, v.get_bit( i ) );
    }
    extend_sign();
    return *this;
}

inline
sc_int_base&
sc_int_base::operator = ( const sc_fxnum& v )
{
    if( ! v.is_normal() ) { /* also triggers OBSERVER_READ call */
	SC_REPORT_ERROR( sc_core::SC_ID_INVALID_FX_VALUE_,
			 "sc_int_base::operator = ( const sc_fxnum& )" );
    }
    for( int i = 0; i < m_len; ++ i ) {
	set( i, v.get_bit( i ) );
    }
    extend_sign();
    return *this;
}

inline
sc_int_base&
sc_int_base::operator = ( const sc_fxnum_fast& v )
{
    if( ! v.is_normal() ) { /* also triggers OBSERVER_READ call */
	SC_REPORT_ERROR( sc_core::SC_ID_INVALID_FX_VALUE_,
			 "sc_int_base::operator = ( const sc_fxnum_fast& )" );
    }
    for( int i = 0; i < m_len; ++ i ) {
	set( i, v.get_bit( i ) );
    }
    extend_sign();
    return *this;
}

#undef NUM_WIDTH


// ----------------------------------------------------------------------------
//  CLASS : sc_uint_base
// ----------------------------------------------------------------------------

// assignment operators

inline
sc_uint_base&
sc_uint_base::operator = ( const sc_fxval& v )
{
    if( ! v.is_normal() ) { /* also triggers OBSERVER_READ call */
	SC_REPORT_ERROR( sc_core::SC_ID_INVALID_FX_VALUE_,
			 "sc_uint_base::operator = ( const sc_fxval& )" );
    }
    for( int i = 0; i < m_len; ++ i ) {
	set( i, v.get_bit( i ) );
    }
    extend_sign();
    return *this;
}

inline
sc_uint_base&
sc_uint_base::operator = ( const sc_fxval_fast& v )
{
    if( ! v.is_normal() ) { /* also triggers OBSERVER_READ call */
	SC_REPORT_ERROR( sc_core::SC_ID_INVALID_FX_VALUE_,
			 "sc_uint_base::operator = ( const sc_fxval_fast& )" );
    }
    for( int i = 0; i < m_len; ++ i ) {
	set( i, v.get_bit( i ) );
    }
    extend_sign();
    return *this;
}

inline
sc_uint_base&
sc_uint_base::operator = ( const sc_fxnum& v )
{
    if( ! v.is_normal() ) { /* also triggers OBSERVER_READ call */
	SC_REPORT_ERROR( sc_core::SC_ID_INVALID_FX_VALUE_,
			 "sc_uint_base::operator = ( const sc_fxnum& )" );
    }
    for( int i = 0; i < m_len; ++ i ) {
	set( i, v.get_bit( i ) );
    }
    extend_sign();
    return *this;
}

inline
sc_uint_base&
sc_uint_base::operator = ( const sc_fxnum_fast& v )
{
    if( ! v.is_normal() ) { /* also triggers OBSERVER_READ call */
	SC_REPORT_ERROR( sc_core::SC_ID_INVALID_FX_VALUE_,
			 "sc_uint_base::operator = ( const sc_fxnum_fast& )" );
    }
    for( int i = 0; i < m_len; ++ i ) {
	set( i, v.get_bit( i ) );
    }
    extend_sign();
    return *this;
}


#endif


// ----------------------------------------------------------------------------
//  FUNCTION : sc_trace
// ----------------------------------------------------------------------------

inline
void
sc_trace( sc_core::sc_trace_file* tf,
	  const sc_fxval& object, const std::string& name )
{
    if( tf )
	tf->trace( object, name );
}

inline
void
sc_trace( sc_core::sc_trace_file* tf,
	  const sc_fxval* object, const std::string& name )
{
    if( tf )
	tf->trace( *object, name );
}

inline
void
sc_trace( sc_core::sc_trace_file* tf,
	  const sc_fxval_fast& object, const std::string& name )
{
    if( tf )
	tf->trace( object, name );
}

inline
void
sc_trace( sc_core::sc_trace_file* tf,
	  const sc_fxval_fast* object, const std::string& name )
{
    if( tf )
	tf->trace( *object, name );
}

inline
void
sc_trace( sc_core::sc_trace_file* tf,
	  const sc_fxnum& object, const std::string& name )
{
    if( tf )
	tf->trace( object, name );
}

inline
void
sc_trace( sc_core::sc_trace_file* tf,
	  const sc_fxnum* object, const std::string& name )
{
    if( tf )
	tf->trace( *object, name );
}

inline
void
sc_trace( sc_core::sc_trace_file* tf,
	  const sc_fxnum_fast& object, const std::string& name )
{
    if( tf )
	tf->trace( object, name );
}

inline
void
sc_trace( sc_core::sc_trace_file* tf,
	  const sc_fxnum_fast* object, const std::string& name )
{
    if( tf )
	tf->trace( *object, name );
}

} // namespace sc_dt


#endif

// Taf!
