/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_object_manager.cpp -- Manager of objects (naming, &c.)

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_object_manager.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

#include <stdio.h>
#include <cstdlib>
#include <cassert>
#include <ctype.h>

#include "sysc/utils/sc_iostream.h"
#include "sysc/kernel/sc_object.h"
#include "sysc/utils/sc_hash.h"
#include "sysc/utils/sc_list.h"
#include "sysc/utils/sc_mempool.h"
#include "sysc/kernel/sc_object_manager.h"
#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_module_name.h"

namespace sc_core {

int
strcmp_void(const void* a, const void* b)
{
  return strcmp(static_cast<const char*>(a),static_cast<const char*>(b));
}


// ----------------------------------------------------------------------------
//  CLASS : sc_object_manager
//
//  Manager of objects.
// ----------------------------------------------------------------------------

sc_object_manager::sc_object_manager()
{
    m_ordered_object_vector = new object_vector_type;

    m_object_table = new object_table_type;
    m_object_table->set_hash_fn(default_str_hash_fn);
    m_object_table->set_cmpr_fn(strcmp_void);

    m_object_hierarchy = new object_hierarchy_type;
    m_ordered_object_vector_dirty = true;
    m_next_object_index = 0;
    m_module_name_stack = 0;
}

sc_object_manager::~sc_object_manager()
{
    delete m_object_hierarchy;

    /* Go through each object in the table, and
       mark the m_simc field of the object NULL */
    object_table_type::iterator it(m_object_table);
    for ( ; !it.empty(); it++) {
        sc_object* obj = it.contents();
        obj->m_simc = 0;
    }
    delete m_object_table;
    delete m_ordered_object_vector;
}

sc_object*
sc_object_manager::find_object(const char* name)
{
    return (*m_object_table)[name];
}

extern "C" {
  static int
    object_name_compare(const void* o1, const void* o2)
    {
      const sc_object* obj1 = *(const sc_object**) o1;
      const sc_object* obj2 = *(const sc_object**) o2;
      return strcmp(obj1->name(), obj2->name());
    }
}

sc_object*
sc_object_manager::first_object()
{
    if (m_ordered_object_vector_dirty) {
        m_ordered_object_vector->erase_all();
        object_table_type::iterator it(m_object_table);
        while (! it.empty()) {
            sc_object* obj = it.contents();
            m_ordered_object_vector->push_back(obj);
            it++;
        }
        m_ordered_object_vector->sort(object_name_compare);
        m_ordered_object_vector_dirty = false;
    }
    m_next_object_index = 0;
    return next_object();
}

sc_object*
sc_object_manager::next_object()
{
    assert( ! m_ordered_object_vector_dirty );
    if ( m_next_object_index >= m_ordered_object_vector->size() )
        return 0;
    else
        return (*m_ordered_object_vector)[m_next_object_index++];
}

void
sc_object_manager::hierarchy_push(sc_object* mdl)
{
    m_object_hierarchy->push_front(mdl);
}

sc_object*
sc_object_manager::hierarchy_pop()
{
    return m_object_hierarchy->pop_front();
}

sc_object*
sc_object_manager::hierarchy_curr()
{
    return m_object_hierarchy->empty() ? 0 : m_object_hierarchy->front();
}

int
sc_object_manager::hierarchy_size()
{
    return m_object_hierarchy->size();
}

void
sc_object_manager::push_module_name(sc_module_name* mod_name)
{
    mod_name->m_next = m_module_name_stack;
    m_module_name_stack = mod_name;
}

sc_module_name*
sc_object_manager::pop_module_name()
{
    sc_module_name* mod_name = m_module_name_stack;
    m_module_name_stack = m_module_name_stack->m_next;
    mod_name->m_next = 0;
    return mod_name;
}

sc_module_name*
sc_object_manager::top_of_module_name_stack()
{
    if( m_module_name_stack == 0 ) {
	SC_REPORT_ERROR( SC_ID_MODULE_NAME_STACK_EMPTY_, 0 );
    }
    return m_module_name_stack;
}

void
sc_object_manager::insert_object(const char* name, sc_object* obj)
{
    m_object_table->insert(name, obj);
    m_ordered_object_vector_dirty = true;
}

void
sc_object_manager::remove_object(const char* name)
{
    m_object_table->remove(name);
    m_ordered_object_vector_dirty = true;
}

} // namespace sc_core
