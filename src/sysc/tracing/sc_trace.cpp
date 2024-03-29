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

  sc_trace.cpp - Functions for tracing signals and variables.

  Original Author: Abhijit Ghosh, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/*****************************************************************************

   Acknowledgement: The tracing mechanism is based on the tracing
   mechanism developed at Infineon (formerly Siemens HL). Though this
   code is somewhat different, the basics are identical to what was
   originally contributed by Infineon.  The contribution of Infineon
   in the development of this tracing technology is hereby
   acknowledged.

 *****************************************************************************/

#include <cstdarg>
#include <cstdio>

#include "sysc/tracing/sc_trace.h"
#include "sysc/tracing/sc_tracing_ids.h"

#include "sysc/communication/sc_signal_ifs.h"
#include "sysc/kernel/sc_event.h"
#include "sysc/utils/sc_report.h"
#include "sysc/utils/sc_utils_ids.h"

namespace sc_core {

// Trace file common functions.

sc_trace_file::sc_trace_file()
{
  /* Intentionally blank */
}

void SC_API tprintf(sc_trace_file* tf,  const char* format, ...)
{
    static char buffer[4096];
    va_list ap;
    va_start(ap, format);
    (void) vsnprintf(buffer, sizeof(buffer), format, ap);
    va_end(ap);
    if (tf) tf->write_comment(buffer);
}

void sc_trace_file::space(int)
{
  /* Intentionally blank */
}

void sc_trace_file::delta_cycles(bool)
{
  /* Intentionally blank */
}

const sc_dt::uint64&
sc_trace_file::event_trigger_stamp(const sc_event& ev) const
{
    return ev.m_trigger_stamp;
}

SC_API void
sc_trace( sc_trace_file* tf,
	  const sc_signal_in_if<char>& object,
	  const std::string& name,
	  int width )
{
    if( tf ) {
	tf->trace( object.read(), name, width );
    }
}

SC_API void
sc_trace( sc_trace_file* tf,
	  const sc_signal_in_if<short>& object,
	  const std::string& name,
	  int width )
{
    if( tf ) {
	tf->trace( object.read(), name, width );
    }
}

SC_API void
sc_trace( sc_trace_file* tf,
	  const sc_signal_in_if<int>& object,
	  const std::string& name,
	  int width )
{
    if( tf ) {
	tf->trace( object.read(), name, width );
    }
}

SC_API void
sc_trace( sc_trace_file* tf,
	  const sc_signal_in_if<long>& object,
	  const std::string& name,
	  int width )
{
    if( tf ) {
	tf->trace( object.read(), name, width );
    }
}


SC_API void
sc_trace(sc_trace_file* /* not used */,
	 const void* /* not used */,
	 const std::string& name)
{
    SC_REPORT_WARNING( SC_ID_TRACING_OBJECT_IGNORED_, name.c_str() );
}


// ----------------------------------------------------------------------------

#define DEFN_TRACE_FUNC_REF_A(tp)                                             \
SC_API void                                                                   \
sc_trace( sc_trace_file* tf, const tp& object, const std::string& name )      \
{                                                                             \
    if( tf ) {                                                                \
	tf->trace( object, name );                                                \
    }                                                                         \
}

#define DEFN_TRACE_FUNC_PTR_A(tp)                                             \
SC_API void                                                                   \
sc_trace( sc_trace_file* tf, const tp* object, const std::string& name )      \
{                                                                             \
    if( tf ) {                                                                \
	tf->trace( *object, name );                                               \
    }                                                                         \
}

#define DEFN_TRACE_FUNC_A(tp)                                                 \
DEFN_TRACE_FUNC_REF_A(tp)                                                     \
DEFN_TRACE_FUNC_PTR_A(tp)


#define DEFN_TRACE_FUNC_REF_B(tp)                                             \
SC_API void                                                                   \
sc_trace( sc_trace_file* tf, const tp& object, const std::string& name,       \
          int width)                                                          \
{                                                                             \
    if( tf ) {                                                                \
	tf->trace( object, name, width );                                         \
    }                                                                         \
}

#define DEFN_TRACE_FUNC_PTR_B(tp)                                             \
SC_API void                                                                   \
sc_trace( sc_trace_file* tf, const tp* object, const std::string& name,       \
          int width)                                                          \
{                                                                             \
    if( tf ) {                                                                \
	tf->trace( *object, name, width );                                        \
    }                                                                         \
}


#define DEFN_TRACE_FUNC_B(tp)                                                 \
DEFN_TRACE_FUNC_REF_B(tp)                                                     \
DEFN_TRACE_FUNC_PTR_B(tp)


DEFN_TRACE_FUNC_A( sc_event )
DEFN_TRACE_FUNC_A( sc_time )

DEFN_TRACE_FUNC_A( bool )
DEFN_TRACE_FUNC_A( float )
DEFN_TRACE_FUNC_A( double )

DEFN_TRACE_FUNC_B( unsigned char )
DEFN_TRACE_FUNC_B( unsigned short )
DEFN_TRACE_FUNC_B( unsigned int )
DEFN_TRACE_FUNC_B( unsigned long )
DEFN_TRACE_FUNC_B( char )
DEFN_TRACE_FUNC_B( short )
DEFN_TRACE_FUNC_B( int )
DEFN_TRACE_FUNC_B( long )
DEFN_TRACE_FUNC_B( sc_dt::int64 )
DEFN_TRACE_FUNC_B( sc_dt::uint64 )

DEFN_TRACE_FUNC_A( sc_dt::sc_bit )
DEFN_TRACE_FUNC_A( sc_dt::sc_logic )

DEFN_TRACE_FUNC_A( sc_dt::sc_int_base )
DEFN_TRACE_FUNC_A( sc_dt::sc_uint_base )
DEFN_TRACE_FUNC_A( sc_dt::sc_signed )
DEFN_TRACE_FUNC_A( sc_dt::sc_unsigned )

DEFN_TRACE_FUNC_A( sc_dt::sc_bv_base )
DEFN_TRACE_FUNC_A( sc_dt::sc_lv_base )

#ifdef SC_INCLUDE_FX

DEFN_TRACE_FUNC_A( sc_dt::sc_fxval )
DEFN_TRACE_FUNC_A( sc_dt::sc_fxval_fast )
DEFN_TRACE_FUNC_A( sc_dt::sc_fxnum )
DEFN_TRACE_FUNC_A( sc_dt::sc_fxnum_fast )

#endif // SC_INCLUDE_FX

#undef DEFN_TRACE_FUNC_REF_A
#undef DEFN_TRACE_FUNC_PTR_A
#undef DEFN_TRACE_FUNC_A

#undef DEFN_TRACE_FUNC_REF_B
#undef DEFN_TRACE_FUNC_PTR_B
#undef DEFN_TRACE_FUNC_B


SC_API void
sc_trace( sc_trace_file* tf,
	  const unsigned int& object,
	  const std::string& name,
	  const char** enum_literals )
{
    static bool warn_sc_trace_literals=true;
    if ( warn_sc_trace_literals )
    {
        warn_sc_trace_literals=false;
        SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
            "tracing of enumerated literals is deprecated" );
    }

    if( tf ) tf->trace( object, name, enum_literals );
}

} // namespace sc_core

// Taf!
