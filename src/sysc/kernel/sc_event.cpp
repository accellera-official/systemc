/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2011 by all Contributors.
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
// Revision 1.9  2011/02/17 19:49:51  acg
//  Andy Goodrich:
//    (1) Changed signature of trigger_dynamic() to return a bool again.
//    (2) Moved process run queue processing into trigger_dynamic().
//
// Revision 1.8  2011/02/16 22:37:30  acg
//  Andy Goodrich: clean up to remove need for ps_disable_pending.
//
// Revision 1.7  2011/02/13 21:47:37  acg
//  Andy Goodrich: update copyright notice.
//
// Revision 1.6  2011/02/01 21:02:28  acg
//  Andy Goodrich: new return code for trigger_dynamic() calls.
//
// Revision 1.5  2011/01/18 20:10:44  acg
//  Andy Goodrich: changes for IEEE1666_2011 semantics.
//
// Revision 1.4  2011/01/06 18:04:05  acg
//  Andy Goodrich: added code to leave disabled processes on the dynamic
//  method and thread queues.
//
// Revision 1.3  2008/05/22 17:06:25  acg
//  Andy Goodrich: updated copyright notice to include 2008.
//
// Revision 1.2  2007/01/17 22:44:30  acg
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
#include "sysc/kernel/sc_process_handle.h"
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


// +----------------------------------------------------------------------------
// |"sc_event::trigger"
// | 
// | This method "triggers" this object instance. This consists of scheduling
// | for execution all the processes that are schedulable and waiting on this 
// | event.
// +----------------------------------------------------------------------------
void
sc_event::trigger()
{
    int       last_i; // index of last element in vector now accessing.
    int       size;   // size of vector now accessing.


    // trigger the static sensitive methods

    if( ( size = m_methods_static.size() ) != 0 ) 
    {
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


    if( ( size = m_methods_dynamic.size() ) != 0 ) 
    {
	last_i = size - 1;
	sc_method_handle* l_methods_dynamic = &m_methods_dynamic[0];
	for ( int i = 0; i <= last_i; i++ )
	{
	    sc_method_handle method_h = l_methods_dynamic[i];
	    if ( method_h->trigger_dynamic( this ) )
	    {
		l_methods_dynamic[i] = l_methods_dynamic[last_i];
		last_i--;
		i--;
	    }
	}
        m_methods_dynamic.resize(last_i+1);
    }


    // trigger the static sensitive threads

    if( ( size = m_threads_static.size() ) != 0 ) 
    {
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

    if( ( size = m_threads_dynamic.size() ) != 0 ) 
    {
	last_i = size - 1;
	sc_thread_handle* l_threads_dynamic = &m_threads_dynamic[0];
	for ( int i = 0; i <= last_i; i++ )
	{
	    sc_thread_handle thread_h = l_threads_dynamic[i];
	    if ( thread_h->trigger_dynamic( this ) )
	    {
		l_threads_dynamic[i] = l_threads_dynamic[last_i];
		i--;
		last_i--;
	    }
	}
        m_threads_dynamic.resize(last_i+1);
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
sc_event_list::push_back( const sc_event_list& el )
{
    m_events.reserve( size() + el.size() );
    for ( int i = el.m_events.size() - 1; i >= 0; --i )
    {
        push_back( *el.m_events[i] );
    }
    el.auto_delete();
}

void
sc_event_list::add_dynamic( sc_method_handle method_h ) const
{
    m_busy++;
    if ( m_events.size() != 0 ) {
      const sc_event* const * l_events = &m_events[0];
      for( int i = m_events.size() - 1; i >= 0; -- i ) {
          l_events[i]->add_dynamic( method_h );
      }
  }
}

void
sc_event_list::add_dynamic( sc_thread_handle thread_h ) const
{
    m_busy++;
    if ( m_events.size() != 0 ) {
      const sc_event* const* l_events = &m_events[0];
      for( int i = m_events.size() - 1; i >= 0; -- i ) {
          l_events[i]->add_dynamic( thread_h );
      }
  }
}

void
sc_event_list::remove_dynamic( sc_method_handle method_h,
                               const sc_event* e_not ) const
{
    if ( m_events.size() != 0 ) {
      const sc_event* const* l_events = &m_events[0];
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
                               const sc_event* e_not ) const
{
    if ( m_events.size() != 0 ) {
      const sc_event* const* l_events = &m_events[0];
      for( int i = m_events.size() - 1; i >= 0; -- i ) {
          const sc_event* e = l_events[i];
          if( e != e_not ) {
              e->remove_dynamic( thread_h );
          }
      }
  }
}

void
sc_event_list::report_premature_destruction() const
{
    // TDB: reliably detect premature destruction
    //
    // If an event list is used as a member of a module,
    // its lifetime may (correctly) end, although there
    // are processes currently waiting for it.
    //
    // Detecting (and ignoring) this corner-case is quite
    // difficult for similar reasons to the sc_is_running()
    // return value during the destruction of the module
    // hierarchy.
    //
    // Ignoring the lifetime checks for now, if no process
    // is currently running (which is only part of the story):

    if( sc_get_current_process_handle().valid() ) {
        // FIXME: improve error-handling
        sc_assert( false && "sc_event_list prematurely destroyed" );
    }

}

void
sc_event_list::report_invalid_modification() const
{
    // FIXME: improve error-handling
    sc_assert( false && "sc_event_list modfied while being waited on" );
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
