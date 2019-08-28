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

  sc_object_manager.cpp -- Manager of objects (naming, &c.)

  Original Author: Stan Y. Liao, Synopsys, Inc.

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/


#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <cctype>
#include <algorithm> // pick up std::sort.

#include "sysc/kernel/sc_object.h"
#include "sysc/utils/sc_hash.h"
#include "sysc/utils/sc_list.h"
#include "sysc/utils/sc_mempool.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_object_manager.h"
#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_process.h"
#include "sysc/kernel/sc_module_name.h"

#include <sstream>

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_object_manager
//
//  Manager of objects.
// ----------------------------------------------------------------------------

sc_object_manager::sc_object_manager() :
    m_event_it(),
    m_event_walk_ok(0),
    m_instance_table(),
    m_module_name_stack(0),
    m_object_it(),
    m_object_stack(),
    m_object_walk_ok()
{
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::~sc_object_manager"
// | 
// | This is the object instance destructor for this class. It goes through
// | each sc_object instance in the instance table and sets its m_simc field
// | to NULL.
// +----------------------------------------------------------------------------
sc_object_manager::~sc_object_manager()
{
    instance_table_t::iterator it;     // instance table iterator.

    for ( it = m_instance_table.begin(); it != m_instance_table.end(); it++) 
    {
        if(it->second.m_name_origin == SC_NAME_OBJECT) {
            sc_object* obj_p = static_cast<sc_object*>(it->second.m_element_p);
            obj_p->m_simc = 0;
        }
    }
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::create_name"
// | 
// | This method creates a hierarchical name based on the name of the active 
// | object and the supplied leaf name. If the resultant name is not unique it 
// | will be made unique and a warning message issued.
// |
// | Arguments:
// |     leaf_name = name to use for the leaf of the hierarchy.
// | Result is an std::string containing the name.
// +----------------------------------------------------------------------------
std::string sc_object_manager::create_name(const char* leaf_name) 
{ 
    bool        clash;                  // true if path name exists in obj table
    std::string leafname_string;        // string containing the leaf name.
    std::string parentname_string;      // parent path name 
    sc_object*  parent_p;               // parent for this instance or NULL.
    std::string result_orig_string;     // save for warning message.
    std::string result_string;          // name to return.
 
    // CONSTRUCT PATHNAME TO THE NAME TO BE RETURNED:
    // 
    // If there is not a leaf name generate one. 

    parent_p = sc_get_curr_simcontext()->active_object();
    parentname_string = parent_p ? parent_p->name() : ""; 
    leafname_string = leaf_name; 
    if (parent_p) {
        result_string = parentname_string;
	result_string += SC_HIERARCHY_CHAR;
	result_string += leafname_string;
    } else { 
        result_string = leafname_string;
    } 

    // SAVE the original path name 

    result_orig_string = result_string;

    // MAKE SURE THE ENTITY NAME IS UNIQUE:
    // 
    // If not use unique name generator to make it unique. 

    clash = false; 
    for (;;)
    {
	    if ( !name_exists(result_string) )
	    {
	        break;
	    }
        clash = true; 
        leafname_string = sc_gen_unique_name(leafname_string.c_str(), false); 
	if (parent_p) {
	    result_string = parentname_string;
	    result_string += SC_HIERARCHY_CHAR;
	    result_string += leafname_string;
	} else { 
	    result_string = leafname_string;
	} 
    } 
    if (clash) { 
	std::string message = result_orig_string;
	message += ". Latter declaration will be renamed to ";
	message += result_string;
        SC_REPORT_WARNING( SC_ID_INSTANCE_EXISTS_, message.c_str());
    } 

    return result_string;
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::name_exists"
// |
// | This method returns true if the supplied name exists in the hierarchy
// | or false if the element does not exist.
// |
// | Arguments:
// |     name = name of the element
// | Result is a boolean
// +----------------------------------------------------------------------------
bool
sc_object_manager::name_exists(const std::string& name)
{
    instance_table_t::const_iterator it = m_instance_table.find(name);
    return (it != m_instance_table.end()) &&
           (it->second.m_name_origin != SC_NAME_NONE);
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::get_name"
// |
// | This method returns the pointer to the supplied name if it exists in the
// | hierarchy or NULL if it doesn't exist.
// |
// | Arguments:
// |     name = name of the element
// | Result is a pointer to the name or NULL if it doesn't exist.
// +----------------------------------------------------------------------------
const char*
sc_object_manager::get_name(const std::string& name)
{
    instance_table_t::iterator it = m_instance_table.find(name);
    if (it != m_instance_table.end() &&
        it->second.m_name_origin != SC_NAME_NONE) {
        return it->first.c_str();
    } else {
        return NULL;
    }
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::find_event"
// | 
// | This method returns the sc_event with the supplied name, or a NULL if
// | the event does not exist.
// |
// | Arguments:
// |     name = name of the event
// | Result is a pointer to the event or NULL if it does not exist.
// +----------------------------------------------------------------------------
sc_event*
sc_object_manager::find_event(const char* name)
{
    instance_table_t::iterator it;
    it = m_instance_table.find(name);
    if(it != m_instance_table.end()
       && it->second.m_name_origin == SC_NAME_EVENT)
    {
        return static_cast<sc_event*>(it->second.m_element_p);
    } else {
        return NULL;
    }
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::find_object"
// | 
// | This method returns the sc_object with the supplied name, or a NULL if
// | the object does not exist.
// |
// | Arguments:
// |     name = name of the object
// | Result is a pointer to the object or NULL if it does not exist.
// +----------------------------------------------------------------------------
sc_object*
sc_object_manager::find_object(const char* name)
{
    instance_table_t::iterator it;
    it = m_instance_table.find(name);
    if(it != m_instance_table.end()
       && it->second.m_name_origin == SC_NAME_OBJECT)
    {
        return static_cast<sc_object*>(it->second.m_element_p);
    } else {
        return NULL;
    }
}


// +----------------------------------------------------------------------------
// |"sc_object_manager::first_object"
// | 
// | This method initializes the object iterator to point to the first object
// | in the instance table, and returns its address. If there are no objects
// | in the table a NULL value is returned.
// +----------------------------------------------------------------------------
sc_object*
sc_object_manager::first_object()
{
    sc_object* result_p; // result to return.

    m_object_walk_ok = true;
    result_p = NULL;
    for ( m_object_it = m_instance_table.begin(); 
          m_object_it != m_instance_table.end(); 
	  m_object_it++ )
    {
        if(m_object_it->second.m_name_origin == SC_NAME_OBJECT) {
            return static_cast<sc_object*>(m_object_it->second.m_element_p);
        }
    }
    return result_p;
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::hierarchy_curr"
// | 
// | This method returns the current object in the object hierarchy or NULL
// | if it does not exist.
// +----------------------------------------------------------------------------
sc_object_host*
sc_object_manager::hierarchy_curr()
{
    return !m_object_stack.empty() ? m_object_stack.back() : NULL;
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::hierarchy_pop"
// | 
// | This method pops the current object off the object hierarchy and returns
// | it.
// +----------------------------------------------------------------------------
sc_object_host*
sc_object_manager::hierarchy_pop()
{
    if ( m_object_stack.empty() )
        return NULL;
    sc_object_host* result_p = m_object_stack.back();
    m_object_stack.pop_back();
    return result_p;
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::hierarchy_push"
// | 
// | This method pushes down the sc_object hierarchy to make the supplied 
// | object the current object in the hierarchy.
// |
// | Arguments:
// |     object_p -> object to become the new current object in the hierarchy.
// +----------------------------------------------------------------------------
void
sc_object_manager::hierarchy_push(sc_object_host* object_p)
{
    m_object_stack.push_back(object_p);
}


// +----------------------------------------------------------------------------
// |"sc_object_manager::hierarchy_size"
// | 
// | This method returns the current size of the object hierarchy stack.
// +----------------------------------------------------------------------------
int
sc_object_manager::hierarchy_size()
{
    return static_cast<int>( m_object_stack.size() );
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::insert_external_name"
// |
// | This method inserts the supplied name into the instance table using
// | the supplied name.
// |
// | Arguments:
// |     name     =  external to be inserted.
// +----------------------------------------------------------------------------
bool
sc_object_manager::insert_external_name(const std::string& name)
{
    if(!name_exists(name)) {
        m_instance_table[name].m_element_p = NULL;
        m_instance_table[name].m_name_origin = SC_NAME_EXTERNAL;
        return true;
    } else {
        table_entry element = m_instance_table[name];
        std::stringstream msg;
        msg << name << " ("
            << ((element.m_name_origin == SC_NAME_OBJECT)
                ? (static_cast<sc_object*>(element.m_element_p))->kind()
                : (element.m_name_origin == SC_NAME_EVENT)
                    ? "event" : "external name")
            << ")";
        SC_REPORT_WARNING( SC_ID_NAME_EXISTS_, msg.str().c_str());
        return false;
    }
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::insert_event"
// | 
// | This method inserts the supplied sc_event instance into the instance
// | table using the supplied name.
// |
// | Arguments:
// |     name    =  name of the event to be inserted.
// |     event_p -> event to be inserted.
// +----------------------------------------------------------------------------
void
sc_object_manager::insert_event(const std::string& name, sc_event* event_p)
{
    m_instance_table[name].m_element_p = static_cast<void*>(event_p);
    m_instance_table[name].m_name_origin = SC_NAME_EVENT;
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::insert_object"
// | 
// | This method inserts the supplied sc_object instance into the instance
// | table using the supplied name.
// |
// | Arguments:
// |     name     =  name of the event to be inserted.
// |     object_p -> object to be inserted.
// +----------------------------------------------------------------------------
void
sc_object_manager::insert_object(const std::string& name, sc_object* object_p)
{
    m_instance_table[name].m_element_p = static_cast<void*>(object_p);
    m_instance_table[name].m_name_origin = SC_NAME_OBJECT;
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::next_object"
// | 
// | This method returns the next object pointed to by the instance iterator.
// +----------------------------------------------------------------------------
sc_object*
sc_object_manager::next_object()
{
    sc_object* result_p; // result to return.

    sc_assert( m_object_walk_ok );

    if ( m_object_it == m_instance_table.end() ) return NULL;
    m_object_it++;

    for ( result_p = NULL; m_object_it != m_instance_table.end(); 
	  m_object_it++ )
    {
        if(m_object_it->second.m_name_origin == SC_NAME_OBJECT) {
            return static_cast<sc_object*>(m_object_it->second.m_element_p);
        }
    }
    return result_p;
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::pop_module_name"
// | 
// | This method pops an entry off the module name stack and returns it.
// +----------------------------------------------------------------------------
sc_module_name*
sc_object_manager::pop_module_name()
{
    sc_module_name* mod_name = m_module_name_stack;
    m_module_name_stack = m_module_name_stack->m_next;
    mod_name->m_next = 0;
    return mod_name;
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::push_module_name"
// | 
// | This method pushes the supplied entry onto the module name stack.
// |
// | Arguments:
// |     mod_name_p -> entry to push onto the module name stack.
// +----------------------------------------------------------------------------
void
sc_object_manager::push_module_name(sc_module_name* mod_name_p)
{
    mod_name_p->m_next = m_module_name_stack;
    m_module_name_stack = mod_name_p;
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::top_of_module_name_stack"
// |
// | This method returns the module name object that is on the top of the module
// | name stack, NULL if stack is empty.
// +----------------------------------------------------------------------------
sc_module_name*
sc_object_manager::top_of_module_name_stack() const
{
    return m_module_name_stack;
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::top_of_module_name_stack_name"
// |
// | This method returns the module name string that is on the top of the
// | module name stack, NULL if stack is empty.
// +----------------------------------------------------------------------------
const char*
sc_object_manager::top_of_module_name_stack_name() const
{
    return ( m_module_name_stack != NULL ) ? *m_module_name_stack : NULL;
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::remove_event"
// | 
// | This method removes the sc_event instance with the supplied name from
// | the table of instances.
// |
// | Arguments:
// |     name = name of the event to be removed.
// +----------------------------------------------------------------------------
void
sc_object_manager::remove_event(const std::string& name)
{
    instance_table_t::iterator it;     // instance table iterator.
    it = m_instance_table.find(name);
    if(it != m_instance_table.end()
       && it->second.m_name_origin == SC_NAME_EVENT)
    {
        it->second.m_element_p = NULL;
        it->second.m_name_origin = SC_NAME_NONE;
    }
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::remove_object"
// | 
// | This method removes the sc_object instance with the supplied name from
// | the table of instances.
// |
// | Arguments:
// |     name = name of the object to be removed.
// +----------------------------------------------------------------------------
void
sc_object_manager::remove_object(const std::string& name)
{
    instance_table_t::iterator it;     // instance table iterator.
    it = m_instance_table.find(name);
    if(it != m_instance_table.end()
       && it->second.m_name_origin == SC_NAME_OBJECT)
    {
        it->second.m_element_p = NULL;
        it->second.m_name_origin = SC_NAME_NONE;
    }
}

// +----------------------------------------------------------------------------
// |"sc_object_manager::remove_external_name"
// |
// | This method removes the name instance with the supplied name from
// | the table of instances.
// |
// | Arguments:
// |     name = external name to be removed.
// +----------------------------------------------------------------------------
bool
sc_object_manager::remove_external_name(const std::string& name)
{
    instance_table_t::iterator it;     // instance table iterator.
    it = m_instance_table.find(name);
    if(it != m_instance_table.end()
       && it->second.m_name_origin == SC_NAME_EXTERNAL)
    {
        it->second.m_element_p = NULL;
        it->second.m_name_origin = SC_NAME_NONE;
        return true;
    } else {
        return false;
    }
}

} // namespace sc_core

// $Log: sc_object_manager.cpp,v $
// Revision 1.13  2011/08/26 20:46:10  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
// Revision 1.12  2011/08/24 22:05:51  acg
//  Torsten Maehne: initialization changes to remove warnings.
//
// Revision 1.11  2011/06/25 17:08:39  acg
//  Andy Goodrich: Jerome Cornet's changes to use libtool to build the
//  library.
//
// Revision 1.10  2011/04/01 21:27:54  acg
//  Andy Goodrich: documentation of event and object insertion methods.
//
// Revision 1.9  2011/03/06 15:55:11  acg
//  Andy Goodrich: Changes for named events.
//
// Revision 1.8  2011/03/05 19:44:20  acg
//  Andy Goodrich: changes for object and event naming and structures.
//
// Revision 1.7  2011/03/05 04:45:16  acg
//  Andy Goodrich: moved active process calculation to the sc_simcontext class.
//
// Revision 1.6  2011/03/05 01:39:21  acg
//  Andy Goodrich: changes for named events.
//
// Revision 1.5  2011/02/18 20:27:14  acg
//  Andy Goodrich: Updated Copyrights.
//
// Revision 1.4  2011/02/13 21:47:37  acg
//  Andy Goodrich: update copyright notice.
//
// Revision 1.3  2010/07/22 20:02:33  acg
//  Andy Goodrich: bug fixes.
//
// Revision 1.2  2008/05/22 17:06:26  acg
//  Andy Goodrich: updated copyright notice to include 2008.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//
