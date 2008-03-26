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

  sc_event.cpp --

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_event.cpp,v $
// Revision 1.2  2007/01/12 21:04:53  acg
//  Andy Goodrich: fix for Microsoft compiler.
//
// Revision 1.7  2006/04/11 23:13:20  acg
//   Andy Goodrich: Changes for reduced reset support that only includes
//   sc_cthread, but has preliminary hooks for expanding to method and thread
//   processes also.
//
// Revision 1.6  2006/01/25 00:31:19  acg
//  Andy Goodrich: Changed over to use a standard message id of
//  SC_ID_IEEE_1666_DEPRECATION for all deprecation messages.
//
// Revision 1.5  2006/01/24 20:59:11  acg
//  Andy Goodrich: fix up of CVS comments, new version roll.
//
// Revision 1.4  2006/01/24 20:48:14  acg
// Andy Goodrich: added deprecation warnings for notify_delayed(). Added two
// new implementation-dependent methods, notify_next_delta() & notify_internal()
// to replace calls to notify_delayed() from within the simulator. These two
// new methods are simpler than notify_delayed() and should speed up simulations
//
// Revision 1.3  2006/01/13 18:44:29  acg
// Added $Log to record CVS changes into the source.
//
#include "sysc/kernel/sc_event.h"
#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_process.h"
#include "sysc/kernel/sc_simcontext_int.h"
#include "sysc/utils/sc_utils_ids.h"

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
        m_delta_event_index = m_simc->add_delta_event( this );
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

static void sc_warn_notify_delayed()
{
    static bool warn_notify_delayed=true;
    if ( warn_notify_delayed )
    {
        warn_notify_delayed = false;
        SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
      "notify_delayed(...) is deprecated, use notify(sc_time) instead" );
    }
}

void
sc_event::notify_delayed()
{
    sc_warn_notify_delayed();
    if( m_notify_type != NONE ) {
        SC_REPORT_ERROR( SC_ID_NOTIFY_DELAYED_, 0 );
    }
    // add this event to the delta events set
    m_delta_event_index = m_simc->add_delta_event( this );
    m_notify_type = DELTA;
}

void
sc_event::notify_delayed( const sc_time& t )
{
    sc_warn_notify_delayed();
    if( m_notify_type != NONE ) {
        SC_REPORT_ERROR( SC_ID_NOTIFY_DELAYED_, 0 );
    }
    if( t == SC_ZERO_TIME ) {
        // add this event to the delta events set
        m_delta_event_index = m_simc->add_delta_event( this );
        m_notify_type = DELTA;
    } else {
        // add this event to the timed events set
        sc_event_timed* et = new sc_event_timed( this,
                                                 m_simc->time_stamp() + t );
        m_simc->add_timed_event( et );
        m_timed = et;
        m_notify_type = TIMED;
    }
}

void
sc_event::reset()
{
    m_notify_type = NONE;
    m_delta_event_index = -1;
    m_timed = 0;
    // clear the dynamic sensitive methods
    m_methods_dynamic.resize(0);
    // clear the dynamic sensitive threads
    m_threads_dynamic.resize(0);
}


void
sc_event::trigger()
{
    int size;

    // trigger the static sensitive methods

    if( ( size = m_methods_static.size() ) != 0 ) {
        sc_method_handle* l_methods_static = &m_methods_static[0];
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
        sc_method_handle* l_methods_dynamic = &m_methods_dynamic[0];
        int i = size - 1;
        do {
            sc_method_handle method_h = l_methods_dynamic[i];
            if( method_h->trigger_dynamic( this ) ) {
                m_simc->push_runnable_method( method_h );
            }
        } while( -- i >= 0 );
        m_methods_dynamic.resize(0);
    }

    // trigger the static sensitive threads

    if( ( size = m_threads_static.size() ) != 0 ) {
        sc_thread_handle* l_threads_static = &m_threads_static[0];
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
        sc_thread_handle* l_threads_dynamic = &m_threads_dynamic[0];
        int i = size - 1;
        do {
            sc_thread_handle thread_h = l_threads_dynamic[i];
            if( thread_h->trigger_dynamic( this ) ) {
                m_simc->push_runnable_thread( thread_h );
            }
        } while( -- i >= 0 );
        m_threads_dynamic.resize(0);
    }

    m_notify_type = NONE;
    m_delta_event_index = -1;
    m_timed = 0;
}


