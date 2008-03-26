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

  sc_event.h --

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_EVENT_H
#define SC_EVENT_H


#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/utils/sc_vector.h"

namespace sc_core {

// forward declarations
class sc_event_timed;
class sc_event_list;
class sc_event_or_list;
class sc_event_and_list;


// ----------------------------------------------------------------------------
//  CLASS : sc_event
//
//  The event class.
// ----------------------------------------------------------------------------

class sc_event
{
	friend class sc_clock;
    friend class sc_event_list;
    friend class sc_event_timed;
    friend class sc_simcontext;
    friend class sc_process_b;
    friend class sc_method_process;
    friend class sc_thread_process;

public:

    sc_event();
    ~sc_event();

    void cancel();

    void notify();
    void notify( const sc_time& );
    void notify( double, sc_time_unit );

    void notify_delayed();
    void notify_delayed( const sc_time& );
    void notify_delayed( double, sc_time_unit );

    sc_event_or_list&  operator | ( const sc_event& ) const;
    sc_event_and_list& operator & ( const sc_event& ) const;

private:

    void reset();

    void trigger();

    void add_static( sc_method_handle ) const;
    void add_static( sc_thread_handle ) const;
    void add_dynamic( sc_method_handle ) const;
    void add_dynamic( sc_thread_handle ) const;

    bool remove_static( sc_method_handle ) const;
    bool remove_static( sc_thread_handle ) const;
    bool remove_dynamic( sc_method_handle ) const;
    bool remove_dynamic( sc_thread_handle ) const;

private:

    enum notify_t { NONE, DELTA, TIMED };

    sc_simcontext*  m_simc;
    notify_t        m_notify_type;
    int             m_delta;
    sc_event_timed* m_timed;

    mutable sc_pvector<sc_method_handle> m_methods_static;
    mutable sc_pvector<sc_method_handle> m_methods_dynamic;
    mutable sc_pvector<sc_thread_handle> m_threads_static;
    mutable sc_pvector<sc_thread_handle> m_threads_dynamic;

private:

