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

  sc_module_registry.cpp -- Registry for all modules.
                            FOR INTERNAL USE ONLY.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Andy Goodrich, Forte
                               Bishnupriya Bhattacharya, Cadence Design Systems,
                               25 August, 2003
  Description of Modification: phase callbacks

 *****************************************************************************/


// $Log: sc_module_registry.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.4  2006/01/26 21:04:54  acg
//  Andy Goodrich: deprecation message changes and additional messages.
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_module_registry.h"
#include "sysc/kernel/sc_simcontext.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_module_registry
//
//  Registry for all modules.
//  FOR INTERNAL USE ONLY!
// ----------------------------------------------------------------------------

void
sc_module_registry::insert( sc_module& module_ )
{
    if( sc_is_running() ) {
	SC_REPORT_ERROR( SC_ID_INSERT_MODULE_, "simulation running" );
    }

#ifdef DEBUG_SYSTEMC
    // check if module_ is already inserted
    for( int i = size() - 1; i >= 0; -- i ) {
	if( &module_ == m_module_vec[i] ) {
	    SC_REPORT_ERROR( SC_ID_INSERT_MODULE_, "already inserted" );
	}
    }
#endif

    // insert
    m_module_vec.push_back( &module_ );
}

void
sc_module_registry::remove( sc_module& module_ )
{
    int i;
    for( i = 0; i < size(); ++ i ) {
	if( &module_ == m_module_vec[i] ) {
	    break;
	}
    }
    if( i == size() ) {
	SC_REPORT_ERROR( SC_ID_REMOVE_MODULE_, 0 );
    }

    // remove
    m_module_vec[i] = m_module_vec[size() - 1];
    m_module_vec.resize(m_module_vec.size()-1);
}


// constructor

sc_module_registry::sc_module_registry( sc_simcontext& simc_ )
: m_simc( &simc_ )
{}


// destructor

sc_module_registry::~sc_module_registry()
{}

// called when construction is done

void
sc_module_registry::construction_done()
{
    for( int i = 0; i < size(); ++ i ) {
	m_module_vec[i]->construction_done();
    }
}

// called when elaboration is done

void
sc_module_registry::elaboration_done()
{
    bool error = false;
    for( int i = 0; i < size(); ++ i ) {
	m_module_vec[i]->elaboration_done( error );
    }
}

// called before simulation begins

void
sc_module_registry::start_simulation()
{
    for( int i = 0; i < size(); ++ i ) {
	m_module_vec[i]->start_simulation();
    }
}

// called after simulation ends

void
sc_module_registry::simulation_done()
{
    for( int i = 0; i < size(); ++ i ) {
	m_module_vec[i]->simulation_done();
    }
}

} // namespace sc_core
// Taf!
