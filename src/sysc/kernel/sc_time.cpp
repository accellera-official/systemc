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

  sc_time.cpp --

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/

#include "sysc/kernel/sc_time.h"

#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/utils/sc_utils_ids.h"

#include <cctype>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <sstream>

#ifdef SC_ENABLE_EARLY_MAXTIME_CREATION
#  define SC_MAXTIME_ALLOWED_ 1
#else
#  define SC_MAXTIME_ALLOWED_ 0
#endif

namespace sc_core {

static
double time_values[] = {
    1,       // fs
    1e3,     // ps
    1e6,     // ns
    1e9,     // us
    1e12,    // ms
    1e15     // s
};

static
const char* time_units[] = {
    "fs",
    "ps",
    "ns",
    "us",
    "ms",
    "s"
};

static
const char* time_units_sc[] = {
    "SC_FS",
    "SC_PS",
    "SC_NS",
    "SC_US",
    "SC_MS",
    "SC_SEC"
};

// ----------------------------------------------------------------------------
//  CLASS : sc_time_tuple
//
//  The time tuple helper class.
// ----------------------------------------------------------------------------

void
sc_time_tuple::init( value_type val )
{
    sc_time_params* time_params = sc_get_curr_simcontext()->m_time_params;
#   if SC_MAXTIME_ALLOWED_
        time_params->time_resolution_fixed = true;
#   endif // SC_MAXTIME_ALLOWED_

    value_type tr  = static_cast<sc_dt::int64>( time_params->time_resolution );
    unsigned scale = 0;
    while( ( tr % 10 ) == 0 ) {
        tr /= 10;
        scale++;
    }
    sc_assert( tr == 1 );

    unsigned tu = scale / 3;
    while( tu < SC_SEC && ( val % 10 ) == 0 ) {
        val /= 10;
        scale++;
        tu += ( 0 == ( scale % 3 ) );
    }

    m_value  = val;
    m_unit   = static_cast<sc_time_unit>( tu );
    m_offset = 1;
    for( scale %= 3; scale != 0 ; scale-- )
        m_offset *= 10;
}

bool
sc_time_tuple::has_value() const
{
    return ( m_value < ( (~sc_dt::UINT64_ZERO) / m_offset ) );
}

sc_time::value_type
sc_time_tuple::value() const
{
    if( !has_value() )
        SC_REPORT_ERROR( SC_ID_TIME_CONVERSION_FAILED_
                       , "sc_time_tuple value overflow" );
    return m_value * m_offset;
}

const char *
sc_time_tuple::unit_symbol() const
{
    return time_units[m_unit];
}

std::string
sc_time_tuple::to_string() const
{
    std::ostringstream oss;

    if ( !m_value ) {
        oss << "0 s";
    } else {
        oss << m_value;
        for( unsigned zeros = m_offset; zeros > 1; zeros /= 10 ) {
            oss << '0';
        }
        oss << ' ' << time_units[m_unit];
    }
    return oss.str();
}


// ----------------------------------------------------------------------------
//  CLASS : sc_time
//
//  The time class.
// ----------------------------------------------------------------------------

// constructors

namespace /* anonymous */ {
static sc_time::value_type
from_value_and_unit( double v, sc_time_unit tu, sc_time_params* tp )
{
    sc_time::value_type t = 0;
    if( v != 0 ) {
        double scale_fac = time_values[tu] / tp->time_resolution;
        // linux bug workaround; don't change next two lines
        volatile double tmp = v * scale_fac + 0.5;
        t = static_cast<sc_dt::int64>( tmp );
        tp->time_resolution_fixed = true;
    }
    return t;
}
} /* anonymous namespace */

sc_time::sc_time( double v, sc_time_unit tu )
  : m_value
      ( from_value_and_unit( v, tu, sc_get_curr_simcontext()->m_time_params ) )
{}

sc_time::sc_time( double v, sc_time_unit tu, sc_simcontext* simc )
  : m_value( from_value_and_unit( v, tu, simc->m_time_params ) )
{}

sc_time::sc_time( double v, bool scale )
: m_value( 0 )
{
    static bool warn_constructor=true;
    if ( warn_constructor ) {
        warn_constructor=false;
        SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
            "deprecated constructor: sc_time(double,bool)");
    }

