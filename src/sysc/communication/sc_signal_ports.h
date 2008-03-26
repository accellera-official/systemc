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

  sc_signal_ports.h -- The sc_signal<T> port classes.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:  Jason Elbaum, Motorola, Inc., 2001-11-12
  Description of Modification:  Added a static, private, otherwise
                                unused data member to the sc_in
                                and sc_inout classes to address
                                a bug in the GNU compiler *only*.
                                This works around a bug in g++ 2.95.2
                                regarding implicit casting from a
                                templated class to a C++ intrinsic type.

 *****************************************************************************/

#ifndef SC_SIGNAL_PORTS_H
#define SC_SIGNAL_PORTS_H


#include "sysc/communication/sc_event_finder.h"
#include "sysc/communication/sc_port.h"
#include "sysc/communication/sc_signal_ifs.h"
#include "sysc/datatypes/bit/sc_logic.h"
#include "sysc/tracing/sc_trace.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  STRUCT : sc_trace_params
//
//  Struct for storing the trace file and object name of an sc_trace call.
//  FOR INTERNAL USE ONLY!
// ----------------------------------------------------------------------------

struct sc_trace_params
{
    sc_trace_file*        tf;
    std::string      name;

    sc_trace_params( sc_trace_file* tf_, const std::string& name_ )
	: tf( tf_ ), name( name_ )
	{}
};


typedef sc_pvector<sc_trace_params*> sc_trace_params_vec;


// ----------------------------------------------------------------------------
//  CLASS : sc_in<T>
//
//  The sc_signal<T> input port class.
// ----------------------------------------------------------------------------

template <class T>
class sc_in
: public sc_port<sc_signal_in_if<T>,1>
{
public:

    // typedefs

    typedef T                             data_type;

    typedef sc_signal_in_if<data_type>    if_type;
    typedef sc_port<if_type,1>            base_type;
    typedef sc_in<data_type>              this_type;

    typedef if_type                       in_if_type;
    typedef base_type                     in_port_type;
    typedef sc_signal_inout_if<data_type> inout_if_type;
    typedef sc_port<inout_if_type,1>      inout_port_type;

public:

    // constructors

    sc_in()
	: base_type(), m_traces( 0 )
	{}

    explicit sc_in( const char* name_ )
	: base_type( name_ ), m_traces( 0 )
	{}

    explicit sc_in( const in_if_type& interface_ )
        : base_type( CCAST<in_if_type&>( interface_ ) ), m_traces( 0 )
        {}

    sc_in( const char* name_, const in_if_type& interface_ )
	: base_type( name_, CCAST<in_if_type&>( interface_ ) ), m_traces( 0 )
	{}

    explicit sc_in( in_port_type& parent_ )
	: base_type( parent_ ), m_traces( 0 )
	{}

    sc_in( const char* name_, in_port_type& parent_ )
	: base_type( name_, parent_ ), m_traces( 0 )
	{}

    explicit sc_in( inout_port_type& parent_ )
	: base_type(), m_traces( 0 )
	{ sc_port_base::bind( parent_ ); }

    sc_in( const char* name_, inout_port_type& parent_ )
	: base_type( name_ ), m_traces( 0 )
	{ sc_port_base::bind( parent_ ); }

    sc_in( this_type& parent_ )
	: base_type( parent_ ), m_traces( 0 )
	{}

    sc_in( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ ), m_traces( 0 )
	{}


    // destructor

    virtual ~sc_in()
	{ remove_traces(); }


    // bind to in interface

    void bind( const in_if_type& interface_ )
	{ sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }

    void operator () ( const in_if_type& interface_ )
	{ sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }


    // bind to parent in port

    void bind( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }

    void operator () ( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }


    // bind to parent inout port

    void bind( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }

    void operator () ( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }


    // interface access shortcut methods

    // get the default event

    const sc_event& default_event() const
	{ return (*this)->default_event(); }


    // get the value changed event

    const sc_event& value_changed_event() const
	{ return (*this)->value_changed_event(); }


    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::value_changed_event );
    }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();

    virtual const char* kind() const
        { return "sc_in"; }


    // called by sc_trace
    void add_trace( sc_trace_file*, const std::string& ) const;

