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

  sc_process.h -- Process base class support.

  Original Author: Andy Goodrich, Forte Design Systems, 04 August 2005


 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Andy Goodrich, Forte Design Systems, 12 Aug 05
  Description of Modification: This is the rewrite of process support. It
                               contains some code from the original
                               sc_process.h by Stan Liao, and the now-defunct
                               sc_process_b.h by Stan Liao and Martin
                               Janssen, all of Synopsys, Inc., It also contains
                               code from the original sc_process_b.h by
                               Andy Goodrich of Forte Design Systems and
                               Bishnupriya Bhattacharya of Cadence Design
                               Systems.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_process.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.7  2006/04/11 23:13:21  acg
//   Andy Goodrich: Changes for reduced reset support that only includes
//   sc_cthread, but has preliminary hooks for expanding to method and thread
//   processes also.
//
// Revision 1.6  2006/03/13 20:26:50  acg
//  Andy Goodrich: Addition of forward class declarations, e.g.,
//  sc_reset, to keep gcc 4.x happy.
//
// Revision 1.5  2006/01/31 20:09:10  acg
//  Andy Goodrich: added explaination of static vs dynamic waits to
//  sc_process_b::trigger_static.
//
// Revision 1.4  2006/01/24 20:49:05  acg
// Andy Goodrich: changes to remove the use of deprecated features within the
// simulator, and to issue warning messages when deprecated features are used.
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

#if !defined(sc_process_h_INCLUDED)
#define sc_process_h_INCLUDED

#include <cassert>
#include "sysc/utils/sc_iostream.h"
#include "sysc/kernel/sc_constants.h"
#include "sysc/kernel/sc_object.h"
#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_reset.h"
#include "sysc/communication/sc_export.h"

namespace sc_core {

// Forward declarations:
class sc_process_handle;
class sc_reset;

// friend function declarations
    const char* sc_gen_unique_name( const char*, bool preserve_first );
    sc_process_handle sc_get_current_process_handle();
    void sc_thread_cor_fn( void* arg );
    void sc_cthread_cor_fn( void* arg );
    bool timed_out( sc_simcontext* );


// Process handles as forward references:

typedef class sc_cthread_process* sc_cthread_handle;
typedef class sc_method_process*  sc_method_handle;
typedef class sc_thread_process*  sc_thread_handle;


// Standard process types:

enum sc_curr_proc_kind
{
    SC_NO_PROC_,
    SC_METHOD_PROC_,
    SC_THREAD_PROC_,
    SC_CTHREAD_PROC_
};

//==============================================================================
// CLASS sc_process_host
//
// This is the base class for objects which may have processes defined for
// their methods (e.g., sc_module)
//==============================================================================

class sc_process_host
{
  public:
    sc_process_host() {}
    virtual ~sc_process_host() { } // Needed for cast check for sc_module.
    void defunct() {}
};


//==============================================================================
// CLASS sc_process_monitor
//
// This class provides a way of monitoring a process' status (e.g., waiting
// for a thread to complete its execution.) This class is intended to be a base
// class for classes which need to monitor a process or processes (e.g.,
// sc_join.) Its methods should be overloaded where notifications are desired.
//==============================================================================

class sc_process_monitor {
  public:
    enum {
        spm_exit = 0
    };
    virtual ~sc_process_monitor() {}
    virtual void signal(sc_thread_handle thread_p, int type) {}
};

//------------------------------------------------------------------------------
// PROCESS INVOCATION METHOD OR FUNCTION:
//
//  Define SC_USE_MEMBER_FUNC_PTR if we want to use member function pointers
//  to implement process dispatch. Otherwise, we'll use a hack that involves
//  creating a templated invocation object which will invoke the member
//  function. This should not be necessary, but some compilers (e.g., VC++)
//  do not allow the conversion from `void (callback_tag::*)()' to
//  `void (sc_process_host::*)()'. This is supposed to be OK as long as the
//  dynamic type is correct.  C++ Standard 5.4 "Explicit type conversion",
//  clause 7: a pointer to member of derived class type may be explicitly
//  converted to a pointer to member of an unambiguous non-virtual base class
//  type.
//-----------------------------------------------------------------------------

#if defined(_MSC_VER)
#if ( _MSC_VER > 1200 )
#   define SC_USE_MEMBER_FUNC_PTR
#endif
#else
#   define SC_USE_MEMBER_FUNC_PTR
#endif


// COMPILER DOES SUPPORT CAST TO void (sc_process_host::*)() from (T::*)():

#if defined(SC_USE_MEMBER_FUNC_PTR)

