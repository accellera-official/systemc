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

  sc_event.cpp --

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "sysc/kernel/sc_event.h"
#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_process_int.h"
#include "sysc/kernel/sc_simcontext_int.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_event
//
//  The event class.
// ----------------------------------------------------------------------------

void
sc_event::cancel()
{
    // cancel a delta or timed notification
    switch( m_notify_type ) {
    case DELTA: {
        // remove this event from the delta events set
        m_simc->remove_delta_event( this );
        m_notify_type = NONE;
        break;
    }
    case TIMED: {
        // remove this event from the timed events set
        assert( m_timed != 0 );
        m_timed->m_event = 0;
        m_timed = 0;
        m_notify_type = NONE;
        break;
    }
    default:
        ;
    }
}


void
sc_event::notify()
{
    // immediate notification
    if( m_simc->update_phase() ) {
        SC_REPORT_ERROR( SC_ID_IMMEDIATE_NOTIFICATION_, "" );
    }
    cancel();
    trigger();
}

void
sc_event::notify( const sc_time& t )
{
    if( m_notify_type == DELTA ) {
        return;
    }
    if( t == SC_ZERO_TIME ) {
        if( m_notify_type == TIMED ) {
            // remove this event from the timed events set
            assert( m_timed != 0 );
            m_timed->m_event = 0;
            m_timed = 0;
        }
        // add this event to the delta events set
        m_delta = m_simc->add_delta_event( this );
        m_notify_type = DELTA;
        return;
    }
    if( m_notify_type == TIMED ) {
        assert( m_timed != 0 );
        if( m_timed->m_notify_time <= m_simc->time_stamp() + t ) {
            return;
        }
        // remove this event from the timed events set
        m_timed->m_event = 0;
        m_timed = 0;
    }
    // add this event to the timed events set
    sc_event_timed* et = new sc_event_timed( this, m_simc->time_stamp() + t );
    m_simc->add_timed_event( et );
    m_timed = et;
    m_notify_type = TIMED;
}


void
sc_event::reset()
{
    m_notify_type = NONE;
    m_delta = -1;
    m_timed = 0;
    // clear the dynamic sensitive methods
    m_methods_dynamic.erase_all();
    // clear the dynamic sensitive threads
    m_threads_dynamic.erase_all();
}


void
sc_event::trigger()
{
    int size;


    // trigger the static sensitive methods
    
    if( ( size = m_methods_static.size() ) != 0 ) {
        sc_method_handle* l_methods_static = m_methods_static.raw_data();
        int i = size - 1;
        do {
            sc_method_handle method_h = l_methods_static[i];
            if( method_h->trigger_static() ) {
                m_simc->push_runnable_method( method_h );
            }
        } while( -- i >= 0 );
    }

    // trigger the dynamic sensitive methods
    
    if( ( size = m_methods_dynamic.size() ) != 0 ) {
        sc_method_handle* l_methods_dynamic = m_methods_dynamic.raw_data();
        int i = size - 1;
        do {
            sc_method_handle method_h = l_methods_dynamic[i];
            if( method_h->trigger_dynamic( this ) ) {
                m_simc->push_runnable_method( method_h );
            }
        } while( -- i >= 0 );
        m_methods_dynamic.erase_all();
    }

    // trigger the static sensitive threads
    
    if( ( size = m_threads_static.size() ) != 0 ) {
        sc_thread_handle* l_threads_static = m_threads_static.raw_data();
        int i = size - 1;
        do {
            sc_thread_handle thread_h = l_threads_static[i];
            if( thread_h->trigger_static() ) {
                m_simc->push_runnable_thread( thread_h );
            }
        } while( -- i >= 0 );
    }

    // trigger the dynamic sensitive threads
    
    if( ( size = m_threads_dynamic.size() ) != 0 ) {
        sc_thread_handle* l_threads_dynamic = m_threads_dynamic.raw_data();
        int i = size - 1;
        do {
            sc_thread_handle thread_h = l_threads_dynamic[i];
            if( thread_h->trigger_dynamic( this ) ) {
                m_simc->push_runnable_thread( thread_h );
            }
        } while( -- i >= 0 );
        m_threads_dynamic.erase_all();
    }

    m_notify_type = NONE;
    m_delta = -1;
    m_timed = 0;
}


bool
sc_event::remove_static( sc_method_handle method_h_ ) const
{
    int size = m_methods_static.size();
    sc_method_handle* l_methods_static = m_methods_static.raw_data();
    for( int i = size - 1; i >= 0; -- i ) {
        if( l_methods_static[i] == method_h_ ) {
            l_methods_static[i] = l_methods_static[size - 1];
            m_methods_static.decr_count();
            return true;
        }
    }
    return false;
}

