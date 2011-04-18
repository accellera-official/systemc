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

  sc_prim_channel.h -- Abstract base class of all primitive channel classes.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Andy Goodrich, Forte,
                               Bishnupriya Bhattacharya, Cadence Design Systems,
                               25 August, 2003
  Description of Modification: phase callbacks
    
 *****************************************************************************/
//$Log: sc_prim_channel.h,v $
//Revision 1.4  2011/04/05 20:48:09  acg
// Andy Goodrich: changes to make sure that event(), posedge() and negedge()
// only return true if the clock has not moved.
//
//Revision 1.3  2011/02/18 20:23:45  acg
// Andy Goodrich: Copyright update.
//
//Revision 1.2  2011/01/20 16:52:15  acg
// Andy Goodrich: changes for IEEE 1666 2011.
//
//Revision 1.1.1.1  2006/12/15 20:20:04  acg
//SystemC 2.3
//
//Revision 1.3  2006/05/08 17:52:47  acg
// Andy Goodrich:
//   (1) added David Long's forward declarations for friend functions,
//       methods, and operators to keep the Microsoft compiler happy.
//   (2) Added delta_count() method to sc_prim_channel for use by
//       sc_signal so that the friend declaration in sc_simcontext.h
//	   can be for a non-templated class (i.e., sc_prim_channel.)
//
//Revision 1.2  2006/01/03 23:18:26  acg
//Changed copyright to include 2006.
//
//Revision 1.1.1.1  2005/12/19 23:16:43  acg
//First check in of SystemC 2.1 into its own archive.
//
//Revision 1.10  2005/07/30 03:44:11  acg
//Changes from 2.1.
//
//Revision 1.9  2005/06/10 22:43:55  acg
//Added CVS change log annotation.
//

#ifndef SC_PRIM_CHANNEL_H
#define SC_PRIM_CHANNEL_H

#include "sysc/kernel/sc_object.h"
#include "sysc/kernel/sc_wait.h"
#include "sysc/kernel/sc_wait_cthread.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_scoped_mutex
//
//  Scoped mutex for use creating a thread-safe environment
// ----------------------------------------------------------------------------
#define SC_ASYNC_UPDATE // @@@@####
#if defined(SC_ASYNC_UPDATE)

#   if defined(WIN32) // windows implementation **** to be implemented ****
#       define SC_SCOPED_MUTEX_INIT(MUTEX) 
        struct sc_scoped_mutex 
        {
	    typedef int mutex_t; // mutex object.
    
	    sc_scoped_mutex(mutex_t& mutex) 
            {
            } 
	    ~sc_scoped_mutex() 
            {
            } 
        };
#   else
#       include <pthread.h>
#       if defined(__hpux)
#           define SC_SCOPED_MUTEX_INIT(MUTEX) \
	        pthread_mutex_init( &MUTEX, cma_c_null );
#       else  // !defined(__hpux)
#           define SC_SCOPED_MUTEX_INIT(MUTEX) \
	        pthread_mutex_init( &MUTEX, NULL );
#       endif // !defined(__hpux)

        class sc_scoped_mutex 
        {
          public:
	    typedef pthread_mutex_t mutex_t; // mutex object.
    
	    sc_scoped_mutex(mutex_t& mutex) : m_mutex(mutex)
	    { 
	        pthread_mutex_lock( &m_mutex ); 
	    }
	    ~sc_scoped_mutex(void) 
	    { 
	        pthread_mutex_unlock( &m_mutex ); 
	    }
          private:
	    mutex_t& m_mutex;
        };

#   endif

#else // no implementation:
#   define SC_SCOPED_MUTEX_INIT(MUTEX) 
    struct sc_scoped_mutex 
    {
	typedef int mutex_t; // mutex object.

	sc_scoped_mutex(mutex_t& mutex) 
        {
	    SC_REPORT_ERROR( SC_ID_NO_ASYNC_UPDATE_, "" );
        } 
	~sc_scoped_mutex() 
        {
        } 
    };
#endif 

// ----------------------------------------------------------------------------
//  CLASS : sc_prim_channel
//
//  Abstract base class of all primitive channel classes.
// ----------------------------------------------------------------------------

