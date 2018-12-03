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

#include "sysc/utils/sc_optional.h"

#if SC_CPLUSPLUS >= 201103L

#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_object.h"
#include "sysc/utils/sc_utils_ids.h"

namespace sc_core {

sc_optional_base::sc_optional_base()
: m_parent_with_flag(sc_get_current_object())
{}

void sc_optional_base::report_double_init(const char* existing_obj_name) const
{
    SC_REPORT_ERROR( SC_ID_OPTIONAL_INIT_CALLED_TWICE_, existing_obj_name);
}

sc_object* sc_optional_base::get_parent_object() const
{
    sc_assert( (reinterpret_cast<const uintptr_t&>(m_parent_with_flag) & 0x1u) == 0x0 );
    return m_parent_with_flag;
}

static_assert( alignof(sc_object) > 1,
"Unsupported platform: sc_optional uses LSB of pointer to store has_value flag" );

void sc_optional_base::set_has_value()
{
    reinterpret_cast<uintptr_t&>(m_parent_with_flag)  |= (uintptr_t)0x1;;
}

} // namespace sc_core

#endif // SC_CPLUSPLUS >= 201103L