protected:

    void remove_traces() const;

    mutable sc_trace_params_vec* m_traces;

protected:

    // called by pbind (for internal use only)
    virtual int vbind( sc_interface& );
    virtual int vbind( sc_port_base& );

private:

    // disabled
    sc_in( const this_type& );
    this_type& operator = ( const this_type& );

#ifdef __GNUC__
    // Needed to circumvent a problem in the g++-2.95.2 compiler:
    // This unused variable forces the compiler to instantiate
    // an object of T template so an implicit conversion from
    // read() to a C++ intrinsic data type will work.
    static data_type dummy;
#endif
};

template<typename T>
::std::ostream& operator << ( ::std::ostream& os, const sc_in<T>& a )
{
    return os << a->read();
}

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII


// called when elaboration is done

template <class T>
inline
void
sc_in<T>::end_of_elaboration()
{
    if( m_traces != 0 ) {
	for( int i = 0; i < m_traces->size(); ++ i ) {
	    sc_trace_params* p = (*m_traces)[i];
	    in_if_type* iface = DCAST<in_if_type*>( this->get_interface() );
	    sc_trace( p->tf, iface->get_data_ref(), p->name );
	}
	remove_traces();
    }
}


// called by sc_trace

template <class T>
inline
void
sc_in<T>::add_trace( sc_trace_file* tf_, const std::string& name_ ) const
{
    if( tf_ != 0 ) {
	if( m_traces == 0 ) {
	    m_traces = new sc_trace_params_vec;
	}
	m_traces->push_back( new sc_trace_params( tf_, name_ ) );
    }
}

template <class T>
inline
void
sc_in<T>::remove_traces() const
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

template <class T>
inline
int
sc_in<T>::vbind( sc_interface& interface_ )
{
    return sc_port_b<if_type>::vbind( interface_ );
}

template <class T>
inline
int
sc_in<T>::vbind( sc_port_base& parent_ )
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
//  CLASS : sc_in<bool>
//
//  Specialization of sc_in<T> for type bool.
// ----------------------------------------------------------------------------

template <>
class sc_in<bool>
: public sc_port<sc_signal_in_if<bool>,1> 
{
public:

    // typedefs

    typedef bool                          data_type;

    typedef sc_signal_in_if<data_type>    if_type;
    typedef sc_port<if_type,1>            base_type;
    typedef sc_in<data_type>              this_type;

    typedef if_type                       in_if_type;
    typedef base_type                     in_port_type;
    typedef sc_signal_inout_if<data_type> inout_if_type;
    typedef sc_port<inout_if_type,1>      inout_port_type;

public:

    // constructors

    sc_in()
	: base_type(), m_traces( 0 )
	{}

    explicit sc_in( const char* name_ )
	: base_type( name_ ), m_traces( 0 )
	{}

    explicit sc_in( const in_if_type& interface_ )
	: base_type( CCAST<in_if_type&>( interface_ ) ), m_traces( 0 )
	{}

    sc_in( const char* name_, const in_if_type& interface_ )
	: base_type( name_, CCAST<in_if_type&>( interface_ ) ), m_traces( 0 )
	{}

    explicit sc_in( in_port_type& parent_ )
	: base_type( parent_ ), m_traces( 0 )
	{}

    sc_in( const char* name_, in_port_type& parent_ )
	: base_type( name_, parent_ ), m_traces( 0 )
	{}

    explicit sc_in( inout_port_type& parent_ )
	: base_type(), m_traces( 0 )
	{ sc_port_base::bind( parent_ ); }

    sc_in( const char* name_, inout_port_type& parent_ )
	: base_type( name_ ), m_traces( 0 )
	{ sc_port_base::bind( parent_ ); }

    sc_in( this_type& parent_ )
	: base_type( parent_ ), m_traces( 0 )
	{}

#if defined(TESTING)
    sc_in( const this_type& parent_ )
	: base_type( *(in_if_type*)parent_.get_interface() ) , m_traces( 0 )
	{}
#endif 

    sc_in( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ ), m_traces( 0 )
	{}


    // destructor

    virtual ~sc_in()
	{ remove_traces(); }


    // bind to in interface

    void bind( const in_if_type& interface_ )
	{ sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }

    void operator () ( const in_if_type& interface_ )
	{ sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }


    // bind to parent in port

    void bind( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }

    void operator () ( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }


    // bind to parent inout port

    void bind( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }

    void operator () ( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }


    // interface access shortcut methods

    // get the default event

    const sc_event& default_event() const
	{ return (*this)->default_event(); }


    // get the value changed event

    const sc_event& value_changed_event() const
	{ return (*this)->value_changed_event(); }

    // get the positive edge event

    const sc_event& posedge_event() const
	{ return (*this)->posedge_event(); }

    // get the negative edge event

    const sc_event& negedge_event() const
	{ return (*this)->negedge_event(); }


    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // use for positive edge sensitivity

    sc_event_finder& pos() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::posedge_event );
    }

    // use for negative edge sensitivity

    sc_event_finder& neg() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::negedge_event );
    }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }

    // was there a positive edge event?

    bool posedge() const
        { return (*this)->posedge(); }

    // was there a negative edge event?

    bool negedge() const
        { return (*this)->negedge(); }


    // delayed evaluation
    const sc_signal_bool_deval& delayed() const;

    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::value_changed_event );
    }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();

    virtual const char* kind() const
        { return "sc_in"; }


    // called by sc_trace
    void add_trace( sc_trace_file*, const std::string& ) const;

