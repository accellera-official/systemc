/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2011 by all Contributors.
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

  sc_object.h -- Abstract base class of all SystemC `simulation' objects.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Andy Goodrich, Forte Design Systems
                               5 September 2003
  Description of Modification: - Made creation of attributes structure      
                                 conditional on its being used. This eliminates
                                 100 bytes of storage for each normal sc_object.

 *****************************************************************************/

// $Log: sc_object.h,v $
// Revision 1.7  2011/02/13 21:47:37  acg
//  Andy Goodrich: update copyright notice.
//
// Revision 1.6  2011/01/25 20:50:37  acg
//  Andy Goodrich: changes for IEEE 1666 2011.
//
// Revision 1.5  2011/01/18 20:10:44  acg
//  Andy Goodrich: changes for IEEE1666_2011 semantics.
//
// Revision 1.4  2010/07/22 20:02:33  acg
//  Andy Goodrich: bug fixes.
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
// Revision 1.4  2006/04/11 23:13:21  acg
//   Andy Goodrich: Changes for reduced reset support that only includes
//   sc_cthread, but has preliminary hooks for expanding to method and thread
//   processes also.
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

#ifndef SC_OBJECT_H
#define SC_OBJECT_H


#include "sysc/utils/sc_iostream.h"
#include "sysc/kernel/sc_attribute.h"

namespace sc_core {

class sc_module;
class sc_runnable;
class sc_simcontext;
class sc_trace_file;


// ----------------------------------------------------------------------------
//  CLASS : sc_object
//
//  Abstract base class of all SystemC `simulation' objects.
// ----------------------------------------------------------------------------

class sc_object 
{
    friend class sc_module;
    friend class sc_module_dynalloc_list;
    friend class sc_object_manager;
    friend class sc_process_b;
    friend class sc_runnable;

public:

    const char* name() const
        { return m_name; }
    const char* basename() const;

    virtual void print(::std::ostream& os=::std::cout ) const;

    // dump() is more detailed than print()
    virtual void dump(::std::ostream& os=::std::cout ) const;

    virtual void trace( sc_trace_file* tf ) const;

    virtual const char* kind() const { return "sc_object"; }

    sc_simcontext* simcontext() const
        { return m_simc; }

    // add attribute
    bool add_attribute( sc_attr_base& );

    // get attribute by name
          sc_attr_base* get_attribute( const std::string& name_ );
    const sc_attr_base* get_attribute( const std::string& name_ ) const;

    // remove attribute by name
    sc_attr_base* remove_attribute( const std::string& name_ );

    // remove all attributes
    void remove_all_attributes();

    // get the number of attributes
    int num_attributes() const;

    // get the attribute collection
          sc_attr_cltn& attr_cltn();
    const sc_attr_cltn& attr_cltn() const;

    virtual const std::vector<sc_object*>& get_child_objects() const
        { return m_no_children; }

    sc_object* get_parent() const { return m_parent; } 
    sc_object* get_parent_object() const { return m_parent; }

protected:

    sc_object();
    sc_object(const char* nm);

    sc_object( const sc_object& );
    sc_object& operator=( const sc_object& );


    virtual ~sc_object();

private:

    void detach();
    virtual void orphan_child_objects();
    void sc_object_init(const char* nm);

private:

    /* Each simulation object is associated with a simulation context */ 
    sc_simcontext*          m_simc;  // simcontext pointer and empty indicator.
    char*                   m_name;
    mutable sc_attr_cltn*   m_attr_cltn_p;
    sc_object*              m_parent;
    static 
    std::vector<sc_object*> m_no_children; // get_child_objects() default value.
};

inline
sc_object&
sc_object::operator=( sc_object const & )
{
  // deliberately do nothing
  return *this;
}

// ----------------------------------------------------------------------------

extern const char SC_HIERARCHY_CHAR;
extern bool sc_enable_name_checking;


inline 
sc_object* sc_get_parent( const sc_object* obj_p ) 
{ 
	return obj_p->get_parent_object(); 
}

} // namespace sc_core

#endif // SC_OBJECT_H
