/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
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

  sc_process_int.h -- Process internals.
                      DO NOT EXPORT THIS INCLUDE FILE.

  Original Author: Stan Y. Liao, Synopsys, Inc.
                   Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Andy Goodrich, Forte Design Systems 20 May 2003
                               Bishnupriya Bhattacharya, Cadence Design Systems,
                               25 August, 2003
  Description of Modification: Changes for dynamic processes.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_PROCESS_INT_H
#define SC_PROCESS_INT_H

#include "sysc/kernel/sc_cor.h"
#include "sysc/kernel/sc_except.h"
#include "sysc/kernel/sc_lambda.h"
#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_process_b.h"
#include "sysc/utils/sc_list.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_process_b
//
//  Process base class.
// ----------------------------------------------------------------------------

// Declaration in sc_process_b.h


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
bool
sc_process_b::do_initialize() const
{
    return m_do_initialize;
}

inline
void
sc_process_b::do_initialize( bool do_initialize_ )
{
    m_do_initialize = do_initialize_;
}


inline
bool
sc_process_b::is_runnable()
{
    return m_runnable_p != 0;
}


inline
void
sc_process_b::execute()
{
#ifndef SC_USE_MEMBER_FUNC_PTR
    entry_fn->invoke( host );
#else
    (host->*entry_fn)(); 
#endif
}


inline
bool
sc_process_b::trigger_static()
{
    return ( !is_runnable() && m_trigger_type == STATIC );
}