protected:

    void remove_traces() const;

    mutable sc_trace_params_vec* m_traces;

protected:

    // called by pbind (for internal use only)
    virtual int vbind( sc_interface& );
    virtual int vbind( sc_port_base& );

private:

    // disabled
#if defined(TESTING)
#else
    sc_in( const this_type& );
#endif 
    this_type& operator = ( const this_type& );

#ifdef __GNUC__
    // Needed to circumvent a problem in the g++-2.95.2 compiler:
    // This unused variable forces the compiler to instantiate
    // an object of T template so an implicit conversion from
    // read() to a C++ intrinsic data type will work.
    static data_type dummy;
#endif
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// delayed evaluation

inline
const sc_signal_bool_deval&
sc_in<bool>::delayed() const
{
    const in_if_type* iface = DCAST<const in_if_type*>( get_interface() );
    if( iface != 0 ) {
	return RCAST<const sc_signal_bool_deval&>( *iface );
    } else {
	// the tricky part
	const sc_port_base* pb = this;
	return RCAST<const sc_signal_bool_deval&>( *pb );
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_in<sc_dt::sc_logic>
//
//  Specialization of sc_in<T> for type sc_dt::sc_logic.
// ----------------------------------------------------------------------------

template <>
class sc_in<sc_dt::sc_logic>
: public sc_port<sc_signal_in_if<sc_dt::sc_logic>,1>
{
public:

    // typedefs

    typedef sc_dt::sc_logic               data_type;

    typedef sc_signal_in_if<data_type>    if_type;
    typedef sc_port<if_type,1>            base_type;
    typedef sc_in<data_type>              this_type;

    typedef if_type                       in_if_type;
    typedef base_type                     in_port_type;
    typedef sc_signal_inout_if<data_type> inout_if_type;
    typedef sc_port<inout_if_type,1>      inout_port_type;

public:

    // constructors

    sc_in()
	: base_type(), m_traces( 0 )
	{}

    explicit sc_in( const char* name_ )
	: base_type( name_ ), m_traces( 0 )
	{}

    explicit sc_in( const in_if_type& interface_ )
	: base_type( CCAST<in_if_type&>( interface_ ) ), m_traces( 0 )
	{}

    sc_in( const char* name_, const in_if_type& interface_ )
	: base_type( name_, CCAST<in_if_type&>( interface_ ) ), m_traces( 0 )
	{}

    explicit sc_in( in_port_type& parent_ )
	: base_type( parent_ ), m_traces( 0 )
	{}

    sc_in( const char* name_, in_port_type& parent_ )
	: base_type( name_, parent_ ), m_traces( 0 )
	{}

    explicit sc_in( inout_port_type& parent_ )
	: base_type(), m_traces( 0 )
	{ sc_port_base::bind( parent_ ); }

    sc_in( const char* name_, inout_port_type& parent_ )
	: base_type( name_ ), m_traces( 0 )
	{ sc_port_base::bind( parent_ ); }

    sc_in( this_type& parent_ )
	: base_type( parent_ ), m_traces( 0 )
	{}

    sc_in( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ ), m_traces( 0 )
	{}


    // destructor

    virtual ~sc_in()
	{ remove_traces(); }


    // bind to in interface

    void bind( const in_if_type& interface_ )
	{ sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }

    void operator () ( const in_if_type& interface_ )
	{ sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }


    // bind to parent in port

    void bind( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }

    void operator () ( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }


    // bind to parent inout port

    void bind( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }

    void operator () ( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }


    // interface access shortcut methods

    // get the default event

    const sc_event& default_event() const
	{ return (*this)->default_event(); }


    // get the value changed event

    const sc_event& value_changed_event() const
	{ return (*this)->value_changed_event(); }

    // get the positive edge event

    const sc_event& posedge_event() const
	{ return (*this)->posedge_event(); }

    // get the negative edge event

    const sc_event& negedge_event() const
	{ return (*this)->negedge_event(); }


    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // use for positive edge sensitivity

    sc_event_finder& pos() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::posedge_event );
    }

    // use for negative edge sensitivity

    sc_event_finder& neg() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::negedge_event );
    }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }

    // was there a positive edge event?

    bool posedge() const
        { return (*this)->posedge(); }

    // was there a negative edge event?

    bool negedge() const
        { return (*this)->negedge(); }


    // delayed evaluation
    const sc_signal_logic_deval& delayed() const;


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::value_changed_event );
    }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();

    virtual const char* kind() const
        { return "sc_in"; }


    // called by sc_trace
    void add_trace( sc_trace_file*, const std::string& ) const;

