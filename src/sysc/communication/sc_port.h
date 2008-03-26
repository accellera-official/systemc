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

  sc_port.h -- Base classes of all port classes.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Andy Goodrich, Forte,
                               Bishnupriya Bhattacharya, Cadence Design Systems,
                               25 August, 2003
  Description of Modification: phase callbacks

      Name, Affiliation, Date: Andy Goodrich, Forte Design Systems
                               12 December, 2005
  Description of Modification: multiport binding policy changes

    
 *****************************************************************************/

/* 
$Log: sc_port.h,v $
Revision 1.1.1.1  2006/12/15 20:31:35  acg
SystemC 2.2

Revision 1.3  2006/01/24 20:46:31  acg
Andy Goodrich: changes to eliminate use of deprecated features. For instance,
using notify(SC_ZERO_TIME) in place of notify_delayed().

Revision 1.2  2006/01/03 23:18:26  acg
Changed copyright to include 2006.

Revision 1.1.1.1  2005/12/19 23:16:43  acg
First check in of SystemC 2.1 into its own archive.

Revision 1.10  2005/09/15 23:01:51  acg
Added std:: prefix to appropriate methods and types to get around
issues with the Edison Front End.

Revision 1.9  2005/08/10 01:35:59  acg
Changes for 64-bit support.

Revision 1.8  2005/04/03 22:52:51  acg
Namespace changes.

Revision 1.7  2005/03/21 22:31:32  acg
Changes to sc_core namespace.

Revision 1.6  2004/09/27 21:02:54  acg
Andy Goodrich - Forte Design Systems, Inc.
   - Added a $Log comment so that CVS checkin comments will appear in
     checked out source.

*/

#ifndef SC_PORT_H
#define SC_PORT_H


#include "sysc/communication/sc_communication_ids.h"
#include "sysc/communication/sc_interface.h"
#include "sysc/kernel/sc_event.h"
#include "sysc/kernel/sc_object.h"
#include "sysc/kernel/sc_process.h"
#include <typeinfo>

namespace sc_core {

class sc_event_finder;

struct sc_bind_info;

enum sc_port_policy 
{ 
    SC_ONE_OR_MORE_BOUND,   // Default 
    SC_ZERO_OR_MORE_BOUND, 
    SC_ALL_BOUND  
}; 


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//  BEWARE: Ports can only be created and bound during elaboration.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


// ----------------------------------------------------------------------------
//  CLASS : sc_port_base
//
//  Abstract base class for class sc_port_b.
// ----------------------------------------------------------------------------

class sc_port_base
: public sc_object
{
    friend class sc_module;
    friend class sc_port_registry;
    friend class sc_sensitive;
    friend class sc_sensitive_pos;
    friend class sc_sensitive_neg;

public:

    // typedefs

    typedef sc_port_base this_type;

public:

    // get the first interface without checking for nil
    virtual       sc_interface* get_interface()       = 0;
    virtual const sc_interface* get_interface() const = 0;

    virtual const char* kind() const
        { return "sc_port_base"; }

protected:

    // constructors
    explicit sc_port_base( int max_size_, 
	    sc_port_policy policy=SC_ONE_OR_MORE_BOUND );
    sc_port_base( const char* name_, int max_size_, 
		  sc_port_policy policy=SC_ONE_OR_MORE_BOUND );

    // destructor
    virtual ~sc_port_base();

    // bind interface to this port
    void bind( sc_interface& interface_ );

    // bind parent port to this port
    void bind( this_type& parent_ );

    // called by pbind (for internal use only)
    virtual int vbind( sc_interface& ) = 0;
    virtual int vbind( sc_port_base& ) = 0;

    // called by complete_binding (for internal use only)
    virtual void add_interface( sc_interface* ) = 0;
	virtual int interface_count() = 0;
    virtual const char* if_typename() const = 0;

    // called by construction_done (does nothing by default)
    virtual void before_end_of_elaboration();

    // called by elaboration_done (does nothing)
    virtual void end_of_elaboration();

    // called by start_simulation (does nothing by default)
    virtual void start_of_simulation();

    // called by simulation_done (does nothing by default)
    virtual void end_of_simulation();

    // error reporting
    void report_error( const char* id, const char* add_msg = 0) const;

protected:
    // called by the sc_sensitive* classes
    virtual void make_sensitive( sc_thread_handle, sc_event_finder* = 0 ) const;
    virtual void make_sensitive( sc_method_handle, sc_event_finder* = 0 ) const;
    void add_static_event(
		sc_method_handle process_p, const sc_event& event) const;
    void add_static_event(
		sc_thread_handle process_p, const sc_event& event) const;

private:

    // called by class sc_module for positional binding
    int pbind( sc_interface& );
    int pbind( sc_port_base& );


    // support methods
    int first_parent();
    void insert_parent( int );

    // called when construction is done
    void construction_done();

    // called when elaboration is done
    void complete_binding();
    void elaboration_done();

    // called before simulation starts
    void start_simulation();

    // called after simulation ends
    void simulation_done();

protected:

    sc_bind_info* m_bind_info;

private:

    // disabled
    sc_port_base();
    sc_port_base( const this_type& );
    this_type& operator = ( const this_type& );
};

// ----------------------------------------------------------------------------
//  CLASS : sc_port_registry
//
//  Registry for all ports.
//  FOR INTERNAL USE ONLY!
// ----------------------------------------------------------------------------

class sc_port_registry
{
    friend class sc_simcontext;

public:

