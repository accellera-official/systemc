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

  sc_trace_file_base.cpp - Shared internal tracing implementation

  Original Author: Philipp A. Hartmann, OFFIS, 2013-11-15

  CHANGE LOG AT END OF FILE
 *****************************************************************************/

/*****************************************************************************

   Acknowledgement: The tracing mechanism is based on the tracing
   mechanism developed at Infineon (formerly Siemens HL). Though this
   code is somewhat different, and significantly enhanced, the basics
   are identical to what was originally contributed by Infineon.  The
   contribution of Infineon in the development of this tracing
   technology is hereby acknowledged.

 *****************************************************************************/

#include <ctime>
#include <sstream>

#include "sysc/tracing/sc_trace_file_base.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_simcontext_int.h"
#include "sysc/kernel/sc_object_int.h"

#ifndef UINT64_C
#if defined(_WIN32) && !defined(__MINGW32__)
# define UINT64_C(v) v ## ui64
#else
# define UINT64_C(v) v ## ULL
#endif
#endif // UINT64_C

namespace sc_core {

bool sc_trace_file_base::tracing_initialized_ = false;


sc_trace_file_base::sc_trace_file_base( const char* name, const char* extension )
  : sc_trace_file()
  , fp(0)
  , trace_unit_fs()
  , kernel_unit_fs()
  , timescale_set_by_user(false)
  , filename_()
  , initialized_(false)
  , trace_delta_cycles_(false)
{
    if( !name || !*name ) {
        SC_REPORT_ERROR( SC_ID_TRACING_FOPEN_FAILED_, "no name given" );
        return;
    } else {
        std::stringstream ss;
        ss << name << "." << extension;
        ss.str().swap( filename_ );
    }

    sc_register_stage_callback( *this, SC_PRE_TIMESTEP );
}

sc_trace_file_base::~sc_trace_file_base()
{
    if( !is_initialized() )
        SC_REPORT_WARNING( SC_ID_TRACING_CLOSE_EMPTY_FILE_, filename() );

    if( fp )
        fclose(fp);

    // unregister from simcontext
    sc_unregister_stage_callback( *this, SC_PRE_TIMESTEP | SC_POST_UPDATE );
}

/*****************************************************************************/
// stage-callback-based trigger
//
//  The tracing updates are triggered
//    (- at the end of the initialization phase [disabled for now])
//    - before an update of the simulation time
//    - before returning to sc_start (via sc_pause() or sc_stop())
//    - after an update phase (if delta cycles need to be traced)
//

void
sc_trace_file_base::stage_callback(const sc_stage & stage)
{
    // delta cycle is traced at the end of an update phase
    cycle( sc_get_curr_simcontext()->get_stage() == SC_POST_UPDATE );
}


/*****************************************************************************/

bool sc_trace_file_base::is_initialized() const
{
    return initialized_;
}

bool
sc_trace_file_base::initialize()
{
    if( initialized_ )
      return false;

    initialized_ = true;

    if( !tracing_initialized_ ) {
        tracing_initialized_ = true;
        bool running_regression = ( getenv( "SYSTEMC_REGRESSION" ) != NULL );

        // hide some messages during regression
        if( running_regression ) {
          sc_report_handler::set_actions( SC_ID_TRACING_TIMESCALE_DEFAULT_
                                        , SC_INFO,    SC_DO_NOTHING );
        }
    }

    // open trace file
    if(!fp) open_fp();

    sc_time_tuple kernel_res_tuple = sc_time_tuple(sc_get_time_resolution());
    kernel_unit_fs = kernel_res_tuple.value() * unit_to_fs(kernel_res_tuple.unit());

    // setup timescale
    if( !timescale_set_by_user )
    {
        trace_unit_fs = kernel_unit_fs;

        std::stringstream ss;
        ss << sc_get_time_resolution() << " (" << filename_ << ")";
        SC_REPORT_INFO( SC_ID_TRACING_TIMESCALE_DEFAULT_
                      , ss.str().c_str() );
    }

    // initialize derived tracing implementation class (VCD/WIF)
    do_initialize();

    return initialized_;
}

void
sc_trace_file_base::open_fp()
{
    sc_assert( !fp && filename() );
    fp = fopen( filename(), "w" );
    if( !fp ) {
        SC_REPORT_ERROR( SC_ID_TRACING_FOPEN_FAILED_, filename() );
        sc_abort(); // can't recover from here
    }
}

void
sc_trace_file_base::delta_cycles( bool flag )
{
    trace_delta_cycles_ = flag;
    if( trace_delta_cycles_ ) {
        sc_register_stage_callback( *this, SC_POST_UPDATE );
    } else {
        sc_unregister_stage_callback( *this, SC_POST_UPDATE );
    }
}

void
sc_trace_file_base::set_time_unit( double v, sc_time_unit tu )
{
    if( initialized_ )
    {
        std::stringstream ss;
        ss << filename_ << "\n"
           "\tTimescale unit cannot be changed once tracing has begun.\n"
           "\tTo change the scale, create a new trace file.";
        SC_REPORT_ERROR( SC_ID_TRACING_ALREADY_INITIALIZED_
                       , ss.str().c_str() );
        return;
    }

    timescale_set_by_user = true;
    trace_unit_fs = static_cast<unit_type>(v * unit_to_fs(tu));

    // EMIT ADVISORY MESSAGE ABOUT CHANGE IN TIME SCALE:
    {
      std::stringstream ss;
      ss << fs_unit_to_str(trace_unit_fs)
         << " (" << filename_ << ")";
      SC_REPORT_INFO( SC_ID_TRACING_TIMESCALE_UNIT_, ss.str().c_str() );
    }
}

bool
sc_trace_file_base::add_trace_check( const std::string & name ) const
{
    if( !initialized_ ) return true;

    std::stringstream ss;
    ss << "sc_trace() failed:\n"
         "\tNo traces can be added to "
         "'" << filename_  << "'"
         " once trace recording has started.\n"
         "\tTo add tracing of '" << name << "', create a new trace file.";

    SC_REPORT_ERROR( SC_ID_TRACING_ALREADY_INITIALIZED_
                   , ss.str().c_str() );
    return false;
}


bool
sc_trace_file_base::has_low_units() const {
    return kernel_unit_fs > trace_unit_fs;
}


int
sc_trace_file_base::low_units_len() const {
    sc_assert(has_low_units());
    double max_low_units = static_cast<double>(kernel_unit_fs / trace_unit_fs);
    return static_cast<int>(log10(max_low_units));
}


void
sc_trace_file_base::timestamp_in_trace_units(unit_type &high, unit_type &low) const
{
    unit_type time_now = sc_time_stamp().value();
    unit_type delta_now = sc_delta_count_at_current_time();

    if (has_low_units()) {
        unit_type max_low_units = kernel_unit_fs / trace_unit_fs;
        low = 0;
        high = time_now;

        if (delta_cycles()) {
            low += delta_now % max_low_units;
            high += delta_now / max_low_units;
        }

    } else {
        unit_type unit_divisor = trace_unit_fs / kernel_unit_fs;
        low = time_now % unit_divisor;
        high = time_now / unit_divisor;

        if (delta_cycles())
            high += delta_now;
    }
}


sc_time::value_type
sc_trace_file_base::unit_to_fs(sc_time_unit tu)
{
    switch ( tu )
    {
        case SC_FS: return UINT64_C(1);
        case SC_PS: return UINT64_C(1000);
        case SC_NS: return UINT64_C(1000000);
        case SC_US: return UINT64_C(1000000000);
        case SC_MS: return UINT64_C(1000000000000);
        case SC_SEC:return UINT64_C(1000000000000000);
        default:
            SC_REPORT_ERROR( SC_ID_TRACING_INVALID_TIMESCALE_UNIT_,
                             "unknown unit value" );
            return 0;
    }
}

std::string
sc_trace_file_base::fs_unit_to_str(sc_trace_file_base::unit_type tu)
{
    switch (tu)
    {
        case UINT64_C(1): return "1 fs";
        case UINT64_C(10): return "10 fs";
        case UINT64_C(100): return "100 fs";
        case UINT64_C(1000): return "1 ps";
        case UINT64_C(10000): return "10 ps";
        case UINT64_C(100000): return "100 ps";
        case UINT64_C(1000000): return "1 ns";
        case UINT64_C(10000000): return "10 ns";
        case UINT64_C(100000000): return "100 ns";
        case UINT64_C(1000000000): return "1 us";
        case UINT64_C(10000000000): return "10 us";
        case UINT64_C(100000000000): return "100 us";
        case UINT64_C(1000000000000): return "1 ms";
        case UINT64_C(10000000000000): return "10 ms";
        case UINT64_C(100000000000000): return "100 ms";
        case UINT64_C(1000000000000000): return "1 sec";
        case UINT64_C(10000000000000000): return "10 sec";
        case UINT64_C(100000000000000000): return "100 sec";
        default: /* fail below */ (void)0;
    }

    std::stringstream ss;
    ss << "not a power of ten: " << tu << " fs";
    SC_REPORT_ERROR( SC_ID_TRACING_INVALID_TIMESCALE_UNIT_,
                     ss.str().c_str() );
    return "";
}

// obtain formatted time string
SC_API std::string
localtime_string()
{
    char buf[200];
    time_t long_time;
    time(&long_time);
    struct tm* p_tm = localtime(&long_time);
    strftime(buf, 199, "%b %d, %Y       %H:%M:%S", p_tm);
    return buf;
}


} // namespace sc_core

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
// Taf!