    typedef void (sc_process_host::*SC_ENTRY_FUNC)();
#   define SC_DECL_HELPER_STRUCT(callback_tag, func) /*EMPTY*/
#   define SC_MAKE_FUNC_PTR(callback_tag, func) \
        static_cast<sc_core::SC_ENTRY_FUNC>(&callback_tag::func)


// COMPILER NOT DOES SUPPORT CAST TO void (sc_process_host::*)() from (T::*)():

#else // !defined(SC_USE_MEMBER_FUNC_PTR)
    class sc_process_call_base {
      public:
        inline sc_process_call_base()
        {
        }

        virtual ~sc_process_call_base()
        {
        }

        virtual void invoke(sc_process_host* host_p)
        {
        }
    };
    extern sc_process_call_base sc_process_defunct;

    template<class T>
    class sc_process_call : public sc_process_call_base {
      public:
        sc_process_call( void (T::*method_p)() ) :
            sc_process_call_base()
        {
             m_method_p = method_p;
        }

        virtual ~sc_process_call()
        {
        }

        virtual void invoke(sc_process_host* host_p)
        {
            (((T*)host_p)->*m_method_p)();
        }

      protected:
        void (T::*m_method_p)();  // Method implementing the process.
    };

    typedef sc_process_call_base* SC_ENTRY_FUNC;
#   define SC_DECL_HELPER_STRUCT(callback_tag, func) /*EMPTY*/
#   define SC_MAKE_FUNC_PTR(callback_tag, func) \
        (::sc_core::SC_ENTRY_FUNC) (new \
        ::sc_core::sc_process_call<callback_tag>(&callback_tag::func))

#endif // !defined(SC_USE_MEMBER_FUNC_PTR)


extern void sc_set_stack_size( sc_thread_handle, std::size_t );

class sc_event;
class sc_event_list;
class sc_name_gen;
class sc_spawn_options;

//==============================================================================
// CLASS sc_process_b - USER INITIATED DYNAMIC PROCESS SUPPORT:
//
// This class implements the base class for a threaded process_base process
// whose semantics are provided by the true virtual method semantics().
// Classes derived from this one will provide a version of semantics which
// implements the desired semantics. See the sc_spawn_xxx classes below.
//
// Notes:
//   (1) Object instances of this class maintain a reference count of
//       outstanding handles. When the handle count goes to zero the
//       object will be deleted.
//   (2) Descriptions of the methods and operators in this class appear with
//       their implementations.
//==============================================================================
class sc_process_b : public sc_object {
    friend class sc_process_handle; // Allow handles to modify ref. count.
    friend class sc_simcontext;     // Allow static processes to have base.

    friend class sc_object;
    friend class sc_port_base;
    friend class sc_runnable;
    friend class sc_sensitive;
    friend class sc_sensitive_pos;
    friend class sc_sensitive_neg;
    friend class sc_module;
    friend class sc_report_handler;
    friend class sc_reset;
    friend class sc_reset_finder;
    friend const char* sc_gen_unique_name( const char*, bool preserve_first );
    friend sc_process_handle sc_get_current_process_handle();
    friend void sc_thread_cor_fn( void* arg );
    friend void sc_cthread_cor_fn( void* arg );
    friend bool timed_out( sc_simcontext* );

  public:
    enum process_throw_type {
        THROW_NONE = 0,
        THROW_RESET
    };
    enum trigger_t
    {
        STATIC,
        EVENT,
        OR_LIST,
        AND_LIST,
        TIMEOUT,
        EVENT_TIMEOUT,
        OR_LIST_TIMEOUT,
        AND_LIST_TIMEOUT
    };

