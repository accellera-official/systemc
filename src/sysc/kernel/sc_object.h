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

  sc_object.h -- Abstract base class of all SystemC `simulation' objects.

  Original Author: Stan Y. Liao, Synopsys, Inc.

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/


#ifndef SC_OBJECT_H
#define SC_OBJECT_H

#include "sysc/kernel/sc_attribute.h"
#include <iostream>

#if defined(_MSC_VER) && !defined(SC_WIN_DLL_WARN)
#pragma warning(push)
#pragma warning(disable: 4251) // DLL import for std::string,vector
#endif

namespace sc_core {

class SC_API sc_event;
class SC_API sc_module;
class sc_name_gen;
class SC_API sc_object;
class SC_API sc_object_host;
class sc_phase_callback_registry;
class sc_runnable;
class SC_API sc_simcontext;
class SC_API sc_trace_file;
class SC_API sc_trace_file_base;

SC_API const char* sc_gen_unique_name( const char*, bool preserve_first );

// ----------------------------------------------------------------------------
//  CLASS : sc_hierarchy_scope
//
//  Scoped manipulation of the current SystemC object hierarchy
// ----------------------------------------------------------------------------

class SC_API sc_hierarchy_scope
{
    friend class sc_object;
    friend class sc_object_host;
    friend class sc_module;

    struct root_tag {};
    struct kernel_tag {};

    [[nodiscard]] sc_hierarchy_scope move()
      { return std::move(*this); }

    sc_hierarchy_scope(kernel_tag, sc_object*);
    sc_hierarchy_scope(kernel_tag, sc_object_host*);
public:
    ~sc_hierarchy_scope() noexcept(false);

    // get root scope
    static sc_hierarchy_scope get_root() { return sc_hierarchy_scope(sc_core::sc_hierarchy_scope::root); }

    sc_hierarchy_scope(sc_hierarchy_scope&&);

private:
    // disabled copying and assignment
    sc_hierarchy_scope(const sc_hierarchy_scope&) /* = delete */;
    sc_hierarchy_scope& operator=(const sc_hierarchy_scope&) /* = delete */;
    // disabled dynamic allocation
    void* operator new(std::size_t) /* = delete */;
    void* operator new[](std::size_t) /* = delete */;

    static const root_tag root;
    sc_hierarchy_scope( root_tag );

private:
    sc_simcontext*  m_simc;
    sc_object_host* m_scoped_top;
}; // class sc_hierarchy_scope

// ----------------------------------------------------------------------------
//  CLASS : sc_object
//
//  Abstract base class of all SystemC `simulation' objects.
// ----------------------------------------------------------------------------

class SC_API sc_object
{
    friend class sc_event;
    friend class sc_invoke_method;
    friend class sc_module;
    friend class sc_object_host;
    friend class sc_object_manager;
    friend class sc_phase_callback_registry;
    friend class sc_process_b;
    friend class sc_runnable;
    friend class sc_simcontext;
    friend class sc_trace_file_base;

    typedef sc_hierarchy_scope hierarchy_scope;

public:
    const char* name() const
        { return m_name.c_str(); }

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

    virtual const std::vector<sc_event*>&  get_child_events() const;
    virtual const std::vector<sc_object*>& get_child_objects() const;

    sc_object* get_parent() const;
    sc_object* get_parent_object() const;

    virtual ~sc_object();

protected:
    sc_object();
    sc_object(const char* nm);

    sc_object( const sc_object& );
    sc_object& operator=( const sc_object& );

    // restore SystemC hierarchy to current object's hierarchical scope
    [[nodiscard]] virtual hierarchy_scope get_hierarchy_scope();

private:
    void detach();
    void sc_object_init(const char* nm);

private:

    /* Each simulation object is associated with a simulation context */ 
    mutable sc_attr_cltn*   m_attr_cltn_p;   // attributes for this object.
    std::string             m_name;          // name of this object.
    sc_object_host*         m_parent;        // parent for this object.
    sc_simcontext*          m_simc;          // simcontext ptr / empty indicator
};

inline sc_object&
sc_object::operator=( sc_object const & )
{
  // deliberately do nothing
  return *this;
}

// ----------------------------------------------------------------------------
//  CLASS : sc_object_host
//
//  Abstract (implementation-defined) base class of all SystemC objects, that
//  can hold child objects/events (i.e. _modules and processes)
// ----------------------------------------------------------------------------

class SC_API sc_object_host : public sc_object
{
    friend class sc_event;
    friend class sc_module;
    friend class sc_object;
    friend class sc_process_b;
    friend SC_API const char* sc_gen_unique_name( const char*, bool preserve_first );
protected:
    sc_object_host();
    sc_object_host(const char* nm);
    virtual ~sc_object_host();

public:
    virtual const std::vector<sc_event*>& get_child_events() const
        { return m_child_events; }

    virtual const std::vector<sc_object*>& get_child_objects() const
        { return m_child_objects; }

protected:
    // restore SystemC hierarchy to current object's hierarchical scope
    [[nodiscard]] virtual hierarchy_scope get_hierarchy_scope();

private:
    virtual void add_child_event( sc_event* event_p );
    virtual void add_child_object( sc_object* object_p );
    virtual bool remove_child_event( sc_event* event_p );
    virtual bool remove_child_object( sc_object* object_p );

    void orphan_child_events();
    void orphan_child_objects();

    const char* gen_unique_name( const char* basename_, bool preserve_first );

private:
    // disabled copying and assignment
    sc_object_host(const sc_object_host&) /*= delete*/;
    sc_object_host& operator=(const sc_object_host&) /*= delete*/;

private:
    std::vector<sc_event*>  m_child_events;  // list of child events.
    std::vector<sc_object*> m_child_objects; // list of child objects.
    sc_name_gen*            m_name_gen_p;    // sub-object name generator
};

// ----------------------------------------------------------------------------

extern const char SC_HIERARCHY_CHAR;
extern bool sc_enable_name_checking;

inline sc_object*
sc_object::get_parent_object() const
{
    return m_parent;
}

inline sc_object*
sc_get_parent( const sc_object* obj_p )
{
    return obj_p->get_parent_object();
}

} // namespace sc_core

#if defined(_MSC_VER) && !defined(SC_WIN_DLL_WARN)
#pragma warning(pop)
#endif

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
// Revision 1.13  2011/08/29 18:04:32  acg
//  Philipp A. Hartmann: miscellaneous clean ups.
//
// Revision 1.12  2011/08/26 20:46:10  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
// Revision 1.11  2011/03/06 15:55:11  acg
//  Andy Goodrich: Changes for named events.
//
// Revision 1.10  2011/03/05 19:44:20  acg
//  Andy Goodrich: changes for object and event naming and structures.
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

#endif // SC_OBJECT_H
