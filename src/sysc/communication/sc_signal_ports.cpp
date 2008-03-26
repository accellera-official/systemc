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

  sc_signal_ports.cpp -- The sc_signal<T> port classes.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-08-20

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/


#include "sysc/communication/sc_signal_ports.h"
#include "sysc/datatypes/int/sc_signed.h"
#include "sysc/datatypes/int/sc_unsigned.h"
#include "sysc/datatypes/bit/sc_lv_base.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_in<bool>
//
//  Specialization of sc_in<T> for type bool.
// ----------------------------------------------------------------------------

// called when elaboration is done

void
sc_in<bool>::end_of_elaboration()
{
    if( m_traces != 0 ) {
	for( int i = 0; i < m_traces->size(); ++ i ) {
	    sc_trace_params* p = (*m_traces)[i];
	    in_if_type* iface = DCAST<in_if_type*>( get_interface() );
	    sc_trace( p->tf, iface->get_data_ref(), p->name );
	}
	remove_traces();
    }
}

// called by sc_trace

void
sc_in<bool>::add_trace(sc_trace_file* tf_, 
	const std::string& name_) const
{
    if( tf_ != 0 ) {
	if( m_traces == 0 ) {
	    m_traces = new sc_trace_params_vec;
	}
	m_traces->push_back( new sc_trace_params( tf_, name_ ) );
    }
}

void
sc_in<bool>::remove_traces() const
{
    if( m_traces != 0 ) {
	for( int i = m_traces->size() - 1; i >= 0; -- i ) {
	    delete (*m_traces)[i];
	}
	delete m_traces;
	m_traces = 0;
    }
}


// called by pbind (for internal use only)

int
sc_in<bool>::vbind( sc_interface& interface_ )
{
    return sc_port_b<if_type>::vbind( interface_ );
}