protected:

    void remove_traces() const;

    mutable sc_trace_params_vec* m_traces;

protected:

    // called by pbind (for internal use only)
    virtual int vbind( sc_interface& );
    virtual int vbind( sc_port_base& );

private:

    // disabled
    sc_in( const this_type& );
    this_type& operator = ( const this_type& );

#ifdef __GNUC__
    // Needed to circumvent a problem in the g++-2.95.2 compiler:
    // This unused variable forces the compiler to instantiate
    // an object of T template so an implicit conversion from
    // read() to a C++ intrinsic data type will work.
    static data_type dummy;
#endif
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// delayed evaluation

inline
const sc_signal_logic_deval&
sc_in<sc_dt::sc_logic>::delayed() const
{
    const in_if_type* iface = DCAST<const in_if_type*>( get_interface() );
    if( iface != 0 ) {
	return RCAST<const sc_signal_logic_deval&>( *iface );
    } else {
	// the tricky part
	const sc_port_base* pb = this;
	return RCAST<const sc_signal_logic_deval&>( *pb );
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_inout<T>
//
//  The sc_signal<T> input/output port class.
// ----------------------------------------------------------------------------

template <class T>
class sc_inout
: public sc_port<sc_signal_inout_if<T>,1>
{
public:

    // typedefs

    typedef T                             data_type;

    typedef sc_signal_inout_if<data_type> if_type;
    typedef sc_port<if_type,1>            base_type;
    typedef sc_inout<data_type>           this_type;

    typedef sc_signal_in_if<data_type>    in_if_type;
    typedef sc_port<in_if_type,1>         in_port_type;
    typedef if_type                       inout_if_type;
    typedef base_type                     inout_port_type;

public:

    // constructors

    sc_inout()
	: base_type(), m_init_val( 0 ), m_traces( 0 )
	{}

    explicit sc_inout( const char* name_ )
	: base_type( name_ ), m_init_val( 0 ), m_traces( 0 )
	{}

    explicit sc_inout( inout_if_type& interface_ )
	: base_type( interface_ ), m_init_val( 0 ), m_traces( 0 )
	{}

    sc_inout( const char* name_, inout_if_type& interface_ )
	: base_type( name_, interface_ ), m_init_val( 0 ), m_traces( 0 )
	{}

    explicit sc_inout( inout_port_type& parent_ )
	: base_type( parent_ ), m_init_val( 0 ), m_traces( 0 )
	{}

    sc_inout( const char* name_, inout_port_type& parent_ )
	: base_type( name_, parent_ ), m_init_val( 0 ), m_traces( 0 )
	{}

    sc_inout( this_type& parent_ )
	: base_type( parent_ ), m_init_val( 0 ), m_traces( 0 )
	{}

    sc_inout( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ ), m_init_val( 0 ), m_traces( 0 )
	{}


    // destructor

    virtual ~sc_inout();


    // interface access shortcut methods

    // get the default event

    const sc_event& default_event() const
	{ return (*this)->default_event(); }


    // get the value changed event

    const sc_event& value_changed_event() const
	{ return (*this)->value_changed_event(); }


    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }


    // write the new value

    void write( const data_type& value_ )
	{ (*this)->write( value_ ); }

    this_type& operator = ( const data_type& value_ )
	{ (*this)->write( value_ ); return *this; }

    this_type& operator = ( const in_if_type& interface_ )
	{ (*this)->write( interface_.read() ); return *this; }

    this_type& operator = ( const in_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const inout_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const this_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }


    // set initial value (can also be called when port is not bound yet)

    void initialize( const data_type& value_ );

    void initialize( const in_if_type& interface_ )
	{ initialize( interface_.read() ); }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::value_changed_event );
    }

    virtual const char* kind() const
        { return "sc_inout"; }

