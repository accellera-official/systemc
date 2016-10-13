/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_tracing_ids.h -- Report ids for the tracing code.

  Original Author: Philipp A. Hartmann, OFFIS, 2013-11-17

  CHANGE LOG AT END OF FILE
 *****************************************************************************/

#ifndef SC_TRACING_IDS_H
#define SC_TRACING_IDS_H

// ----------------------------------------------------------------------------
//  Report ids (tracing)
//
//  Report ids in the range of 700-799.
// ----------------------------------------------------------------------------

#ifndef SC_DEFINE_MESSAGE
#define SC_DEFINE_MESSAGE(id,unused1,unused2) \
    namespace sc_core { extern const char id[]; }
#endif

SC_DEFINE_MESSAGE( SC_ID_TRACING_FOPEN_FAILED_,         701,
                   "cannot open tracefile for writing" )

SC_DEFINE_MESSAGE( SC_ID_TRACING_TIMESCALE_DEFAULT_,    702,
                  "default timescale unit used for tracing" )
SC_DEFINE_MESSAGE( SC_ID_TRACING_TIMESCALE_UNIT_,       703,
                   "tracing timescale unit set" )
SC_DEFINE_MESSAGE( SC_ID_TRACING_VCD_DELTA_CYCLE_,      704,
   "VCD delta cycle tracing with pseudo timesteps (1 unit)" )
/* unused IDs 705-709 */
SC_DEFINE_MESSAGE( SC_ID_TRACING_OBJECT_IGNORED_,       710,
                   "object cannot not be traced" )
SC_DEFINE_MESSAGE( SC_ID_TRACING_OBJECT_NAME_FILTERED_, 711,
                   "traced object name filtered" )
SC_DEFINE_MESSAGE( SC_ID_TRACING_INVALID_ENUM_VALUE_,   712,
                "traced value of enumerated type undefined" )
SC_DEFINE_MESSAGE( SC_ID_TRACING_VCD_DUPLICATE_TIME_,   713,
  "multiple VCD tracing cycles with the same time detected" )
SC_DEFINE_MESSAGE( SC_ID_TRACING_VCD_REVERSED_TIME_,    714,
             "VCD tracing cycle with falling time detected" )
/* unused IDs 715-719 */
SC_DEFINE_MESSAGE( SC_ID_TRACING_ALREADY_INITIALIZED_,  720,
                   "sc_trace_file already initialized" )


/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#endif // SC_TRACING_IDS_H
// Taf!