class sc_prim_channel
: public sc_object
{
    friend class sc_prim_channel_registry;

public:
    enum { list_end = 0xdb };
public:
    virtual const char* kind() const
        { return "sc_prim_channel"; }

    inline bool update_requested() 
	{ return m_update_next_p != (sc_prim_channel*)list_end; }

    // request the update method to be executed during the update phase
    inline void request_update();

    // request the update method to be executed during the update phase
    // from a process external to the simulator.
    inline void async_request_update();

protected:

    // constructors
    sc_prim_channel();
    explicit sc_prim_channel( const char* );

    // destructor
    virtual ~sc_prim_channel();

    // the update method (does nothing by default)
    virtual void update();

    // called by construction_done (does nothing by default)
    virtual void before_end_of_elaboration();

    // called by elaboration_done (does nothing by default)
    virtual void end_of_elaboration();

    // called by start_simulation (does nothing by default)
    virtual void start_of_simulation();

    // called by simulation_done (does nothing by default)
    virtual void end_of_simulation();

protected:

    // to avoid calling sc_get_curr_simcontext()

    // static sensitivity for SC_THREADs and SC_CTHREADs

    void wait()
        { sc_core::wait( simcontext() ); }


    // dynamic sensitivity for SC_THREADs and SC_CTHREADs

    void wait( const sc_event& e )
        { sc_core::wait( e, simcontext() ); }

    void wait( const sc_event_or_list& el )
	{ sc_core::wait( el, simcontext() ); }

    void wait( const sc_event_and_list& el )
	{ sc_core::wait( el, simcontext() ); }

    void wait( const sc_time& t )
        { sc_core::wait( t, simcontext() ); }

    void wait( double v, sc_time_unit tu )
        { sc_core::wait( sc_time( v, tu, simcontext() ), simcontext() ); }

    void wait( const sc_time& t, const sc_event& e )
        { sc_core::wait( t, e, simcontext() ); }

    void wait( double v, sc_time_unit tu, const sc_event& e )
        { sc_core::wait( sc_time( v, tu, simcontext() ), e, simcontext() ); }

    void wait( const sc_time& t, const sc_event_or_list& el )
        { sc_core::wait( t, el, simcontext() ); }

    void wait( double v, sc_time_unit tu, const sc_event_or_list& el )
        { sc_core::wait( sc_time( v, tu, simcontext() ), el, simcontext() ); }

    void wait( const sc_time& t, const sc_event_and_list& el )
        { sc_core::wait( t, el, simcontext() ); }

    void wait( double v, sc_time_unit tu, const sc_event_and_list& el )
        { sc_core::wait( sc_time( v, tu, simcontext() ), el, simcontext() ); }

    void wait( int n )
        { sc_core::wait( n, simcontext() ); }


    // static sensitivity for SC_METHODs

    void next_trigger()
	{ sc_core::next_trigger( simcontext() ); }


    // dynamic sensitivity for SC_METHODs

    void next_trigger( const sc_event& e )
        { sc_core::next_trigger( e, simcontext() ); }

    void next_trigger( const sc_event_or_list& el )
        { sc_core::next_trigger( el, simcontext() ); }

    void next_trigger( const sc_event_and_list& el )
        { sc_core::next_trigger( el, simcontext() ); }

    void next_trigger( const sc_time& t )
        { sc_core::next_trigger( t, simcontext() ); }

    void next_trigger( double v, sc_time_unit tu )
        {sc_core::next_trigger( sc_time( v, tu, simcontext() ), simcontext() );}

    void next_trigger( const sc_time& t, const sc_event& e )
        { sc_core::next_trigger( t, e, simcontext() ); }

    void next_trigger( double v, sc_time_unit tu, const sc_event& e )
        { sc_core::next_trigger( 
	    sc_time( v, tu, simcontext() ), e, simcontext() ); }

    void next_trigger( const sc_time& t, const sc_event_or_list& el )
        { sc_core::next_trigger( t, el, simcontext() ); }

    void next_trigger( double v, sc_time_unit tu, const sc_event_or_list& el )
        { sc_core::next_trigger( 
	    sc_time( v, tu, simcontext() ), el, simcontext() ); }

    void next_trigger( const sc_time& t, const sc_event_and_list& el )
        { sc_core::next_trigger( t, el, simcontext() ); }

    void next_trigger( double v, sc_time_unit tu, const sc_event_and_list& el )
        { sc_core::next_trigger( 
	    sc_time( v, tu, simcontext() ), el, simcontext() ); }


    // for SC_METHODs and SC_THREADs and SC_CTHREADs

    bool timed_out()
	{ return sc_core::timed_out( simcontext() ); }


#if 0 // @@@@####
    // delta count maintenance
    sc_dt::uint64 delta_count()
	{ return simcontext()->m_delta_count; }
#endif

private:

    // called during the update phase of a delta cycle (if requested)
    void perform_update();

    // called when construction is done
    void construction_done();

    // called when elaboration is done
    void elaboration_done();

    // called before simulation starts
    void start_simulation();

    // called after simulation ends
    void simulation_done();

    // disabled
    sc_prim_channel( const sc_prim_channel& );
    sc_prim_channel& operator = ( const sc_prim_channel& );

private:

    sc_prim_channel_registry* m_registry;      // Update list manager.
    sc_prim_channel*          m_update_next_p; // Next entry in update list.
};