protected:

    data_type* m_init_val;

public:

    // called by sc_trace
    void add_trace( sc_trace_file*, const std::string& ) const;

protected:

    void remove_traces() const;

    mutable sc_trace_params_vec* m_traces;

private:

    // disabled
    sc_inout( const this_type& );

#ifdef __GNUC__
    // Needed to circumvent a problem in the g++-2.95.2 compiler:
    // This unused variable forces the compiler to instantiate
    // an object of T template so an implicit conversion from
    // read() to a C++ intrinsic data type will work.
    static data_type dummy;
#endif
};

template<typename T>
::std::ostream& operator << ( ::std::ostream& os, const sc_inout<T>& a )
{
    return os << a->read();
}

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII


// destructor

template <class T>
inline
sc_inout<T>::~sc_inout()
{
    if( m_init_val != 0 ) {
	delete m_init_val;
    }
    remove_traces();
}


// set initial value (can also be called when port is not bound yet)

template <class T>
inline
void
sc_inout<T>::initialize( const data_type& value_ )
{
    inout_if_type* iface = DCAST<inout_if_type*>( this->get_interface() );
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

template <class T>
inline
void
sc_inout<T>::end_of_elaboration()
{
    if( m_init_val != 0 ) {
	write( *m_init_val );
	delete m_init_val;
	m_init_val = 0;
    }
    if( m_traces != 0 ) {
	for( int i = 0; i < m_traces->size(); ++ i ) {
	    sc_trace_params* p = (*m_traces)[i];
	    in_if_type* iface = DCAST<in_if_type*>( this->get_interface() );
	    sc_trace( p->tf, iface->get_data_ref(), p->name );
	}
	remove_traces();
    }
}


// called by sc_trace

template <class T>
inline
void
sc_inout<T>::add_trace( sc_trace_file* tf_, const std::string& name_) const
{
    if( tf_ != 0 ) {
	    if( m_traces == 0 ) {
	        m_traces = new sc_trace_params_vec;
	    }
	    m_traces->push_back( new sc_trace_params( tf_, name_ ) );
    }
}

template <class T>
inline
void
sc_inout<T>::remove_traces() const
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
//  CLASS : sc_inout<bool>
//
//  Specialization of sc_inout<T> for type bool.
// ----------------------------------------------------------------------------

template <>
class sc_inout<bool>
: public sc_port<sc_signal_inout_if<bool>,1>
{
public:

    // typedefs

    typedef bool                          data_type;

    typedef sc_signal_inout_if<data_type> if_type;
    typedef sc_port<if_type,1>            base_type;
    typedef sc_inout<data_type>           this_type;

    typedef sc_signal_in_if<data_type>    in_if_type;
    typedef sc_port<in_if_type,1>         in_port_type;
    typedef if_type                       inout_if_type;
    typedef base_type                     inout_port_type;

public:

    // constructors

    sc_inout()
	: base_type(), m_init_val( 0 ), m_traces( 0 )
	{}

    explicit sc_inout( const char* name_ )
	: base_type( name_ ), m_init_val( 0 ), m_traces( 0 )
	{}

    explicit sc_inout( inout_if_type& interface_ )
	: base_type( interface_ ), m_init_val( 0 ), m_traces( 0 )
	{}

    sc_inout( const char* name_, inout_if_type& interface_ )
	: base_type( name_, interface_ ), m_init_val( 0 ), m_traces( 0 )
	{}

    explicit sc_inout( inout_port_type& parent_ )
	: base_type( parent_ ), m_init_val( 0 ), m_traces( 0 )
	{}

    sc_inout( const char* name_, inout_port_type& parent_ )
	: base_type( name_, parent_ ), m_init_val( 0 ), m_traces( 0 )
	{}

    sc_inout( this_type& parent_ )
	: base_type( parent_ ), m_init_val( 0 ), m_traces( 0 )
	{}

    sc_inout( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ ), m_init_val( 0 ), m_traces( 0 )
	{}


    // destructor

    virtual ~sc_inout();


    // interface access shortcut methods

    // get the default event

    const sc_event& default_event() const
	{ return (*this)->default_event(); }


    // get the value changed event

    const sc_event& value_changed_event() const
	{ return (*this)->value_changed_event(); }

    // get the positive edge event

    const sc_event& posedge_event() const
	{ return (*this)->posedge_event(); }

    // get the negative edge event

    const sc_event& negedge_event() const
	{ return (*this)->negedge_event(); }


    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // use for positive edge sensitivity

    sc_event_finder& pos() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::posedge_event );
    }

    // use for negative edge sensitivity

    sc_event_finder& neg() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::negedge_event );
    }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }

    // was there a positive edge event?

    bool posedge() const
        { return (*this)->posedge(); }

    // was there a negative edge event?

    bool negedge() const
        { return (*this)->negedge(); }


    // delayed evaluation
    const sc_signal_bool_deval& delayed() const;

    // write the new value

    void write( const data_type& value_ )
	{ (*this)->write( value_ ); }

    this_type& operator = ( const data_type& value_ )
	{ (*this)->write( value_ ); return *this; }

    this_type& operator = ( const in_if_type& interface_ )
	{ (*this)->write( interface_.read() ); return *this; }

    this_type& operator = ( const in_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const inout_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const this_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }


    // set initial value (can also be called when port is not bound yet)

    void initialize( const data_type& value_ );

    void initialize( const in_if_type& interface_ )
	{ initialize( interface_.read() ); }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::value_changed_event );
    }

    virtual const char* kind() const
        { return "sc_inout"; }