    if( v != 0 ) {
	sc_time_params* time_params = sc_get_curr_simcontext()->m_time_params;
	if( scale ) {
	    double scale_fac = sc_dt::uint64_to_double(
		time_params->default_time_unit );
	    // linux bug workaround; don't change next two lines
	    volatile double tmp = v * scale_fac + 0.5;
	    m_value = static_cast<sc_dt::int64>( tmp );
	} else {
	    // linux bug workaround; don't change next two lines
	    volatile double tmp = v + 0.5;
	    m_value = static_cast<sc_dt::int64>( tmp );
	}
	time_params->time_resolution_fixed = true;
    }
}

sc_time::sc_time( value_type v, bool scale )
: m_value( 0 )
{
    static bool warn_constructor=true;
    if ( warn_constructor ) {
        warn_constructor=false;
        SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
            "deprecated constructor: sc_time(uint64,bool)");
    }

    if( v != 0 ) {
	sc_time_params* time_params = sc_get_curr_simcontext()->m_time_params;
	if( scale ) {
	    double scale_fac = sc_dt::uint64_to_double(
		time_params->default_time_unit );
	    // linux bug workaround; don't change next two lines
	    volatile double tmp = sc_dt::uint64_to_double( v ) *
		                  scale_fac + 0.5;
	    m_value = static_cast<sc_dt::int64>( tmp );
	} else {
	    m_value = v;
	}
	time_params->time_resolution_fixed = true;
    }
}

sc_time
sc_time::from_value( value_type v )
{
    sc_time t;
    if( v != 0 && !(SC_MAXTIME_ALLOWED_ && v == ~sc_dt::UINT64_ZERO) ) {
        sc_time_params* time_params = sc_get_curr_simcontext()->m_time_params;
        time_params->time_resolution_fixed = true;
    }
    t.m_value = v;
    return t;
}

namespace /* anonymous */ {
static sc_time::value_type
from_value_and_unit_symbol( double v, const char* unit, sc_time_params* tp )
{
    sc_time::value_type t = 0;
    if( !unit || !*unit ) {
        SC_REPORT_ERROR( SC_ID_TIME_CONVERSION_FAILED_, "no time unit given" );
        return t;
    }
    unsigned tu = SC_FS;
    while( tu <= SC_SEC
          && std::strcmp( unit, time_units[tu] ) != 0
          && std::strcmp( unit, time_units_sc[tu] ) != 0 )
      { ++tu; }

    if( tu > SC_SEC ) {
        SC_REPORT_ERROR( SC_ID_TIME_CONVERSION_FAILED_, "invalid unit given" );
        return t;
    }

    return from_value_and_unit( v, static_cast<sc_time_unit>(tu), tp );
}
} /* anonymous namespace */

sc_time::sc_time( double v, const char* unit )
  : m_value
     ( from_value_and_unit_symbol( v, unit, sc_get_curr_simcontext()->m_time_params ) )
{}

sc_time::sc_time( double v, const char* unit, sc_simcontext* simc )
  : m_value( from_value_and_unit_symbol( v, unit, simc->m_time_params ) )
{}

sc_time
sc_time::from_string( const char * str )
{
    char * endptr = NULL;
    double v = str ? std::strtod( str, &endptr ) : 0.0;
    if( str == endptr || v < 0.0 ) {
        SC_REPORT_ERROR( SC_ID_TIME_CONVERSION_FAILED_, "invalid value given" );
        return SC_ZERO_TIME;
    }
    while( *endptr && std::isspace( *endptr ) ) ++endptr; // skip whitespace
    return sc_time( v, endptr );
}