inline
bool
sc_process_b::timed_out() const
{
    return m_timed_out;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_method_process
//
//  Process class for SC_METHODs.
// ----------------------------------------------------------------------------

class sc_method_process
: public sc_process_b
{
    friend class sc_event;
    friend class sc_module;
    friend class sc_process_host;
    friend class sc_process_table;
    friend class sc_simcontext;
    friend class sc_runnable;

    friend void next_trigger( sc_simcontext* );
    friend void next_trigger( const sc_event&,
			      sc_simcontext* );
    friend void next_trigger( sc_event_or_list&,
			      sc_simcontext* );
    friend void next_trigger( sc_event_and_list&,
			      sc_simcontext* );
    friend void next_trigger( const sc_time&,
			      sc_simcontext* );
    friend void next_trigger( const sc_time&, const sc_event&,
			      sc_simcontext* );
    friend void next_trigger( const sc_time&, sc_event_or_list&,
			      sc_simcontext* );
    friend void next_trigger( const sc_time&, sc_event_and_list&,
			      sc_simcontext* );

public:

    virtual const char* kind() const
        { return "sc_method_process"; }

protected:

    sc_method_process( const char*   nm,
		       SC_ENTRY_FUNC fn,
		       sc_process_host*    host );
    virtual ~sc_method_process();

    void clear_trigger();

    void next_trigger( const sc_event& );
    void next_trigger( sc_event_or_list& );
    void next_trigger( sc_event_and_list& );
    void next_trigger( const sc_time& );
    void next_trigger( const sc_time&, const sc_event& );
    void next_trigger( const sc_time&, sc_event_or_list& );
    void next_trigger( const sc_time&, sc_event_and_list& );

    bool trigger_dynamic( sc_event* );

    void set_next_exist( sc_method_handle next_p );
    sc_method_handle next_exist();
    void set_next_runnable( sc_method_handle next_p );
    sc_method_handle next_runnable();

};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
void
sc_method_process::next_trigger( const sc_event& e )
{
    clear_trigger();
    e.add_dynamic( this );
    m_event = &e;
    m_trigger_type = EVENT;
}

inline
void
sc_method_process::next_trigger( sc_event_or_list& el )
{
    clear_trigger();
    el.add_dynamic( this );
    m_event_list = &el;
    m_trigger_type = OR_LIST;
}

inline
void
sc_method_process::next_trigger( sc_event_and_list& el )
{
    clear_trigger();
    el.add_dynamic( this );
    m_event_list = &el;
    m_event_count = el.size();
    m_trigger_type = AND_LIST;
}

inline
void
sc_method_process::next_trigger( const sc_time& t )
{
    clear_trigger();
    m_timeout_event.notify_delayed( t );
    m_timeout_event.add_dynamic( this );
    m_trigger_type = TIMEOUT;
}

inline
void
sc_method_process::next_trigger( const sc_time& t, const sc_event& e )
{
    clear_trigger();
    m_timeout_event.notify_delayed( t );
    m_timeout_event.add_dynamic( this );
    e.add_dynamic( this );
    m_event = &e;
    m_trigger_type = EVENT_TIMEOUT;
}

inline
void
sc_method_process::next_trigger( const sc_time& t, sc_event_or_list& el )
{
    clear_trigger();
    m_timeout_event.notify_delayed( t );
    m_timeout_event.add_dynamic( this );
    el.add_dynamic( this );
    m_event_list = &el;
    m_trigger_type = OR_LIST_TIMEOUT;
}

inline
void
sc_method_process::next_trigger( const sc_time& t, sc_event_and_list& el )
{
    clear_trigger();
    m_timeout_event.notify_delayed( t );
    m_timeout_event.add_dynamic( this );
    el.add_dynamic( this );
    m_event_list = &el;
    m_event_count = el.size();
    m_trigger_type = AND_LIST_TIMEOUT;
}

inline
void sc_method_process::set_next_exist(sc_method_handle next_p)
{
    m_exist_p = next_p;
}

inline
sc_method_handle sc_method_process::next_exist()
{
    return (sc_method_handle)m_exist_p;
}


inline
void sc_method_process::set_next_runnable(sc_method_handle next_p)
{
    m_runnable_p = next_p;
}

inline
sc_method_handle sc_method_process::next_runnable()
{
    return (sc_method_handle)m_runnable_p;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_thread_process
//
//  Process class for SC_THREADs.
// ----------------------------------------------------------------------------

class sc_thread_process
: public sc_process_b
{
    friend class sc_event;
    friend class sc_module;
    friend class sc_process_table;
    friend class sc_simcontext;
    friend class sc_runnable;

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

    virtual const char* kind() const
        { return "sc_thread_process"; }

public:

    void add_monitor( sc_process_monitor* monitor_p );
    void remove_monitor( sc_process_monitor* monitor_p);
    void signal_monitors( int type = 0 );

protected:

    friend void sc_thread_cor_fn( void* );
    friend void sc_cthread_cor_fn( void* );

    sc_thread_process( const char*   nm,
		       SC_ENTRY_FUNC fn,
		       sc_process_host*    host);

    virtual ~sc_thread_process();

    void set_stack_size( size_t size );

    virtual void prepare_for_simulation();

    virtual bool ready_to_run();

    void wait( const sc_event& );
    void wait( sc_event_or_list& );
    void wait( sc_event_and_list& );
    void wait( const sc_time& );
    void wait( const sc_time&, const sc_event& );
    void wait( const sc_time&, sc_event_or_list& );
    void wait( const sc_time&, sc_event_and_list& );

    bool trigger_dynamic( sc_event* );

    friend void sc_set_stack_size( sc_thread_handle, size_t );

    void set_next_exist( sc_thread_handle next_p );
    sc_thread_handle next_exist();
    void set_next_runnable( sc_thread_handle next_p );
    sc_thread_handle next_runnable();

protected:

    size_t  m_stack_size;
    sc_cor* m_cor;         // the thread's coroutine
    sc_pvector<sc_process_monitor*> m_monitor_q; // thread monitors.
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
void
sc_switch_thread( sc_simcontext* simc )
{
    simc->cor_pkg()->yield( simc->next_cor() );
}


// ----------------------------------------------------------------------------


inline
bool
sc_thread_process::ready_to_run()
{
    return true;
}


inline
void
sc_thread_process::wait( const sc_event& e )
{
    e.add_dynamic( this );
    m_trigger_type = EVENT;
    sc_switch_thread( simcontext() );
}

inline
void
sc_thread_process::wait( sc_event_or_list& el )
{
    el.add_dynamic( this );
    m_event_list = &el;
    m_trigger_type = OR_LIST;
    sc_switch_thread( simcontext() );
}

inline
void
sc_thread_process::wait( sc_event_and_list& el )
{
    el.add_dynamic( this );
    m_event_list = &el;
    m_event_count = el.size();
    m_trigger_type = AND_LIST;
    sc_switch_thread( simcontext() );
}

inline
void
sc_thread_process::wait( const sc_time& t )
{
    m_timeout_event.notify_delayed( t );
    m_timeout_event.add_dynamic( this );
    m_trigger_type = TIMEOUT;
    sc_switch_thread( simcontext() );
}

inline
void
sc_thread_process::wait( const sc_time& t, const sc_event& e )
{
    m_timeout_event.notify_delayed( t );
    m_timeout_event.add_dynamic( this );
    e.add_dynamic( this );
    m_event = &e;
    m_trigger_type = EVENT_TIMEOUT;
    sc_switch_thread( simcontext() );
}

inline
void
sc_thread_process::wait( const sc_time& t, sc_event_or_list& el )
{
    m_timeout_event.notify_delayed( t );
    m_timeout_event.add_dynamic( this );
    el.add_dynamic( this );
    m_event_list = &el;
    m_trigger_type = OR_LIST_TIMEOUT;
    sc_switch_thread( simcontext() );
}

inline
void
sc_thread_process::wait( const sc_time& t, sc_event_and_list& el )
{
    m_timeout_event.notify_delayed( t );
    m_timeout_event.add_dynamic( this );
    el.add_dynamic( this );
    m_event_list = &el;
    m_event_count = el.size();
    m_trigger_type = AND_LIST_TIMEOUT;
    sc_switch_thread( simcontext() );
}

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
            m_monitor_q.decr_count();
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



// ----------------------------------------------------------------------------
//  CLASS : sc_cthread_process
//
//  Process class for SC_CTHREADs.
// ----------------------------------------------------------------------------

class sc_cthread_process
: public sc_thread_process
{
    friend class sc_module;
    friend class sc_process_table;
    friend class sc_simcontext;
    friend class sc_thread_process;

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

    friend void halt( sc_simcontext* );
    friend void wait( int,
		      sc_simcontext* );
    friend void wait_until( const sc_lambda_ptr&,
			    sc_simcontext* );
    friend void watching_before_simulation( const sc_lambda_ptr&,
					    sc_simcontext* );
    friend void watching_during_simulation( const sc_lambda_ptr&,
					    sc_simcontext* );

    friend void __reset_watching( sc_cthread_handle );
    friend void __open_watching( sc_cthread_handle );
    friend void __close_watching( sc_cthread_handle );
    friend int  __watch_level( sc_cthread_handle );
    friend void __watching_first( sc_cthread_handle );
    friend void __sanitycheck_watchlists( sc_cthread_handle );

    void set_next_exist( sc_cthread_handle next_p );
    sc_cthread_handle next_exist();

public:

    virtual const char* kind() const
        { return "sc_cthread_process"; }

protected:
    friend void sc_cthread_cor_fn( void* );
    virtual ~sc_cthread_process();

private:

    sc_cthread_process( const char*   nm,
			SC_ENTRY_FUNC fn,
			sc_process_host*    host );
     
    virtual bool is_cthread() const;

    virtual void prepare_for_simulation();

    virtual bool ready_to_run();


    bool eval_watchlist();
    bool eval_watchlist_curr_level();

    void wait_halt();
    void wait_clock( int = 1 );
    void wait_lambda( const sc_lambda_ptr& );
    void add_lambda( const sc_lambda_ptr& );

    void __reset_watching();
    void __open_watching();
    void __close_watching();
    int  __watch_level() const;

private:

    typedef sc_pvector<sc_plist<sc_lambda_ptr*>* > watchlists_t;

    enum { UNKNOWN, CLOCK, LAMBDA } m_wait_state;
    int                             m_wait_cycles;
    sc_lambda_ptr                   m_wait_lambda;
    int                             m_exception_level;

    int                             m_watch_level;

    watchlists_t                    m_watchlists;
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
inline
void sc_cthread_process::set_next_exist(sc_cthread_handle next_p)
{
    m_exist_p = next_p;
}

inline
sc_cthread_handle sc_cthread_process::next_exist()
{
    return (sc_cthread_handle)m_exist_p;
}


inline
void
sc_cthread_process::wait_halt()
{
    m_wait_cycles = 1;
    m_wait_state = CLOCK;
    sc_switch_thread( simcontext() );
    throw sc_halt();
}

inline
void
sc_cthread_process::wait_clock( int n  )
{
    m_wait_cycles = n;
    m_wait_state = CLOCK;
    sc_switch_thread( simcontext() );
    m_wait_state = UNKNOWN;
    if( m_exception_level == 0 ) {

	throw sc_user();

    } else if( m_exception_level > 0 ) {
	throw m_exception_level;
    }
}

inline
void
sc_cthread_process::wait_lambda( const sc_lambda_ptr& lambda )
{
    m_wait_lambda = lambda;
    m_wait_state = LAMBDA;
    sc_switch_thread( simcontext() );
    m_wait_state = UNKNOWN;
    if( m_exception_level == 0 ) {

	throw sc_user();

    } else if( m_exception_level > 0 ) {
	throw m_exception_level;
    }
}

inline
void
sc_cthread_process::add_lambda( const sc_lambda_ptr& lambda )
{
    assert( m_watch_level < SC_MAX_WATCH_LEVEL );
    m_watchlists[m_watch_level]->push_back( new sc_lambda_ptr( lambda ) );
}


inline
void
sc_cthread_process::__reset_watching()
{
    m_watch_level = 0;
}

inline
void
sc_cthread_process::__open_watching()
{
    m_watch_level ++;
}

inline
void
sc_cthread_process::__close_watching()
{
    m_watch_level --;
    assert( m_watch_level >= 0 );
}

inline
int
sc_cthread_process::__watch_level() const
{
    return m_watch_level;
}

} // namespace sc_core

#endif

// Taf!
