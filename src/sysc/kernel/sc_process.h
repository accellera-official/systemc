/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2011 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
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
// Revision 1.21  2011/03/07 17:38:43  acg
//  Andy Goodrich: tightening up of checks for undefined interaction between
//  synchronous reset and suspend.
//
// Revision 1.20  2011/03/06 19:57:11  acg
//  Andy Goodrich: refinements for the illegal suspend - synchronous reset
//  interaction.
//
// Revision 1.19  2011/03/05 19:44:20  acg
//  Andy Goodrich: changes for object and event naming and structures.
//
// Revision 1.18  2011/02/19 08:30:53  acg
//  Andy Goodrich: Moved process queueing into trigger_static from
//  sc_event::notify.
//
// Revision 1.17  2011/02/18 20:27:14  acg
//  Andy Goodrich: Updated Copyrights.
//
// Revision 1.16  2011/02/18 20:10:44  acg
//  Philipp A. Hartmann: force return expression to be a bool to keep MSVC
//  happy.
//
// Revision 1.15  2011/02/17 19:52:45  acg
//  Andy Goodrich:
//    (1) Simplified process control usage.
//    (2) Changed dump_status() to dump_state() with new signature.
//
// Revision 1.14  2011/02/16 22:37:30  acg
//  Andy Goodrich: clean up to remove need for ps_disable_pending.
//
// Revision 1.13  2011/02/13 21:47:37  acg
//  Andy Goodrich: update copyright notice.
//
// Revision 1.12  2011/02/13 21:41:34  acg
//  Andy Goodrich: get the log messages for the previous check in correct.
//
// Revision 1.11  2011/02/13 21:32:24  acg
//  Andy Goodrich: moved sc_process_b::reset_process() implementation
//  from header to cpp file . Added dump_status() to print out the status of a
//  process.
//
// Revision 1.10  2011/02/11 13:25:24  acg
//  Andy Goodrich: Philipp A. Hartmann's changes:
//    (1) Removal of SC_CTHREAD method overloads.
//    (2) New exception processing code.
//
// Revision 1.9  2011/02/04 15:27:36  acg
//  Andy Goodrich: changes for suspend-resume semantics.
//
// Revision 1.8  2011/02/01 21:06:12  acg
//  Andy Goodrich: new layout for the process_state enum.
//
// Revision 1.7  2011/01/25 20:50:37  acg
//  Andy Goodrich: changes for IEEE 1666 2011.
//
// Revision 1.6  2011/01/19 23:21:50  acg
//  Andy Goodrich: changes for IEEE 1666 2011
//
// Revision 1.5  2011/01/18 20:10:45  acg
//  Andy Goodrich: changes for IEEE1666_2011 semantics.
//
// Revision 1.4  2010/07/22 20:02:33  acg
//  Andy Goodrich: bug fixes.
//
// Revision 1.3  2009/05/22 16:06:29  acg
//  Andy Goodrich: process control updates.
//
// Revision 1.2  2008/05/22 17:06:26  acg
//  Andy Goodrich: updated copyright notice to include 2008.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.11  2006/05/08 17:58:10  acg
// Andy Goodrich: added David Long's forward declarations for friend
//   functions, methods, and operators to keep the Microsoft compiler happy.
//
// Revision 1.10  2006/04/28 23:29:01  acg
//  Andy Goodrich: added an sc_core:: prefix to SC_FUNC_PTR in the
//  SC_MAKE_FUNC_PTR macro to allow its transpareuse outside of the sc_core
//  namespace.
//
// Revision 1.9  2006/04/28 21:52:57  acg
//  Andy Goodrich: changed SC_MAKE_FUNC_PTR to use a static cast to address
//  and AIX issue wrt sc_module's inherited classes.
//
// Revision 1.8  2006/04/20 17:08:17  acg
//  Andy Goodrich: 3.0 style process changes.
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
class sc_thread_process;
class sc_reset;

const char* sc_gen_unique_name( const char*, bool preserve_first );
sc_process_handle sc_get_current_process_handle();
void sc_thread_cor_fn( void* arg );
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

// Descendant information for process hierarchy operations:

