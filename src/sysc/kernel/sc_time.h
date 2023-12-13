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

  sc_time.h -- The time class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/


#ifndef SC_TIME_H
#define SC_TIME_H


#include "sysc/datatypes/int/sc_nbdefs.h"
#include "sysc/datatypes/fx/scfx_ieee.h"

#include <iostream>

#if SC_CPLUSPLUS >= 201703L
#include <string_view>
#endif

namespace sc_core {

// forward declarations

class sc_simcontext;
class sc_module;
class sc_event_or_list;
class sc_event_and_list;
class sc_prim_channel;
class sc_event;
class SC_API sc_time_tuple;

// friend operator declarations

    const sc_time operator + ( const sc_time&, const sc_time& );
    const sc_time operator - ( const sc_time&, const sc_time& );
    const sc_time operator * ( const sc_time&, double );
    const sc_time operator * ( double, const sc_time& );
    const sc_time operator / ( const sc_time&, double );
    double        operator / ( const sc_time&, const sc_time& );

// ----------------------------------------------------------------------------
// Internal time representation
//
// Implementation defined
// LRM: Time shall be represented internally as an unsigned
// integer of at least 64 bits declared as sc_time::value_type.
// ----------------------------------------------------------------------------

#define SC_TIME_DT sc_dt::uint64 

// ----------------------------------------------------------------------------
//  ENUM : sc_time_unit
//
//  Enumeration of time units.
//  NOTE: From IEEE Std 1666-2023 onwards, enumeration constant values are 
//        implementation-defined. The constant values for SC_SEC, SC_MS, 
//        SC_US, SC_NS, SC_PS and SC_FS follow IEEE Std 1666-2011 to enable 
//        backwards compatibility.
// ----------------------------------------------------------------------------

enum sc_time_unit { SC_SEC = 5, SC_MS = 4, SC_US = 3, SC_NS = 2, 
                    SC_PS = 1, SC_FS = 0, SC_AS = -1, SC_ZS = -2, SC_YS = -3 };

// ----------------------------------------------------------------------------
//  CLASS : sc_time
//
//  The time class.
// ----------------------------------------------------------------------------

class SC_API sc_time
{
    friend class sc_module;
    friend class sc_prim_channel;
    friend class sc_event;
    friend class sc_clock;

    friend void next_trigger( const sc_time&, const sc_event_or_list&, sc_simcontext* );
    friend void next_trigger( const sc_time&, const sc_event&, sc_simcontext* );
    friend void next_trigger( const sc_time&, const sc_event_and_list&, sc_simcontext* );
    friend void next_trigger( double v, sc_time_unit, sc_simcontext* );
    friend void next_trigger( double v, sc_time_unit, const sc_event&, sc_simcontext* );
    friend void next_trigger( double v, sc_time_unit, const sc_event_and_list&, sc_simcontext* );
    friend void next_trigger( double v, sc_time_unit, const sc_event_or_list&, sc_simcontext* );
    friend void wait( const sc_time&, sc_simcontext* );
    friend void wait( const sc_time&, const sc_event&, sc_simcontext* );
    friend void wait( const sc_time&, const sc_event_and_list&, sc_simcontext* );
    friend void wait( double v, sc_time_unit, sc_simcontext* );
    friend void wait( double v, sc_time_unit, const sc_event&, sc_simcontext* );
    friend void wait( double v, sc_time_unit, const sc_event_and_list&, sc_simcontext* );
    friend void wait( double v, sc_time_unit, const sc_event_or_list&, sc_simcontext* );

public:

    typedef SC_TIME_DT value_type;

    // constructors

    sc_time();
    sc_time( const sc_time& );
    sc_time( double, sc_time_unit );

    // convert time object from string
    // For C++ versions prior to C++17, offer some (non-standard) backwards compatibility
    // using std::string instead of std::string_view
#if SC_CPLUSPLUS >= 201703L
    explicit sc_time( std::string_view strv );
    static sc_time from_string( std::string_view strv );
#else
    explicit sc_time( const std::string& str );
    static sc_time from_string( const std::string& str );
#endif

    // deprecated, use from_value(v)
    sc_time( double, bool scale );
    sc_time( value_type, bool scale );

    // assignment operator

    sc_time& operator = ( const sc_time& );

    // conversion functions

    value_type value() const;  // relative to the time resolution
    double to_double() const;  // relative to the time resolution
    double to_seconds() const;
    double to_default_time_units() const;
    const std::string to_string() const;

    static sc_time from_value( value_type );
    static sc_time from_seconds( double );

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
    sc_time& operator *= ( double );
    sc_time& operator /= ( double );
    sc_time& operator %= ( const sc_time& );