  public:
    sc_process_b( const char* name_p, bool free_host,
        SC_ENTRY_FUNC method_p, sc_process_host* host_p,
        const sc_spawn_options* opt_p );
    virtual ~sc_process_b();

  public:
    bool dont_initialize() const { return m_dont_init; }
    virtual void dont_initialize( bool dont );
    const ::std::vector<sc_object*>& get_child_objects() const;
    inline sc_curr_proc_kind proc_kind() const;
    sc_event& terminated_event();

  public:
    static inline sc_process_handle last_created_process_handle();

  protected:
    void add_static_event( const sc_event& );
    bool dynamic() const { return m_dynamic_proc; }
    const char* gen_unique_name( const char* basename_, bool preserve_first );
    inline sc_report* get_last_report() { return m_last_report_p; }
    inline bool is_runnable();
    static inline sc_process_b* last_created_process_base();
    inline void set_last_report( sc_report* last_p )
        {
            if ( m_last_report_p ) delete m_last_report_p;
            m_last_report_p = last_p;
        }
    void remove_static_events();
    bool trigger_static();
    inline bool timed_out() const;

  private: // structure support:
    void add_child_object( sc_object* );
    void remove_child_object( sc_object* );


  protected: // process control methods:
    virtual void kill_process();
    inline void reset_changed();
    virtual bool terminated() const;

  private:
    inline void reference_decrement();
    inline void reference_increment();

  protected:
    inline void semantics();

    // debugging stuff:

  public:
    const char*                 file;
    int                         lineno;
    int                         proc_id;

  protected:
    std::vector<sc_object*>      m_child_objects;  // Child processes.
    bool                         m_dont_init;      // True: no initialize call.
    bool                         m_dynamic_proc;   // True: after elaboration.
    const sc_event*              m_event_p;        // Dynamic event waiting on.
    int                          m_event_count;    // Number of events.
    sc_event_list*               m_event_list_p;   // Event list waiting on.
    sc_process_b*                m_exist_p;        // Process existence link.
    bool                         m_free_host;      // Free sc_semantic_host_p.
    sc_report*                   m_last_report_p;  // Last report this process.
    sc_name_gen*                 m_name_gen_p;     // Subprocess name generator.
    sc_curr_proc_kind            m_process_kind;   // Type of process.
    int                          m_references_n;   // Outstanding handles.
    bool                         m_reset_level;    // Level for reset.
    sc_reset*                    m_reset_p;        // Reset object.
    sc_process_b*                m_runnable_p;     // sc_runnable link
    sc_process_host*             m_semantics_host_p;   // Host for semantics.
    SC_ENTRY_FUNC                m_semantics_method_p; // Method for semantics.
    std::vector<const sc_event*> m_static_events;   // Static events waiting on.
    sc_event*                    m_term_event_p;    // Terminated event.
    process_throw_type           m_throw_type;      // Throw type.
    bool                         m_timed_out;       // True if we timed out.
    sc_event*                    m_timeout_event_p; // Timeout event.
    trigger_t                    m_trigger_type;    // Type of trigger using.
    bool                         m_zombie;          // True if terminated.