protected:

    data_type* m_init_val;

public:

    // called by sc_trace
    void add_trace( sc_trace_file*, const std::string& ) const;

protected:

    void remove_traces() const;

    mutable sc_trace_params_vec* m_traces;

private:

    // disabled
    sc_inout( const this_type& );

#ifdef __GNUC__
    // Needed to circumvent a problem in the g++-2.95.2 compiler:
    // This unused variable forces the compiler to instantiate
    // an object of T template so an implicit conversion from
    // read() to a C++ intrinsic data type will work.
    static data_type dummy;
#endif
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// delayed evaluation

inline
const sc_signal_bool_deval&
sc_inout<bool>::delayed() const
{
    const in_if_type* iface = DCAST<const in_if_type*>( get_interface() );
    if( iface != 0 ) {
	return RCAST<const sc_signal_bool_deval&>( *iface );
    } else {
	// the tricky part
	const sc_port_base* pb = this;
	return RCAST<const sc_signal_bool_deval&>( *pb );
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_inout<sc_dt::sc_logic>
//
//  Specialization of sc_inout<T> for type sc_dt::sc_logic.
// ----------------------------------------------------------------------------

template <>
class sc_inout<sc_dt::sc_logic>
: public sc_port<sc_signal_inout_if<sc_dt::sc_logic>,1>
{
public:

    // typedefs

    typedef sc_dt::sc_logic               data_type;

    typedef sc_signal_inout_if<data_type> if_type;
    typedef sc_port<if_type,1>            base_type;
    typedef sc_inout<data_type>           this_type;

    typedef sc_signal_in_if<data_type>    in_if_type;
    typedef sc_port<in_if_type,1>         in_port_type;
    typedef if_type                       inout_if_type;
    typedef base_type                     inout_port_type;

public:

    // constructors

    sc_inout()
	: base_type(), m_init_val( 0 ), m_traces( 0 )
	{}

    explicit sc_inout( const char* name_ )
	: base_type( name_ ), m_init_val( 0 ), m_traces( 0 )
	{}

    explicit sc_inout( inout_if_type& interface_ )
	: base_type( interface_ ), m_init_val( 0 ), m_traces( 0 )
	{}

    sc_inout( const char* name_, inout_if_type& interface_ )
	: base_type( name_, interface_ ), m_init_val( 0 ), m_traces( 0 )
	{}

    explicit sc_inout( inout_port_type& parent_ )
	: base_type( parent_ ), m_init_val( 0 ), m_traces( 0 )
	{}

    sc_inout( const char* name_, inout_port_type& parent_ )
	: base_type( name_, parent_ ), m_init_val( 0 ), m_traces( 0 )
	{}

    sc_inout( this_type& parent_ )
	: base_type( parent_ ), m_init_val( 0 ), m_traces( 0 )
	{}

    sc_inout( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ ), m_init_val( 0 ), m_traces( 0 )
	{}


    // destructor

    virtual ~sc_inout();


    // interface access shortcut methods

    // get the default event

    const sc_event& default_event() const
	{ return (*this)->default_event(); }


    // get the value changed event

    const sc_event& value_changed_event() const
	{ return (*this)->value_changed_event(); }

    // get the positive edge event

    const sc_event& posedge_event() const
	{ return (*this)->posedge_event(); }

    // get the negative edge event

    const sc_event& negedge_event() const
	{ return (*this)->negedge_event(); }


    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // use for positive edge sensitivity

    sc_event_finder& pos() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::posedge_event );
    }

    // use for negative edge sensitivity

    sc_event_finder& neg() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::negedge_event );
    }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }

    // was there a positive edge event?

    bool posedge() const
        { return (*this)->posedge(); }

    // was there a negative edge event?

    bool negedge() const
        { return (*this)->negedge(); }


    // delayed evaluation
    const sc_signal_logic_deval& delayed() const;


    // write the new value

    void write( const data_type& value_ )
	{ (*this)->write( value_ ); }

    this_type& operator = ( const data_type& value_ )
	{ (*this)->write( value_ ); return *this; }

    this_type& operator = ( const in_if_type& interface_ )
	{ (*this)->write( interface_.read() ); return *this; }

    this_type& operator = ( const in_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const inout_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const this_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }


    // set initial value (can also be called when port is not bound yet)

    void initialize( const data_type& value_ );

    void initialize( const in_if_type& interface_ )
	{ initialize( interface_.read() ); }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::value_changed_event );
    }

    virtual const char* kind() const
        { return "sc_inout"; }