    friend const sc_time operator + ( const sc_time&, const sc_time& );
    friend const sc_time operator - ( const sc_time&, const sc_time& );
    friend const sc_time operator * ( const sc_time&, double );
    friend const sc_time operator * ( double, const sc_time& );
    friend const sc_time operator / ( const sc_time&, double );
    friend double        operator / ( const sc_time&, const sc_time& );
    friend const sc_time operator % ( const sc_time&, const sc_time& );

    // print function

    void print( ::std::ostream& os = std::cout ) const;

private: // implementation-defined
    sc_time( double, sc_time_unit, sc_simcontext* );

private:
    value_type m_value;
};

// ----------------------------------------------------------------------------
//  CLASS : sc_time_tuple
//
//  The time tuple helper class.
// ----------------------------------------------------------------------------

class SC_API sc_time_tuple
{
    typedef sc_time::value_type value_type;
    friend class sc_time;

private:
    explicit sc_time_tuple( value_type v );
    void init( value_type v );

public:
    sc_time_tuple()
      : m_value(), m_unit( SC_SEC ), m_offset(1) {}

    sc_time_tuple( const sc_time & t );

    bool         has_value()   const;
    value_type   value()       const;
    sc_time_unit unit()        const { return m_unit; } // normalized unit
    const char * unit_symbol() const;                   // normalized unit symbol

    operator sc_time() const { return sc_time( to_double(), m_unit ); }

    double      to_double() const; // relative to the normalized unit
    std::string to_string() const;

private:
    value_type   m_value;
    sc_time_unit m_unit;
    unsigned     m_offset;
};

// stream operator for printing

inline ::std::ostream& operator << ( ::std::ostream&, const sc_time& );

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

extern SC_API const sc_time SC_ZERO_TIME;

// constructors

inline
sc_time::sc_time()
: m_value( 0 )
{}

inline
sc_time::sc_time( const sc_time& t )
: m_value( t.m_value )
{}

inline
sc_time_tuple::sc_time_tuple( value_type v )
  : m_value(), m_unit( SC_SEC ), m_offset(1)
{
    if( v )
        init( v );
}

inline
sc_time_tuple::sc_time_tuple( const sc_time& t )
  : m_value(), m_unit( SC_SEC ), m_offset(1)
{
    if( t != SC_ZERO_TIME )
        init( t.value() );
}

inline
sc_time
sc_time::from_seconds( double v )
{
    return sc_time( v, SC_SEC );
}


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
sc_time::value_type
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


inline
double
sc_time_tuple::to_double() const // relative to the normalized time unit
{
    return sc_dt::uint64_to_double( m_value ) * m_offset;
}


inline
const std::string
sc_time::to_string() const
{
    return sc_time_tuple( *this ).to_string();
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
    m_value = static_cast<sc_dt::int64>( tmp );
    return *this;
}

inline
sc_time&
sc_time::operator /= ( double d )
{
    // linux bug workaround; don't change next two lines
    volatile double tmp = sc_dt::uint64_to_double( m_value ) / d + 0.5;
    m_value = static_cast<sc_dt::int64>( tmp );
    return *this;
}

inline
sc_time&
sc_time::operator %= ( const sc_time& t )
{
    m_value %= t.m_value;
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

inline
const sc_time
operator % ( const sc_time& t1, const sc_time& t2 )
{
    sc_time tmp(t1);
    return tmp %= t2;
}

// operator<< for printing

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

struct SC_API sc_time_params
{
    double time_resolution;		// in femto seconds
    bool   time_resolution_specified;
    bool   time_resolution_fixed;

    sc_time::value_type default_time_unit;		// in time resolution
    bool                default_time_unit_specified;

    sc_time_params();
    ~sc_time_params();
};


// ----------------------------------------------------------------------------

// functions for accessing the time resolution and default time unit

SC_API extern void    sc_set_time_resolution( double, sc_time_unit );
SC_API extern sc_time sc_get_time_resolution();

SC_API extern void    sc_set_default_time_unit( double, sc_time_unit );
SC_API extern sc_time sc_get_default_time_unit();

} // namespace sc_core

#endif

// $Log: sc_time.h,v $
// Revision 1.5  2011/08/26 20:46:11  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
// Revision 1.4  2011/02/18 20:27:14  acg
//  Andy Goodrich: Updated Copyrights.
//
// Revision 1.3  2011/02/13 21:47:38  acg
//  Andy Goodrich: update copyright notice.
//
// Revision 1.2  2008/05/22 17:06:27  acg
//  Andy Goodrich: updated copyright notice to include 2008.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.4  2006/05/08 18:02:06  acg
//  Andy Goodrich: added David Long's forward declarations for friend
//   functions, methods, and operators to keep the Microsoft compiler happy.
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.

// Taf!