enum sc_descendant_inclusion_info {
    SC_NO_DESCENDANTS=0,
    SC_INCLUDE_DESCENDANTS,
    SC_INVALID_DESCENDANTS
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
class sc_unwind_exception;

//==============================================================================
// CLASS sc_throw_it<EXCEPT> - ARBITRARY EXCEPTION CLASS
//
// This class serves as a way of throwing an execption for an aribtrary type
// without knowing what that type is. A true virtual method in the base
// class is used to actually throw the execption. A pointer to the base 
// class is used internally removing the necessity of knowing what the type
// of EXCEPT is for code internal to the library.
//
// Note the clone() true virtual method. This is used to allow instances
// of the sc_throw_it<EXCEPT> class to be easily garbage collected. Since
// an exception may be propogated to more than one process knowing when
// to garbage collect is non-trivial. So when a call is made to 
// sc_process_handle::throw_it() an instance of sc_throw_it<EXCEPT> is 
// allocated on the stack. For each process throwing the exception a copy is 
// made via clone(). That allows those objects to be deleted by the individual
// processes when they are no longer needed (in this implementation of SystemC 
// that deletion will occur each time a new exception is thrown ( see 
// sc_thread_process::suspend_me() ).
//==============================================================================
class sc_throw_it_helper {
  public:
    virtual sc_throw_it_helper* clone() const = 0;
    virtual void throw_it() = 0;
    sc_throw_it_helper() {}
    virtual ~sc_throw_it_helper() {}
};

template<typename EXCEPT>
class sc_throw_it : public sc_throw_it_helper
{
    typedef sc_throw_it<EXCEPT> this_type;
  public:
    sc_throw_it( const EXCEPT& value ) : m_value(value) { }
    virtual ~sc_throw_it() {}
    virtual inline this_type* clone() const { return new this_type(m_value); }
    virtual inline void throw_it() { throw m_value; }
  protected:
    EXCEPT m_value;  // value to be thrown.
};

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
//   (3) The m_sticky_reset field is used to handle synchronous resets that
//       are enabled via the sc_process_handle::sync_reset_on() method. These
//       resets are not generated by a signal, but rather are modal by 
//       method call: sync_reset_on - sync_reset_off.
//   (4) Actions in upper case, process states in lower case:
//       
//        Actions:
//          * DISABLE - disabled_process() invoked
//          * ENABLE - enable_process() invoked
//          * RESUME - resume_process() invoked
//          * SUSPEND - suspend_process() invoked
//          * WAIT - process performs a wait
//          * WAIT_DONE - wait satisfied
//       
//        States:
//          * normal::ready - process on run queue or executing.
//          * normal::waiting - process waiting on sensitivity
//          * disabled - disabled
//          * disabled/ready - disabled ready to run
//          * disabled/suspended  - disabled/suspended
//          * disabled/suspended/ready - disabled/suspended/ready to run
//          * suspended  - suspended
//          * suspended/ready - suspended/ready to run
//       
//        normal::ready_to_run State Transitions:
//          * normal::ready + DISABLE   -> disabled
//          * normal::ready + ENABLE    -> normal::ready
//          * normal::ready + RESUME    -> normal::ready
//          * normal::ready + SUSPEND   -> suspended/ready
//          * normal::ready + WAIT      -> normal::waiting
//       
//        normal::waiting State Transitions:
//          * normal::waiting + DISABLE   -> disabled
//          * normal::waiting + ENABLE    -> normal::waiting
//          * normal::waiting + RESUME    -> normal::waiting
//          * normal::waiting + SUSPEND   -> suspended
//          * normal::waiting + WAIT_DONE -> normal::ready
//       
//        disabled State Transitions:
//          * disabled + DISABLE -> disabled
//          * disabled + ENABLE  -> normal::waiting
//          * disabled + RESUME  -> disabled
//          * disabled + SUSPEND -> disabled/suspended
//       
//        disabled/ready State Transitions:
//          * disabled/ready + DISABLE -> disabled/ready
//          * disabled/ready + ENABLE -> disabled/ready
//          * disabled/ready + RESUME -> disabled/ready
//          * disabled/ready + SUSPEND -> disabled/suspended/ready
//       
//        disabled/suspended State Transitions:
//          * disabled/suspended + DISABLE -> disabled/suspended
//          * disabled/suspended + ENABLE -> suspended
//          * disabled/suspended + RESUME -> disabled
//          * disabled/suspended + SUSPEND -> disabled/suspended
//       
//        disabled/suspended/ready State Transitions:
//          * disabled/suspended/ready + DISABLE -> disabled/suspended/ready
//          * disabled/suspended/ready + ENABLE -> suspended/ready
//          * disabled/suspended/ready + RESUME -> disabled/ready
//          * disabled/suspended/ready + SUSPEND -> disabled/suspended/ready
//       
//        suspended State Transitions:
//          * suspended + DISABLE -> disabled/suspended
//          * suspended + ENABLE -> suspended
//          * suspended + RESUME -> normal::waiting
//          * suspended + SUSPEND -> suspended
//          * suspended + WAIT_DONE -> suspended/ready
//       
//        suspended/ready State Transitions:
//          * suspended/ready + DISABLE -> disabled/suspended/ready
//          * suspended/ready + ENABLE -> suspended/ready
//          * suspended/ready + RESUME -> normal::ready_to_run
//          * suspended/ready + SUSPEND -> suspended/ready
//          * suspended/ready + WAIT_DONE -> suspended/ready
//       
//==============================================================================
class sc_process_b : public sc_object { 
    friend class sc_simcontext;      // Allow static processes to have base.
    friend class sc_cthread_process; // Child can access parent.
    friend class sc_method_process;  // Child can access parent.
    friend class sc_process_handle;  // Allow handles to modify ref. count.
    friend class sc_thread_process;  // Child can access parent.

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
    friend class sc_unwind_exception;