bool
sc_event::remove_static( sc_method_handle method_h_ ) const
{
    int size;
    if ( ( size = m_methods_static.size() ) != 0 ) {
      sc_method_handle* l_methods_static = &m_methods_static[0];
      for( int i = size - 1; i >= 0; -- i ) {
          if( l_methods_static[i] == method_h_ ) {
              l_methods_static[i] = l_methods_static[size - 1];
              m_methods_static.resize(size-1);
              return true;
          }
      }
    }
    return false;
}

bool
sc_event::remove_static( sc_thread_handle thread_h_ ) const
{
    int size;
    if ( ( size = m_threads_static.size() ) != 0 ) {
      sc_thread_handle* l_threads_static = &m_threads_static[0];
      for( int i = size - 1; i >= 0; -- i ) {
          if( l_threads_static[i] == thread_h_ ) {
              l_threads_static[i] = l_threads_static[size - 1];
              m_threads_static.resize(size-1);
              return true;
          }
      }
    }
    return false;
}

bool
sc_event::remove_dynamic( sc_method_handle method_h_ ) const
{
    int size;
    if ( ( size = m_methods_dynamic.size() ) != 0 ) {
      sc_method_handle* l_methods_dynamic = &m_methods_dynamic[0];
      for( int i = size - 1; i >= 0; -- i ) {
          if( l_methods_dynamic[i] == method_h_ ) {
              l_methods_dynamic[i] = l_methods_dynamic[size - 1];
              m_methods_dynamic.resize(size-1);
              return true;
          }
      }
    }
    return false;
}

bool
sc_event::remove_dynamic( sc_thread_handle thread_h_ ) const
{
    int size;
    if ( ( size= m_threads_dynamic.size() ) != 0 ) {
      sc_thread_handle* l_threads_dynamic = &m_threads_dynamic[0];
      for( int i = size - 1; i >= 0; -- i ) {
          if( l_threads_dynamic[i] == thread_h_ ) {
              l_threads_dynamic[i] = l_threads_dynamic[size - 1];
              m_threads_dynamic.resize(size-1);
              return true;
          }
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
    if ( m_events.size() != 0 ) {
      const sc_event** l_events = &m_events[0];
      for( int i = m_events.size() - 1; i >= 0; -- i ) {
          if( &e == l_events[i] ) {
              // event already in the list; ignore
              return;
          }
      }
    }
    m_events.push_back( &e );
}


void
sc_event_list::add_dynamic( sc_method_handle method_h )
{
    if ( m_events.size() != 0 ) {
      const sc_event** l_events = &m_events[0];
      for( int i = m_events.size() - 1; i >= 0; -- i ) {
          l_events[i]->add_dynamic( method_h );
      }
  }
}

void
sc_event_list::add_dynamic( sc_thread_handle thread_h )
{
    if ( m_events.size() != 0 ) {
      const sc_event** l_events = &m_events[0];
      for( int i = m_events.size() - 1; i >= 0; -- i ) {
          l_events[i]->add_dynamic( thread_h );
      }
  }
}

void
sc_event_list::remove_dynamic( sc_method_handle method_h,
                               const sc_event* e_not )
{
    if ( m_events.size() != 0 ) {
      const sc_event** l_events = &m_events[0];
      for( int i = m_events.size() - 1; i >= 0; -- i ) {
          const sc_event* e = l_events[i];
          if( e != e_not ) {
              e->remove_dynamic( method_h );
          }
      }
  }
}

void
sc_event_list::remove_dynamic( sc_thread_handle thread_h,
                               const sc_event* e_not )
{
    if ( m_events.size() != 0 ) {
      const sc_event** l_events = &m_events[0];
      for( int i = m_events.size() - 1; i >= 0; -- i ) {
          const sc_event* e = l_events[i];
          if( e != e_not ) {
              e->remove_dynamic( thread_h );
          }
      }
  }
}

// ----------------------------------------------------------------------------
//  Deprecated functional notation for notifying events.
// ----------------------------------------------------------------------------

static void sc_warn_notify()
{
    static bool warn_notify=true;
    if ( warn_notify )
    {
  SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
      "the notify() function is deprecated use sc_event::notify()" );
  warn_notify = false;
    }
}

void
notify( sc_event& e )
{
    sc_warn_notify();
    e.notify();
}

void
notify( const sc_time& t, sc_event& e )
{
    sc_warn_notify();
    e.notify( t );
}

void
notify( double v, sc_time_unit tu, sc_event& e )
{
    sc_warn_notify();
    e.notify( v, tu );
}

} // namespace sc_core

// Taf!