    // disabled
    sc_event( const sc_event& );
    sc_event& operator = ( const sc_event& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_event_timed
//
//  Class for storing the time to notify a timed event.
// ----------------------------------------------------------------------------

class sc_event_timed
{
    friend class sc_event;
    friend class sc_simcontext;

    friend int sc_notify_time_compare( const void*, const void* );

private:

    sc_event_timed( sc_event* e, const sc_time& t )
        : m_event( e ), m_notify_time( t )
        {}

    ~sc_event_timed()
        { if( m_event != 0 ) { m_event->m_timed = 0; } }

    sc_event* event() const
        { return m_event; }

    const sc_time& notify_time() const
        { return m_notify_time; }

    static void* operator new( size_t )
        { return allocate(); }

    static void operator delete( void* p, size_t )
        { deallocate( p ); }

private:

    // dedicated memory management
    static void* allocate();
    static void  deallocate( void* );

private:

    sc_event* m_event;
    sc_time   m_notify_time;

private:

    // disabled
    sc_event_timed();
    sc_event_timed( const sc_event_timed& );
    sc_event_timed& operator = ( const sc_event_timed& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
sc_event::sc_event()
: m_simc( sc_get_curr_simcontext() ),
  m_notify_type( NONE ),
  m_delta( -1 ),
  m_timed( 0 )
{}

inline
sc_event::~sc_event()
{
    cancel();
}


inline
void
sc_event::notify( double v, sc_time_unit tu )
{
    notify( sc_time( v, tu, m_simc ) );
}


inline
void
sc_event::notify_delayed()
{
    if( m_notify_type != NONE ) {
        SC_REPORT_ERROR( SC_ID_NOTIFY_DELAYED_, 0 );
    }
    // add this event to the delta events set
    m_delta = m_simc->add_delta_event( this );
    m_notify_type = DELTA;
}

inline
void
sc_event::notify_delayed( const sc_time& t )
{
    if( m_notify_type != NONE ) {
        SC_REPORT_ERROR( SC_ID_NOTIFY_DELAYED_, 0 );
    }
    if( t == SC_ZERO_TIME ) {
        // add this event to the delta events set
        m_delta = m_simc->add_delta_event( this );
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

inline
void
sc_event::notify_delayed( double v, sc_time_unit tu )
{
    notify_delayed( sc_time( v, tu, m_simc ) );
}


inline
void
sc_event::add_static( sc_method_handle method_h ) const
{
    m_methods_static.push_back( method_h );
}

inline
void
sc_event::add_static( sc_thread_handle thread_h ) const
{
    m_threads_static.push_back( thread_h );
}

inline
void
sc_event::add_dynamic( sc_method_handle method_h ) const
{
    m_methods_dynamic.push_back( method_h );
}

inline
void
sc_event::add_dynamic( sc_thread_handle thread_h ) const
{
    m_threads_dynamic.push_back( thread_h );
}


// ----------------------------------------------------------------------------
//  Functional notation for notifying events.
// ----------------------------------------------------------------------------

inline
void
notify( sc_event& e )
{
    e.notify();
}

inline
void
notify( const sc_time& t, sc_event& e )
{
    e.notify( t );
}

inline
void
notify( double v, sc_time_unit tu, sc_event& e )
{
    e.notify( v, tu );
}


// ----------------------------------------------------------------------------
//  CLASS : sc_event_list
//
//  Base class for lists of events.
// ----------------------------------------------------------------------------

class sc_event_list
{
    friend class sc_method_process;
    friend class sc_thread_process;

protected:

    void push_back( const sc_event& );

    sc_event_list( const sc_event&,
                   bool and_list_,
                   bool auto_delete_ = false );

    int size() const;
    bool and_list() const;

    void add_dynamic( sc_method_handle );
    void add_dynamic( sc_thread_handle );
    void remove_dynamic( sc_method_handle, const sc_event* );
    void remove_dynamic( sc_thread_handle, const sc_event* );

    void auto_delete();

private:

    sc_pvector<const sc_event*> m_events;
    bool                        m_and_list;
    bool                        m_auto_delete;

private:

    // disabled
    sc_event_list();
    sc_event_list( const sc_event_list& );
    sc_event_list& operator = ( const sc_event_list& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
sc_event_list::sc_event_list( const sc_event& e,
                              bool and_list_,
                              bool auto_delete_ )
: m_and_list( and_list_ ),
  m_auto_delete( auto_delete_ )
{
    m_events.push_back( &e );
}


inline
int
sc_event_list::size() const
{
    return m_events.size();
}

inline
bool
sc_event_list::and_list() const
{
    return m_and_list;
}


inline
void
sc_event_list::auto_delete()
{
    if( m_auto_delete ) {
        delete this;
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_event_or_list
//
//  OR list of events.
// ----------------------------------------------------------------------------

class sc_event_or_list
: public sc_event_list
{
    friend class sc_event;
    friend class sc_method_process;
    friend class sc_thread_process;

protected:

    sc_event_or_list( const sc_event&, bool auto_delete_ = false );

public:

    sc_event_or_list& operator | ( const sc_event& );

private:

    // disabled
    sc_event_or_list();
    sc_event_or_list( const sc_event_or_list& );
    sc_event_or_list& operator = ( const sc_event_or_list& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
sc_event_or_list::sc_event_or_list( const sc_event& e,
                                    bool auto_delete_ )
: sc_event_list( e, false, auto_delete_ )
{}


inline
sc_event_or_list&
sc_event_or_list::operator | ( const sc_event& e )
{
    push_back( e );
    return *this;
}


// sc_event

inline
sc_event_or_list&
sc_event::operator | ( const sc_event& e2 ) const
{
    sc_event_or_list* el = new sc_event_or_list( *this, true );
    el->push_back( e2 );
    return *el;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_event_and_list
//
//  AND list of events.
// ----------------------------------------------------------------------------

class sc_event_and_list
: public sc_event_list
{
    friend class sc_event;
    friend class sc_method_process;
    friend class sc_thread_process;

protected:

    sc_event_and_list( const sc_event&, bool auto_delete_ = false );

public:

    sc_event_and_list& operator & ( const sc_event& );

private:

    // disabled
    sc_event_and_list();
    sc_event_and_list( const sc_event_and_list& );
    sc_event_and_list& operator = ( const sc_event_and_list& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
sc_event_and_list::sc_event_and_list( const sc_event& e,
                                      bool auto_delete_ )
: sc_event_list( e, true, auto_delete_ )
{}


inline
sc_event_and_list&
sc_event_and_list::operator & ( const sc_event& e )
{
    push_back( e );
    return *this;
}


// sc_event

inline
sc_event_and_list&
sc_event::operator & ( const sc_event& e2 ) const
{
    sc_event_and_list* el = new sc_event_and_list( *this, true );
    el->push_back( e2 );
    return *el;
}

} // namespace sc_core

#endif

// Taf!
