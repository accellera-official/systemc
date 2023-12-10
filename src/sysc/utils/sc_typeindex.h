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

  sc_typeindex.h -- Wrapper around std::typeinfo to allow usage in containers

  Original Author: Philipp A. Hartmann, Intel Corporation

  NOTE: The type sc_type_index is implementation-defined and should not be
        used within user models.  After C++11 is fully adopted, all uses shall
        be replaced by std::type_index.

 *****************************************************************************/
#ifndef SYSC_UTILS_SC_TYPEINDEX_H_INCLUDED_
#define SYSC_UTILS_SC_TYPEINDEX_H_INCLUDED_

#include "sysc/kernel/sc_cmnhdr.h" 

#include <typeinfo>

#include <typeindex> // C++11 has std::typeindex to serve our needs

namespace sc_core {
typedef std::type_index sc_type_index;
} // namespace sc_core

#endif // SYSC_UTILS_SC_TYPEINDEX_H_INCLUDED_
// Taf!