    friend const char* sc_gen_unique_name( const char*, bool preserve_first );
    friend sc_process_handle sc_get_current_process_handle();
    friend void sc_thread_cor_fn( void* arg );
    friend bool timed_out( sc_simcontext* );

  public:
    enum process_throw_type {
        THROW_NONE = 0,
	THROW_KILL,
        THROW_USER,
        THROW_ASYNC_RESET,
        THROW_SYNC_RESET,
	THROWING_NOW
    };

    enum process_state {
	ps_bit_disabled = 1,      // process is disabled.
	ps_bit_ready_to_run = 2,  // process is ready to run.
        ps_bit_suspended = 4,     // process is suspended.
	ps_bit_zombie = 8,        // process is a zombie.
        ps_normal = 0             // must be zero.
    };

    enum reset_type {
        reset_asynchronous = 0,
        reset_synchronous_off,
        reset_synchronous_on
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
    sc_process_b( const char* name_p, bool is_thread, bool free_host, 
        SC_ENTRY_FUNC method_p, sc_process_host* host_p, 
        const sc_spawn_options* opt_p );
    virtual ~sc_process_b();

  public:
    inline int current_state() { return m_state; }
    bool dont_initialize() const { return m_dont_init; }
    virtual void dont_initialize( bool dont );
    std::string dump_state() const;
    const ::std::vector<sc_object*>& get_child_objects() const;
    inline sc_curr_proc_kind proc_kind() const;
    sc_event& reset_event();
    sc_event& terminated_event();

  public:
    static inline sc_process_handle last_created_process_handle();
        
  protected:
    virtual void add_child_object( sc_object* );
    void add_static_event( const sc_event& );
    bool dynamic() const { return m_dynamic_proc; }
    const char* gen_unique_name( const char* basename_, bool preserve_first );
    inline sc_report* get_last_report() { return m_last_report_p; }
    inline bool is_disabled() const;
    inline bool is_runnable() const;
    static inline sc_process_b* last_created_process_base();
    virtual void queue_for_execution()=0;
    virtual bool remove_child_object( sc_object* );
    void remove_dynamic_events( bool skip_timeout = false );
    void remove_static_events();
    inline void set_last_report( sc_report* last_p )
        {  
            if ( m_last_report_p ) delete m_last_report_p;
            m_last_report_p = last_p;
        }
    inline bool timed_out() const;