    void insert( sc_port_base* );
    void remove( sc_port_base* );

    int size() const
        { return m_port_vec.size(); }

private:

    // constructor
    explicit sc_port_registry( sc_simcontext& simc_ );

    // destructor
    ~sc_port_registry();

    // called when by construction_done and elaboration done
    void complete_binding();

    // called when construction is done
    void construction_done();

    // called when elaboration is done
    void elaboration_done();

    // called before simulation starts
    void start_simulation();

    // called after simulation ends
    void simulation_done();

    static void replace_port( sc_port_registry* );

private:

    sc_simcontext*              m_simc;
    std::vector<sc_port_base*>  m_port_vec;

private:

    // disabled
    sc_port_registry();
    sc_port_registry( const sc_port_registry& );
    sc_port_registry& operator = ( const sc_port_registry& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_port_b
//
//  Abstract base class for class sc_port.
// ----------------------------------------------------------------------------

template <class IF>
class sc_port_b
: public sc_port_base
{
public:

    friend class sc_sensitive;
    friend class sc_sensitive_neg;
    friend class sc_sensitive_pos;

    // typedefs

    typedef sc_port_base  base_type;
    typedef sc_port_b<IF> this_type;

public:

    // bind an interface of type IF to this port

    void bind( IF& interface_ )
	{ base_type::bind( interface_ ); }

    void operator () ( IF& interface_ )
	{ base_type::bind( interface_ ); }


    // bind a parent port with type IF to this port

    void bind( this_type& parent_ )
	{ base_type::bind( parent_ ); }

    void operator () ( this_type& parent_ )
	{ base_type::bind( parent_ ); }


    // number of connected interfaces

    int size() const
	{ return m_interface_vec.size(); }


    // allow to call methods provided by the first interface
    IF* operator -> ();
    const IF* operator -> () const;


    // allow to call methods provided by interface at index
    inline const IF* get_interface( int iface_i ) const;
    inline IF* get_interface( int iface_i );
    IF* operator [] ( int index_ )
        { return get_interface( index_ ); }
    const IF* operator [] ( int index_ ) const
        { return get_interface( index_ ); }


    // get the first interface without checking for nil

    virtual sc_interface* get_interface()
        { return m_interface; }

    virtual const sc_interface* get_interface() const
        { return m_interface; }

protected:

    // constructors

    explicit sc_port_b( int max_size_, 
	    sc_port_policy policy=SC_ONE_OR_MORE_BOUND )
	: base_type( max_size_, policy ), m_interface( 0 )
	{}

    sc_port_b( const char* name_, int max_size_, 
               sc_port_policy policy=SC_ONE_OR_MORE_BOUND )
	: base_type( name_, max_size_, policy ), m_interface( 0 )
	{}


    // destructor (does nothing)

    virtual ~sc_port_b()
	{}


    // called by pbind (for internal use only)
    virtual int vbind( sc_interface& );
    virtual int vbind( sc_port_base& );

protected:

    // called by the sc_sensitive* classes
    virtual void make_sensitive( sc_thread_handle, sc_event_finder* = 0 ) const;
    virtual void make_sensitive( sc_method_handle, sc_event_finder* = 0 ) const;

private:

    // called by complete_binding (for internal use only)
    virtual void add_interface( sc_interface* );
    virtual const char* if_typename() const;
	virtual int interface_count();

    // disabled
    sc_port_b();
    sc_port_b( const this_type& );
    this_type& operator = ( const this_type& );

private:

    IF*              m_interface;	// first interface in interface vec
    std::vector<IF*> m_interface_vec;
};


// ----------------------------------------------------------------------------
//  CLASS : sc_port
//
//  Generic port class and base class for other port classes.
//  N is the maximum number of channels (with interface IF) that can be bound
//  to this port. N <= 0 means no maximum.
// ----------------------------------------------------------------------------

extern void sc_warn_port_constructor();

template <class IF, int N = 1, sc_port_policy P=SC_ONE_OR_MORE_BOUND>
class sc_port
: public sc_port_b<IF>
{
    // typdefs

    typedef sc_port_b<IF> base_type;
    typedef sc_port<IF,N,P> this_type;

public:

    // constructors

    sc_port()
	: base_type( N, P )
	{}

    explicit sc_port( const char* name_ )
	: base_type( name_, N, P )
	{}

    explicit sc_port( IF& interface_ )
	: base_type( N, P )
	{ sc_warn_port_constructor(); base_type::bind( interface_ ); }

    sc_port( const char* name_, IF& interface_ )
	: base_type( name_, N, P )
	{ sc_warn_port_constructor(); base_type::bind( interface_ ); }

    explicit sc_port( base_type& parent_ )
	: base_type( N, P )
	{ sc_warn_port_constructor(); base_type::bind( parent_ ); }

    sc_port( const char* name_, base_type& parent_ )
	: base_type( name_, N, P )
	{ sc_warn_port_constructor(); base_type::bind( parent_ ); }

    sc_port( this_type& parent_ )
	: base_type( N, P )
	{ sc_warn_port_constructor(); base_type::bind( parent_ ); }

    sc_port( const char* name_, this_type& parent_ )
	: base_type( name_, N, P )
	{ sc_warn_port_constructor(); base_type::bind( parent_ ); }


    // destructor (does nothing)

    virtual ~sc_port()
	{}

    virtual const char* kind() const
        { return "sc_port"; }

private:

    // disabled
    sc_port( const this_type& );
    this_type& operator = ( const this_type& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS : sc_port_b
//
//  Abstract base class for class sc_port.
// ----------------------------------------------------------------------------

// allow to call methods provided by the first interface

template <class IF>
inline
IF*
sc_port_b<IF>::operator -> ()
{
    if( m_interface == 0 ) {
	report_error( SC_ID_GET_IF_, "port is not bound" );
    }
    return m_interface;
}

template <class IF>
inline
const IF*
sc_port_b<IF>::operator -> () const
{
    if( m_interface == 0 ) {
	report_error( SC_ID_GET_IF_, "port is not bound" );
    }
    return m_interface;
}


// allow to call methods provided by interface at index
//
// note that we special-case index of zero, since the method may be
// called before binding has occurred, and we need to return a zero
// in that case not an error.

template <class IF>
inline
IF*
sc_port_b<IF>::get_interface( int index_ )
{
    if ( index_ == 0 ) {
    	return m_interface;
    }
    else if( index_ < 0 || index_ >= size() ) {
	report_error( SC_ID_GET_IF_, "index out of range" );
    }
    return m_interface_vec[index_];
}

template <class IF>
inline
const IF*
sc_port_b<IF>::get_interface( int index_ ) const
{
    if ( index_ == 0 ) {
    	return m_interface;
    }
    else if( index_ < 0 || index_ >= size() ) {
	report_error( SC_ID_GET_IF_, "index out of range" );
    }
    return m_interface_vec[index_];
}


// called by pbind (for internal use only)

template <class IF>
inline
int
sc_port_b<IF>::vbind( sc_interface& interface_ )
{
    IF* iface = DCAST<IF*>( &interface_ );
    if( iface == 0 ) {
	// type mismatch
	return 2;
    }
    base_type::bind( *iface );
    return 0;
}

template <class IF>
inline
int
sc_port_b<IF>::vbind( sc_port_base& parent_ )
{
    this_type* parent = DCAST<this_type*>( &parent_ );
    if( parent == 0 ) {
	// type mismatch
	return 2;
    }
    base_type::bind( *parent );
    return 0;
}


// called by complete_binding (for internal use only)

template <class IF>
inline
void
sc_port_b<IF>::add_interface( sc_interface* interface_ )
{
    IF* iface = DCAST<IF*>( interface_ );
    assert( iface != 0 );

    // make sure that the interface is not already bound:

    int size = m_interface_vec.size();
    for ( int i = 0; i < size; i++ )
    {
    	if ( iface == m_interface_vec[i] )
	{
	    report_error( SC_ID_BIND_IF_TO_PORT_, 
	    	"interface already bound to port" );
	}
    }

    // "bind" the interface and make sure our short cut for 0 is set up.

    m_interface_vec.push_back( iface );
    m_interface = m_interface_vec[0]; 
}

template <class IF>
inline
const char*
sc_port_b<IF>::if_typename() const
{
    return typeid( IF ).name();
}

template <class IF>
inline
int
sc_port_b<IF>::interface_count()
{
	return m_interface_vec.size();
}

template <class IF>
void
sc_port_b<IF>::make_sensitive( sc_thread_handle handle_p,
                  sc_event_finder* event_finder_ ) const
{
    if ( m_bind_info == 0 )
    {
        int if_n = m_interface_vec.size();
        for ( int if_i = 0; if_i < if_n; if_i++ )
	{
	    IF* iface_p = m_interface_vec[if_i];
	    assert( iface_p != 0 );
	    add_static_event( handle_p, iface_p->default_event() );
	}
    }
    else
    {
        sc_port_base::make_sensitive( handle_p, event_finder_ );
    }
}

template <class IF>
void
sc_port_b<IF>::make_sensitive( sc_method_handle handle_p,
                  sc_event_finder* event_finder_ ) const
{
    if ( m_bind_info == 0 )
    {
        int if_n = m_interface_vec.size();
        for ( int if_i = 0; if_i < if_n; if_i++ )
	{
	    IF* iface_p = m_interface_vec[if_i];
	    assert( iface_p != 0 );
	    add_static_event( handle_p, iface_p->default_event() );
	}
    }
    else
    {
        sc_port_base::make_sensitive( handle_p, event_finder_ );
    }
}

// ----------------------------------------------------------------------------
//  CLASS : sc_port
//
//  Generic port class and base class for other port classes.
//  N is the maximum number of channels (with interface IF) that can be bound
//  to this port. N <= 0 means no maximum.
// ----------------------------------------------------------------------------

} // namespace sc_core

#endif

// Taf!
