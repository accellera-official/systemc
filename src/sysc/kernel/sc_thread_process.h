/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
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

  sc_thread_process.h -- Thread process declarations

  Original Author: Andy Goodrich, Forte Design Systems, 4 August 2005
               

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_thread_process.h,v $
// Revision 1.13  2011/02/11 13:25:24  acg
//  Andy Goodrich: Philipp A. Hartmann's changes:
//    (1) Removal of SC_CTHREAD method overloads.
//    (2) New exception processing code.
//
// Revision 1.12  2011/02/01 23:01:53  acg
//  Andy Goodrich: removed dead code.
//
// Revision 1.11  2011/02/01 21:18:01  acg
//  Andy Goodrich:
//  (1) Changes in throw processing for new process control rules.
//  (2) Support of new process_state enum values.
//
// Revision 1.10  2011/01/25 20:50:37  acg
//  Andy Goodrich: changes for IEEE 1666 2011.
//
// Revision 1.9  2011/01/19 23:21:50  acg
//  Andy Goodrich: changes for IEEE 1666 2011
//
// Revision 1.8  2011/01/18 20:10:45  acg
//  Andy Goodrich: changes for IEEE1666_2011 semantics.
//
// Revision 1.7  2011/01/06 17:59:58  acg
//  Andy Goodrich: removed debugging output.
//
// Revision 1.6  2010/07/22 20:02:33  acg
//  Andy Goodrich: bug fixes.
//
// Revision 1.5  2009/07/28 01:10:53  acg
//  Andy Goodrich: updates for 2.3 release candidate.
//
// Revision 1.4  2009/05/22 16:06:29  acg
//  Andy Goodrich: process control updates.
//
// Revision 1.3  2009/03/12 22:59:58  acg
//  Andy Goodrich: updates for 2.4 stuff.
//
// Revision 1.2  2008/05/22 17:06:06  acg
//  Andy Goodrich: formatting and comments.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.7  2006/05/08 17:57:13  acg
//  Andy Goodrich: Added David Long's forward declarations for friend functions
//  to keep the Microsoft C++ compiler happy.
//
// Revision 1.6  2006/04/20 17:08:17  acg
//  Andy Goodrich: 3.0 style process changes.
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

#if !defined(sc_thread_process_h_INCLUDED)
#define sc_thread_process_h_INCLUDED

#include "sysc/kernel/sc_spawn_options.h"
#include "sysc/kernel/sc_process.h"
#include "sysc/kernel/sc_cor.h"
#include "sysc/kernel/sc_event.h"
#include "sysc/kernel/sc_except.h"
#include "sysc/kernel/sc_reset.h"


namespace sc_core {

// forward references:
class sc_event_and_list;
class sc_event_or_list;
class sc_reset;
void sc_thread_cor_fn( void* );
void sc_set_stack_size( sc_thread_handle, std::size_t );
class sc_event;
class sc_join;
class sc_module;
class sc_process_handle;
class sc_process_table;
class sc_simcontext;
class sc_runnable;

sc_cor* get_cor_pointer( sc_process_b* process_p );
void sc_set_stack_size( sc_thread_handle thread_h, std::size_t size );
void wait( sc_simcontext* );
void wait( const sc_event&, sc_simcontext* );
void wait( const sc_event_or_list&, sc_simcontext* );
void wait( const sc_event_and_list&, sc_simcontext* );
void wait( const sc_time&, sc_simcontext* );
void wait( const sc_time&, const sc_event&, sc_simcontext* );
void wait( const sc_time&, const sc_event_or_list&, sc_simcontext* );
void wait( const sc_time&, const sc_event_and_list&, sc_simcontext* );

//==============================================================================
// sc_thread_process -
//
//==============================================================================
class sc_thread_process : public sc_process_b {
    friend void sc_thread_cor_fn( void* );
    friend void sc_set_stack_size( sc_thread_handle, std::size_t );
    friend class sc_event;
    friend class sc_join;
    friend class sc_module;
    friend class sc_process_b;
    friend class sc_process_handle;
    friend class sc_process_table;
    friend class sc_simcontext;
    friend class sc_runnable;
    friend sc_cor* get_cor_pointer( sc_process_b* process_p );