  protected: // process control methods:
    virtual void disable_process(
        sc_descendant_inclusion_info descendants = SC_NO_DESCENDANTS ) = 0;
    void disconnect_process();
    virtual void enable_process(
        sc_descendant_inclusion_info descendants = SC_NO_DESCENDANTS ) = 0;
    inline void initially_in_reset( bool async );
    inline bool is_unwinding() const;
    inline bool start_unwinding();
    inline bool clear_unwinding();
    virtual void kill_process(
        sc_descendant_inclusion_info descendants = SC_NO_DESCENDANTS ) = 0;
    inline void reset_changed( bool async, bool asserted );
    void reset_process( reset_type rt,
        sc_descendant_inclusion_info descendants = SC_NO_DESCENDANTS );
    virtual void resume_process(
        sc_descendant_inclusion_info descendants = SC_NO_DESCENDANTS ) = 0;
    virtual void suspend_process(
        sc_descendant_inclusion_info descendants = SC_NO_DESCENDANTS ) = 0;
    virtual void throw_user( const sc_throw_it_helper& helper,
        sc_descendant_inclusion_info descendants = SC_NO_DESCENDANTS ) = 0;
    virtual void throw_reset( bool async ) = 0;
    virtual bool terminated() const;

  private:
    void        delete_process();
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
    int                          m_active_areset_n; // number of aresets active.
    int                          m_active_reset_n;  // number of resets active.
    bool                         m_dont_init;       // true: no initialize call.
    bool                         m_dynamic_proc;    // true: after elaboration.
    const sc_event*              m_event_p;         // Dynamic event waiting on.
    int                          m_event_count;     // number of events.
    const sc_event_list*         m_event_list_p;    // event list waiting on.
    sc_process_b*                m_exist_p;         // process existence link.
    bool                         m_free_host;       // free sc_semantic_host_p.
    bool                         m_has_sync_reset;  // true has reset_signal_is.
    bool                         m_is_thread;       // true if this is thread.
    sc_report*                   m_last_report_p;   // last report this process.
    sc_name_gen*                 m_name_gen_p;      // subprocess name generator
    sc_curr_proc_kind            m_process_kind;    // type of process.
    int                          m_references_n;    // outstanding handles.
    std::vector<sc_reset*>       m_resets;          // resets for process.
    sc_event*                    m_reset_event_p;   // reset event.
    sc_event*                    m_resume_event_p;  // resume event.
    sc_process_b*                m_runnable_p;      // sc_runnable link
    sc_process_host*             m_semantics_host_p;   // host for semantics.
    SC_ENTRY_FUNC                m_semantics_method_p; // method for semantics.
    int                          m_state;           // process state.
    std::vector<const sc_event*> m_static_events;   // static events waiting on.
    bool                         m_sticky_reset;    // see note 3 above.
    sc_event*                    m_term_event_p;    // terminated event.
    sc_throw_it_helper*          m_throw_helper_p;  // what to throw.
    process_throw_type           m_throw_status;    // exception throwing status
    bool                         m_timed_out;       // true if we timed out.
    sc_event*                    m_timeout_event_p; // timeout event.
    trigger_t                    m_trigger_type;    // type of trigger using.

