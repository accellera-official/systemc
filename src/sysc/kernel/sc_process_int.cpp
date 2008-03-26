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

  sc_process_int.cpp -- Process internals.

  Original Author: Stan Y. Liao, Synopsys, Inc.

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


#include <assert.h>
#include <cstdlib>
#include <cstddef>

#include "sysc/kernel/sc_process_int.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_simcontext_int.h"
#include "sysc/kernel/sc_name_gen.h"

namespace sc_core {

static sc_thread_handle dead_thread_h = 0;  // Last deleted thread.
// ----------------------------------------------------------------------------
//  CLASS : sc_process_monitor
//
//  Monitoring mechanism for SC_THREAD and SC_CTHREAD instances.
// ----------------------------------------------------------------------------

void sc_process_monitor::signal( sc_thread_handle thread_p, int type )
{
}

#if !defined(SC_USE_MEMBER_FUNC_PTR)
    sc_process_call_base sc_process_defunct;
#endif

// ----------------------------------------------------------------------------
//  CLASS : sc_process_b
//
//  Process base class.
// ----------------------------------------------------------------------------

sc_process_b* sc_process_b::m_last_created_p = 0;


sc_process_b::sc_process_b( const char*   nm,
			    SC_ENTRY_FUNC fn,
			    sc_process_host*    host )
: sc_object( nm ),
  entry_fn( fn ),
  host( host ),
  proc_id( simcontext()->next_proc_id() ),
  m_do_initialize( true ),
  m_event( 0 ),
  m_event_count( 0 ),
  m_event_list( 0 ),
  m_exist_p( 0 ),
  m_last_report(0),
  m_name_gen_p(0),
  m_runnable_p( 0 ),
  m_timed_out( false ),
  m_trigger_type( STATIC )
{
    m_last_created_p = this;
}

// This is the object instance destructor for this class. If we still have
// a live thread we remove it from event's static callback queues.

sc_process_b::~sc_process_b()
{
    if ( entry_fn != SC_DEFUNCT_PROCESS_FUNCTION )
    {
#       if !defined(SC_USE_MEMBER_FUNC_PTR)
            delete entry_fn;
#       endif
    }
    // redirect my children as children of the simcontext
    int size = m_child_objects.size();
    for(int i = 0; i < size; i++) {
      sc_object* obj =  m_child_objects[i];
      obj->m_parent = NULL;
    }
    if ( m_name_gen_p ) delete m_name_gen_p;
	if ( m_last_report ) delete m_last_report;
}

sc_process_b*
sc_process_b::get_last_created_process()
{
    return m_last_created_p;
}

const ::std::vector<sc_object*>&
sc_process_b::get_child_objects() const
{
    return m_child_objects;
}

void
sc_process_b::add_child_object( sc_object* object_ )
{
    // no check if object_ is already in the set
    m_child_objects.push_back( object_ );
}

void
sc_process_b::remove_child_object( sc_object* object_ )
{
    int size = m_child_objects.size();
    for( int i = 0; i < size; ++ i ) {
        if( object_ == m_child_objects[i] ) {
            m_child_objects[i] = m_child_objects[size - 1];
            m_child_objects.decr_count();
            return;
        }
    }
    // no check if object_ is really in the set
}

void
sc_process_b::add_static_event( const sc_event& e )
{
    // check if already in static events set
    for( int i = m_static_events.size() - 1; i >= 0; -- i ) {
	if( &e == m_static_events[i] ) {
	    return;
	}
    }
    m_static_events.push_back( &e );
    // not very nice, but it works
    sc_method_handle method_h = DCAST<sc_method_handle>( this );
    if( method_h != 0 ) {
	e.add_static( method_h );
	return;
    }
    sc_thread_handle thread_h = DCAST<sc_thread_handle>( this );
    if( thread_h != 0 ) {
	e.add_static( thread_h );
	return;
    }
    assert( false );
}

const char* sc_process_b::gen_unique_name( const char* basename_, 
    bool preserve_first )
{
    if ( ! m_name_gen_p ) m_name_gen_p = new sc_name_gen;
    return m_name_gen_p->gen_unique_name( basename_, preserve_first );
}
	

bool sc_process_b::is_cthread() const
{
    return false;
}


void
sc_process_b::remove_static_events()
{
    // not very nice, but it works
    sc_method_handle method_h = DCAST<sc_method_handle>( this );
    if( method_h != 0 ) {
	for( int i = m_static_events.size() - 1; i >= 0; -- i ) {
	    m_static_events[i]->remove_static( method_h );
	}
	m_static_events.erase_all();
	return;
    }
    sc_thread_handle thread_h = DCAST<sc_thread_handle>( this );
    if( thread_h != 0 ) {
	for( int i = m_static_events.size() - 1; i >= 0; -- i ) {
	    m_static_events[i]->remove_static( thread_h );
	}
	m_static_events.erase_all();
	return;
    }
    assert( false );
}


// ----------------------------------------------------------------------------
//  CLASS : sc_method_process
//
//  Process class for SC_METHODs.
// ----------------------------------------------------------------------------

sc_method_process::sc_method_process( const char*   nm,
				      SC_ENTRY_FUNC fn,
				      sc_process_host*    host )
: sc_process_b( nm, fn, host )
{}


// This is the object instance destructor for this class. We delete the
// thread stack, and remove this instance from any dynamic event notifications.
// The sc_process_b destructor will take care of removing this object instance
// from static event notifications.

sc_method_process::~sc_method_process()
{
    if ( m_event ) m_event->remove_dynamic( this );
    if ( m_event_list ) 
    {
	m_event_list->remove_dynamic( this, 0 );
	m_event_list->auto_delete();
    }
    remove_static_events();
}

void
sc_method_process::clear_trigger()
{
    switch( m_trigger_type ) {
    case STATIC: {
	return;
    }
    case EVENT: {
	m_event->remove_dynamic( this );
	m_event = 0;
	break;
    }
    case OR_LIST: {
	m_event_list->remove_dynamic( this, 0 );
	m_event_list->auto_delete();
	m_event_list = 0;
	break;
    }
    case AND_LIST: {
	m_event_list->remove_dynamic( this, 0 );
	m_event_list->auto_delete();
	m_event_list = 0;
	m_event_count = 0;
	break;
    }
    case TIMEOUT: {
	m_timeout_event.cancel();
	m_timeout_event.reset();
	break;
    }
    case EVENT_TIMEOUT: {
	m_timeout_event.cancel();
	m_timeout_event.reset();
	m_event->remove_dynamic( this );
	m_event = 0;
	break;
    }
    case OR_LIST_TIMEOUT: {
	m_timeout_event.cancel();
	m_timeout_event.reset();
	m_event_list->remove_dynamic( this, 0 );
	m_event_list->auto_delete();
	m_event_list = 0;
	break;
    }
    case AND_LIST_TIMEOUT: {
	m_timeout_event.cancel();
	m_timeout_event.reset();
	m_event_list->remove_dynamic( this, 0 );
	m_event_list->auto_delete();
	m_event_list = 0;
	m_event_count = 0;
	break;
    }
    }
    m_trigger_type = STATIC;
}


bool
sc_method_process::trigger_dynamic( sc_event* e )
{
    if( is_runnable() ) {
	return false;
    }
    m_timed_out = false;
    switch( m_trigger_type ) {
    case EVENT: {
	m_event = 0;
	m_trigger_type = STATIC;
	return true;
    }
    case OR_LIST: {
	m_event_list->remove_dynamic( this, e );
	m_event_list->auto_delete();
	m_event_list = 0;
	m_trigger_type = STATIC;
	return true;
    }
    case AND_LIST: {
	if( -- m_event_count == 0 ) {
	    // no need to remove_dynamic
	    m_event_list->auto_delete();
	    m_event_list = 0;
	    m_trigger_type = STATIC;
	    return true;
	}
	return false;
    }
    case TIMEOUT: {
	m_trigger_type = STATIC;
	return true;
    }
    case EVENT_TIMEOUT: {
	if( e == m_event ) {
	    m_timeout_event.cancel();
	    m_timeout_event.reset();
	} else {
	    m_timed_out = true;
	    m_event->remove_dynamic( this );
	}
	m_event = 0;
	m_trigger_type = STATIC;
	return true;
    }
    case OR_LIST_TIMEOUT: {
	if( e != &m_timeout_event ) {
	    m_timeout_event.cancel();
	    m_timeout_event.reset();
	} else {
	    m_timed_out = true;
	}
	m_event_list->remove_dynamic( this, e );
	m_event_list->auto_delete();
	m_event_list = 0;
	m_trigger_type = STATIC;
	return true;
    }
    case AND_LIST_TIMEOUT: {
	if( e == &m_timeout_event ) {
	    m_timed_out = true;
	    m_event_list->remove_dynamic( this, e );
	    m_event_list->auto_delete();
	    m_event_list = 0;
	    m_trigger_type = STATIC;
	    return true;
	} else if( -- m_event_count == 0 ) {
	    m_timeout_event.cancel();
	    m_timeout_event.reset();
	    // no need to remove_dynamic
	    m_event_list->auto_delete();
	    m_event_list = 0;
	    m_trigger_type = STATIC;
	    return true;
	}
	return false;
    }
    case STATIC: {
	// we should never get here
	assert( false );
    }
    }
    return false;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_thread_process
//
//  Process class for SC_THREADs.
// ----------------------------------------------------------------------------

sc_thread_process::sc_thread_process( const char*   nm,
                                      SC_ENTRY_FUNC fn,
                                      sc_process_host*    host )
: sc_process_b( nm, fn, host ),
  m_stack_size( SC_DEFAULT_STACK_SIZE ),
  m_cor( 0 )
{
    if( is_cthread() ) {
	do_initialize( false );
    }
}


// This is the object instance destructor for this class. We delete the
// thread stack, and remove this instance from any dynamic event notifications.
// The sc_process_b destructor will take care of removing this object instance
// from static event notifications.

sc_thread_process::~sc_thread_process()
{
    if( m_cor != 0 ) {
	m_cor->stack_protect( false );
	delete m_cor;
    }
    if ( m_event ) m_event->remove_dynamic( this );
    if ( m_event_list ) 
    {
	m_event_list->remove_dynamic( this, 0 );
	m_event_list->auto_delete();
    }
    if ( entry_fn != SC_DEFUNCT_PROCESS_FUNCTION ) {
        remove_static_events();
    }
}

void
sc_thread_process::set_stack_size( size_t size )
{
    m_stack_size = size;
}


void
sc_thread_process::prepare_for_simulation()
{
    m_cor = simcontext()->cor_pkg()->create( m_stack_size,
					     sc_thread_cor_fn, this );
    m_cor->stack_protect( true );
}


void
sc_thread_cor_fn( void* arg )
{
    int mon_n;
    sc_thread_handle thread_h = RCAST<sc_thread_handle>( arg );

    try {
	thread_h->execute();
    }
    catch( const sc_report& ex ) {
	std::cout << "\n" << ex.what() << std::endl;
	thread_h->simcontext()->set_error();
    }

    // if control reaches this point, then the process has gone to heaven
    // delete any previous one that is waiting and put our thread in its
    // place.

    if (dead_thread_h) delete dead_thread_h;
    dead_thread_h = thread_h;

    // If there are monitors watching this thread call their signal method.
    mon_n = thread_h->m_monitor_q.size();
    if ( mon_n )
    {
    	for ( int mon_i = 0; mon_i < mon_n; mon_i++ )
	    thread_h->m_monitor_q[mon_i]->signal(
		thread_h, sc_process_monitor::spm_exit);
    }

    thread_h->remove_static_events();
    thread_h->entry_fn = SC_DEFUNCT_PROCESS_FUNCTION;  // mark defunct

    // abort the current coroutine and resume the next
    sc_simcontext* simc = thread_h->simcontext();
    simc->cor_pkg()->abort( simc->next_cor() );
}

void sc_thread_process::signal_monitors(int type)
{
    int mon_n;	// # of monitors present.

    mon_n = m_monitor_q.size();
    for ( int mon_i = 0; mon_i < mon_n; mon_i++ )
	m_monitor_q[mon_i]->signal(this, type);
}

bool
sc_thread_process::trigger_dynamic( sc_event* e )
{
    if( is_runnable() ) {
	return false;
    }
    m_timed_out = false;
    switch( m_trigger_type ) {
    case EVENT: {
	m_trigger_type = STATIC;
	return true;
    }
    case OR_LIST: {
	m_event_list->remove_dynamic( this, e );
	m_event_list->auto_delete();
	m_event_list = 0;
	m_trigger_type = STATIC;
	return true;
    }
    case AND_LIST: {
	if( -- m_event_count == 0 ) {
	    // no need to remove_dynamic
	    m_event_list->auto_delete();
	    m_event_list = 0;
	    m_trigger_type = STATIC;
	    return true;
	}
	return false;
    }
    case TIMEOUT: {
	m_trigger_type = STATIC;
	return true;
    }
    case EVENT_TIMEOUT: {
	if( e == m_event ) {
	    m_timeout_event.cancel();
	    m_timeout_event.reset();
	} else {
	    m_timed_out = true;
	    m_event->remove_dynamic( this );
	}
	m_event = 0;
	m_trigger_type = STATIC;
	return true;
    }
    case OR_LIST_TIMEOUT: {
	if( e != &m_timeout_event ) {
	    m_timeout_event.cancel();
	    m_timeout_event.reset();
	} else {
	    m_timed_out = true;
	}
	m_event_list->remove_dynamic( this, e );
	m_event_list->auto_delete();
	m_event_list = 0;
	m_trigger_type = STATIC;
	return true;
    }
    case AND_LIST_TIMEOUT: {
	if( e == &m_timeout_event ) {
	    m_timed_out = true;
	    m_event_list->remove_dynamic( this, e );
	    m_event_list->auto_delete();
	    m_event_list = 0;
	    m_trigger_type = STATIC;
	    return true;
	} else if( -- m_event_count == 0 ) {
	    m_timeout_event.cancel();
	    m_timeout_event.reset();
	    // no need to remove_dynamic
	    m_event_list->auto_delete();
	    m_event_list = 0;
	    m_trigger_type = STATIC;
	    return true;
	}
	return false;
    }
    case STATIC: {
	// we should never get here
	assert( false );
    }
    }
    return false;
}


void
sc_set_stack_size( sc_thread_handle thread_h, size_t size )
{
    thread_h->set_stack_size( size );
}


// ----------------------------------------------------------------------------
//  CLASS : sc_cthread_process
//
//  Process class for SC_CTHREADs.
// ----------------------------------------------------------------------------

sc_cthread_process::sc_cthread_process( const char*   nm,
					SC_ENTRY_FUNC fn,
					sc_process_host*    host )
: sc_thread_process( nm, fn, host ),
  m_wait_state( UNKNOWN ),
  m_wait_cycles( 0 ),
  m_exception_level( -1 ),
  m_watchlists( SC_MAX_WATCH_LEVEL )
{
    __reset_watching();
    for( int i = 0; i < SC_MAX_WATCH_LEVEL; ++ i ) {
        m_watchlists.push_back(new sc_plist<sc_lambda_ptr*>);
    }
}

sc_cthread_process::~sc_cthread_process()
{
    for( int i = 0; i < SC_MAX_WATCH_LEVEL; ++ i ) {
        sc_plist<sc_lambda_ptr*>::iterator it( m_watchlists[i] );
        while( ! it.empty() ) {
            delete *it;  // this was created with `new sc_lambda_ptr'
            it.remove(); // and go on to next
        }
        delete m_watchlists[i];
    }
}


bool sc_cthread_process::is_cthread() const
{
    return true;
}


void
sc_cthread_process::prepare_for_simulation()
{
    m_cor = simcontext()->cor_pkg()->create( m_stack_size,
					     sc_cthread_cor_fn, this );
    m_cor->stack_protect( true );
}


bool
sc_cthread_process::ready_to_run()
{
    bool exception = eval_watchlist();
    if( exception ) {
	return true;
    }

    bool ready;
    switch( m_wait_state ) {
    case CLOCK:
        ready = ( -- m_wait_cycles == 0 );
        break;
    case LAMBDA:
        ready = m_wait_lambda->eval();
        break;
    default:
        ready = true;
        break;
    }
    return ready;
}


void
sc_cthread_cor_fn( void* arg )
{
    sc_cthread_handle cthread_h = RCAST<sc_cthread_handle>( arg );
    int mon_n;

    while( true ) {

        try {
            cthread_h->execute();
        }
        catch( sc_user ) {
	    assert( cthread_h->m_watch_level == 0 );
	    continue;
        }
        catch( sc_halt ) {
            ::std::cout << "Terminating process " 
		      << cthread_h->name() << ::std::endl;
        }
	catch( const sc_report& ex ) {
	    ::std::cout << "\n" << ex.what() << ::std::endl;
	    cthread_h->simcontext()->set_error();
	}

	break;
    }

    // if control reaches this point, then the process has gone to heaven
    if ( dead_thread_h ) delete dead_thread_h;
    dead_thread_h = (sc_thread_handle)cthread_h;

    mon_n = cthread_h->m_monitor_q.size();
    if ( mon_n )
    {
    	for ( int mon_i = 0; mon_i < mon_n; mon_i++ )
	{
	    cthread_h->m_monitor_q[mon_i]->signal(cthread_h, 
		sc_process_monitor::spm_exit);
	}
    }
    cthread_h->remove_static_events();
    cthread_h->entry_fn = SC_DEFUNCT_PROCESS_FUNCTION;  // mark defunct

    // abort the current coroutine and resume the next
    sc_simcontext* simc = cthread_h->simcontext();
    simc->cor_pkg()->abort( simc->next_cor() );
}


bool
sc_cthread_process::eval_watchlist()
{
    int wlevel = __watch_level();
    for( int i = 0; i <= wlevel; ++ i ) {
        sc_plist<sc_lambda_ptr*>& li = *(m_watchlists[i]);
        sc_plist<sc_lambda_ptr*>::iterator wit( li );
        while( ! wit.empty() ) {
            if( (**wit)->eval() ) {
                m_exception_level = i;
                if( i == 0 ) /* never remove global watching */
                    i = 1;
                for( ; i <= wlevel; ++ i ) {
                    sc_plist<sc_lambda_ptr*>& l = *(m_watchlists[i]);
                    sc_plist<sc_lambda_ptr*>::iterator it( l );
                    while( ! it.empty() ) {
                        delete *it;
                        it.remove();
                    }
                }
                return true;
            }
            wit ++;
        }
    }
    m_exception_level = -1;
    return false;
}

bool
sc_cthread_process::eval_watchlist_curr_level()
{
    int wlevel = m_watch_level;
    assert( wlevel > 0 );
    
    sc_plist<sc_lambda_ptr*>& li = *m_watchlists[wlevel];
    sc_plist<sc_lambda_ptr*>::iterator wit( li );
    while( ! wit.empty() ) {
        if( (**wit)->eval() ) {
            m_exception_level = wlevel;
            wit.reset( li );
            while( ! wit.empty() ) {
                delete *wit;
                wit.remove();
            }
            return true;
        }
        wit ++;
    }
    m_exception_level = -1;
    return false;
}

} // namespace sc_core

// Taf!
