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

  sc_time.h -- The time class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_time.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

#ifndef SC_TIME_H
#define SC_TIME_H


#include "sysc/datatypes/int/sc_nbdefs.h"
#include "sysc/datatypes/fx/scfx_ieee.h"
#include "sysc/utils/sc_iostream.h"
#include "sysc/utils/sc_string.h"

namespace sc_core {

class sc_simcontext;

// friend operator declarations

    const sc_time operator + ( const sc_time&, const sc_time& );
    const sc_time operator - ( const sc_time&, const sc_time& );
    const sc_time operator * ( const sc_time&, double );
    const sc_time operator * ( double, const sc_time& );
    const sc_time operator / ( const sc_time&, double );
    double        operator / ( const sc_time&, const sc_time& );


// ----------------------------------------------------------------------------
//  ENUM : sc_time_unit
//
//  Enumeration of time units.
// ----------------------------------------------------------------------------

enum sc_time_unit
{
    SC_FS = 0,
    SC_PS,
    SC_NS,
    SC_US,
    SC_MS,
    SC_SEC
};


// ----------------------------------------------------------------------------
//  CLASS : sc_time
//
//  The time class.
// ----------------------------------------------------------------------------

class sc_time
{
public:

    // constructors

    sc_time();
    sc_time( double, sc_time_unit );
    sc_time( double, sc_time_unit, sc_simcontext* );
    sc_time( double, bool scale );
    sc_time( sc_dt::uint64, bool scale );
    sc_time( const sc_time& );


    // assignment operator

    sc_time& operator = ( const sc_time& );


    // conversion functions

    sc_dt::uint64 value() const;      // relative to the time resolution
    double to_double() const;  // relative to the time resolution
    double to_default_time_units() const;
    double to_seconds() const;
    const std::string to_string() const;


    // relational operators

    bool operator == ( const sc_time& ) const;
    bool operator != ( const sc_time& ) const;
    bool operator <  ( const sc_time& ) const;
    bool operator <= ( const sc_time& ) const;
    bool operator >  ( const sc_time& ) const;
    bool operator >= ( const sc_time& ) const;


    // arithmetic operators

    sc_time& operator += ( const sc_time& );
    sc_time& operator -= ( const sc_time& );

    friend const sc_time operator + ( const sc_time&, const sc_time& );
    friend const sc_time operator - ( const sc_time&, const sc_time& );

    sc_time& operator *= ( double );
    sc_time& operator /= ( double );

    friend const sc_time operator * ( const sc_time&, double );
    friend const sc_time operator * ( double, const sc_time& );
    friend const sc_time operator / ( const sc_time&, double );
    friend double        operator / ( const sc_time&, const sc_time& );


    // print function

    void print( ::std::ostream& os = std::cout ) const;

private:

    sc_dt::uint64 m_value;
};


// print operator

inline ::std::ostream& operator << ( ::std::ostream&, const sc_time& );


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// constructors

inline
sc_time::sc_time()
: m_value( 0 )
{}

inline
sc_time::sc_time( const sc_time& t )
: m_value( t.m_value )
{}


// assignment operator

inline
sc_time&
sc_time::operator = ( const sc_time& t )
{
    m_value = t.m_value;
    return *this;
}


// conversion functions

inline
sc_dt::uint64
sc_time::value() const  // relative to the time resolution
{
    return m_value;
}


inline
double
sc_time::to_double() const  // relative to the time resolution
{
    return sc_dt::uint64_to_double( m_value );
}


// relational operators

inline
bool
sc_time::operator == ( const sc_time& t ) const
{
    return ( m_value == t.m_value );
}

inline
bool
sc_time::operator != ( const sc_time& t ) const
{
    return ( m_value != t.m_value );
}

inline
bool
sc_time::operator < ( const sc_time& t ) const
{
    return ( m_value < t.m_value );
}

inline
bool
sc_time::operator <= ( const sc_time& t ) const
{
    return ( m_value <= t.m_value );
}

inline
bool
sc_time::operator > ( const sc_time& t ) const
{
    return ( m_value > t.m_value );
}

inline
bool
sc_time::operator >= ( const sc_time& t ) const
{
    return ( m_value >= t.m_value );
}


// arithmetic operators

inline
sc_time&
sc_time::operator += ( const sc_time& t )
{
    m_value += t.m_value;
    return *this;
}

inline
sc_time&
sc_time::operator -= ( const sc_time& t )
{
    m_value -= t.m_value;
    return *this;
}


inline
const sc_time
operator + ( const sc_time& t1, const sc_time& t2 )
{
    return sc_time( t1 ) += t2;
}

inline
const sc_time
operator - ( const sc_time& t1, const sc_time& t2 )
{
    return sc_time( t1 ) -= t2;
}


inline
sc_time&
sc_time::operator *= ( double d )
{
    // linux bug workaround; don't change next two lines
    volatile double tmp = sc_dt::uint64_to_double( m_value ) * d + 0.5;
    m_value = SCAST<sc_dt::int64>( tmp );
    return *this;
}

inline
sc_time&
sc_time::operator /= ( double d )
{
    // linux bug workaround; don't change next two lines
    volatile double tmp = sc_dt::uint64_to_double( m_value ) / d + 0.5;
    m_value = SCAST<sc_dt::int64>( tmp );
    return *this;
}


inline
const sc_time
operator * ( const sc_time& t, double d )
{
    sc_time tmp( t );
    return tmp *= d;
}

inline
const sc_time
operator * ( double d, const sc_time& t )
{
    sc_time tmp( t );
    return tmp *= d;
}

inline
const sc_time
operator / ( const sc_time& t, double d )
{
    sc_time tmp( t );
    return tmp /= d;
}

inline
double
operator / ( const sc_time& t1, const sc_time& t2 )
{
    return ( t1.to_double() / t2.to_double() );
}


// print operator

inline
::std::ostream&
operator << ( ::std::ostream& os, const sc_time& t )
{
    t.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  STRUCT : sc_time_params
//
//  Struct that holds the time resolution and default time unit.
// ----------------------------------------------------------------------------

struct sc_time_params
{
    double time_resolution;		// in femto seconds
    bool   time_resolution_specified;
    bool   time_resolution_fixed;

    sc_dt::uint64 default_time_unit;		// in time resolution
    bool   default_time_unit_specified;

    sc_time_params();
    ~sc_time_params();
};


// ----------------------------------------------------------------------------

extern const sc_time SC_ZERO_TIME;


// functions for accessing the time resolution and default time unit

extern void    sc_set_time_resolution( double, sc_time_unit );
extern sc_time sc_get_time_resolution();

extern void    sc_set_default_time_unit( double, sc_time_unit );
extern sc_time sc_get_default_time_unit();

} // namespace sc_core

#endif

// Taf!
