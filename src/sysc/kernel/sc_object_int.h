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

  sc_object_int.h -- For inline definitions of some utility functions.
                     DO NOT EXPORT THIS INCLUDE FILE.

  Original Author: Philipp A. Hartmann, OFFIS, 2013-02-10

 *****************************************************************************/

#ifndef SC_OBJECT_INT_H_INCLUDED_
#define SC_OBJECT_INT_H_INCLUDED_

#include "sysc/kernel/sc_object.h"
#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_simcontext_int.h"

namespace sc_core {

inline
sc_object_host::sc_object_host()
 : sc_object()
 , m_child_events()
 , m_child_objects()
 , m_name_gen_p()
{}

inline
sc_object_host::sc_object_host(const char* nm)
 : sc_object(nm)
 , m_child_events()
 , m_child_objects()
 , m_name_gen_p()
{}

// -----------------------------------------------------------------------

inline
sc_hierarchy_scope::sc_hierarchy_scope( kernel_tag, sc_object* obj )
  : m_simc( (obj) ? obj->simcontext() : sc_get_curr_simcontext() )
  , m_scoped_top()
{
  if( obj == m_simc->hierarchy_curr() ) {
    m_simc = NULL;
    return; // scope already matches
  }

  if( obj == NULL ) { // new root scope
      m_simc->hierarchy_push(NULL);
      return;
  }

  m_scoped_top = dynamic_cast<sc_object_host*>(obj);
  if( m_scoped_top == NULL )
    m_scoped_top = static_cast<sc_object_host*>(obj->get_parent_object());
  m_simc->hierarchy_push(m_scoped_top);
}

inline
sc_hierarchy_scope::sc_hierarchy_scope( kernel_tag, sc_object_host* objh )
  : m_simc( (objh) ? objh->simcontext() : sc_get_curr_simcontext() )
  , m_scoped_top(objh)
{
  if( objh == m_simc->hierarchy_curr() ) {
    m_simc = NULL;
    return; // scope already matches
  }

  m_simc->hierarchy_push(m_scoped_top);
}

} // namespace sc_core

#endif // SC_OBJECT_INT_H_INCLUDED_
// Taf!
