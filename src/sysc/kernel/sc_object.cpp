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

  sc_object.cpp -- Abstract base class of all SystemC objects.

  Original Author: Stan Y. Liao, Synopsys, Inc.

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/

#include "sysc/kernel/sc_object.h"

#include <cstdlib>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <sstream>

#include "sysc/kernel/sc_externs.h"
#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_name_gen.h"
#include "sysc/kernel/sc_object_manager.h"
#include "sysc/kernel/sc_object_int.h"
#include "sysc/kernel/sc_process_handle.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_event.h"
#include "sysc/utils/sc_hash.h"
#include "sysc/utils/sc_list.h"
#include "sysc/utils/sc_utils_ids.h"
#include "sysc/utils/sc_mempool.h"

namespace sc_core {

const char SC_HIERARCHY_CHAR = '.';

/* This will be gotten rid after multiple-processes
   are implemented.  This is to fix some regression
   problems. */
bool sc_enable_name_checking = true;

// ----------------------------------------------------------------------------
//  CLASS : sc_hierarchy_scope
//
//  Scoped manipulation of the current SystemC object hierarchy
// ----------------------------------------------------------------------------

const sc_hierarchy_scope::root_tag sc_hierarchy_scope::root = {};

sc_hierarchy_scope::sc_hierarchy_scope( root_tag )
  : m_simc( sc_get_curr_simcontext() )
  , m_scoped_top(NULL)
{
    if( m_simc->active_object() == NULL ) {
        m_simc = NULL; // nothing to do
    } else {
        // explicitly push NULL to object hierarchy
        m_simc->hierarchy_push( m_scoped_top );
    }
}

sc_hierarchy_scope::sc_hierarchy_scope(sc_hierarchy_scope&& that)
  : m_simc(that.m_simc)
  , m_scoped_top(that.m_scoped_top)
{
    that.m_simc = nullptr;
}

sc_hierarchy_scope::~sc_hierarchy_scope() noexcept(false)
{
    if (m_simc)
    {
        sc_object* active = m_simc->active_object();
        if( SC_UNLIKELY_(active != m_scoped_top) )
        {
            std::stringstream sstr;
            sstr << "current scope: "
                 << ((active) ? active->name() : "(root)" )
                 << ", expected scope: "
                 << ((m_scoped_top) ? m_scoped_top->name() : "(root)");
            SC_REPORT_ERROR(SC_ID_CORRUPT_HIERARCHY_SCOPE_, sstr.str().c_str() );
            sc_abort();
        }
        m_simc->hierarchy_pop();
    }
}

// ----------------------------------------------------------------------------
//  CLASS : sc_object
//
//  Abstract base class of all SystemC `simulation' objects.
// ----------------------------------------------------------------------------

static const std::vector<sc_event*>  no_child_events;
static const std::vector<sc_object*> no_child_objects;

const std::vector<sc_event*>&
sc_object::get_child_events() const
{
    return no_child_events;
}

const std::vector<sc_object*>&
sc_object::get_child_objects() const
{
    return no_child_objects;
}

const char*
sc_object::basename() const
{
    std::size_t pos; // position of last SC_HIERARCHY_CHAR.
    pos = m_name.rfind( (char)SC_HIERARCHY_CHAR );
    return ( pos == m_name.npos ) ? m_name.c_str() : &(m_name.c_str()[pos+1]);
} 

void
sc_object::print(::std::ostream& os) const
{
    os << name();
}

void
sc_object::dump(::std::ostream& os) const
{
    os << "name = " << name() << "\n";
    os << "kind = " << kind() << "\n";
}

// +----------------------------------------------------------------------------
// |"sc_object::sc_object_init"
// | 
// | This method initializes this object instance and places it in to the
// | object hierarchy.
// |
// | Arguments:
// |     nm = leaf name for the object.
// +----------------------------------------------------------------------------
void 
sc_object::sc_object_init(const char* nm)
{ 
    // SET UP POINTERS TO OBJECT MANAGER, PARENT, AND SIMULATION CONTEXT: 
    //
    // Make the current simcontext the simcontext for this object 

    m_simc = sc_get_curr_simcontext(); 
    m_attr_cltn_p = 0; 
    sc_object_manager* object_manager = m_simc->get_object_manager(); 
    m_parent = m_simc->active_object();

    // CONSTRUCT PATHNAME TO OBJECT BEING CREATED: 

    sc_assert( nm );
    m_name = object_manager->create_name(nm);


    // PLACE THE OBJECT INTO THE HIERARCHY

    object_manager->insert_object(m_name, this);
    if ( m_parent )
        m_parent->add_child_object( this );
    else
        m_simc->add_child_object( this );
} 

sc_object::sc_object()
  : m_attr_cltn_p(0), m_name()
  , m_parent(0), m_simc(0)
{
    sc_object_init( sc_gen_unique_name("object") );
}

sc_object::sc_object( const sc_object& that )
  : m_attr_cltn_p(0), m_name()
  , m_parent(0), m_simc(0)
{
    sc_object_init( sc_gen_unique_name( that.basename() ) );
}


static bool
object_name_illegal_char(char ch)
{
    return (ch == SC_HIERARCHY_CHAR) || std::isspace(ch);
}

sc_object::sc_object(const char* nm)
  : m_attr_cltn_p(0), m_name()
  , m_parent(0), m_simc(0)
{
    int namebuf_alloc = 0;
    char* namebuf = 0;
    const char* p;

    // null name or "" uses machine generated name.
    
    if ( !nm || !*nm )
        nm = sc_gen_unique_name("object");
    p = nm;

    if (nm && sc_enable_name_checking) {
        namebuf_alloc = 1 + std::strlen(nm);
        namebuf = (char*) sc_mempool::allocate(namebuf_alloc);
        char* q = namebuf;
        const char* r = nm;
        bool has_illegal_char = false;
        while (*r) {
            if (object_name_illegal_char(*r)) {
                has_illegal_char = true;
                *q = '_';
            } else {
                *q = *r;
            }
            r++;
            q++;
        }
        *q = '\0';
        p = namebuf;
        if (has_illegal_char)
	{
	    std::string message = nm;
	    message += " substituted by ";
	    message += namebuf;
            SC_REPORT_WARNING( SC_ID_ILLEGAL_CHARACTERS_, message.c_str());
	}
    }
    sc_object_init(p);
    sc_mempool::release( namebuf, namebuf_alloc );
}

sc_object::~sc_object()
{
    detach();
    delete m_attr_cltn_p;
}

//------------------------------------------------------------------------------
//"sc_object::detach"
//
// This method detaches this object instance from the object hierarchy.
// It is called in two places: ~sc_object() and sc_process_b::kill_process().
//------------------------------------------------------------------------------
void sc_object::detach()
{
    if (m_simc) {

        // REMOVE OBJECT FROM THE OBJECT MANAGER:

        sc_object_manager* object_manager = m_simc->get_object_manager();
        object_manager->remove_object(m_name);

        // REMOVE OBJECT FROM PARENT'S LIST OF OBJECTS:

        if ( m_parent )
            m_parent->remove_child_object( this );
        else
            m_simc->remove_child_object( this );
    }
}


void
sc_object::trace( sc_trace_file * /* unused */) const
{
    /* This space is intentionally left blank */
}


// add attribute

bool
sc_object::add_attribute( sc_attr_base& attribute_ )
{
    if ( !m_attr_cltn_p ) m_attr_cltn_p = new sc_attr_cltn;
    return ( m_attr_cltn_p->push_back( &attribute_ ) );
}


// get attribute by name

sc_attr_base*
sc_object::get_attribute( const std::string& name_ )
{
    if ( !m_attr_cltn_p ) m_attr_cltn_p = new sc_attr_cltn;
    return ( (*m_attr_cltn_p)[name_] );
}

const sc_attr_base*
sc_object::get_attribute( const std::string& name_ ) const
{
    if ( !m_attr_cltn_p ) m_attr_cltn_p = new sc_attr_cltn;
    return ( (*m_attr_cltn_p)[name_] );
}


// remove attribute by name

sc_attr_base*
sc_object::remove_attribute( const std::string& name_ )
{
    if ( m_attr_cltn_p )
	return ( m_attr_cltn_p->remove( name_ ) );
    else
	return 0;
}


// remove all attributes

void
sc_object::remove_all_attributes()
{
    if ( m_attr_cltn_p )
	m_attr_cltn_p->remove_all();
}


// get the number of attributes

int
sc_object::num_attributes() const
{
    if ( m_attr_cltn_p )
	return ( m_attr_cltn_p->size() );
    else
	return 0;
}


// get the attribute collection

sc_attr_cltn&
sc_object::attr_cltn()
{
    if ( !m_attr_cltn_p ) m_attr_cltn_p = new sc_attr_cltn;
    return *m_attr_cltn_p;
}

const sc_attr_cltn&
sc_object::attr_cltn() const
{
    if ( !m_attr_cltn_p ) m_attr_cltn_p = new sc_attr_cltn;
    return *m_attr_cltn_p;
}

sc_object*
sc_object::get_parent() const
{
    static bool warn_sc_get_parent_deprecated=true;
    if ( warn_sc_get_parent_deprecated )
    {
        warn_sc_get_parent_deprecated=false;
        SC_REPORT_INFO(sc_core::SC_ID_IEEE_1666_DEPRECATION_,
          "sc_object::get_parent() is deprecated, "
          "use get_parent_object() instead");
    }
    return get_parent_object();
}

[[nodiscard]] sc_object::hierarchy_scope
sc_object::get_hierarchy_scope()
{
    sc_object_host* parent = static_cast<sc_object_host*>( get_parent_object() );
    return sc_hierarchy_scope( sc_hierarchy_scope::kernel_tag(), parent ).move();
}

// ----------------------------------------------------------------------------
//  CLASS : sc_object_host
//
//  Abstract (implementation-defined) base class of all SystemC objects, that
//  can hold child objects/events (i.e. _modules and processes)
// ----------------------------------------------------------------------------

sc_object_host::~sc_object_host()
{
    orphan_child_events();
    orphan_child_objects();
    delete m_name_gen_p;
}

void
sc_object_host::add_child_event( sc_event* event_p )
{
    // no check if event_p is already in the set
    m_child_events.push_back( event_p );
}

void
sc_object_host::add_child_object( sc_object* object_p )
{
    // no check if object_ is already in the set
    m_child_objects.push_back( object_p );
}

// +----------------------------------------------------------------------------
// |"sc_object_host::remove_child_event"
// |
// | This virtual method removes the supplied event from the list of child
// | events if it is present.
// |
// | Arguments:
// |     event_p -> event to be removed.
// | Returns true if the event was present, false if not.
// +----------------------------------------------------------------------------
bool
sc_object_host::remove_child_event( sc_event* event_p )
{
    std::vector< sc_event* > & events = m_child_events;
    std::vector< sc_event* >::iterator it  = events.begin(), end = events.end();

    for( ; it != end; ++it  ) {
        if( *it == event_p )
        {
            (*it)->m_parent_with_hierarchy_flag = NULL;
            *it = events.back();
            events.pop_back();
            return true;
        }
    }
    return false;
}

// +----------------------------------------------------------------------------
// |"sc_object_host::remove_child_object"
// |
// | This virtual method removes the supplied object from the list of child
// | objects if it is present.
// |
// | Arguments:
// |     object_p -> object to be removed.
// | Returns true if the object was present, false if not.
// +----------------------------------------------------------------------------
bool
sc_object_host::remove_child_object( sc_object* object_p )
{
    std::vector< sc_object* > & objects = m_child_objects;
    std::vector< sc_object* >::iterator it  = objects.begin(), end = objects.end();

    for( ; it != end; ++it  ) {
        if( *it == object_p )
        {
            (*it)->m_parent = NULL;
            *it = objects.back();
            objects.pop_back();
            return true;
        }
    }
    return false;
}

// +----------------------------------------------------------------------------
// |"sc_object_host::orphan_child_events"
// |
// | This method moves the children of this object instance to be children
// | of the simulator.
// +----------------------------------------------------------------------------
void sc_object_host::orphan_child_events()
{
    std::vector< sc_event* > const & events = m_child_events;

    std::vector< sc_event* >::const_iterator
            it  = events.begin(), end = events.end();
    for( ; it != end; ++it  )
    {
        (*it)->m_parent_with_hierarchy_flag = NULL;
        simcontext()->add_child_event(*it);
    }
    m_child_events.clear();
}

// +----------------------------------------------------------------------------
// |"sc_object_host::orphan_child_objects"
// |
// | This method moves the children of this object instance to be children
// | of the simulator.
// +----------------------------------------------------------------------------
void sc_object_host::orphan_child_objects()
{
    std::vector< sc_object* > const & children = m_child_objects;

    std::vector< sc_object* >::const_iterator
            it  = children.begin(), end = children.end();
    for( ; it != end; ++it  )
    {
        (*it)->m_parent = NULL;
        simcontext()->add_child_object(*it);
    }
    m_child_objects.clear();
}

// +----------------------------------------------------------------------------
// |"sc_object_host::gen_unique_name"
// |
// | This method generates a unique name within this object instance's namespace.
// +----------------------------------------------------------------------------
const char*
sc_object_host::gen_unique_name( const char* basename_, bool preserve_first )
{
    if ( ! m_name_gen_p )
        m_name_gen_p = new sc_name_gen;
    return m_name_gen_p->gen_unique_name( basename_, preserve_first );
}

[[nodiscard]] sc_object::hierarchy_scope
sc_object_host::get_hierarchy_scope()
{
    return sc_hierarchy_scope( sc_hierarchy_scope::kernel_tag(), this ).move();
}

} // namespace sc_core

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Bishnupriya Bhattacharya, Cadence Design Systems,
                               25 August, 2003
  Description of Modification: if module name hierarchy is empty, sc_object 
                               ctor assumes the currently executing process 
                               as the parent object to support dynamic process
                               creation similar to other sc_objects

      Name, Affiliation, Date: Andy Goodrich, Forte Design Systems
                               5 September 2003
  Description of Modification: - Made creation of attributes structure
                                 conditional on its being used. This eliminates
                                 100 bytes of storage for each normal sc_object.

 *****************************************************************************/


