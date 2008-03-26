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

  sc_object_manager.h -- Manager of objects (naming, &c.)

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_object_manager.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

#ifndef SC_OBJECT_MANAGER_H
#define SC_OBJECT_MANAGER_H

#include "sysc/utils/sc_vector.h"

namespace sc_core {

class sc_module_name;
template <class K, class C> class sc_phash;


// ----------------------------------------------------------------------------
//  CLASS : sc_object_manager
//
//  Manager of objects.
// ----------------------------------------------------------------------------

class sc_object_manager
{
    friend class sc_simcontext;

public:

    typedef sc_phash<const char*, sc_object*> object_table_type;
    typedef sc_pvector<sc_object*>            object_vector_type;
    typedef sc_plist<sc_object*>              object_hierarchy_type;

    sc_object_manager();
    ~sc_object_manager();

    sc_object* find_object(const char* name);
    sc_object* first_object();
    sc_object* next_object();

    void hierarchy_push(sc_object* mdl);
    sc_object* hierarchy_pop();
    sc_object* hierarchy_curr();
    int hierarchy_size();

    void push_module_name(sc_module_name* mod_name);
    sc_module_name* pop_module_name();
    sc_module_name* top_of_module_name_stack();

    void insert_object(const char* name, sc_object* obj);
    void remove_object(const char* name);

private:

    object_table_type*     m_object_table;
    object_vector_type*    m_ordered_object_vector;
    bool                   m_ordered_object_vector_dirty;
    int                    m_next_object_index;
    object_hierarchy_type* m_object_hierarchy;
    sc_module_name*        m_module_name_stack;
};

} // namespace sc_core

#endif
