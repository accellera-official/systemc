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

  sc_trace_file_base.h - Shared internal tracing implementation

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

#ifndef SC_TRACE_FILE_BASE_H_INCLUDED_
#define SC_TRACE_FILE_BASE_H_INCLUDED_

#include <cstdio>

#  include "sysc/kernel/sc_stage_callback_if.h"
#include "sysc/tracing/sc_trace.h"
#include "sysc/tracing/sc_tracing_ids.h"

#if defined(_MSC_VER) && !defined(SC_WIN_DLL_WARN)
#pragma warning(push)
#pragma warning(disable: 4251) // DLL import for std::string
#endif

namespace sc_core {

// shared implementation of trace files
class SC_API sc_trace_file_base
  : public sc_trace_file
  , private sc_stage_callback_if // to be used as callback target
{
public:
    typedef sc_time::value_type  unit_type;

    const char* filename() const
      { return filename_.c_str(); }

    bool delta_cycles() const
      { return trace_delta_cycles_; }

    // Also trace transitions between delta cycles if flag is true.
    virtual void delta_cycles(bool flag);

    // set a user-define timescale unit for the trace file
    virtual void set_time_unit( double v, sc_time_unit tu);

protected:
    sc_trace_file_base( const char* name, const char* extension );

    // returns true, if trace file is already initialized
    bool is_initialized() const;
    // returns true, iff initialization has been performed in this call
    bool initialize();
    // ensure that file has been opened (needed for early write_comment())
    void open_fp();
    // perform format specific initialization
    virtual void do_initialize() = 0;

    // returns true, if new trace objects can still be added
    // (i.e. trace file is not yet initialized)
    bool add_trace_check( const std::string& name ) const;

    // tracefile time unit < kernel unit, extra units will be placed in low part
    bool has_low_units() const;

    // number of decimal digits in low units
    int  low_units_len() const;

    // get current kernel time in trace time units
    void timestamp_in_trace_units(unit_type &high, unit_type &low) const;

    // Flush results and close file.
    virtual ~sc_trace_file_base();

    static sc_time::value_type unit_to_fs(sc_time_unit tu);

    static std::string fs_unit_to_str(sc_trace_file_base::unit_type tu);

private:
    virtual void stage_callback(const sc_stage & stage);

protected:
    FILE* fp;                          // pointer to the trace file

    unit_type   trace_unit_fs;         // tracefile timescale unit in femtoseconds
    unit_type   kernel_unit_fs;        // kernel timescale unit in femtoseconds
    bool        timescale_set_by_user; // = true means set by user

private:
    std::string filename_;             // name of the file (for reporting)
    bool        initialized_;          // tracing started?
    bool        trace_delta_cycles_;   // also trace delta transitions?

    static bool tracing_initialized_;  // shared setup of tracing implementation

private: // disabled
    sc_trace_file_base( const sc_trace_file_base& ) /* = delete */;
    sc_trace_file_base& operator=( const sc_trace_file_base& ) /* = delete */;

}; // class sc_trace_file_base

// -----------------------------------------------------------------------


// obtain formatted time string
SC_API std::string localtime_string();

} // namespace sc_core

#if defined(_MSC_VER) && !defined(SC_WIN_DLL_WARN)
#pragma warning(pop)
#endif

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#endif // SC_TRACE_FILE_BASE_H_INCLUDED_
// Taf!