    friend void wait( sc_simcontext* );
    friend void wait( const sc_event&, sc_simcontext* );
    friend void wait( const sc_event_or_list&, sc_simcontext* );
    friend void wait( const sc_event_and_list&, sc_simcontext* );
    friend void wait( const sc_time&, sc_simcontext* );
    friend void wait( const sc_time&, const sc_event&, sc_simcontext* );
    friend void wait( const sc_time&, const sc_event_or_list&, sc_simcontext* );
    friend void wait( const sc_time&, const sc_event_and_list&, sc_simcontext*);
  public:
    sc_thread_process( const char* name_p, bool free_host, 
        SC_ENTRY_FUNC method_p, sc_process_host* host_p, 
        const sc_spawn_options* opt_p );

    virtual ~sc_thread_process();

    virtual const char* kind() const
        { return "sc_thread_process"; }

  protected:
    virtual void disable_process( 
        sc_descendant_inclusion_info descendants = SC_NO_DESCENDANTS );
    virtual void enable_process( 
        sc_descendant_inclusion_info descendants = SC_NO_DESCENDANTS );
    virtual void kill_process(
        sc_descendant_inclusion_info descendants = SC_NO_DESCENDANTS );
    sc_thread_handle next_exist();
    sc_thread_handle next_runnable();
    virtual void prepare_for_simulation();
    virtual void queue_for_execution();
    inline bool ready_to_run();
    virtual void resume_process( 
        sc_descendant_inclusion_info descendants = SC_NO_DESCENDANTS );
    void set_next_exist( sc_thread_handle next_p );
    void set_next_runnable( sc_thread_handle next_p );

    void set_stack_size( std::size_t size );
    inline void suspend_me();
    virtual void suspend_process( 
        sc_descendant_inclusion_info descendants = SC_NO_DESCENDANTS );
    virtual void throw_reset( bool async );
    virtual void throw_user( const sc_throw_it_helper& helper,
        sc_descendant_inclusion_info descendants = SC_NO_DESCENDANTS );

    sc_event::dt_status trigger_dynamic( sc_event* );

    void wait( const sc_event& );
    void wait( const sc_event_or_list& );
    void wait( const sc_event_and_list& );
    void wait( const sc_time& );
    void wait( const sc_time&, const sc_event& );
    void wait( const sc_time&, const sc_event_or_list& );
    void wait( const sc_time&, const sc_event_and_list& );
    void wait_cycles( int n=1 );

  protected:
    void add_monitor( sc_process_monitor* monitor_p );
    void remove_monitor( sc_process_monitor* monitor_p);
    void signal_monitors( int type = 0 );

  protected:
    sc_cor*                          m_cor_p;        // Thread's coroutine.
    static sc_cor*                   m_dead_cor_p;   // Coroutine to delete.
    std::vector<sc_process_monitor*> m_monitor_q;    // Thread monitors.
    std::size_t                      m_stack_size;   // Thread stack size.
    int                              m_wait_cycle_n; // # of waits to be done.

