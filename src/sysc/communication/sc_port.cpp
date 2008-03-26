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

  sc_port.cpp -- Base classes of all port classes.

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


#include "sysc/kernel/sc_lambda.h"
#include "sysc/kernel/sc_process_int.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/communication/sc_communication_ids.h"
#include "sysc/communication/sc_event_finder.h"
#include "sysc/communication/sc_port.h"
#include "sysc/communication/sc_signal_ifs.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  STRUCT : sc_bind_elem
// ----------------------------------------------------------------------------

struct sc_bind_elem
{
    // constructors
    sc_bind_elem();
    explicit sc_bind_elem( sc_interface* interface_ );
    explicit sc_bind_elem( sc_port_base* parent_ );

    sc_interface* iface;
    sc_port_base* parent;
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// constructors

sc_bind_elem::sc_bind_elem()
: iface( 0 ),
  parent( 0 )
{}

sc_bind_elem::sc_bind_elem( sc_interface* interface_ )
: iface( interface_ ),
  parent( 0 )
{}

sc_bind_elem::sc_bind_elem( sc_port_base* parent_ )
: iface( 0 ),
  parent( parent_ )
{}


// ----------------------------------------------------------------------------
//  STRUCT : sc_bind_ef
// ----------------------------------------------------------------------------

struct sc_bind_ef
{
    // constructor
    sc_bind_ef( sc_process_b* , sc_event_finder* );

    // destructor
    ~sc_bind_ef();

    sc_process_b*    handle;
    sc_event_finder* event_finder;
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// constructor

sc_bind_ef::sc_bind_ef( sc_process_b* handle_,
			sc_event_finder* event_finder_ )
: handle( handle_ ),
  event_finder( event_finder_ )
{}


// destructor

sc_bind_ef::~sc_bind_ef()
{
    if( event_finder != 0 ) {
	delete event_finder;
    }
}


// ----------------------------------------------------------------------------
//  STRUCT : sc_bind_info
// ----------------------------------------------------------------------------

struct sc_bind_info
{
    // constructor
    explicit sc_bind_info( int max_size_ );

    // destructor
    ~sc_bind_info();

    int size() const;

    int                       max_size;
    sc_pvector<sc_bind_elem*> vec;
    bool                      has_parent;
    int                       last_add;
    bool                      is_leaf;
    bool                      complete;