protected:

    data_type* m_init_val;

public:

    // called by sc_trace
    void add_trace( sc_trace_file*, const std::string& ) const;

protected:

    void remove_traces() const;

    mutable sc_trace_params_vec* m_traces;

private:

    // disabled
    sc_inout( const this_type& );

#ifdef __GNUC__
    // Needed to circumvent a problem in the g++-2.95.2 compiler:
    // This unused variable forces the compiler to instantiate
    // an object of T template so an implicit conversion from
    // read() to a C++ intrinsic data type will work.
    static data_type dummy;
#endif
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// delayed evaluation

inline
const sc_signal_logic_deval&
sc_inout<sc_dt::sc_logic>::delayed() const
{
    const in_if_type* iface = DCAST<const in_if_type*>( get_interface() );
    if( iface != 0 ) {
	return RCAST<const sc_signal_logic_deval&>( *iface );
    } else {
	// the tricky part
	const sc_port_base* pb = this;
	return RCAST<const sc_signal_logic_deval&>( *pb );
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_out<T>
//
//  The sc_signal<T> output port class.
// ----------------------------------------------------------------------------

// sc_out can also read from its port, hence no difference with sc_inout.
// For debugging reasons, a class is provided instead of a define.

template <class T>
class sc_out
: public sc_inout<T>
{
public:

    // typedefs

    typedef T                                   data_type;

    typedef sc_out<data_type>                   this_type;
    typedef sc_inout<data_type>                 base_type;

    typedef typename base_type::in_if_type      in_if_type;
    typedef typename base_type::in_port_type    in_port_type;
    typedef typename base_type::inout_if_type   inout_if_type;
    typedef typename base_type::inout_port_type inout_port_type;

public:

    // constructors

    sc_out()
	: base_type()
	{}

    explicit sc_out( const char* name_ )
	: base_type( name_ )
	{}

    explicit sc_out( inout_if_type& interface_ )
	: base_type( interface_ )
	{}

    sc_out( const char* name_, inout_if_type& interface_ )
	: base_type( name_, interface_ )
	{}

    explicit sc_out( inout_port_type& parent_ )
	: base_type( parent_ )
	{}

    sc_out( const char* name_, inout_port_type& parent_ )
	: base_type( name_, parent_ )
	{}

    sc_out( this_type& parent_ )
	: base_type( parent_ )
	{}

    sc_out( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ )
	{}


    // destructor (does nothing)

    virtual ~sc_out()
	{}


    // write the new value

    this_type& operator = ( const data_type& value_ )
	{ (*this)->write( value_ ); return *this; }

    this_type& operator = ( const in_if_type& interface_ )
	{ (*this)->write( interface_.read() ); return *this; }

    this_type& operator = ( const in_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const inout_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const this_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    virtual const char* kind() const
        { return "sc_out"; }

private:

    // disabled
    sc_out( const this_type& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII


// ----------------------------------------------------------------------------
//  FUNCTION : sc_trace
// ----------------------------------------------------------------------------

template <class T>
inline
void
sc_trace(sc_trace_file* tf, const sc_in<T>& port, const std::string& name)
{
    const sc_signal_in_if<T>* iface = 0;
    if (sc_get_curr_simcontext()->elaboration_done() )
    {
	iface = DCAST<const sc_signal_in_if<T>*>( port.get_interface() );
    }

    if ( iface )
	sc_trace( tf, iface->get_data_ref(), name );
    else
	port.add_trace( tf, name );
}

template <class T>
inline
void
sc_trace( sc_trace_file* tf, const sc_inout<T>& port, 
    const std::string& name )
{
    const sc_signal_in_if<T>* iface = 0;
    if (sc_get_curr_simcontext()->elaboration_done() )
    {
	iface =DCAST<const sc_signal_in_if<T>*>( port.get_interface() );
    }

    if ( iface )
	sc_trace( tf, iface->get_data_ref(), name );
    else
	port.add_trace( tf, name );
}

} // namespace sc_core

#endif

// Taf!
