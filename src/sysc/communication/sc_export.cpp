
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

  sc_export.cpp -- 

  Original Author: Bishnupriya Bhattachary, Cadence, Design Systems, 
                   25 August, 2003

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

// $Log: sc_export.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:35  acg
// SystemC 2.2
//
// Revision 1.4  2006/01/26 21:00:50  acg
//  Andy Goodrich: conversion to use sc_event::notify(SC_ZERO_TIME) instead of
//  sc_event::notify_delayed()
//
// Revision 1.3  2006/01/13 18:47:42  acg
// Added $Log command so that CVS comments are reproduced in the source.
//

#include "sysc/communication/sc_export.h"
#include "sysc/kernel/sc_simcontext.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_export_base
//
// ----------------------------------------------------------------------------

sc_export_base::sc_export_base() : sc_object(sc_gen_unique_name("export"))
{
    simcontext()->get_export_registry()->insert(this);
}
    
sc_export_base::sc_export_base(const char* name_) : sc_object(name_)
{
    simcontext()->get_export_registry()->insert(this);
}
    
sc_export_base::~sc_export_base()
{
    simcontext()->get_export_registry()->remove(this);
}

// called when construction is done

void
sc_export_base::before_end_of_elaboration()
{
}

// called when elaboration is done (does nothing)

void
sc_export_base::end_of_elaboration()
{}

// called before simulation starts (does nothing)

void
sc_export_base::start_of_simulation()
{}

// called after simulation ends (does nothing)

void
sc_export_base::end_of_simulation()
{}


// ----------------------------------------------------------------------------
//  CLASS : sc_export_registry
//
//  Registry for all exports.
//  FOR INTERNAL USE ONLY!
// ----------------------------------------------------------------------------

void
sc_export_registry::insert( sc_export_base* export_ )
{
    if( sc_is_running() ) {
	SC_REPORT_ERROR(SC_ID_SC_EXPORT_AFTER_START_, export_->name());
    }

#ifdef DEBUG_SYSTEMC
    // check if port_ is already inserted
    for( int i = size() - 1; i >= 0; -- i ) {
	if( export_ == m_export_vec[i] ) {
	    SC_REPORT_ERROR(SC_ID_SC_EXPORT_ALREADY_REGISTERED_, export_->name());
	}
    }
#endif

/* 
    //TBD:  maybe we want to do this stuf for later

    // append the port to the current module's vector of ports
    sc_module* curr_module = m_simc->hierarchy_curr();
    if( curr_module == 0 ) {
	port_->report_error( SC_ID_PORT_OUTSIDE_MODULE_ );
    }
    curr_module->append_port( port_ );
*/

    // insert
    m_export_vec.push_back( export_ );
}

void
sc_export_registry::remove( sc_export_base* export_ )
{
    if (size()==0) return;
    int i;
    for( i = size() - 1; i >= 0; -- i ) {
	if( export_ == m_export_vec[i] ) {
	    break;
	}
    }
    if( i == -1 ) {
	SC_REPORT_ERROR(SC_ID_SC_EXPORT_NOT_REGISTERED_, export_->name());
    }

    // remove
    m_export_vec[i] = m_export_vec[size() - 1];
    m_export_vec.resize(size()-1);
}

// constructor

sc_export_registry::sc_export_registry( sc_simcontext& simc_ )
: m_simc( &simc_ )
{
}


// destructor

sc_export_registry::~sc_export_registry()
{
}

// called when construction is done

void
sc_export_registry::construction_done()
{
    for( int i = size() - 1; i >= 0; -- i ) {
        sc_export_base* e = m_export_vec[i];
        if (e->get_interface() == 0) {
	    SC_REPORT_ERROR(SC_ID_SC_EXPORT_NOT_BOUND_AFTER_CONSTRUCTION_,
	        e->name());
        }
	e->before_end_of_elaboration();
    }
}

// called when elaboration is done

void
sc_export_registry::elaboration_done()
{
    for( int i = size() - 1; i >= 0; -- i ) {
	m_export_vec[i]->end_of_elaboration();
    }
}

// called before simulation begins

void
sc_export_registry::start_simulation()
{
    for( int i = size() - 1; i >= 0; -- i ) {
	m_export_vec[i]->start_of_simulation();
    }
}

void
sc_export_registry::simulation_done()
{
    for( int i = size() - 1; i >= 0; -- i ) {
	m_export_vec[i]->end_of_simulation();
    }
}

} // namespace sc_core

// Taf!