bool
sc_event::remove_static( sc_thread_handle thread_h_ ) const
{
    int size = m_threads_static.size();
    sc_thread_handle* l_threads_static = m_threads_static.raw_data();
    for( int i = size - 1; i >= 0; -- i ) {
        if( l_threads_static[i] == thread_h_ ) {
            l_threads_static[i] = l_threads_static[size - 1];
            m_threads_static.decr_count();
            return true;
        }
    }
    return false;
}

bool
sc_event::remove_dynamic( sc_method_handle method_h_ ) const
{
    int size = m_methods_dynamic.size();
    sc_method_handle* l_methods_dynamic = m_methods_dynamic.raw_data();
    for( int i = size - 1; i >= 0; -- i ) {
        if( l_methods_dynamic[i] == method_h_ ) {
            l_methods_dynamic[i] = l_methods_dynamic[size - 1];
            m_methods_dynamic.decr_count();
            return true;
        }
    }
    return false;
}

bool
sc_event::remove_dynamic( sc_thread_handle thread_h_ ) const
{
    int size = m_threads_dynamic.size();
    sc_thread_handle* l_threads_dynamic = m_threads_dynamic.raw_data();
    for( int i = size - 1; i >= 0; -- i ) {
        if( l_threads_dynamic[i] == thread_h_ ) {
            l_threads_dynamic[i] = l_threads_dynamic[size - 1];
            m_threads_dynamic.decr_count();
            return true;
        }
    }
    return false;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_event_timed
//
//  Class for storing the time to notify a timed event.
// ----------------------------------------------------------------------------

// dedicated memory management; not MT-Safe

union sc_event_timed_u
{
    sc_event_timed_u* next;
    char              dummy[sizeof( sc_event_timed )];
};

static
sc_event_timed_u* free_list = 0;

void*
sc_event_timed::allocate()
{
    const int ALLOC_SIZE = 64;

    if( free_list == 0 ) {
        free_list = (sc_event_timed_u*) malloc( ALLOC_SIZE *
                                                sizeof( sc_event_timed ) );
        int i = 0;
        for( ; i < ALLOC_SIZE - 1; ++ i ) {
            free_list[i].next = &free_list[i + 1];
        }
        free_list[i].next = 0;
    }

    sc_event_timed_u* q = free_list;
    free_list = free_list->next;
    return q;
}

void
sc_event_timed::deallocate( void* p )
{
    if( p != 0 ) {
        sc_event_timed_u* q = RCAST<sc_event_timed_u*>( p );
        q->next = free_list;
        free_list = q;
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_event_list
//
//  Base class for lists of events.
// ----------------------------------------------------------------------------

void
sc_event_list::push_back( const sc_event& e )
{
    // make sure e is not already in the list
    const sc_event** l_events = m_events.raw_data();
    for( int i = m_events.size() - 1; i >= 0; -- i ) {
        if( &e == l_events[i] ) {
            // event already in the list; ignore
            return;
        }
    }
    m_events.push_back( &e );
}


void
sc_event_list::add_dynamic( sc_method_handle method_h )
{
    const sc_event** l_events = m_events.raw_data();
    for( int i = m_events.size() - 1; i >= 0; -- i ) {
        l_events[i]->add_dynamic( method_h );
    }
}

void
sc_event_list::add_dynamic( sc_thread_handle thread_h )
{
    const sc_event** l_events = m_events.raw_data();
    for( int i = m_events.size() - 1; i >= 0; -- i ) {
        l_events[i]->add_dynamic( thread_h );
    }
}

void
sc_event_list::remove_dynamic( sc_method_handle method_h,
                               const sc_event* e_not )
{
    const sc_event** l_events = m_events.raw_data();
    for( int i = m_events.size() - 1; i >= 0; -- i ) {
        const sc_event* e = l_events[i];
        if( e != e_not ) {
            e->remove_dynamic( method_h );
        }
    }
}

void
sc_event_list::remove_dynamic( sc_thread_handle thread_h,
                               const sc_event* e_not )
{
    const sc_event** l_events = m_events.raw_data();
    for( int i = m_events.size() - 1; i >= 0; -- i ) {
        const sc_event* e = l_events[i];
        if( e != e_not ) {
            e->remove_dynamic( thread_h );
        }
    }
}

} // namespace sc_core

// Taf!
