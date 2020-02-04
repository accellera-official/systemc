/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_signal.h -- The sc_signal<T> primitive channel class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

  CHANGE LOG IS AT THE END OF THE FILE
 *****************************************************************************/

#ifndef SC_SIGNAL_H
#define SC_SIGNAL_H

#include "sysc/communication/sc_port.h"
#include "sysc/communication/sc_prim_channel.h"
#include "sysc/communication/sc_signal_ifs.h"
#include "sysc/communication/sc_writer_policy.h"
#include "sysc/kernel/sc_event.h"
#include "sysc/kernel/sc_process.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/datatypes/bit/sc_logic.h"
#include "sysc/tracing/sc_trace.h"
#include <typeinfo>

namespace sc_core {

inline
bool
sc_writer_policy_check_write::check_write( sc_object* target, bool /*value_changed*/ )
{
  sc_process_b* writer_p = sc_get_curr_simcontext()->get_current_writer();
  if( SC_UNLIKELY_( !m_writer_p.valid() ) ) {
    // always store first writer
    sc_process_handle( writer_p ).swap( m_writer_p );
  } else if( SC_UNLIKELY_(m_writer_p != writer_p && writer_p != 0) ) {
    // Alternative option: only flag error, if either
    //  - we enforce conflicts across multiple evaluation phases, or
    //  - the new value is different from the previous write
    //if( !m_delta_only || value_changed )
    {
      sc_signal_invalid_writer( target, m_writer_p, writer_p, m_delta_only );
      // error has been suppressed, accept check as well,
      // but update current writer to the last "successful" one
      sc_process_handle( writer_p ).swap( m_writer_p );
    }
  }
  return true;
}


inline void
sc_writer_policy_check_write::update()
{
  if( m_delta_only ) // reset, if we're only checking for delta conflicts
    sc_process_handle().swap( m_writer_p );
}


// ----------------------------------------------------------------------------
//  CLASS : sc_signal_channel
//
//  The sc_signal type-agnostic primitive channel base class.
// ----------------------------------------------------------------------------

class SC_API sc_signal_channel
  : public sc_prim_channel
{
protected:

    sc_signal_channel( const char* name_ )
      : sc_prim_channel( name_ )
      , m_change_event_p( 0 )
      , m_change_stamp( ~sc_dt::UINT64_ONE )
    {}

public:

    virtual ~sc_signal_channel();

    // interface methods

    virtual const char* kind() const
        { return "sc_signal_channel"; }

    // get the default event
    const sc_event& default_event() const
        { return value_changed_event(); }

    // get the value changed event
    const sc_event& value_changed_event() const;

    // was there an event?
    bool event() const
        { return simcontext()->event_occurred(m_change_stamp); }

protected:
    void do_update();

    // reporting to avoid code bloat in sc_signal_t

    void deprecated_get_data_ref()  const;
    void deprecated_get_new_value() const;
    void deprecated_trace()         const;

    sc_event* lazy_kernel_event( sc_event**, const char* ) const;
    void notify_next_delta( sc_event* ev ) const
        { if( ev ) ev->notify_next_delta(); }

protected:
    mutable sc_event* m_change_event_p;  // value change event if present.
    sc_dt::uint64     m_change_stamp;    // delta of last event

private:
    // disabled
    sc_signal_channel( const sc_signal_channel& ) /* = delete */;
    sc_signal_channel& operator=( const sc_signal_channel& ) /* = delete */;
};


inline
::std::ostream&
operator << ( ::std::ostream& os, const sc_signal_channel& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_signal_t<T, POL> (implementation-defined)
//
//  The generic sc_signal<T,POL> primitive channel base class
//  (to reduce complexity of specialisations for bool and sc_logic)
// ----------------------------------------------------------------------------

template< class T, sc_writer_policy POL >
class sc_signal_t
  : public    sc_signal_inout_if<T>
  , public    sc_signal_channel
  , protected sc_writer_policy_check<POL>
{
protected:
    typedef sc_signal_inout_if<T>       if_type;
    typedef sc_signal_channel           base_type;
    typedef sc_signal_t<T,POL>          this_type;
    typedef sc_writer_policy_check<POL> policy_type;

protected:

    // constructor and destructor

    sc_signal_t( const char* name_, const T& initial_value_ )
      : base_type( name_ )
      , m_cur_val( initial_value_ )
      , m_new_val( initial_value_ )
    {}

    virtual ~sc_signal_t() {} /* = default; */

public:

    // interface methods

    virtual const char* kind() const
        { return "sc_signal"; }

    virtual void register_port( sc_port_base&, const char* );

    virtual sc_writer_policy get_writer_policy() const
      { return POL; }

    // get the default event
    virtual const sc_event& default_event() const
      { return value_changed_event(); }

    // get the value changed event
    virtual const sc_event& value_changed_event() const
        { return base_type::value_changed_event(); }

    // read the current value
    virtual const T& read() const
	{ return m_cur_val; }

    // get a reference to the current value (for tracing)
    virtual const T& get_data_ref() const
    {
        deprecated_get_data_ref();
        return m_cur_val;
    }

    // was there an event?
    virtual bool event() const
        { return base_type::event(); }

    // write the new value
    virtual void write( const T& );


    // other methods

    operator const T& () const
	{ return read(); }


    // assignment
    this_type& operator = ( const T& a )
      { write( a ); return *this; }

    this_type& operator = ( const sc_signal_in_if<T>& a )
      { write( a.read() ); return *this; }

    this_type& operator = ( const this_type& a )
      { write( a.read() ); return *this; }


    const T& get_new_value() const
    {
        deprecated_get_new_value();
        return m_new_val;
    }


    void trace( sc_trace_file* tf ) const
	{ 
            deprecated_trace();
#           ifdef DEBUG_SYSTEMC
	        sc_trace( tf, read(), name() ); 
#           else
                if ( tf ) {}
#	    endif
	}


    virtual void print( ::std::ostream& = ::std::cout ) const;
    virtual void dump( ::std::ostream& = ::std::cout ) const;


protected:

    virtual void update();
            void do_update();

protected:
    T m_cur_val;         // current value of object.
    T m_new_val;         // next value of object.

private:
    // disabled
    sc_signal_t( const sc_signal_t& ) /* = delete */;
};

// ----------------------------------------------------------------------------

template< class T, sc_writer_policy POL >
inline
void
sc_signal_t<T,POL>::register_port( sc_port_base& port_
                                 , const char* if_typename_ )
{
    bool is_output = std::string( if_typename_ ) == typeid(if_type).name();
    if( !policy_type::check_port( this, &port_, is_output ) )
       ((void)0); // fallback? error has been suppressed ...
}


// write the new value

template< class T, sc_writer_policy POL >
inline
void
sc_signal_t<T,POL>::write( const T& value_ )
{
    // first write per eval phase: m_new_val == m_cur_val
    bool value_changed = !( m_new_val == value_ );
    if ( !policy_type::check_write(this, value_changed) )
        return;

    m_new_val = value_;
    if( value_changed || policy_type::needs_update() ) {
        request_update();
    }
}


template< class T, sc_writer_policy POL >
inline
void
sc_signal_t<T,POL>::print( ::std::ostream& os ) const
{
    os << m_cur_val;
}

template< class T, sc_writer_policy POL >
void
sc_signal_t<T,POL>::dump( ::std::ostream& os ) const
{
    os << "     name = " << name() << ::std::endl;
    os << "    value = " << m_cur_val << ::std::endl;
    os << "new value = " << m_new_val << ::std::endl;
}


template< class T, sc_writer_policy POL >
void
sc_signal_t<T,POL>::update()
{
    policy_type::update();
    if( !( m_new_val == m_cur_val ) ) {
        do_update();
    }
}

template< class T, sc_writer_policy POL >
inline void
sc_signal_t<T,POL>::do_update()
{
    base_type::do_update();
    m_cur_val = m_new_val;
}

// ----------------------------------------------------------------------------
//  CLASS : sc_signal<T, POL>
//
//  The sc_signal<T,POL> primitive channel class
// ----------------------------------------------------------------------------

template< class T, sc_writer_policy POL = SC_DEFAULT_WRITER_POLICY >
class sc_signal
  : public sc_signal_t<T,POL>
{
public:
    typedef sc_signal_inout_if<T>       if_type;
    typedef sc_signal_t<T,POL>          base_type;
    typedef sc_signal<T,POL>            this_type;
    typedef T                           value_type;
    typedef sc_writer_policy_check<POL> policy_type;

    // constructors and destructor

    sc_signal()
      : base_type( sc_gen_unique_name( "signal" ), value_type() )
    {}

    explicit
    sc_signal( const char* name_ )
      : base_type( name_, value_type() )
    {}

    sc_signal( const char* name_, const value_type& initial_value_ )
      : base_type( name_, initial_value_ )
    {}

    virtual ~sc_signal() {} /* = default; */

    // assignment
    using base_type::operator=;
    this_type& operator = ( const this_type& a )
      { base_type::operator=(a); return *this; }

private:
    // disabled
    sc_signal( const sc_signal& ) /* = delete */;
};


// ----------------------------------------------------------------------------
//  CLASS : sc_signal<bool>
//
//  Specialization of sc_signal<T> for type bool.
// ----------------------------------------------------------------------------

class SC_API sc_reset;

SC_API_TEMPLATE_DECL_ sc_signal_t<bool,SC_ONE_WRITER>;
SC_API_TEMPLATE_DECL_ sc_signal_t<bool,SC_MANY_WRITERS>;
SC_API_TEMPLATE_DECL_ sc_signal_t<bool,SC_UNCHECKED_WRITERS>;

template< sc_writer_policy POL >
class SC_API sc_signal<bool,POL>
  : public sc_signal_t<bool,POL>
{
protected:
    typedef sc_signal_t<bool,POL>       base_type;
    typedef sc_signal<bool,POL>         this_type;
    typedef bool                        value_type;
    typedef sc_writer_policy_check<POL> policy_type;

public:

    // constructors and destructor

    sc_signal()
      : base_type( sc_gen_unique_name( "signal" ), value_type() )
      , m_negedge_event_p( 0 ) , m_posedge_event_p( 0 ) , m_reset_p( 0 )
    {}

    explicit
    sc_signal( const char* name_ )
      : base_type( name_, value_type() )
      , m_negedge_event_p( 0 ) , m_posedge_event_p( 0 ) , m_reset_p( 0 )
    {}

    sc_signal( const char* name_, const value_type& initial_value_ )
      : base_type( name_, initial_value_ )
      , m_negedge_event_p( 0 ) , m_posedge_event_p( 0 ) , m_reset_p( 0 )
    {}

    virtual ~sc_signal();

    // get the positive edge event
    virtual const sc_event& posedge_event() const;

    // get the negative edge event
    virtual const sc_event& negedge_event() const;

    // was there a positive edge event?
    virtual bool posedge() const
        { return ( this->event() && this->m_cur_val ); }

    // was there a negative edge event?
    virtual bool negedge() const
        { return ( this->event() && ! this->m_cur_val ); }


    // assignment
    using base_type::operator=;
    this_type& operator = ( const this_type& a )
      { base_type::operator=(a); return *this; }

protected:

    virtual void update();
            void do_update();

    virtual bool is_clock() const { return false; }

protected:
    mutable sc_event* m_negedge_event_p; // negative edge event if present.
    mutable sc_event* m_posedge_event_p; // positive edge event if present.
    mutable sc_reset* m_reset_p;         // reset mechanism if present.

private:
    // reset creation
    virtual sc_reset* is_reset() const;

    // disabled
    sc_signal( const this_type& ) /* = delete */;
};


// ----------------------------------------------------------------------------
//  CLASS : sc_signal<sc_dt::sc_logic>
//
//  Specialization of sc_signal<T> for type sc_dt::sc_logic.
// ----------------------------------------------------------------------------

SC_API_TEMPLATE_DECL_ sc_signal_t<sc_dt::sc_logic,SC_ONE_WRITER>;
SC_API_TEMPLATE_DECL_ sc_signal_t<sc_dt::sc_logic,SC_MANY_WRITERS>;
SC_API_TEMPLATE_DECL_ sc_signal_t<sc_dt::sc_logic,SC_UNCHECKED_WRITERS>;

template< sc_writer_policy POL >
class SC_API sc_signal<sc_dt::sc_logic,POL>
  : public sc_signal_t<sc_dt::sc_logic,POL>
{
protected:
    typedef sc_signal_t<sc_dt::sc_logic,POL>    base_type;
    typedef sc_signal<sc_dt::sc_logic,POL>      this_type;
    typedef sc_dt::sc_logic                     value_type;
    typedef sc_writer_policy_check<POL> policy_type;

public:

    sc_signal()
      : base_type( sc_gen_unique_name( "signal" ), value_type() )
      , m_negedge_event_p( 0 ) , m_posedge_event_p( 0 )
    {}

    explicit
    sc_signal( const char* name_ )
      : base_type( name_, value_type() )
      , m_negedge_event_p( 0 ) , m_posedge_event_p( 0 )
    {}

    sc_signal( const char* name_, const value_type& initial_value_ )
      : base_type( name_, initial_value_ )
      , m_negedge_event_p( 0 ) , m_posedge_event_p( 0 )
    {}

    virtual ~sc_signal();

    // get the positive edge event
    virtual const sc_event& posedge_event() const;

    // get the negative edge event
    virtual const sc_event& negedge_event() const;


    // was there a positive edge event?
    virtual bool posedge() const
        { return ( this->event() && this->m_cur_val == sc_dt::SC_LOGIC_1 ); }

    // was there a negative edge event?
    virtual bool negedge() const
        { return ( this->event() && this->m_cur_val == sc_dt::SC_LOGIC_0 ); }


    // assignment
    using base_type::operator=;
    this_type& operator = ( const this_type& a )
      { base_type::operator=(a); return *this; }

protected:

    virtual void update();
            void do_update();

protected:
    mutable sc_event* m_negedge_event_p; // negative edge event if present.
    mutable sc_event* m_posedge_event_p; // positive edge event if present.

private:
    // disabled
    sc_signal( const this_type& ) /* = delete */;
};

} // namespace sc_core

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
//$Log: sc_signal.h,v $
//Revision 1.16  2011/08/26 20:45:42  acg
// Andy Goodrich: moved the modification log to the end of the file to
// eliminate source line number skew when check-ins are done.
//
//Revision 1.15  2011/08/15 16:43:24  acg
// Torsten Maehne: changes to remove unused argument warnings.
//
//Revision 1.14  2011/06/25 17:08:38  acg
// Andy Goodrich: Jerome Cornet's changes to use libtool to build the
// library.
//
//Revision 1.13  2011/04/13 02:59:09  acg
// Andy Goodrich: made events internal to signals into kernel events.
//
//Revision 1.12  2011/04/08 18:22:46  acg
// Philipp A. Hartmann: use the context of the primitive channel to get
// the change stamp value.
//
//Revision 1.11  2011/04/05 20:48:09  acg
// Andy Goodrich: changes to make sure that event(), posedge() and negedge()
// only return true if the clock has not moved.
//
//Revision 1.10  2011/04/05 07:10:55  acg
// Andy Goodrich: added line that I dropped in sc_signal<sc_dt::sc_logic>.
//
//Revision 1.9  2011/04/05 06:15:18  acg
// Philipp A. Hartmann: sc_writer_policy: ignore no-ops in delta check.
//
//Revision 1.8  2011/03/23 16:17:22  acg
// Andy Goodrich: hide the sc_events that are kernel related.
//
//Revision 1.7  2011/03/06 15:55:08  acg
// Andy Goodrich: Changes for named events.
//
//Revision 1.6  2011/02/18 20:23:45  acg
// Andy Goodrich: Copyright update.
//
//Revision 1.5  2011/02/07 19:16:50  acg
// Andy Goodrich: changes for handling multiple writers.
//
//Revision 1.4  2011/01/25 20:50:37  acg
// Andy Goodrich: changes for IEEE 1666 2011.
//
//Revision 1.3  2010/12/07 19:50:37  acg
// Andy Goodrich: addition of writer policies, courtesy of Philipp Hartmann.
//
//Revision 1.1.1.1  2006/12/15 20:20:04  acg
//SystemC 2.3
//
//Revision 1.14  2006/05/08 17:52:47  acg
// Andy Goodrich:
//   (1) added David Long's forward declarations for friend functions,
//       methods, and operators to keep the Microsoft compiler happy.
//   (2) Added delta_count() method to sc_prim_channel for use by
//       sc_signal so that the friend declaration in sc_simcontext.h
//	   can be for a non-templated class (i.e., sc_prim_channel.)
//
//Revision 1.12  2006/04/11 23:11:57  acg
//  Andy Goodrich: Changes for reset support that only includes
//  sc_cthread_process instances.
//
//Revision 1.11  2006/03/13 20:19:44  acg
// Andy Goodrich: changed sc_event instances into pointers to sc_event instances
// that are allocated as needed. This saves considerable storage for large
// numbers of signals, etc.
//
//Revision 1.10  2006/01/26 21:00:50  acg
// Andy Goodrich: conversion to use sc_event::notify(SC_ZERO_TIME) instead of
// sc_event::notify_delayed()
//
//Revision 1.9  2006/01/24 20:45:41  acg
//Andy Goodrich: converted notify_delayed() calls into notify_next_delta() calls
//to eliminate deprecation warnings. notify_next_delta() is an implemenation-
//dependent method of sc_event. It is simpler than notify_delayed(), and should
//speed up simulation speeds.
//
//Revision 1.8  2006/01/19 19:18:25  acg
//Andy Goodrich: eliminated check_writer in favor of inline code within the
//write() method since we always execute the check_writer code even when
//check writing is turned off.
//
//Revision 1.7  2006/01/19 00:30:57  acg
//Andy Goodrich: Yet another implementation for disabling write checks on
//signals. This version uses an environment variable, SC_SIGNAL_WRITE_CHECK,
//that when set to DISABLE will turn off write checking.
//
//Revision 1.6  2006/01/18 21:42:26  acg
//Andy Goodrich: Changes for check writer support, and tightening up sc_clock
//port usage.
//
//Revision 1.5  2006/01/13 20:41:59  acg
//Andy Goodrich: Changes to add port registration to the things that are
//checked when SC_NO_WRITE_CHECK is not defined.
//
//Revision 1.4  2006/01/13 18:47:20  acg
//Reversed sense of multiwriter signal check. It now defaults to ON unless the
//user defines SC_NO_WRITE_CHEK before inclusion of the file.
//
//Revision 1.3  2006/01/03 23:18:26  acg
//Changed copyright to include 2006.
//
//Revision 1.2  2005/12/20 21:58:18  acg
//Removed Makefile.in, changed the event() methods to use sc_simcontext::event_occurred.
//
//Revision 1.1.1.1  2005/12/19 23:16:43  acg
//First check in of SystemC 2.1 into its own archive.
//
//Revision 1.19  2005/09/15 23:01:51  acg
//Added std:: prefix to appropriate methods and types to get around
//issues with the Edison Front End.
//
//Revision 1.18  2005/06/10 22:43:55  acg
//Added CVS change log annotation.
//

#endif

// Taf!