  protected:
    static sc_process_b* m_last_created_process_p; // Last process created.
};

typedef sc_process_b sc_process_b;  // For compatibility.


//------------------------------------------------------------------------------
//"sc_process_b::XXXX_child_YYYYY"
//
// These methods provide child object support.
//------------------------------------------------------------------------------
inline void
sc_process_b::add_child_object( sc_object* object_p )
{
    sc_object::add_child_object( object_p );
    reference_increment();
}

inline bool
sc_process_b::remove_child_object( sc_object* object_p )
{
    if ( sc_object::remove_child_object( object_p ) ) {
	    reference_decrement();
            return true;
    }
    else
    {
        return false;
    }
}

inline const ::std::vector<sc_object*>&
sc_process_b::get_child_objects() const
{
    return m_child_objects;
}


//------------------------------------------------------------------------------
//"sc_process_b::initially_in_reset"
//
// This inline method is a callback to indicate that a reset is active at
// start up. This is because the signal will have been initialized before
// a reset linkage for it is set up, so we won't get a reset_changed()
// callback.
//     async = true if this an asynchronous reset.
//------------------------------------------------------------------------------
inline void sc_process_b::initially_in_reset( bool async )
{
    if ( async ) 
        m_active_areset_n++;
    else
        m_active_reset_n++;
}

//------------------------------------------------------------------------------
//"sc_process_b::is_disabled"
//
// This method returns true if this process is disabled. 
//------------------------------------------------------------------------------
inline bool sc_process_b::is_disabled() const
{
    return (m_state & ps_bit_disabled) ? true : false;
}

//------------------------------------------------------------------------------
//"sc_process_b::is_runnable"
//
// This method returns true if this process is runnable. That is indicated
// by a non-zero m_runnable_p field.
//------------------------------------------------------------------------------
inline bool sc_process_b::is_runnable() const
{
    return m_runnable_p != 0;
}

//------------------------------------------------------------------------------
//"sc_process_b::is_unwinding"
//
// This method returns true if this process is unwinding from a kill or reset.
//------------------------------------------------------------------------------
inline bool sc_process_b::is_unwinding() const
{
    switch( m_throw_status )
    {
      case THROW_KILL:
      case THROW_ASYNC_RESET:
      case THROW_SYNC_RESET:
      case THROWING_NOW:
      // case THROW_USER:
        return true;
      default:
        return false;
    }
}

//------------------------------------------------------------------------------
//"sc_process_b::start_unwinding"
//
// This method returns whether this process should start unwinding or not.
//------------------------------------------------------------------------------
inline bool sc_process_b::start_unwinding()
{
    switch( m_throw_status )
    {
      case THROW_KILL:
      case THROW_ASYNC_RESET:
      case THROW_SYNC_RESET:
        m_throw_status = THROWING_NOW;
         return true;
      case THROWING_NOW:
      case THROW_USER:
       default:
         return false;
     }
}

//------------------------------------------------------------------------------
//"sc_process_b::clear_unwinding"
//
// This method clears this object instance's throw status and always returns
// true.
//------------------------------------------------------------------------------
inline bool sc_process_b::clear_unwinding()
{
    m_throw_status = THROW_NONE;
    return true;
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
// can be deleted in "sc_process_b::delete_process()".
//------------------------------------------------------------------------------
inline void sc_process_b::reference_decrement()
{
    m_references_n--;
    if ( m_references_n == 0 ) delete_process();
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
// signal that was specified via reset_signal_is, or the value of the
// m_sticky_reset field. We get called any time m_sticky_reset changes
// or a signal value changes since, since we may need to throw an exception 
// or clear one. Note that this method may be called when there is no
// active process, but rather the main simulator is executing so we must
// check for that case.
//
// Arguments:
//     async    = true if this is an asynchronous reset.
//     asserted = true if reset being asserted, false if being deasserted.
//------------------------------------------------------------------------------
inline void sc_process_b::reset_changed( bool async, bool asserted )
{       

    // Reset is being asserted:

    if ( asserted )
    {
        if ( async )
	{
	    m_active_areset_n++;
	    throw_reset(true);
	}
	else if ( m_state & (ps_bit_suspended|ps_bit_ready_to_run) == (
	          ps_bit_suspended|ps_bit_ready_to_run) )
	{
	    SC_REPORT_ERROR( SC_ID_PROCESS_CONTROL_CORNER_CASE_,
	       ": synchronous reset changed on suspended ready to run process");
	}
	else
	{
	    m_active_reset_n++;
	    throw_reset(false);
	}
    }

    // Reset is being deasserted:

    else 
    {
        if ( async )
	{
	    m_active_areset_n--;
	}
	else if ( m_state & (ps_bit_suspended|ps_bit_ready_to_run) == (
	          ps_bit_suspended|ps_bit_ready_to_run) )
	{
	    SC_REPORT_ERROR( SC_ID_PROCESS_CONTROL_CORNER_CASE_,
	       ": synchronous reset changed on suspended ready to run process");
	}
	else
	{
	    m_active_reset_n--;
	}
    }

    // Clear the throw type if there are no active resets.

    if ( (m_throw_status == THROW_SYNC_RESET || 
          m_throw_status == THROW_ASYNC_RESET) &&
         m_active_areset_n == 0 && m_active_reset_n == 0 && !m_sticky_reset )
    {
        m_throw_status = THROW_NONE;
    }
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

    m_throw_status = m_active_areset_n ? THROW_ASYNC_RESET : 
        ( m_active_reset_n  ?  THROW_SYNC_RESET : THROW_NONE);
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
    return (m_state & ps_bit_zombie) != 0;
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

} // namespace sc_core

#endif // !defined(sc_process_h_INCLUDED)
