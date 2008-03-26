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

  sc_prim_channel.cpp -- Abstract base class of all primitive channel
                         classes.

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


#include "sysc/communication/sc_prim_channel.h"
#include "sysc/communication/sc_communication_ids.h"
#include "sysc/kernel/sc_simcontext.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_prim_channel
//
//  Abstract base class of all primitive channel classes.
// ----------------------------------------------------------------------------

// constructors

sc_prim_channel::sc_prim_channel()
: sc_object( 0 ),
  m_registry( simcontext()->get_prim_channel_registry() ),
  m_update_requested( false )
{
    m_registry->insert( *this );
}

sc_prim_channel::sc_prim_channel( const char* name_ )
: sc_object( name_ ),
  m_registry( simcontext()->get_prim_channel_registry() ),
  m_update_requested( false )
{
    m_registry->insert( *this );
}


// destructor

sc_prim_channel::~sc_prim_channel()
{
    m_registry->remove( *this );
}


// the update method (does nothing by default)

void
sc_prim_channel::update()
{}


// called by construction_done (does nothing by default)

void sc_prim_channel::before_end_of_elaboration() 
{}

// called when construction is done

void
sc_prim_channel::construction_done()
{
    before_end_of_elaboration();
}

// called by elaboration_done (does nothing by default)

void
sc_prim_channel::end_of_elaboration()
{}


// called when elaboration is done

void
sc_prim_channel::elaboration_done()
{
    end_of_elaboration();
}

// called by start_simulation (does nothing)

void
sc_prim_channel::start_of_simulation()
{}

// called before simulation begins

void
sc_prim_channel::start_simulation()
{
    start_of_simulation();
}

// called by simulation_done (does nothing)

void
sc_prim_channel::end_of_simulation()
{}

// called after simulation ends

void
sc_prim_channel::simulation_done()
{
    end_of_simulation();
}

// ----------------------------------------------------------------------------
//  CLASS : sc_prim_channel_registry
//
//  Registry for all primitive channels.
//  FOR INTERNAL USE ONLY!
// ----------------------------------------------------------------------------

void
sc_prim_channel_registry::insert( sc_prim_channel& prim_channel_ )
{
    if( m_simc->is_running() ) {
	SC_REPORT_ERROR( SC_ID_INSERT_PRIM_CHANNEL_, "simulation running" );
    }

#ifdef DEBUG_SYSTEMC
    // check if prim_channel_ is already inserted
    for( int i = 0; i < size(); ++ i ) {
	if( &prim_channel_ == m_prim_channel_vec[i] ) {
	    SC_REPORT_ERROR( SC_ID_INSERT_PRIM_CHANNEL_, "already inserted" );
	}
    }
#endif

    // insert
    m_prim_channel_vec.push_back( &prim_channel_ );

    // resize update array, if needed
    if( m_update_size < size() ) {
	m_update_size *= 2;
	sc_prim_channel** tmp = new sc_prim_channel*[m_update_size];
	assert( m_update_last < m_update_size / 2 );
	for( int i = m_update_last; i >= 0; -- i ) {
	    tmp[i] = m_update_array[i];
	}
	delete[] m_update_array;
	m_update_array = tmp;
    }
}

void
sc_prim_channel_registry::remove( sc_prim_channel& prim_channel_ )
{
    int i;
    for( i = 0; i < size(); ++ i ) {
	if( &prim_channel_ == m_prim_channel_vec[i] ) {
	    break;
	}
    }
    if( i == size() ) {
	SC_REPORT_ERROR( SC_ID_REMOVE_PRIM_CHANNEL_, 0 );
    }

    // remove
    m_prim_channel_vec[i] = m_prim_channel_vec[size() - 1];
    m_prim_channel_vec.decr_count();
}


// constructor

sc_prim_channel_registry::sc_prim_channel_registry( sc_simcontext& simc_ )
: m_simc( &simc_ )
{
    m_update_size = 16;
    m_update_array = new sc_prim_channel*[m_update_size];
    m_update_last = -1;
}


// destructor

sc_prim_channel_registry::~sc_prim_channel_registry()
{
    delete[] m_update_array;
}

// called when construction is done

void
sc_prim_channel_registry::construction_done()
{
    for( int i = 0; i < size(); ++ i ) {
	m_prim_channel_vec[i]->construction_done();
    }
}


// called when elaboration is done

void
sc_prim_channel_registry::elaboration_done()
{
    for( int i = 0; i < size(); ++ i ) {
	m_prim_channel_vec[i]->elaboration_done();
    }
}

// called before simulation begins

void
sc_prim_channel_registry::start_simulation()
{
    for( int i = 0; i < size(); ++ i ) {
	m_prim_channel_vec[i]->start_simulation();
    }
}

// called after simulation ends

void
sc_prim_channel_registry::simulation_done()
{
    for( int i = 0; i < size(); ++ i ) {
	m_prim_channel_vec[i]->simulation_done();
    }
}

} // namespace sc_core

// Taf!