// $Log: sc_object.cpp,v $
// Revision 1.16  2011/08/29 18:04:32  acg
//  Philipp A. Hartmann: miscellaneous clean ups.
//
// Revision 1.15  2011/08/26 20:46:10  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
// Revision 1.14  2011/08/24 22:05:51  acg
//  Torsten Maehne: initialization changes to remove warnings.
//
// Revision 1.13  2011/04/01 21:24:57  acg
//  Andy Goodrich: removed unused code.
//
// Revision 1.12  2011/03/06 15:55:11  acg
//  Andy Goodrich: Changes for named events.
//
// Revision 1.11  2011/03/05 19:44:20  acg
//  Andy Goodrich: changes for object and event naming and structures.
//
// Revision 1.10  2011/03/05 04:45:16  acg
//  Andy Goodrich: moved active process calculation to the sc_simcontext class.
//
// Revision 1.9  2011/03/05 01:39:21  acg
//  Andy Goodrich: changes for named events.
//
// Revision 1.8  2011/02/18 20:27:14  acg
//  Andy Goodrich: Updated Copyrights.
//
// Revision 1.7  2011/02/13 21:47:37  acg
//  Andy Goodrich: update copyright notice.
//
// Revision 1.6  2011/01/25 20:50:37  acg
//  Andy Goodrich: changes for IEEE 1666 2011.
//
// Revision 1.5  2011/01/18 20:10:44  acg
//  Andy Goodrich: changes for IEEE1666_2011 semantics.
//
// Revision 1.4  2010/08/03 17:02:39  acg
//  Andy Goodrich: formatting changes.
//
// Revision 1.3  2009/02/28 00:26:58  acg
//  Andy Goodrich: changed boost name space to sc_boost to allow use with
//  full boost library applications.
//
// Revision 1.2  2008/05/22 17:06:26  acg
//  Andy Goodrich: updated copyright notice to include 2008.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.5  2006/04/20 17:08:17  acg
//  Andy Goodrich: 3.0 style process changes.
//
// Revision 1.4  2006/03/21 00:00:34  acg
//   Andy Goodrich: changed name of sc_get_current_process_base() to be
//   sc_get_current_process_b() since its returning an sc_process_b instance.
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//
