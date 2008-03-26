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

#if !defined(sc_thread_process_h_INCLUDED)
#define sc_thread_process_h_INCLUDED

#include "sysc/kernel/sc_spawn_options.h"
#include "sysc/kernel/sc_process.h"
#include "sysc/kernel/sc_cor.h"
#include "sysc/kernel/sc_event.h"
#include "sysc/kernel/sc_except.h"
#include "sysc/kernel/sc_reset.h"


namespace sc_core {

class sc_event_and_list;
class sc_event_or_list;
class sc_reset;

// friend function declarations
    void sc_thread_cor_fn( void* );
    void sc_cthread_cor_fn( void* );
    void sc_set_stack_size( sc_thread_handle, std::size_t );
    class sc_event;
    class sc_join;
    class sc_module;
    class sc_process_handle;
    class sc_process_table;
    class sc_simcontext;
    class sc_runnable;
    sc_cor* get_cor_pointer( sc_process_b* process_p );

    void wait( sc_simcontext* );
    void wait( const sc_event&,
              sc_simcontext* );
    void wait( sc_event_or_list&,
              sc_simcontext* );
    void wait( sc_event_and_list&,
              sc_simcontext* );
    void wait( const sc_time&,
              sc_simcontext* );
    void wait( const sc_time&, const sc_event&,
              sc_simcontext* );
    void wait( const sc_time&, sc_event_or_list&,
              sc_simcontext* );
    void wait( const sc_time&, sc_event_and_list&,
              sc_simcontext* );

//==============================================================================

void sc_set_stack_size( sc_thread_handle thread_h, std::size_t size );

//==============================================================================
// sc_thread_process -
//
//==============================================================================
class sc_thread_process : public sc_process_b {
    friend void sc_thread_cor_fn( void* );
    friend void sc_cthread_cor_fn( void* );
    friend void sc_set_stack_size( sc_thread_handle, std::size_t );
    friend class sc_event;
    friend class sc_join;
    friend class sc_module;
    friend class sc_process_handle;
    friend class sc_process_table;
    friend class sc_simcontext;
    friend class sc_runnable;
    friend sc_cor* get_cor_pointer( sc_process_b* process_p );

    friend void wait( sc_simcontext* );
    friend void wait( const sc_event&,
              sc_simcontext* );
    friend void wait( sc_event_or_list&,
              sc_simcontext* );
    friend void wait( sc_event_and_list&,
              sc_simcontext* );
    friend void wait( const sc_time&,
              sc_simcontext* );
    friend void wait( const sc_time&, const sc_event&,
              sc_simcontext* );
    friend void wait( const sc_time&, sc_event_or_list&,
              sc_simcontext* );
    friend void wait( const sc_time&, sc_event_and_list&,
              sc_simcontext* );
  public:
    sc_thread_process( const char* name_p, bool free_host,
        SC_ENTRY_FUNC method_p, sc_process_host* host_p,
        const sc_spawn_options* opt_p );

    virtual ~sc_thread_process();

    virtual const char* kind() const
        { return "sc_thread_process"; }

  protected:
    virtual void kill_process();
    sc_thread_handle next_exist();
    sc_thread_handle next_runnable();
    virtual void prepare_for_simulation();
    inline bool ready_to_run();
    void set_next_exist( sc_thread_handle next_p );
    void set_next_runnable( sc_thread_handle next_p );

    void set_stack_size( std::size_t size );
    inline void suspend_me();

    bool trigger_dynamic( sc_event* );

    void wait( const sc_event& );
    void wait( sc_event_or_list& );
    void wait( sc_event_and_list& );
    void wait( const sc_time& );
    void wait( const sc_time&, const sc_event& );
    void wait( const sc_time&, sc_event_or_list& );
    void wait( const sc_time&, sc_event_and_list& );
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
    if ( (m_throw_type == THROW_NONE) && ( m_wait_cycle_n > 0 ) )
    {
        --m_wait_cycle_n;
        return false;
    }
    return true;
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
//------------------------------------------------------------------------------
inline void sc_thread_process::suspend_me()
{
    sc_simcontext* simc_p = simcontext();
    simc_p->cor_pkg()->yield( simc_p->next_cor() );

    switch ( m_throw_type )
    {
      case THROW_NONE:
        break;
      case THROW_RESET:
        throw sc_user();
      default:
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
    e.add_dynamic( this );
    m_trigger_type = EVENT;
    suspend_me();
}

inline
void
sc_thread_process::wait( sc_event_or_list& el )
{
    el.add_dynamic( this );
    m_event_list_p = &el;
    m_trigger_type = OR_LIST;
    suspend_me();
}

inline
void
sc_thread_process::wait( sc_event_and_list& el )
{
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
    m_timeout_event_p->notify_internal( t );
    m_timeout_event_p->add_dynamic( this );
    m_trigger_type = TIMEOUT;
    suspend_me();
}

inline
void
sc_thread_process::wait( const sc_time& t, const sc_event& e )
{
    m_timeout_event_p->notify_internal( t );
    m_timeout_event_p->add_dynamic( this );
    e.add_dynamic( this );
    m_event_p = &e;
    m_trigger_type = EVENT_TIMEOUT;
    suspend_me();
}

inline
void
sc_thread_process::wait( const sc_time& t, sc_event_or_list& el )
{
    m_timeout_event_p->notify_internal( t );
    m_timeout_event_p->add_dynamic( this );
    el.add_dynamic( this );
    m_event_list_p = &el;
    m_trigger_type = OR_LIST_TIMEOUT;
    suspend_me();
}

inline
void
sc_thread_process::wait( const sc_time& t, sc_event_and_list& el )
{
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