  private: // disabled
    sc_thread_process( const sc_thread_process& );
    const sc_thread_process& operator = ( const sc_thread_process& );

};


//------------------------------------------------------------------------------
//"sc_thread_process::ready_to_run"
//
//------------------------------------------------------------------------------
inline bool sc_thread_process::ready_to_run()
{
    // IF OUR THREAD IS ALREADY GONE WE CAN'T EXECUTE IT:
    
    if ( m_cor_p == NULL ) 
    {
    	return false;
    }

    // SPECIAL CASE NORMAL STATE FOR DISPATCHING SPEED:

    if ( m_state == ps_normal )
    {
	if ( m_throw_status != THROW_NONE )  return true;
        if ( m_wait_cycle_n > 0 )
        {
            --m_wait_cycle_n;
            return false;
        }
        return true;
    }

    // WE ARE THROWING AN EXCEPTION THAT TRUMPS SUSPEND/DISABLE 
    //
    // Schedule the process so it can throw the exception.

    if ( m_throw_status == THROW_KILL ||
	 m_throw_status == THROW_ASYNC_RESET ||
	 m_throw_status == THROW_USER ) 
    {
	 return true;
    }

    // SEE IF WE CAN DISPATCH THE THREAD:

    switch( m_state )
    {
      // No-op: its handled above.
      case ps_normal:
        return true; 

      // Down count the number of waits necessary to wake the process, if
      // its ready to be awakened change its status.

      case ps_suspended:
        if ( m_wait_cycle_n > 0 )
        {
            --m_wait_cycle_n;
            return false;
        }
        m_state = ps_suspended_ready_to_run;
        break;

      // The process has been disabled, but its on the run queue, so let it
      // run once, unless its still waiting on an event and then disable it.

      case ps_disable_pending:
        m_state = ps_disabled;
        if ( m_wait_cycle_n > 0 )
        {
            --m_wait_cycle_n;
            return false;
        }
	return true;

      default:
        break;
    }
    return false;
}


//------------------------------------------------------------------------------
//"sc_thread_process::set_stack_size"
//
//------------------------------------------------------------------------------
inline void sc_thread_process::set_stack_size( std::size_t size )
{
    assert( size );
    m_stack_size = size;
}

//------------------------------------------------------------------------------
//"sc_thread_process::suspend_me"
//
// This method suspends this object instance in favor of the next runnable
// process. Upon awakening we check to see if an exception should be thrown. 
// There are two types of exceptions that can be thrown, synchronous reset
// and asynchronous reset. At a future time there may be more asynchronous
// exceptions.  If an asynchronous reset is seen and there is not static reset
// specified, or the static reset is not active then clear the throw
// type for the next time this method is called.
//
// Notes:
//   (1) The m_sticky_reset field is used to handle synchronous resets that
//       are enabled via the sc_process_handle::sync_reset_on() method. These
//       resets are not generated by a signal, but rather are modal by 
//       method call: sync_reset_on() - sync_reset_off().
//------------------------------------------------------------------------------
inline void sc_thread_process::suspend_me()
{
    // remember, if we're currently unwinding

    bool unwinding_preempted = ( m_throw_status == THROWING_NOW );

    sc_simcontext* simc_p = simcontext();
    sc_cor*         cor_p = simc_p->next_cor();

    // do not switch, if we're about to execute next (e.g. suicide)

    if( m_cor_p != cor_p )
        simc_p->cor_pkg()->yield( cor_p );

    // IF THERE IS A THROW TO BE DONE FOR THIS PROCESS DO IT NOW:
    //
    // Optimize THROW_NONE for speed as it is the normal case.

    if ( m_throw_status == THROW_NONE ) return;

    switch( m_throw_status )
    {
      case THROW_ASYNC_RESET:
      case THROW_SYNC_RESET:
	if ( m_reset_event_p ) m_reset_event_p->notify();
        throw sc_unwind_exception( this, true ); 
	break;

      case THROW_USER:
	m_throw_status = THROW_NONE;
        m_throw_helper_p->throw_it();
	break;

      case  THROW_KILL:
	throw sc_unwind_exception( this, false );
	break;

      default: // THROWING_NOW
        sc_assert( unwinding_preempted );
        break;
    }
}


//------------------------------------------------------------------------------
//"sc_thread_process::wait"
//
//------------------------------------------------------------------------------
inline
void
sc_thread_process::wait( const sc_event& e )
{   
    if( m_throw_status == THROWING_NOW )
        SC_REPORT_ERROR( SC_ID_WAIT_DURING_UNWINDING_, name() );

    m_event_p = &e; // for cleanup.
    e.add_dynamic( this );
    m_trigger_type = EVENT;
    suspend_me();
}

inline
void
sc_thread_process::wait( const sc_event_or_list& el )
{   
    if( m_throw_status == THROWING_NOW )
        SC_REPORT_ERROR( SC_ID_WAIT_DURING_UNWINDING_, name() );

    el.add_dynamic( this );
    m_event_list_p = &el;
    m_trigger_type = OR_LIST;
    suspend_me();
}

inline
void
sc_thread_process::wait( const sc_event_and_list& el )
{
    if( m_throw_status == THROWING_NOW )
        SC_REPORT_ERROR( SC_ID_WAIT_DURING_UNWINDING_, name() );

    el.add_dynamic( this );
    m_event_list_p = &el;
    m_event_count = el.size();
    m_trigger_type = AND_LIST;
    suspend_me();
}

inline
void
sc_thread_process::wait( const sc_time& t )
{
    if( m_throw_status == THROWING_NOW )
        SC_REPORT_ERROR( SC_ID_WAIT_DURING_UNWINDING_, name() );

    m_timeout_event_p->notify_internal( t );
    m_timeout_event_p->add_dynamic( this );
    m_trigger_type = TIMEOUT;
    suspend_me();
}

inline
void
sc_thread_process::wait( const sc_time& t, const sc_event& e )
{
    if( m_throw_status == THROWING_NOW )
        SC_REPORT_ERROR( SC_ID_WAIT_DURING_UNWINDING_, name() );

    m_timeout_event_p->notify_internal( t );
    m_timeout_event_p->add_dynamic( this );
    e.add_dynamic( this );
    m_event_p = &e;
    m_trigger_type = EVENT_TIMEOUT;
    suspend_me();
}

inline
void
sc_thread_process::wait( const sc_time& t, const sc_event_or_list& el )
{
    if( m_throw_status == THROWING_NOW )
        SC_REPORT_ERROR( SC_ID_WAIT_DURING_UNWINDING_, name() );

    m_timeout_event_p->notify_internal( t );
    m_timeout_event_p->add_dynamic( this );
    el.add_dynamic( this );
    m_event_list_p = &el;
    m_trigger_type = OR_LIST_TIMEOUT;
    suspend_me();
}

inline
void
sc_thread_process::wait( const sc_time& t, const sc_event_and_list& el )
{
    if( m_throw_status == THROWING_NOW )
        SC_REPORT_ERROR( SC_ID_WAIT_DURING_UNWINDING_, name() );

    m_timeout_event_p->notify_internal( t );
    m_timeout_event_p->add_dynamic( this );
    el.add_dynamic( this );
    m_event_list_p = &el;
    m_event_count = el.size();
    m_trigger_type = AND_LIST_TIMEOUT;
    suspend_me();
}

//------------------------------------------------------------------------------
//"sc_thread_process::wait_cycles"
//
// This method suspends this object instance for the specified number of cycles.
// A cycle is defined as the event the thread is set up to staticly wait on.
// The field m_wait_cycle_n is set to one less than the number of cycles to
// be waited for, since the value is tested before being decremented in 
// the simulation kernel.
//------------------------------------------------------------------------------
inline
void
sc_thread_process::wait_cycles( int n )
{   
    m_wait_cycle_n = n-1;
    suspend_me();
}

//------------------------------------------------------------------------------
//"sc_thread_process::miscellaneous support"
//
//------------------------------------------------------------------------------
inline
void sc_thread_process::add_monitor(sc_process_monitor* monitor_p)
{
    m_monitor_q.push_back(monitor_p);
}


inline
void sc_thread_process::remove_monitor(sc_process_monitor* monitor_p)
{
    int mon_n = m_monitor_q.size();

    for ( int mon_i = 0; mon_i < mon_n; mon_i++ )
    {
    if  ( m_monitor_q[mon_i] == monitor_p )
        {
            m_monitor_q[mon_i] = m_monitor_q[mon_n-1];
            m_monitor_q.resize(mon_n-1);
        }
    }
}

inline
void sc_thread_process::set_next_exist(sc_thread_handle next_p)
{
    m_exist_p = next_p;
}

inline
sc_thread_handle sc_thread_process::next_exist()
{
    return (sc_thread_handle)m_exist_p;
}

inline
void sc_thread_process::queue_for_execution()
{
    if ( !is_runnable() )
	simcontext()->push_runnable_thread(this);
}

inline
void sc_thread_process::set_next_runnable(sc_thread_handle next_p)
{
    m_runnable_p = next_p;
}

inline
sc_thread_handle sc_thread_process::next_runnable()
{
    return (sc_thread_handle)m_runnable_p;
}

inline sc_cor* get_cor_pointer( sc_process_b* process_p )
{
    sc_thread_handle thread_p = DCAST<sc_thread_handle>(process_p);
    return thread_p->m_cor_p;
}

} // namespace sc_core 

#endif // !defined(sc_thread_process_h_INCLUDED)