  protected:
    static sc_process_b* m_delete_next_p;          // Next process to delete.
    static sc_process_b* m_last_created_process_p; // Last process created.
};

typedef sc_process_b sc_process_b;  // For compatibility.


//------------------------------------------------------------------------------
//"sc_process_b::XXXX_child_YYYYY"
//
// These methods provide child object support.
//------------------------------------------------------------------------------
inline void
sc_process_b::add_child_object( sc_object* object_ )
{
    // no check if object_ is already in the set
    m_child_objects.push_back( object_ );
}

inline void
sc_process_b::remove_child_object( sc_object* object_ )
{
    int size = m_child_objects.size();
    for( int i = 0; i < size; ++ i ) {
        if( object_ == m_child_objects[i] ) {
            m_child_objects[i] = m_child_objects[size - 1];
            m_child_objects.resize(size-1);
            return;
        }
    }
    // no check if object_ is really in the set
}

inline const ::std::vector<sc_object*>&
sc_process_b::get_child_objects() const
{
    return m_child_objects;
}


//------------------------------------------------------------------------------
//"sc_process_b::is_runnable"
//
// This method returns true if this process is runnable. That is indicated
// by a non-zero m_runnable_p field.
//------------------------------------------------------------------------------
inline bool sc_process_b::is_runnable()
{
    return m_runnable_p != 0;
}


//------------------------------------------------------------------------------
//"sc_process_b::last_created_process_base"
//
// This virtual method returns the sc_process_b pointer for the last
// created process. It is only used internally by the simulator.
//------------------------------------------------------------------------------
inline sc_process_b* sc_process_b::last_created_process_base()
{
    return m_last_created_process_p;
}



//------------------------------------------------------------------------------
//"sc_process_b::proc_kind"
//
// This method returns the kind of this process.
//------------------------------------------------------------------------------
inline sc_curr_proc_kind sc_process_b::proc_kind() const
{
    return m_process_kind;
}


//------------------------------------------------------------------------------
//"sc_process_b::reference_decrement"
//
// This inline method decrements the number of outstanding references to this
// object instance. If the number of references goes to zero, this object
// instance is placed on the deletion queue, after deleting any process that
// is already there. The reason for the two step deletion process is that the
// process from which reference_decrement() is called is likely to be the
// running process, so we have to wait until it goes idle.
//------------------------------------------------------------------------------
inline void sc_process_b::reference_decrement()
{
    m_references_n--;
    if ( m_references_n == 0 )
    {
        if ( m_delete_next_p ) delete m_delete_next_p;
        assert(m_delete_next_p != this);
        m_delete_next_p = this;
    }
}


//------------------------------------------------------------------------------
//"sc_process_b::reference_increment"
//
// This inline method increments the number of outstanding references to this
// object instance.
//------------------------------------------------------------------------------
inline void sc_process_b::reference_increment()
{
    assert(m_references_n != 0);
    m_references_n++;
}

//------------------------------------------------------------------------------
//"sc_process_b::reset_changed"
//
// This method is called when there is a change in the value of the
// signal specified via reset_signal_is. If reset_signal_is was not
// called this method won't be either.
//------------------------------------------------------------------------------
inline void sc_process_b::reset_changed()
{
    m_throw_type = ( m_reset_p->read() == m_reset_level ) ?
        THROW_RESET : THROW_NONE;
}


//------------------------------------------------------------------------------
//"sc_process_b::semantics"
//
// This inline method invokes the semantics for this object instance.
// We check to see if we are initially in reset and then invoke the
// process semantics.
//------------------------------------------------------------------------------
inline void sc_process_b::semantics()
{
    assert( m_process_kind != SC_NO_PROC_ );
    m_throw_type = ( m_reset_p && m_reset_p->read() == m_reset_level ) ?
        THROW_RESET : THROW_NONE;
#   ifndef SC_USE_MEMBER_FUNC_PTR
        m_semantics_method_p->invoke( m_semantics_host_p );
#   else
        (m_semantics_host_p->*m_semantics_method_p)();
#   endif
}


//------------------------------------------------------------------------------
//"sc_process_b::terminated"
//
// This inline method returns true if this object has terminated.
//------------------------------------------------------------------------------
inline bool sc_process_b::terminated() const
{
    return m_zombie;
}


//------------------------------------------------------------------------------
//"sc_process_b::timed_out"
//
// This inline method returns true if this object instance timed out.
//------------------------------------------------------------------------------
inline bool sc_process_b::timed_out() const
{
    return m_timed_out;
}


//------------------------------------------------------------------------------
//"sc_process_b::trigger_static"
//
// This inline method returns true if this object instance is waiting on
// a static trigger and is not runnable. If there trigger is not static
// then the process is waiting on a dynamically specified event, so the
// static trigger should be ignored until that dynamic event occurs.
//------------------------------------------------------------------------------
inline
bool
sc_process_b::trigger_static()
{
    return ( !is_runnable() && m_trigger_type == STATIC );
}


} // namespace sc_core

#endif // !defined(sc_process_h_INCLUDED)