    sc_pvector<sc_bind_ef*>   thread_vec;
    sc_pvector<sc_bind_ef*>   method_vec;
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// constructor

sc_bind_info::sc_bind_info( int max_size_ )
: max_size( max_size_ ),
  has_parent( false ),
  last_add( -1 ),
  is_leaf( true ),
  complete( false )
{}


// destructor

sc_bind_info::~sc_bind_info()
{
    for( int i = size() - 1; i >= 0; -- i ) {
	delete vec[i];
    }
}


int
sc_bind_info::size() const
{
    return vec.size();
}


// ----------------------------------------------------------------------------
//  CLASS : sc_port_base
//
//  Abstract base class for class sc_port_b.
// ----------------------------------------------------------------------------

// error reporting

void
sc_port_base::report_error( const char* id, const char* add_msg ) const
{
    char msg[BUFSIZ];
    if( add_msg != 0 ) {
	sprintf( msg, "%s: port '%s' (%s)", add_msg, name(), kind() );
    } else {
	sprintf( msg, "port '%s' (%s)", name(), kind() );
    }
    SC_REPORT_ERROR( id, msg );
}


// constructors

sc_port_base::sc_port_base( int max_size_ )
: sc_object( sc_gen_unique_name( "port" ) ),
  m_bind_info( new sc_bind_info( max_size_ ) )
{
    simcontext()->get_port_registry()->insert( this );
}

sc_port_base::sc_port_base( const char* name_, int max_size_ )
: sc_object( name_ ),
  m_bind_info( new sc_bind_info( max_size_ ) )
{
    simcontext()->get_port_registry()->insert( this );
}


// destructor

sc_port_base::~sc_port_base()
{
    simcontext()->get_port_registry()->remove( this );
    if( m_bind_info != 0 ) {
	delete m_bind_info;
    }
}


// bind interface to this port

void
sc_port_base::bind( sc_interface& interface_ )
{
    if( m_bind_info == 0 ) {
	// cannot bind an interface after elaboration
	report_error( SC_ID_BIND_IF_TO_PORT_, "simulation running" );
    }

    if( m_bind_info->size() == m_bind_info->max_size &&
	m_bind_info->max_size > 0 ) {
	report_error( SC_ID_BIND_IF_TO_PORT_, "maximum reached" );
    }

    // check if interface is already bound to this port
    for( int i = m_bind_info->size() - 1; i >= 0; -- i ) {
	if( &interface_ == m_bind_info->vec[i]->iface ) {
	    report_error( SC_ID_BIND_IF_TO_PORT_, "already bound" );
	}
    }

    m_bind_info->vec.push_back( new sc_bind_elem( &interface_ ) );
    
    if( ! m_bind_info->has_parent ) {
	// add (cache) the interface
	add_interface( &interface_ );
	m_bind_info->last_add ++;
    }
}


// bind parent port to this port

void
sc_port_base::bind( this_type& parent_ )
{
    if( m_bind_info == 0 ) {
	// cannot bind a parent port after elaboration
	report_error( SC_ID_BIND_PORT_TO_PORT_, "simulation running" );
    }

    if( m_bind_info->size() == m_bind_info->max_size &&
	m_bind_info->max_size > 0 ) {
	report_error( SC_ID_BIND_PORT_TO_PORT_, "maximum reached" );
    }

    if( &parent_ == this ) {
	report_error( SC_ID_BIND_PORT_TO_PORT_, "same port" );
    }

    // check if parent port is already bound to this port
    for( int i = m_bind_info->size() - 1; i >= 0; -- i ) {
	if( &parent_ == m_bind_info->vec[i]->parent ) {
	    report_error( SC_ID_BIND_PORT_TO_PORT_, "already bound" );
	}
    }

    m_bind_info->vec.push_back( new sc_bind_elem( &parent_ ) );
    m_bind_info->has_parent = true;
    parent_.m_bind_info->is_leaf = false;
}

// called by sc_port_registry::construction_done (null by default)

void sc_port_base::before_end_of_elaboration() 
{}

// called by elaboration_done (does nothing)

void
sc_port_base::end_of_elaboration()
{}

// called by sc_port_registry::start_simulation (does nothing by default)

void sc_port_base::start_of_simulation()
{}

// called by sc_port_registry::simulation_done (does nothing by default)

void sc_port_base::end_of_simulation()
{}


// called by class sc_module for positional binding

int
sc_port_base::pbind( sc_interface& interface_ )
{
    if( m_bind_info == 0 ) {
	// cannot bind an interface after elaboration
	report_error( SC_ID_BIND_IF_TO_PORT_, "simulation running" );
    }
    
    if( m_bind_info->size() != 0 ) {
	// first interface already bound
	return 1;
    }

    return vbind( interface_ );
}

int
sc_port_base::pbind( sc_port_base& parent_ )
{
    if( m_bind_info == 0 ) {
	// cannot bind a parent port after elaboration
	report_error( SC_ID_BIND_PORT_TO_PORT_, "simulation running" );
    }
    
    if( m_bind_info->size() != 0 ) {
	// first interface already bound
	return 1;
    }

    return vbind( parent_ );
}


// called by the sc_sensitive* classes

void
sc_port_base::make_sensitive( sc_thread_handle handle_,
			      sc_event_finder* event_finder_ ) const
{
    assert( m_bind_info != 0 );
    m_bind_info->thread_vec.push_back( new sc_bind_ef( handle_,
						       event_finder_ ) );
}

void
sc_port_base::make_sensitive( sc_method_handle handle_,
			      sc_event_finder* event_finder_ ) const
{
    assert( m_bind_info != 0 );
    m_bind_info->method_vec.push_back( new sc_bind_ef( handle_,
						       event_finder_ ) );
}


// support methods

int
sc_port_base::first_parent()
{
    for( int i = 0; i < m_bind_info->size(); ++ i ) {
	if( m_bind_info->vec[i]->parent != 0 ) {
	    return i;
	}
    }
    return -1;
}

void
sc_port_base::insert_parent( int i )
{
    sc_pvector<sc_bind_elem*>& vec = m_bind_info->vec;

    this_type* parent = vec[i]->parent;

    vec[i]->iface = parent->m_bind_info->vec[0]->iface;
    vec[i]->parent = 0;

    int n = parent->m_bind_info->size() - 1;
    if( n > 0 ) {
	if( m_bind_info->size() + n > m_bind_info->max_size &&
	    m_bind_info->max_size > 0 ) {
	    report_error( SC_ID_COMPLETE_BINDING_, "maximum reached" );
	}
	// resize the bind vector (by adding new elements)
	for( int k = 0; k < n; ++ k ) {
	    vec.push_back( new sc_bind_elem() );
	}
	// move elements in the bind vector
	for( int k = m_bind_info->size() - n - 1; k > i; -- k ) {
	    vec[k + n]->iface = vec[k]->iface;
	    vec[k + n]->parent = vec[k]->parent;
	}
	// insert parent interfaces into the bind vector
	for( int k = i + 1; k <= i + n; ++ k ) {
	    vec[k]->iface = parent->m_bind_info->vec[k - i]->iface;
	    vec[k]->parent = 0;
	}
    }
}


// called when elaboration is done

void
sc_port_base::complete_binding()
{
    assert( m_bind_info != 0 );
    
    if( m_bind_info->complete ) {
	return;
    }
    
    if( m_bind_info->size() == 0 ) {
	report_error( SC_ID_COMPLETE_BINDING_, "port not bound" );
    }

    int i = first_parent();
    while( i >= 0 ) {
	m_bind_info->vec[i]->parent->complete_binding();
	insert_parent( i );
	i = first_parent();
    }

    int size;

    for( int j = 0; j < m_bind_info->size(); ++ j ) {
        sc_interface* iface = m_bind_info->vec[j]->iface;

	// check the state
	if( iface == 0 || m_bind_info->vec[j]->parent != 0 ) {
	    report_error( SC_ID_COMPLETE_BINDING_, "inconsistent state" );
	}

	if( j > m_bind_info->last_add ) {
	    // add (cache) the interface
	    add_interface( iface );
	}
	
	if( m_bind_info->is_leaf ) {
	    // only register "leaf" ports (ports without children)
	    iface->register_port( *this, if_typename() );
	}

	// complete static sensitivity for methods
	size = m_bind_info->method_vec.size();
	for( int k = 0; k < size; ++ k ) {
	    sc_bind_ef* p = m_bind_info->method_vec[k];
	    const sc_event& event = ( p->event_finder != 0 )
                                  ? p->event_finder->find_event()
                                  : iface->default_event();
	    p->handle->add_static_event( event );
	}

	// complete static sensitivity for threads
	size = m_bind_info->thread_vec.size();
	for( int k = 0; k < size; ++ k ) {
	    sc_bind_ef* p = m_bind_info->thread_vec[k];
	    const sc_event& event = ( p->event_finder != 0 )
                                  ? p->event_finder->find_event()
                                  : iface->default_event();
	    p->handle->add_static_event( event );
	}
    }

    // cleanup

    size = m_bind_info->method_vec.size();
    for( int k = 0; k < size; ++ k ) {
        delete m_bind_info->method_vec[k];
    }
    m_bind_info->method_vec.erase_all();

    size = m_bind_info->thread_vec.size();
    for( int k = 0; k < size; ++ k ) {
        delete m_bind_info->thread_vec[k];
    }
    m_bind_info->thread_vec.erase_all();

    m_bind_info->complete = true;
}

void
sc_port_base::construction_done()
{
    before_end_of_elaboration();
}

void
sc_port_base::elaboration_done()
{
    assert( m_bind_info != 0 && m_bind_info->complete );
    delete m_bind_info;
    m_bind_info = 0;

    end_of_elaboration();
}

void
sc_port_base::start_simulation()
{
    start_of_simulation();
}

void
sc_port_base::simulation_done()
{
    end_of_simulation();
}


// ----------------------------------------------------------------------------
//  CLASS : sc_port_registry
//
//  Registry for all ports.
//  FOR INTERNAL USE ONLY!
// ----------------------------------------------------------------------------

void
sc_port_registry::insert( sc_port_base* port_ )
{
    if( m_simc->is_running() ) {
	port_->report_error( SC_ID_INSERT_PORT_, "simulation running" );
    }

#ifdef DEBUG_SYSTEMC
    // check if port_ is already inserted
    for( int i = size() - 1; i >= 0; -- i ) {
	if( port_ == m_port_vec[i] ) {
	    port_->report_error( SC_ID_INSERT_PORT_, "port already inserted" );
	}
    }
#endif

    // append the port to the current module's vector of ports
    sc_module* curr_module = m_simc->hierarchy_curr();
    if( curr_module == 0 ) {
	port_->report_error( SC_ID_PORT_OUTSIDE_MODULE_ );
    }
    curr_module->append_port( port_ );

    // insert
    m_port_vec.push_back( port_ );
}

void
sc_port_registry::remove( sc_port_base* port_ )
{
    int i;
    for( i = size() - 1; i >= 0; -- i ) {
	if( port_ == m_port_vec[i] ) {
	    break;
	}
    }
    if( i == -1 ) {
	port_->report_error( SC_ID_REMOVE_PORT_, "port not registered" );
    }

    // remove
    m_port_vec[i] = m_port_vec[size() - 1];
    m_port_vec.decr_count();
}


void
sc_port_registry::add_lambda_for_resolution( const sc_lambda_ptr& lambda_ )
{
    sc_lambda_ptr* lambda_copy = new sc_lambda_ptr( lambda_ );
    m_unresolved_lambdas->push_back( lambda_copy );
}


// constructor

sc_port_registry::sc_port_registry( sc_simcontext& simc_ )
: m_simc( &simc_ )
{
    m_unresolved_lambdas = new sc_pvector<sc_lambda_ptr*>;
}


// destructor

sc_port_registry::~sc_port_registry()
{
    delete_unresolved_lambdas();
}

// called when construction is done

void
sc_port_registry::construction_done()
{
    for( int i = size() - 1; i >= 0; -- i ) {
        m_port_vec[i]->construction_done();
    }
}

// called when elaboration is done

void
sc_port_registry::elaboration_done()
{
    for( int i = size() - 1; i >= 0; -- i ) {
        m_port_vec[i]->complete_binding();
    }

    resolve_lambdas();

    for( int i = size() - 1; i >= 0; -- i ) {
        m_port_vec[i]->elaboration_done();
    }
}

// called before simulation begins

void
sc_port_registry::start_simulation()
{
    for( int i = size() - 1; i >= 0; -- i ) {
        m_port_vec[i]->start_simulation();
    }
}

// called after simulation ends

void
sc_port_registry::simulation_done()
{
    for( int i = size() - 1; i >= 0; -- i ) {
        m_port_vec[i]->simulation_done();
    }
}

void
sc_port_registry::resolve_lambdas()
{
    int sz = m_unresolved_lambdas->size();
    for( int i = 0; i < sz; ++ i ) {
        sc_lambda_ptr* lambda_copy = m_unresolved_lambdas->fetch( i );
        (*lambda_copy)->replace_ports( &replace_port, this );
    }
    delete_unresolved_lambdas();
}

void
sc_port_registry::delete_unresolved_lambdas()
{
    if( m_unresolved_lambdas != 0 ) {
	int sz = m_unresolved_lambdas->size();
        for( int i = 0; i < sz; ++ i ) {
            delete m_unresolved_lambdas->fetch( i );
        }
        delete m_unresolved_lambdas;
	m_unresolved_lambdas = 0;
    }
}


// This is a static member function.

void
sc_port_registry::replace_port( sc_port_registry* registry,
				sc_lambda_rand* rand )
{
    switch( rand->rand_ty ) {
    case SC_LAMBDA_RAND_SIGNAL_BOOL: {
	// the tricky part, reversed
	const sc_port_base* pb = RCAST<const sc_port_base*>( rand->edgy_sig );
	// check if pb is a port
	bool is_port = false;
	for( int i = registry->size() - 1; i >= 0; -- i ) {
	    if( pb == registry->m_port_vec[i] ) {
		is_port = true;
		break;
	    }
	}
	if( ! is_port ) {
	    break;
	}
	// cast pb to the appropriate port type
	const sc_port_b<sc_signal_in_if<bool> >* in_port =
            DCAST<const sc_port_b<sc_signal_in_if<bool> >*>( pb );
	if( in_port != 0 ) {
	    rand->edgy_sig =
                DCAST<const sc_signal_in_if<bool>*>(
		    in_port->get_interface() );
	    assert( rand->edgy_sig != 0 );
	    break;
	}
	const sc_port_b<sc_signal_inout_if<bool> >* inout_port =
            DCAST<const sc_port_b<sc_signal_inout_if<bool> >*>( pb );
        if( inout_port != 0 ) {
	    rand->edgy_sig =
                DCAST<const sc_signal_in_if<bool>*>(
		    inout_port->get_interface() );
	    assert( rand->edgy_sig != 0 );
	    break;
	}
	// this should not happen
	assert( false );
	break;
    }
    case SC_LAMBDA_RAND_SIGNAL_SUL: {
	// the tricky part, reversed
	const sc_port_base* pb = RCAST<const sc_port_base*>( rand->sul_sig );
	// check if pb is a port
	bool is_port = false;
	for( int i = registry->size() - 1; i >= 0; -- i ) {
	    if( pb == registry->m_port_vec[i] ) {
		is_port = true;
		break;
	    }
	}
	if( ! is_port ) {
	    break;
	}
	// cast pb to the appropriate port type
	const sc_port_b<sc_signal_in_if<sc_dt::sc_logic> >* in_port =
            DCAST<const sc_port_b<sc_signal_in_if<sc_dt::sc_logic> >*>( pb );
	if( in_port != 0 ) {
	    rand->sul_sig =
                DCAST<const sc_signal_in_if<sc_dt::sc_logic>*>(
		    in_port->get_interface() );
	    assert( rand->sul_sig != 0 );
	    break;
	}
	const sc_port_b<sc_signal_inout_if<sc_dt::sc_logic> >* inout_port =
            DCAST<const sc_port_b<sc_signal_inout_if<sc_dt::sc_logic> >*>( pb );
        if( inout_port != 0 ) {
	    rand->sul_sig =
                DCAST<const sc_signal_in_if<sc_dt::sc_logic>*>(
		    inout_port->get_interface() );
	    assert( rand->sul_sig != 0 );
	    break;
	}
	// this should not happen
	assert( false );
	break;
    }
    default:
        break;
    }
}

} // namespace sc_core

// Taf!
