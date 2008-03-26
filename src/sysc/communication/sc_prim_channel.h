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

#ifndef SC_PRIM_CHANNEL_H
#define SC_PRIM_CHANNEL_H

#include "sysc/kernel/sc_object.h"
#include "sysc/kernel/sc_wait.h"
#include "sysc/utils/sc_vector.h"

namespace sc_core {

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
    virtual const char* kind() const
        { return "sc_prim_channel"; }

    inline bool update_requested() 
	{ return m_update_requested; }

    // request the update method (to be executed during the update phase)
    void request_update();


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

    void wait( sc_event_or_list& el )
	{ sc_core::wait( el, simcontext() ); }

    void wait( sc_event_and_list& el )
	{ sc_core::wait( el, simcontext() ); }

    void wait( const sc_time& t )
        { sc_core::wait( t, simcontext() ); }

    void wait( double v, sc_time_unit tu )
        { sc_core::wait( sc_time( v, tu, simcontext() ), simcontext() ); }

    void wait( const sc_time& t, const sc_event& e )
        { sc_core::wait( t, e, simcontext() ); }

    void wait( double v, sc_time_unit tu, const sc_event& e )
        { sc_core::wait( sc_time( v, tu, simcontext() ), e, simcontext() ); }

    void wait( const sc_time& t, sc_event_or_list& el )
        { sc_core::wait( t, el, simcontext() ); }

    void wait( double v, sc_time_unit tu, sc_event_or_list& el )
        { sc_core::wait( sc_time( v, tu, simcontext() ), el, simcontext() ); }

    void wait( const sc_time& t, sc_event_and_list& el )
        { sc_core::wait( t, el, simcontext() ); }

    void wait( double v, sc_time_unit tu, sc_event_and_list& el )
        { sc_core::wait( sc_time( v, tu, simcontext() ), el, simcontext() ); }


    // static sensitivity for SC_METHODs

    void next_trigger()
	{ sc_core::next_trigger( simcontext() ); }


    // dynamic sensitivity for SC_METHODs

    void next_trigger( const sc_event& e )
        { sc_core::next_trigger( e, simcontext() ); }

    void next_trigger( sc_event_or_list& el )
        { sc_core::next_trigger( el, simcontext() ); }

    void next_trigger( sc_event_and_list& el )
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

    void next_trigger( const sc_time& t, sc_event_or_list& el )
        { sc_core::next_trigger( t, el, simcontext() ); }

    void next_trigger( double v, sc_time_unit tu, sc_event_or_list& el )
        { sc_core::next_trigger( 
	    sc_time( v, tu, simcontext() ), el, simcontext() ); }

    void next_trigger( const sc_time& t, sc_event_and_list& el )
        { sc_core::next_trigger( t, el, simcontext() ); }

    void next_trigger( double v, sc_time_unit tu, sc_event_and_list& el )
        { sc_core::next_trigger( 
	    sc_time( v, tu, simcontext() ), el, simcontext() ); }


    // for SC_METHODs and SC_THREADs and SC_CTHREADs

    bool timed_out()
	{ return sc_core::timed_out( simcontext() ); }

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

    sc_prim_channel_registry* m_registry;

    bool m_update_requested;
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

    void request_update( sc_prim_channel& );

private:

    // constructor
    explicit sc_prim_channel_registry( sc_simcontext& simc_ );

    // destructor
    ~sc_prim_channel_registry();

    // called during the update phase of a delta cycle
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
    sc_prim_channel_registry();
    sc_prim_channel_registry( const sc_prim_channel_registry& );
    sc_prim_channel_registry& operator = ( const sc_prim_channel_registry& );

private:

    sc_simcontext*               m_simc;
    sc_pvector<sc_prim_channel*> m_prim_channel_vec;

    sc_prim_channel** m_update_array;
    int               m_update_size;
    int               m_update_last;
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

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
    if( ! m_update_requested ) {
	m_registry->request_update( *this );
	m_update_requested = true;
    }
}


// called during the update phase of a delta cycle (if requested)

inline
void
sc_prim_channel::perform_update()
{
    update();
    m_update_requested = false;
}


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
    m_update_array[++ m_update_last] = &prim_channel_;
}


// called during the update phase of a delta cycle

inline
void
sc_prim_channel_registry::perform_update()
{
    for( int i = m_update_last; i >= 0; -- i ) {
	m_update_array[i]->perform_update();
    }
    m_update_last = -1;
}

} // namespace sc_core

#endif

// Taf!