// ----------------------------------------------------------------------------
//  CLASS : sc_prim_channel_registry
//
//  Registry for all primitive channels.
//  FOR INTERNAL USE ONLY!
// ----------------------------------------------------------------------------

class sc_prim_channel_registry
{
    friend class sc_simcontext;

public:

    void insert( sc_prim_channel& );
    void remove( sc_prim_channel& );


    int size() const
        { return m_prim_channel_vec.size(); }

    inline void request_update( sc_prim_channel& );
    inline void async_request_update( sc_prim_channel& );

    bool pending_updates() const
    { 
        return m_update_list_p != (sc_prim_channel*)sc_prim_channel::list_end ||
               m_async_update_list_p != 
               (sc_prim_channel*)sc_prim_channel::list_end;
    }   

    bool pending_async_updates() const
    { 
        return m_async_update_list_p != 
               (sc_prim_channel*)sc_prim_channel::list_end; 
    }   

private:

    // constructor
    explicit sc_prim_channel_registry( sc_simcontext& simc_ );

    // destructor
    ~sc_prim_channel_registry();

    // called during the update phase of a delta cycle
    inline void perform_update();

    // called when construction is done
    void construction_done();

    // called when elaboration is done
    void elaboration_done();

    // called before simulation starts
    void start_simulation();

    // called after simulation ends
    void simulation_done();

    // disabled
    sc_prim_channel_registry();
    sc_prim_channel_registry( const sc_prim_channel_registry& );
    sc_prim_channel_registry& operator = ( const sc_prim_channel_registry& );

private:

    sc_simcontext*                m_simc;
    std::vector<sc_prim_channel*> m_prim_channel_vec;

    
    sc_prim_channel*              m_async_update_list_p; // external updates.
    sc_scoped_mutex::mutex_t      m_async_update_mutex;  // safety mutex.
    sc_prim_channel*              m_update_list_p;       // internal updates.
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS : sc_prim_channel_registry
//
//  Registry for all primitive channels.
//  FOR INTERNAL USE ONLY!
// ----------------------------------------------------------------------------

inline
void
sc_prim_channel_registry::request_update( sc_prim_channel& prim_channel_ )
{
    prim_channel_.m_update_next_p = m_update_list_p;
    m_update_list_p = &prim_channel_;
}

inline 
void 
sc_prim_channel_registry::async_request_update( sc_prim_channel& prim_channel_ )
{ 
    sc_scoped_mutex lock(m_async_update_mutex); 
    if ( !prim_channel_.m_update_next_p )
    {
	prim_channel_.m_update_next_p = m_async_update_list_p; 
	m_async_update_list_p = &prim_channel_; 
    }
    // return unlocks the mutex.
}

// called during the update phase of a delta cycle

// +----------------------------------------------------------------------------
// |"sc_prim_channel_registry::perform_update"
// | 
// | This method updates the values of the primitive channels in its update 
// | lists.
// +----------------------------------------------------------------------------
inline
void
sc_prim_channel_registry::perform_update()
{
    sc_prim_channel* next_p; // Next update to perform.
    sc_prim_channel* now_p;  // Update now performing.

    // Update the values for the primitive channels in the simulator's list.

    now_p = m_update_list_p;
    m_update_list_p = (sc_prim_channel*)sc_prim_channel::list_end;
    for ( ; now_p != (sc_prim_channel*)sc_prim_channel::list_end; 
    	now_p = next_p )
    {
        next_p = now_p->m_update_next_p;
	now_p->perform_update();
    }

    // Update the values for the primitive channels set external to the
    // simulator.

    now_p = m_async_update_list_p;
    if ( now_p != (sc_prim_channel*)sc_prim_channel::list_end )
    {
        sc_scoped_mutex lock(m_async_update_mutex);
	m_async_update_list_p = (sc_prim_channel*)sc_prim_channel::list_end;
	for ( ; now_p != (sc_prim_channel*)sc_prim_channel::list_end; 
	    now_p = next_p )
	{
	    next_p = now_p->m_update_next_p;
	    now_p->perform_update();
	}
	// exiting the if releases the mutex.
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_prim_channel
//
//  Abstract base class of all primitive channel classes.
// ----------------------------------------------------------------------------

// request the update method (to be executed during the update phase)

inline
void
sc_prim_channel::request_update()
{
    if( ! m_update_next_p ) {
	m_registry->request_update( *this );
    }
}

// request the update method from external to the simulator (to be executed 
// during the update phase)

inline
void
sc_prim_channel::async_request_update()
{
    if( ! m_update_next_p ) {
	m_registry->async_request_update( *this );
    }
}


// called during the update phase of a delta cycle (if requested)

inline
void
sc_prim_channel::perform_update()
{
    update();
    m_update_next_p = 0;
}


} // namespace sc_core

#endif

// Taf!