// conversion functions

double
sc_time::to_default_time_units() const
{
    sc_time_params* time_params = sc_get_curr_simcontext()->m_time_params;
#   if SC_MAXTIME_ALLOWED_
        if( m_value == 0 )
            return 0.0;
        time_params->time_resolution_fixed = true;
#   endif // SC_MAXTIME_ALLOWED_
    return ( sc_dt::uint64_to_double( m_value ) /
	     sc_dt::uint64_to_double( time_params->default_time_unit ) );
}

double
sc_time::to_seconds() const
{
    sc_time_params* time_params = sc_get_curr_simcontext()->m_time_params;
#   if SC_MAXTIME_ALLOWED_
        if( m_value == 0 )
            return 0.0;
        time_params->time_resolution_fixed = true;
#   endif // SC_MAXTIME_ALLOWED_
    return ( sc_dt::uint64_to_double( m_value ) *
	     time_params->time_resolution * 1e-15 );
}


// print function

void
sc_time::print( ::std::ostream& os ) const
{
    os << to_string();
}


// ----------------------------------------------------------------------------
//  STRUCT : sc_time_params
//
//  Struct that holds the time resolution and default time unit.
// ----------------------------------------------------------------------------

sc_time_params::sc_time_params()
: time_resolution( 1000 ),		// default 1 ps
  time_resolution_specified( false ),
  time_resolution_fixed( false ),
  default_time_unit( 1000 ),		// default 1 ns
  default_time_unit_specified( false )
{}

sc_time_params::~sc_time_params()
{}


// ----------------------------------------------------------------------------

// functions for accessing the time resolution and default time unit

void
sc_set_time_resolution( double v, sc_time_unit tu )
{
    // first perform the necessary checks

    // must be positive
    if( v < 0.0 ) {
	SC_REPORT_ERROR( SC_ID_SET_TIME_RESOLUTION_, "value not positive" );
    }

    // must be a power of ten
    double dummy;
#if defined( __HP_aCC ) || defined(__ppc__)
    // aCC seems to have a bug in modf()
    if( modf( log10( v < 1.0 ? 1.0/v : v ), &dummy ) != 0.0 ) {
#else
    if( modf( log10( v ), &dummy ) != 0.0 ) {
#endif
	SC_REPORT_ERROR( SC_ID_SET_TIME_RESOLUTION_,
			 "value not a power of ten" );
    }

    sc_simcontext* simc = sc_get_curr_simcontext();

    // can only be specified during elaboration
    if( sc_is_running() ) {
	SC_REPORT_ERROR( SC_ID_SET_TIME_RESOLUTION_, "simulation running" );
    }

    sc_time_params* time_params = simc->m_time_params;

    // can be specified only once
    if( time_params->time_resolution_specified ) {
	SC_REPORT_ERROR( SC_ID_SET_TIME_RESOLUTION_, "already specified" );
    }

    // can only be specified before any sc_time is constructed
    if( time_params->time_resolution_fixed ) {
	SC_REPORT_ERROR( SC_ID_SET_TIME_RESOLUTION_,
			 "sc_time object(s) constructed" );
    }

    // must be larger than or equal to 1 fs
    volatile double resolution = v * time_values[tu];
    if( resolution < 1.0 ) {
	SC_REPORT_ERROR( SC_ID_SET_TIME_RESOLUTION_,
			 "value smaller than 1 fs" );
    }

    // recalculate the default time unit
    volatile double time_unit = sc_dt::uint64_to_double(
	time_params->default_time_unit ) *
	( time_params->time_resolution / resolution );
    if( time_unit < 1.0 ) {
	SC_REPORT_WARNING( SC_ID_DEFAULT_TIME_UNIT_CHANGED_, 0 );
	time_params->default_time_unit = 1;
    } else {
	time_params->default_time_unit = static_cast<sc_dt::int64>( time_unit );
    }

    time_params->time_resolution = resolution;
    time_params->time_resolution_specified = true;
}

sc_time
sc_get_time_resolution()
{
    return sc_time::from_value( sc_dt::UINT64_ONE );
}


void
sc_set_default_time_unit( double v, sc_time_unit tu )
{
    static bool warn_default_time_unit=true;
    if ( warn_default_time_unit )
    {
        warn_default_time_unit=false;
        SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
	    "deprecated function: sc_set_default_time_unit");
    }

    // first perform the necessary checks

    // must be positive
    if( v < 0.0 ) {
	SC_REPORT_ERROR( SC_ID_SET_DEFAULT_TIME_UNIT_, "value not positive" );
    }

    // must be a power of ten
    double dummy;
    if( modf( log10( v ), &dummy ) != 0.0 ) {
	SC_REPORT_ERROR( SC_ID_SET_DEFAULT_TIME_UNIT_,
			 "value not a power of ten" );
    }

    sc_simcontext* simc = sc_get_curr_simcontext();

    // can only be specified during elaboration
    if( sc_is_running() ) {
	SC_REPORT_ERROR( SC_ID_SET_DEFAULT_TIME_UNIT_, "simulation running" );
    }

    sc_time_params* time_params = simc->m_time_params;

    // can only be specified before any sc_time is constructed
    if( time_params->time_resolution_fixed ) {
        SC_REPORT_ERROR( SC_ID_SET_DEFAULT_TIME_UNIT_,
                         "sc_time object(s) constructed" );
    }

    // can be specified only once
    if( time_params->default_time_unit_specified ) {
	SC_REPORT_ERROR( SC_ID_SET_DEFAULT_TIME_UNIT_, "already specified" );
    }

    // must be larger than or equal to the time resolution
    volatile double time_unit = ( v * time_values[tu] ) /
	                        time_params->time_resolution;
    if( time_unit < 1.0 ) {
	SC_REPORT_ERROR( SC_ID_SET_DEFAULT_TIME_UNIT_,
			 "value smaller than time resolution" );
    }

    time_params->default_time_unit = static_cast<sc_dt::int64>( time_unit );
    time_params->default_time_unit_specified = true;
}

