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

  sc_process_handle.h -- Process access support.

  Original Author: Andy Goodrich, Forte Design Systems, 17 June 2003


 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_process_handle.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.5  2006/04/11 23:13:21  acg
//   Andy Goodrich: Changes for reduced reset support that only includes
//   sc_cthread, but has preliminary hooks for expanding to method and thread
//   processes also.
//
// Revision 1.4  2006/01/24 20:49:05  acg
// Andy Goodrich: changes to remove the use of deprecated features within the
// simulator, and to issue warning messages when deprecated features are used.
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

#if !defined(sc_process_handle_h_INCLUDED)
#define sc_process_handle_h_INCLUDED

#include "sysc/kernel/sc_module.h"

namespace sc_core {

class sc_process_handle;
// friend operator declarations
    bool operator == ( const sc_process_handle& left, const sc_process_handle& right );
    bool operator != ( const sc_process_handle& left, const sc_process_handle& right );


//=============================================================================
// CLASS sc_process_handle
//
// This class provides access to an sc_process_b object instance in a
// manner which allows some persistence after the deletion of the actual
// process.
//=============================================================================
class sc_simcontext;
class sc_process_handle {
    typedef sc_process_handle this_type;

    friend bool operator == ( const this_type& left, const this_type& right );
    friend bool operator != ( const this_type& left, const this_type& right );
    friend class sc_object;
    friend class sc_join;
    friend class sc_module;
    friend class sc_reset;
    friend class sc_sensitive;
    friend class sc_sensitive_pos;
    friend class sc_sensitive_neg;
    friend class sc_thread_process;

  public:
    inline sc_process_handle();
    inline explicit sc_process_handle( sc_object* object_p );
    inline sc_process_handle( const sc_process_handle& orig );
    inline ~sc_process_handle();
    inline sc_process_handle& operator = ( const sc_process_handle& src );

  public:
    inline bool dynamic() const;
    inline const std::vector<sc_object*>& get_child_objects() const;
    inline sc_object* get_parent_object() const;
    inline sc_object* get_process_object() const;
    inline const char* name() const;
    inline sc_curr_proc_kind proc_kind() const;
    inline sc_event& terminated_event();
    inline bool terminated() const;
    inline bool valid() const;

  protected:
    inline bool dont_initialize() const
        { return m_target_p ? m_target_p->dont_initialize() : false; }
    inline void dont_initialize( bool dont );

  public:
    operator sc_process_b* ()
        { return m_target_p; }
    operator sc_cthread_handle ();
    operator sc_method_handle ();
    operator sc_thread_handle ();

  protected:
    sc_process_b* m_target_p;   // Target for this object instance.

  protected:
    static std::vector<sc_object*> empty_vector; // Returned if m_target_p == 0.
    static sc_event                non_event;    // Returned if m_target_p == 0.
};

inline bool operator == (
    const sc_process_handle& left, const sc_process_handle& right )
{
    return (left.m_target_p != 0) && (right.m_target_p != 0) &&
        (left.m_target_p == right.m_target_p);
}

inline bool operator != (
    const sc_process_handle& left, const sc_process_handle& right )
{
    return (left.m_target_p == 0) || (right.m_target_p == 0) ||
        (left.m_target_p != right.m_target_p);
}

//------------------------------------------------------------------------------
//"sc_process_handle::sc_process_handle - non-pointer constructor"
//
// This version of the object instance constructor for this class creates
// an object instance whose target needs to be supplied via an assignment.
//------------------------------------------------------------------------------
inline sc_process_handle::sc_process_handle()
{
    m_target_p = (sc_process_b*)0;
}


//------------------------------------------------------------------------------
//"sc_process_handle::sc_process_handle - pointer constructor"
//
// This version of the object instance constructor for this class creates
// an object instance whose target is the supplied sc_object instance.
// The supplied sc_object must in fact be an sc_process_b instance.
//     object_p -> sc_object instance this is handle for.
//------------------------------------------------------------------------------
inline sc_process_handle::sc_process_handle( sc_object* object_p )
{
    m_target_p = object_p ? DCAST<sc_process_b*>(object_p) : (sc_process_b*)0;
    if ( m_target_p ) m_target_p->reference_increment();
}


//------------------------------------------------------------------------------
//"sc_process_handle::sc_process_handle - copy constructor"
//
// This version of the object instance constructor for this class provides
// the copy constructor for the class. It clones the supplied original
// handle and increments the references to its target.
//     orig = sc_process_handle object instance to be copied from.
//------------------------------------------------------------------------------
inline sc_process_handle::sc_process_handle( const sc_process_handle& orig )
{
    m_target_p = orig.m_target_p;
    if ( m_target_p ) m_target_p->reference_increment();
}

//------------------------------------------------------------------------------
//"sc_process_handle::operator = - copy constructor"
//
// This version of the object instance constructor for this class provides
// the copy constructor for the class. It clones the supplied original
// handle and increments the references to its target.
//     orig = sc_process_handle object instance to be copied from.
//------------------------------------------------------------------------------
inline sc_process_handle&
sc_process_handle::operator = ( const sc_process_handle& orig )
{
    if ( m_target_p ) m_target_p->reference_decrement();
    m_target_p = orig.m_target_p;
    if ( m_target_p ) m_target_p->reference_increment();
    return *this;
}


//------------------------------------------------------------------------------
//"sc_process_handle::~sc_process_handle"
//
// This is the object instance destructor for this class. It decrements
// the reference count for its target.
//------------------------------------------------------------------------------
inline sc_process_handle::~sc_process_handle()
{
    if ( m_target_p ) m_target_p->reference_decrement();
}

//------------------------------------------------------------------------------
//"sc_process_handle::inline methods"
//
// These are short inline methods.
//------------------------------------------------------------------------------
inline void sc_process_handle::dont_initialize( bool dont )
{
    if ( m_target_p ) m_target_p->dont_initialize( dont );
}

inline bool sc_process_handle::dynamic() const
{
    return m_target_p ? m_target_p->dynamic() : false;
}

inline
const std::vector<sc_object*>& sc_process_handle::get_child_objects() const
{
    return m_target_p ? m_target_p->get_child_objects() :
        sc_process_handle::empty_vector;
}

inline sc_object* sc_process_handle::get_parent_object() const
{
    return m_target_p ? m_target_p->get_parent_object() : (sc_object*)0;
}

inline sc_object* sc_process_handle::get_process_object() const
{
    return m_target_p;
}

inline const char* sc_process_handle::name() const
{
    return m_target_p ? m_target_p->name() : "";
}

inline sc_curr_proc_kind sc_process_handle::proc_kind() const
{
    return m_target_p ? m_target_p->proc_kind() : SC_NO_PROC_;
}

inline bool sc_process_handle::terminated() const
{
    return m_target_p ? m_target_p->terminated() : false;
}

inline sc_event& sc_process_handle::terminated_event()
{
    return m_target_p ? m_target_p->terminated_event() :
        sc_process_handle::non_event;
}

inline bool sc_process_handle::valid() const
{
    return m_target_p ? true : false;
}


//------------------------------------------------------------------------------
//"sc_process_b::last_created_process_handle"
//
// This method returns the kind of this process.
//------------------------------------------------------------------------------
inline sc_process_handle sc_process_b::last_created_process_handle()
{
    return sc_process_handle(m_last_created_process_p);
}

inline sc_process_handle sc_get_last_created_process_handle()
{
    return sc_process_b::last_created_process_handle();
}

} // namespace sc_core


#endif // !defined(sc_spawn_h_INCLUDED)
