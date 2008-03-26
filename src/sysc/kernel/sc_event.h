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

  sc_event.h --

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_event.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.6  2006/04/11 23:13:20  acg
//   Andy Goodrich: Changes for reduced reset support that only includes
//   sc_cthread, but has preliminary hooks for expanding to method and thread
//   processes also.
//
// Revision 1.5  2006/01/24 20:56:00  acg
//  Andy Goodrich: fixed up CVS comment.
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

#ifndef SC_EVENT_H
#define SC_EVENT_H


#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/datatypes/bit/sc_logic.h"

namespace sc_core {

// forward declarations
class sc_event_timed;
class sc_event_list;
class sc_event_or_list;
class sc_event_and_list;

// friend function declarations
    int sc_notify_time_compare( const void*, const void* );



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
	template<typename IF> friend class sc_signal;
	friend class sc_signal<bool>;
	friend class sc_signal<sc_dt::sc_logic>;

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

    void add_static( sc_method_handle ) const;
    void add_static( sc_thread_handle ) const;
    void add_dynamic( sc_method_handle ) const;
    void add_dynamic( sc_thread_handle ) const;

    void notify_internal( const sc_time& );
    void notify_next_delta();

    bool remove_static( sc_method_handle ) const;
    bool remove_static( sc_thread_handle ) const;
    bool remove_dynamic( sc_method_handle ) const;
    bool remove_dynamic( sc_thread_handle ) const;

    void reset();

    void trigger();

private:

    enum notify_t { NONE, DELTA, TIMED };

    sc_simcontext*  m_simc;
    notify_t        m_notify_type;
    int             m_delta_event_index;
    sc_event_timed* m_timed;

    mutable std::vector<sc_method_handle> m_methods_static;
    mutable std::vector<sc_method_handle> m_methods_dynamic;
    mutable std::vector<sc_thread_handle> m_threads_static;
    mutable std::vector<sc_thread_handle> m_threads_dynamic;

private:

    // disabled
    sc_event( const sc_event& );
    sc_event& operator = ( const sc_event& );
};

extern sc_event sc_non_event; // Event that never happens.

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

    static void* operator new( std::size_t )
        { return allocate(); }

    static void operator delete( void* p, std::size_t )
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
  m_delta_event_index( -1 ),
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
sc_event::notify_internal( const sc_time& t )
{
    if( t == SC_ZERO_TIME ) {
        // add this event to the delta events set
        m_delta_event_index = m_simc->add_delta_event( this );
        m_notify_type = DELTA;
    } else {
        sc_event_timed* et =
		new sc_event_timed( this, m_simc->time_stamp() + t );
        m_simc->add_timed_event( et );
        m_timed = et;
        m_notify_type = TIMED;
    }
}

inline
void
sc_event::notify_next_delta()
{
    if( m_notify_type != NONE ) {
        SC_REPORT_ERROR( SC_ID_NOTIFY_DELAYED_, 0 );
    }
    // add this event to the delta events set
    m_delta_event_index = m_simc->add_delta_event( this );
    m_notify_type = DELTA;
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
//  Deprecated functional notation for notifying events.
// ----------------------------------------------------------------------------

extern void notify( sc_event& e );
extern void notify( const sc_time& t, sc_event& e );
extern void notify( double v, sc_time_unit tu, sc_event& e );


// ----------------------------------------------------------------------------
//  CLASS : sc_event_list
//
//  Base class for lists of events.
// ----------------------------------------------------------------------------

class sc_event_list
{
    friend class sc_process_b;
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

    std::vector<const sc_event*> m_events;
    bool                         m_and_list;
    bool                         m_auto_delete;

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
	friend class sc_process_b;
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
	friend class sc_process_b;
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