sc_time
sc_get_default_time_unit()
{
    static bool warn_get_default_time_unit = true;
    if ( warn_get_default_time_unit )
    {
        warn_get_default_time_unit=false;
        SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
            "deprecated function: sc_get_default_time_unit");
    }
    return sc_time::from_value(
              sc_get_curr_simcontext()->m_time_params->default_time_unit
           );
}


// ----------------------------------------------------------------------------

SC_API const sc_time SC_ZERO_TIME;

#undef SC_MAXTIME_ALLOWED_

} // namespace sc_core

// $Log: sc_time.cpp,v $
// Revision 1.7  2011/08/26 20:46:11  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
// Revision 1.6  2011/07/24 16:08:36  acg
//  Philipp A. Hartmann: fix C99 format specifiers for Solaris.
//
// Revision 1.5  2011/02/18 20:27:14  acg
//  Andy Goodrich: Updated Copyrights.
//
// Revision 1.4  2011/02/13 21:47:38  acg
//  Andy Goodrich: update copyright notice.
//
// Revision 1.3  2011/01/19 23:21:50  acg
//  Andy Goodrich: changes for IEEE 1666 2011
//
// Revision 1.2  2008/05/22 17:06:27  acg
//  Andy Goodrich: updated copyright notice to include 2008.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.6  2006/01/26 21:04:55  acg
//  Andy Goodrich: deprecation message changes and additional messages.
//
// Revision 1.5  2006/01/25 00:31:19  acg
//  Andy Goodrich: Changed over to use a standard message id of
//  SC_ID_IEEE_1666_DEPRECATION for all deprecation messages.
//
// Revision 1.4  2006/01/24 20:49:05  acg
// Andy Goodrich: changes to remove the use of deprecated features within the
// simulator, and to issue warning messages when deprecated features are used.
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

// Taf!
