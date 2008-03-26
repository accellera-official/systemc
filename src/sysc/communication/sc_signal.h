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

  sc_signal.h -- The sc_signal<T> primitive channel class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

  Modification log appears at the end of the file
 *****************************************************************************/


#ifndef SC_SIGNAL_H
#define SC_SIGNAL_H


#include "sysc/communication/sc_port.h"
#include "sysc/communication/sc_prim_channel.h"
#include "sysc/communication/sc_signal_ifs.h"
#include "sysc/utils/sc_string.h"
#include "sysc/kernel/sc_event.h"
#include "sysc/kernel/sc_process.h"
#include "sysc/kernel/sc_reset.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/datatypes/bit/sc_logic.h"
#include "sysc/tracing/sc_trace.h"
#include <typeinfo>

namespace sc_core {

// to avoid code bloat in sc_signal<T>

extern void sc_deprecated_get_data_ref();
extern void sc_deprecated_get_new_value();
extern void sc_deprecated_trace();

extern
void
sc_signal_invalid_writer(
    sc_object* target, sc_object* first_writer, sc_object* second_writer );


// ----------------------------------------------------------------------------
//  CLASS : sc_signal<T>
//
//  The sc_signal<T> primitive channel class.
// ----------------------------------------------------------------------------

template <class T>
class sc_signal
: public sc_signal_inout_if<T>,
  public sc_prim_channel
{
  public: // constructors and destructor:

    sc_signal()
	: sc_prim_channel( sc_gen_unique_name( "signal" ) ),
	  m_change_event_p( 0 ), m_cur_val( T() ), 
	  m_delta( ~sc_dt::UINT64_ONE ), m_new_val( T() ), m_output( 0 ), 
	  m_writer( 0 ) 
	{}

    explicit sc_signal( const char* name_ )
	: sc_prim_channel( name_ ),
	  m_change_event_p( 0 ), m_cur_val( T() ), 
	  m_delta( ~sc_dt::UINT64_ONE ), m_new_val( T() ), m_output( 0 ), 
	  m_writer( 0 ) 
    {}


    virtual ~sc_signal()
	{
	    if ( !m_change_event_p ) delete m_change_event_p;
	}


    // interface methods

    virtual void register_port( sc_port_base&, const char* );


    // get the default event
    virtual const sc_event& default_event() const
	{ 
	    if ( !m_change_event_p ) m_change_event_p = new sc_event; 
	    return *m_change_event_p; 
	}


    // get the value changed event
    virtual const sc_event& value_changed_event() const
	{ 
	    if ( !m_change_event_p ) m_change_event_p = new sc_event; 
	    return *m_change_event_p; 
	}


    // read the current value
    virtual const T& read() const
	{ return m_cur_val; }

    // get a reference to the current value (for tracing)
    virtual const T& get_data_ref() const
        { sc_deprecated_get_data_ref(); return m_cur_val; }


    // was there an event?
    virtual bool event() const
        { return simcontext()->event_occurred(m_delta); }

    // write the new value
    virtual void write( const T& );


    // other methods

    operator const T& () const
	{ return read(); }


    sc_signal<T>& operator = ( const T& a )
	{ write( a ); return *this; }

    sc_signal<T>& operator = ( const sc_signal<T>& a )
	{ write( a.read() ); return *this; }


    const T& get_new_value() const
	{ return m_new_val; }


    void trace( sc_trace_file* tf ) const
	{ 
	    sc_deprecated_trace();
#           ifdef DEBUG_SYSTEMC
	        sc_trace( tf, read(), name() ); 
#	    endif
	}


    virtual void print( ::std::ostream& = ::std::cout ) const;
    virtual void dump( ::std::ostream& = ::std::cout ) const;

    virtual const char* kind() const
        { return "sc_signal"; }

protected:

    virtual void update();

protected:

    mutable sc_event*  m_change_event_p;
    T                  m_cur_val;
    sc_dt::uint64      m_delta;   // delta of last event
    T                  m_new_val;
    sc_port_base*      m_output; // used for static design rule checking
    sc_object*         m_writer; // used for dynamic design rule checking



private:

    // disabled
    sc_signal( const sc_signal<T>& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII


template <class T>
inline
void
sc_signal<T>::register_port( sc_port_base& port_, const char* if_typename_ )
{
    if ( sc_get_curr_simcontext()->write_check() )
    {
	std::string nm( if_typename_ );
	if( nm == typeid( sc_signal_inout_if<T> ).name() ) {
	    // an out or inout port; only one can be connected
	    if( m_output != 0) {
		sc_signal_invalid_writer( this, m_output, &port_ );
	    }
	    m_output = &port_;
	}
    }
}


// write the new value

template <class T>
inline
void
sc_signal<T>::write( const T& value_ )
{
    sc_object* writer = sc_get_curr_simcontext()->get_current_writer();
    if( m_writer == 0 ) {
	m_writer = writer;
    } else if( m_writer != writer ) {
	sc_signal_invalid_writer( this, m_writer, writer );
    }

    m_new_val = value_;
    if( !( m_new_val == m_cur_val ) ) {
	request_update();
    }
}


template <class T>
inline
void
sc_signal<T>::print( ::std::ostream& os ) const
{
    os << m_cur_val;
}

template <class T>
inline
void
sc_signal<T>::dump( ::std::ostream& os ) const
{
    os << "     name = " << name() << ::std::endl;
    os << "    value = " << m_cur_val << ::std::endl;
    os << "new value = " << m_new_val << ::std::endl;
}


template <class T>
inline
void
sc_signal<T>::update()
{
    if( !( m_new_val == m_cur_val ) ) {
	m_cur_val = m_new_val;
	if ( m_change_event_p ) m_change_event_p->notify_next_delta();
	m_delta = delta_count();
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_signal<bool>
//
//  Specialization of sc_signal<T> for type bool.
// ----------------------------------------------------------------------------

class sc_reset;

template <>
class sc_signal<bool>
: public sc_signal_inout_if<bool>,
  public sc_prim_channel
{
public: // constructors and destructor:

    sc_signal()
	: sc_prim_channel( sc_gen_unique_name( "signal" ) ),
	  m_change_event_p( 0 ),
          m_cur_val( false ),
          m_delta( ~sc_dt::UINT64_ONE ),
	  m_negedge_event_p( 0 ),
          m_new_val( false ),
          m_output( 0 ),
	  m_posedge_event_p( 0 ),
          m_reset_p( 0 ),
          m_writer( 0 )
	{}

    explicit sc_signal( const char* name_ )
	: sc_prim_channel( name_ ),
	  m_change_event_p( 0 ),
          m_cur_val( false ),
          m_delta( ~sc_dt::UINT64_ONE ),
	  m_negedge_event_p( 0 ),
          m_new_val( false ),
          m_output( 0 ),
	  m_posedge_event_p( 0 ),
          m_reset_p( 0 ),
          m_writer( 0 )
	{}

    virtual ~sc_signal();


    // interface methods

    virtual void register_port( sc_port_base&, const char* );


    // get the default event
    virtual const sc_event& default_event() const
	{ 
	    if ( !m_change_event_p ) m_change_event_p = new sc_event; 
	    return *m_change_event_p; 
	}


    // get the value changed event
    virtual const sc_event& value_changed_event() const
	{ 
	    if ( !m_change_event_p ) m_change_event_p = new sc_event; 
	    return *m_change_event_p; 
	}

    // get the positive edge event
    virtual const sc_event& posedge_event() const
	{ 
	    if ( !m_posedge_event_p )
	        m_posedge_event_p = new sc_event; 
	    return *m_posedge_event_p; 
	}

    // get the negative edge event
    virtual const sc_event& negedge_event() const
	{ 
	    if ( !m_negedge_event_p )
	        m_negedge_event_p = new sc_event; 
	    return *m_negedge_event_p; 
	}


    // read the current value
    virtual const bool& read() const
	{ return m_cur_val; }

    // get a reference to the current value (for tracing)
    virtual const bool& get_data_ref() const
        { sc_deprecated_get_data_ref(); return m_cur_val; }


    // was there a value changed event?
    virtual bool event() const
        { return simcontext()->event_occurred(m_delta); }

    // was there a positive edge event?
    virtual bool posedge() const
	{ return ( event() && m_cur_val ); }

    // was there a negative edge event?
    virtual bool negedge() const
	{ return ( event() && ! m_cur_val ); }

    // reset creation

    virtual sc_reset* is_reset() const;

    // write the new value
    virtual void write( const bool& );

    // other methods

    operator const bool& () const
	{ return read(); }


    sc_signal<bool>& operator = ( const bool& a )
	{ write( a ); return *this; }

    sc_signal<bool>& operator = ( const sc_signal<bool>& a )
	{ write( a.read() ); return *this; }


    const bool& get_new_value() const
	{ return m_new_val; }


    void trace( sc_trace_file* tf ) const
	{
	    sc_deprecated_trace();
#           ifdef DEBUG_SYSTEMC
	        sc_trace( tf, read(), name() ); 
#           endif
	}


    virtual void print( ::std::ostream& = ::std::cout ) const;
    virtual void dump( ::std::ostream& = ::std::cout ) const;

    virtual const char* kind() const
        { return "sc_signal"; }

protected:

    virtual void update();

    virtual bool is_clock() const { return false; }

protected:
    mutable sc_event* m_change_event_p;  // value change event if present.
    bool              m_cur_val;         // current value of object.
    sc_dt::uint64     m_delta;           // delta of last event
    mutable sc_event* m_negedge_event_p; // negative edge event if present.
    bool              m_new_val;         // next value of object.
    sc_port_base*     m_output;          // used for static design rule checking
    mutable sc_event* m_posedge_event_p; // positive edge event if present.
    mutable sc_reset* m_reset_p;         // reset mechanism if present.
    sc_object*        m_writer;          // process writing this object's value.

private:

    // disabled
    sc_signal( const sc_signal<bool>& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
void
sc_signal<bool>::register_port( sc_port_base& port_, const char* if_typename_ )
{
    if ( sc_get_curr_simcontext()->write_check() )
    {
	std::string nm( if_typename_ );
	if( nm == typeid( sc_signal_inout_if<bool> ).name() ) {
	    // an out or inout port; only one can be connected
	    if( m_output != 0 ) {
		sc_signal_invalid_writer( this, m_output, &port_ );
	    }
	    m_output = &port_;
	}
    }
}


// write the new value

inline
void
sc_signal<bool>::write( const bool& value_ )
{
    sc_object* writer = sc_get_curr_simcontext()->get_current_writer();
    if( m_writer == 0 ) {
	m_writer = writer;
    } else if( m_writer != writer ) {
	sc_signal_invalid_writer( this, m_writer, writer );
    }

    m_new_val = value_;
    if( !( m_new_val == m_cur_val ) ) {
	request_update();
    }
}


inline
void
sc_signal<bool>::print( ::std::ostream& os ) const
{
    os << m_cur_val;
}

inline
void
sc_signal<bool>::dump( ::std::ostream& os ) const
{
    os << "     name = " << name() << ::std::endl;
    os << "    value = " << m_cur_val << ::std::endl;
    os << "new value = " << m_new_val << ::std::endl;
}


inline
void
sc_signal<bool>::update()
{
    if( !( m_new_val == m_cur_val ) ) {
        // order of execution below is important, the notify_processes() call
        // must come after the update of m_cur_val for things to work properly!
        m_cur_val = m_new_val;
	if ( m_reset_p ) m_reset_p->notify_processes();

        if ( m_change_event_p ) m_change_event_p->notify_next_delta();
        if( m_cur_val ) {
            if ( m_posedge_event_p ) m_posedge_event_p->notify_next_delta();
        } else {
            if ( m_negedge_event_p ) m_negedge_event_p->notify_next_delta();
        }
        m_delta = delta_count();
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_signal<sc_dt::sc_logic>
//
//  Specialization of sc_signal<T> for type sc_dt::sc_logic.
// ----------------------------------------------------------------------------

template <>
class sc_signal<sc_dt::sc_logic>
: public sc_signal_inout_if<sc_dt::sc_logic>,
  public sc_prim_channel
{
  public: // constructors and destructor:

    sc_signal()
	: sc_prim_channel( sc_gen_unique_name( "signal" ) ),
	  m_change_event_p( 0 ),
	  m_cur_val(),
          m_delta( ~sc_dt::UINT64_ONE ),
	  m_negedge_event_p( 0 ),
	  m_new_val(),
          m_output( 0 ),
	  m_posedge_event_p( 0 ),
	  m_writer( 0 )
	{}

    explicit sc_signal( const char* name_ )
	: sc_prim_channel( name_ ),
	  m_change_event_p( 0 ),
	  m_cur_val(),
          m_delta( ~sc_dt::UINT64_ONE ),
	  m_negedge_event_p( 0 ),
	  m_new_val(),
          m_output( 0 ),
	  m_posedge_event_p( 0 ),
	  m_writer( 0 )
	{}

    virtual ~sc_signal()
	{
	    if ( !m_change_event_p ) delete m_change_event_p;
	    if ( !m_negedge_event_p ) delete m_negedge_event_p;
	    if ( !m_posedge_event_p ) delete m_posedge_event_p;
	}


    // interface methods

    virtual void register_port( sc_port_base&, const char* );


    // get the default event
    virtual const sc_event& default_event() const
	{ 
	    if ( !m_change_event_p ) m_change_event_p = new sc_event; 
	    return *m_change_event_p; 
	}


    // get the value changed event
    virtual const sc_event& value_changed_event() const
	{ 
	    if ( !m_change_event_p ) m_change_event_p = new sc_event; 
	    return *m_change_event_p; 
	}

    // get the positive edge event
    virtual const sc_event& posedge_event() const
	{ 
	    if ( !m_posedge_event_p )
	        m_posedge_event_p = new sc_event; 
	    return *m_posedge_event_p; 
	}

    // get the negative edge event
    virtual const sc_event& negedge_event() const
	{ 
	    if ( !m_negedge_event_p )
	        m_negedge_event_p = new sc_event; 
	    return *m_negedge_event_p; 
	}


    // read the current value
    virtual const sc_dt::sc_logic& read() const
	{ return m_cur_val; }

    // get a reference to the current value (for tracing)
    virtual const sc_dt::sc_logic& get_data_ref() const
        { sc_deprecated_get_data_ref(); return m_cur_val; }


    // was there an event?
    virtual bool event() const
        { return simcontext()->event_occurred(m_delta); }

    // was there a positive edge event?
    virtual bool posedge() const
	{ return ( event() && m_cur_val == sc_dt::SC_LOGIC_1 ); }

    // was there a negative edge event?
    virtual bool negedge() const
	{ return ( event() && m_cur_val == sc_dt::SC_LOGIC_0 ); }


    // write the new value
    virtual void write( const sc_dt::sc_logic& );


    // other methods

    operator const sc_dt::sc_logic& () const
	{ return read(); }


    sc_signal<sc_dt::sc_logic>& operator = ( const sc_dt::sc_logic& a )
	{ write( a ); return *this; }

    sc_signal<sc_dt::sc_logic>& operator = (const sc_signal<sc_dt::sc_logic>& a)
	{ write( a.read() ); return *this; }


    const sc_dt::sc_logic& get_new_value() const
	{ return m_new_val; }


    void trace( sc_trace_file* tf ) const
	{
	    sc_deprecated_trace();
#           ifdef DEBUG_SYSTEMC
	        sc_trace( tf, read(), name() ); 
#           endif
	}

    virtual void print( ::std::ostream& = ::std::cout ) const;
    virtual void dump( ::std::ostream& = ::std::cout ) const;

    virtual const char* kind() const
        { return "sc_signal"; }

protected:

    virtual void update();

protected:

    mutable sc_event* m_change_event_p;  // value change event if present.
    sc_dt::sc_logic   m_cur_val;         // current value of object.
    sc_dt::uint64     m_delta;           // delta of last event
    mutable sc_event* m_negedge_event_p; // negative edge event if present.
    sc_dt::sc_logic   m_new_val;         // next value of object.
    sc_port_base*     m_output;          // used for static design rule checking
    mutable sc_event* m_posedge_event_p; // positive edge event if present.
    sc_object*        m_writer;          // process writing this object's value.

private:

    // disabled
    sc_signal( const sc_signal<sc_dt::sc_logic>& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
void
sc_signal<sc_dt::sc_logic>::register_port( sc_port_base& port_,
				    const char* if_typename_ )
{
    if ( sc_get_curr_simcontext()->write_check() )
    {
	std::string nm( if_typename_ );
	if( nm == typeid( sc_signal_inout_if<sc_dt::sc_logic> ).name() ) {
	    // an out or inout port; only one can be connected
	    if( m_output != 0 ) {
		sc_signal_invalid_writer( this, m_output, &port_ );
	    }
	    m_output = &port_;
	}
    }
}


// write the new value

inline
void
sc_signal<sc_dt::sc_logic>::write( const sc_dt::sc_logic& value_ )
{
    sc_object* writer = sc_get_curr_simcontext()->get_current_writer();
    if( m_writer == 0 ) {
	m_writer = writer;
    } else if( m_writer != writer ) {
	sc_signal_invalid_writer( this, m_writer, writer );
    }

    m_new_val = value_;
    if( !( m_new_val == m_cur_val ) ) {
	request_update();
    }
}


inline
void
sc_signal<sc_dt::sc_logic>::print( ::std::ostream& os ) const
{
    os << m_cur_val;
}

inline
void
sc_signal<sc_dt::sc_logic>::dump( ::std::ostream& os ) const
{
    os << "     name = " << name() << ::std::endl;
    os << "    value = " << m_cur_val << ::std::endl;
    os << "new value = " << m_new_val << ::std::endl;
}


inline
void
sc_signal<sc_dt::sc_logic>::update()
{
    if( !( m_new_val == m_cur_val ) ) {
	m_cur_val = m_new_val;
	if ( m_change_event_p ) m_change_event_p->notify_next_delta();
	if( m_posedge_event_p && (m_cur_val == sc_dt::SC_LOGIC_1) ) {
	    m_posedge_event_p->notify_next_delta();
	} else if( m_negedge_event_p && (m_cur_val == sc_dt::SC_LOGIC_0) ) {
	    m_negedge_event_p->notify_next_delta();
	}
	m_delta = delta_count();
    }
}

// ----------------------------------------------------------------------------

template <class T>
inline
::std::ostream&
operator << ( ::std::ostream& os, const sc_signal<T>& a )
{
    return ( os << a.read() );
}

} // namespace sc_core

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/
//$Log: sc_signal.h,v $
//Revision 1.1.1.1  2006/12/15 20:31:35  acg
//SystemC 2.2
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
