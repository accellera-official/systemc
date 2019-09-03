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

#ifndef SC_DATATYPES_ANY_VALUE_IDS_H_INCLUDED_
#define SC_DATATYPES_ANY_VALUE_IDS_H_INCLUDED_

#include "sysc/utils/sc_report.h"

// ----------------------------------------------------------------------------
//  Report ids (datatypes/variant)
//
//  Report IDs in the range of 500-599.
// ----------------------------------------------------------------------------

#ifndef SC_DEFINE_MESSAGE
#define SC_DEFINE_MESSAGE(id,unused1,unused2) \
    namespace sc_core { extern const char id[]; }
#endif

SC_DEFINE_MESSAGE( SC_ID_ANY_VALUE_ACCESS_FAILED_,     500,
                   "sc_any_value access failed" )
SC_DEFINE_MESSAGE( SC_ID_ANY_VALUE_CONVERSION_FAILED_, 501,
                   "sc_any_value conversion failed" )
SC_DEFINE_MESSAGE( SC_ID_ANY_VALUE_PARSING_FAILED_,    502,
                   "sc_any_value JSON parsing failed" )
SC_DEFINE_MESSAGE( SC_ID_ANY_VALUE_ASSERTION_FAILED_,  503,
                   "sc_any_value assertion failed" )

#endif // SC_DATATYPES_ANY_VALUE_H_INCLUDED_
// Taf!