int
sc_in<bool>::vbind( sc_port_base& parent_ )
{
    in_port_type* in_parent = DCAST<in_port_type*>( &parent_ );
    if( in_parent != 0 ) {
	sc_port_base::bind( *in_parent );
	return 0;
    }
    inout_port_type* inout_parent = DCAST<inout_port_type*>( &parent_ );
    if( inout_parent != 0 ) {
	sc_port_base::bind( *inout_parent );
	return 0;
    }
    // type mismatch
    return 2;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_in<sc_logic>
//
//  Specialization of sc_in<T> for type sc_logic.
// ----------------------------------------------------------------------------

// called when elaboration is done

void
sc_in<sc_dt::sc_logic>::end_of_elaboration()
{
    if( m_traces != 0 ) {
	for( int i = 0; i < m_traces->size(); ++ i ) {
	    sc_trace_params* p = (*m_traces)[i];
	    in_if_type* iface = DCAST<in_if_type*>( get_interface() );
	    sc_trace( p->tf, iface->get_data_ref(), p->name );
	}
	remove_traces();
    }
}


// called by sc_trace

void
sc_in<sc_dt::sc_logic>::add_trace( sc_trace_file* tf_, 
    const std::string& name_ ) const
{
    if( tf_ != 0 ) {
	if( m_traces == 0 ) {
	    m_traces = new sc_trace_params_vec;
	}
	m_traces->push_back( new sc_trace_params( tf_, name_ ) );
    }
}

void
sc_in<sc_dt::sc_logic>::remove_traces() const
{
    if( m_traces != 0 ) {
	for( int i = m_traces->size() - 1; i >= 0; -- i ) {
	    delete (*m_traces)[i];
	}
	delete m_traces;
	m_traces = 0;
    }
}


// called by pbind (for internal use only)

int
sc_in<sc_dt::sc_logic>::vbind( sc_interface& interface_ )
{
    return sc_port_b<if_type>::vbind( interface_ );
}

int
sc_in<sc_dt::sc_logic>::vbind( sc_port_base& parent_ )
{
    in_port_type* in_parent = DCAST<in_port_type*>( &parent_ );
    if( in_parent != 0 ) {
	sc_port_base::bind( *in_parent );
	return 0;
    }
    inout_port_type* inout_parent = DCAST<inout_port_type*>( &parent_ );
    if( inout_parent != 0 ) {
	sc_port_base::bind( *inout_parent );
	return 0;
    }
    // type mismatch
    return 2;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_inout<bool>
//
//  Specialization of sc_inout<T> for type bool.
// ----------------------------------------------------------------------------

// destructor

sc_inout<bool>::~sc_inout()
{
    if( m_init_val != 0 ) {
	delete m_init_val;
    }
    remove_traces();
}


// set initial value (can also be called when port is not bound yet)

void
sc_inout<bool>::initialize( const data_type& value_ )
{
    inout_if_type* iface = DCAST<inout_if_type*>( get_interface() );
    if( iface != 0 ) {
	iface->write( value_ );
    } else {
	if( m_init_val == 0 ) {
	    m_init_val = new data_type;
	}
	*m_init_val = value_;
    }
}


// called when elaboration is done

void
sc_inout<bool>::end_of_elaboration()
{
    if( m_init_val != 0 ) {
	write( *m_init_val );
	delete m_init_val;
	m_init_val = 0;
    }
    if( m_traces != 0 ) {
	for( int i = 0; i < m_traces->size(); ++ i ) {
	    sc_trace_params* p = (*m_traces)[i];
	    in_if_type* iface = DCAST<in_if_type*>( get_interface() );
	    sc_trace( p->tf, iface->get_data_ref(), p->name );
	}
	remove_traces();
    }
}

// called by sc_trace

void
sc_inout<bool>::add_trace( sc_trace_file* tf_, 
    const std::string& name_ ) const
{
    if( tf_ != 0 ) {
	if( m_traces == 0 ) {
	    m_traces = new sc_trace_params_vec;
	}
	m_traces->push_back( new sc_trace_params( tf_, name_ ) );
    }
}

void
sc_inout<bool>::remove_traces() const
{
    if( m_traces != 0 ) {
	for( int i = m_traces->size() - 1; i >= 0; -- i ) {
	    delete (*m_traces)[i];
	}
	delete m_traces;
	m_traces = 0;
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_inout<sc_dt::sc_logic>
//
//  Specialization of sc_inout<T> for type sc_dt::sc_logic.
// ----------------------------------------------------------------------------

// destructor

sc_inout<sc_dt::sc_logic>::~sc_inout()
{
    if( m_init_val != 0 ) {
	delete m_init_val;
    }
    remove_traces();
}


// set initial value (can also be called when port is not bound yet)

void
sc_inout<sc_dt::sc_logic>::initialize( const data_type& value_ )
{
    inout_if_type* iface = DCAST<inout_if_type*>( get_interface() );
    if( iface != 0 ) {
	iface->write( value_ );
    } else {
	if( m_init_val == 0 ) {
	    m_init_val = new data_type;
	}
	*m_init_val = value_;
    }
}


// called when elaboration is done

void
sc_inout<sc_dt::sc_logic>::end_of_elaboration()
{
    if( m_init_val != 0 ) {
	write( *m_init_val );
	delete m_init_val;
	m_init_val = 0;
    }
    if( m_traces != 0 ) {
	for( int i = 0; i < m_traces->size(); ++ i ) {
	    sc_trace_params* p = (*m_traces)[i];
	    in_if_type* iface = DCAST<in_if_type*>( get_interface() );
	    sc_trace( p->tf, iface->get_data_ref(), p->name );
	}
	remove_traces();
    }
}


// called by sc_trace

void
sc_inout<sc_dt::sc_logic>::add_trace( sc_trace_file* tf_,
			       const std::string& name_ ) const
{
    if( tf_ != 0 ) {
	if( m_traces == 0 ) {
	    m_traces = new sc_trace_params_vec;
	}
	m_traces->push_back( new sc_trace_params( tf_, name_ ) );
    }
}

void
sc_inout<sc_dt::sc_logic>::remove_traces() const
{
    if( m_traces != 0 ) {
	for( int i = m_traces->size() - 1; i >= 0; -- i ) {
	    delete (*m_traces)[i];
	}
	delete m_traces;
	m_traces = 0;
    }
}

} // namespace sc_core

// Taf!
