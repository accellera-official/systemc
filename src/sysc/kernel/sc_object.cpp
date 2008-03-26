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

  sc_object.cpp -- Abstract base class of all SystemC objects.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

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
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.4  2006/03/21 00:00:34  acg
//   Andy Goodrich: changed name of sc_get_current_process_base() to be
//   sc_get_current_process_b() since its returning an sc_process_b instance.
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

#include <stdio.h>
#include <cstdlib>
#include <cassert>
#include <ctype.h>

#include "sysc/kernel/sc_externs.h"
#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_object.h"
#include "sysc/kernel/sc_object_manager.h"
#include "sysc/kernel/sc_process_handle.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/utils/sc_hash.h"
#include "sysc/utils/sc_iostream.h"
#include "sysc/utils/sc_list.h"
#include "sysc/utils/sc_mempool.h"

namespace sc_core {

typedef int (*STRCMP)(const void*, const void*);

const char SC_HIERARCHY_CHAR = '.';

/* This will be gotten rid after multiple-processes
   are implemented.  This is to fix some regression
   problems. */
bool sc_enable_name_checking = true;


// ----------------------------------------------------------------------------
//  CLASS : sc_object
//
//  Abstract base class of all SystemC `simulation' objects.
// ----------------------------------------------------------------------------

const char*
sc_object::basename() const
{
    const char* p = strrchr( m_name, SC_HIERARCHY_CHAR );
    return p ? (p + 1) : m_name;
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

static int sc_object_num = 0;

static char*
sc_object_newname(char* name)
{
    std::sprintf(name, "{%d}", sc_object_num);
    sc_object_num++;
    return name;
}

void 
sc_object::sc_object_init(const char* nm) 
{ 
    bool        clash;                  // True if path name exists in obj table
    const char* leafname_p;             // Leaf name (this object) 
    char        pathname[BUFSIZ];       // Path name 
    char        pathname_orig[BUFSIZ];  // Original path name which may clash 
    const char* parentname_p;           // Parent path name 
    bool        put_in_table;           // True if should put in object table 
 
    // SET UP POINTERS TO OBJECT MANAGER, PARENT, AND SIMULATION CONTEXT: 
	//
    // Make the current simcontext the simcontext for this object 

    m_simc = sc_get_curr_simcontext(); 
    m_attr_cltn_p = 0; 
    sc_object_manager* object_manager = m_simc->get_object_manager(); 
    sc_object*         parent_p = object_manager->hierarchy_curr(); 
    if (!parent_p) { 
        sc_object* proc = (sc_object*)sc_get_current_process_b();
        parent_p = proc; 
    } 
    m_parent = parent_p; 


    // CONSTRUCT PATHNAME TO OBJECT BEING CREATED: 
    // 
    // If there is not a leaf name generate one. 

    parentname_p = parent_p ? parent_p->name() : ""; 
    if (nm && nm[0] ) 
    { 
        leafname_p = nm; 
        put_in_table = true; 
    } 
    else 
    { 
        leafname_p = sc_object_newname(pathname_orig); 
        put_in_table = false; 
    } 
    if (parent_p) { 
        std::sprintf(pathname, "%s%c%s", parentname_p, 
                SC_HIERARCHY_CHAR, leafname_p 
        ); 
    } else { 
        strcpy(pathname, leafname_p); 
    } 

    // SAVE the original path name 
    // 
    strcpy(pathname_orig, pathname); 

    // MAKE SURE THE OBJECT NAME IS UNIQUE 
    // 
    // If not use unique name generator to make it unique. 

    clash = false; 
    while (object_manager->find_object(pathname)) { 
        clash = true; 
        leafname_p = sc_gen_unique_name(leafname_p); 
        if (parent_p) { 
            std::sprintf(pathname, "%s%c%s", parentname_p, 
                    SC_HIERARCHY_CHAR, leafname_p 
            ); 
        } else { 
            strcpy(pathname, leafname_p); 
        } 
    } 
    if (clash) { 
	std::string message = pathname_orig;
	message += ". Latter declaration will be renamed to ";
	message += pathname;
        SC_REPORT_WARNING( SC_ID_OBJECT_EXISTS_, message.c_str());
    } 


    // MOVE OBJECT NAME TO PERMANENT STORAGE 
    // 
    // Note here we pull a little trick -- use the first byte to store 
    // information about whether to put the object in the object table in the 
    // object manager 

    char* ptr = new char[strlen( pathname ) + 2]; 
    ptr[0] = put_in_table; 
    m_name = ptr + 1; 
    strcpy(m_name, pathname); 

    if (put_in_table) { 
        object_manager->insert_object(m_name, this); 
        sc_module* curr_module = m_simc->hierarchy_curr(); 
        if( curr_module != 0 ) { 
            curr_module->add_child_object( this ); 
        } else { 
            sc_process_b* curr_proc = sc_get_current_process_b();
            if (curr_proc) { 
                curr_proc->add_child_object( this ); 
            } else { 
                m_simc->add_child_object( this ); 
            } 
        } 
    } 
} 

sc_object::sc_object() : m_parent(0)
{
    sc_object_init( sc_gen_unique_name("object") );
}

static bool
object_name_illegal_char(char ch)
{
    return (ch == SC_HIERARCHY_CHAR) || isspace(ch);
}

sc_object::sc_object(const char* nm) : m_parent(0)
{
    int namebuf_alloc = 0;
    char* namebuf = 0;
	const char* p;

	// null name or "" uses machine generated name.
    if ( !nm || strlen(nm) == 0 )
	nm = sc_gen_unique_name("object");
    p = nm;

    if (nm && sc_enable_name_checking) {
        namebuf_alloc = 1 + strlen(nm);
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
    if (m_name[-1] && m_simc) {
        sc_object_manager* object_manager = m_simc->get_object_manager();
        object_manager->remove_object(m_name);

        sc_module* parent_mod = DCAST<sc_module*>(m_parent);
        if (parent_mod) {
            parent_mod->remove_child_object( this );
        } else {
            sc_process_b* parent_proc = DCAST<sc_process_b*>(m_parent);
            if (parent_proc) {
                parent_proc->remove_child_object( this );
            } else {
                m_simc->remove_child_object( this );
            }
        }
    }
    delete [] (m_name-1);
    if ( m_attr_cltn_p ) delete m_attr_cltn_p;
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

} // namespace sc_core
