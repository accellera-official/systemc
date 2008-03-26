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

  sc_fifo_ports.h -- The sc_fifo<T> port classes.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_FIFO_PORTS_H
#define SC_FIFO_PORTS_H


#include "sysc/communication/sc_port.h"
#include "sysc/communication/sc_fifo_ifs.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_fifo_in<T>
//
//  The sc_fifo<T> input port class.
// ----------------------------------------------------------------------------

template <class T>
class sc_fifo_in
: public sc_port<sc_fifo_in_if<T>,0>
{
public:

    // typedefs

    typedef T                        data_type;

    typedef sc_fifo_in_if<data_type> if_type;
    typedef sc_port<if_type,0>       base_type;
    typedef sc_fifo_in<data_type>    this_type;

    typedef if_type                  in_if_type;
    typedef sc_port_b<in_if_type>    in_port_type;

public:

    // constructors

    sc_fifo_in()
	: base_type()
	{}

    explicit sc_fifo_in( const char* name_ )
	: base_type( name_ )
	{}

    explicit sc_fifo_in( in_if_type& interface_ )
	: base_type( interface_ )
	{}

    sc_fifo_in( const char* name_, in_if_type& interface_ )
	: base_type( name_, interface_ )
	{}

    explicit sc_fifo_in( in_port_type& parent_ )
	: base_type( parent_ )
	{}

    sc_fifo_in( const char* name_, in_port_type& parent_ )
	: base_type( name_, parent_ )
	{}

    sc_fifo_in( this_type& parent_ )
	: base_type( parent_ )
	{}

    sc_fifo_in( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ )
	{}


    // destructor (does nothing)

    virtual ~sc_fifo_in()
	{}


    // interface access shortcut methods

    // blocking read

    void read( data_type& value_ )
        { (*this)->read( value_ ); }

    data_type read()
        { return (*this)->read(); }


    // non-blocking read

    bool nb_read( data_type& value_ )
        { return (*this)->nb_read( value_ ); }


    // get the number of available samples

    int num_available() const
        { return (*this)->num_available(); }


    // get the data written event

    const sc_event& data_written_event() const
	{ return (*this)->data_written_event(); }


    // use for static sensitivity to data written event

    sc_event_finder& data_written() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::data_written_event );
    }

    virtual const char* kind() const
        { return "sc_fifo_in"; }

private:

    // disabled
    sc_fifo_in( const this_type& );
    this_type& operator = ( const this_type& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS : sc_fifo_out<T>
//
//  The sc_fifo<T> output port class.
// ----------------------------------------------------------------------------

template <class T>
class sc_fifo_out
: public sc_port<sc_fifo_out_if<T>,0>
{
public:

    // typedefs

    typedef T                         data_type;

    typedef sc_fifo_out_if<data_type> if_type;
    typedef sc_port<if_type,0>        base_type;
    typedef sc_fifo_out<data_type>    this_type;

    typedef if_type                   out_if_type;
    typedef sc_port_b<out_if_type>    out_port_type;

public:

    // constructors

    sc_fifo_out()
	: base_type()
	{}

    explicit sc_fifo_out( const char* name_ )
	: base_type( name_ )
	{}

    explicit sc_fifo_out( out_if_type& interface_ )
	: base_type( interface_ )
	{}

    sc_fifo_out( const char* name_, out_if_type& interface_ )
	: base_type( name_, interface_ )
	{}

    explicit sc_fifo_out( out_port_type& parent_ )
	: base_type( parent_ )
	{}

    sc_fifo_out( const char* name_, out_port_type& parent_ )
	: base_type( name_, parent_ )
	{}

    sc_fifo_out( this_type& parent_ )
	: base_type( parent_ )
	{}

    sc_fifo_out( const char* name_, this_type& parent_ )
	: base_type( name_, parent_ )
	{}


    // destructor (does nothing)

    virtual ~sc_fifo_out()
	{}


    // interface access shortcut methods

    // blocking write

    void write( const data_type& value_ )
        { (*this)->write( value_ ); }


    // non-blocking write

    bool nb_write( const data_type& value_ )
        { return (*this)->nb_write( value_ ); }


    // get the number of free spaces

    int num_free() const
        { return (*this)->num_free(); }


    // get the data read event

    const sc_event& data_read_event() const
	{ return (*this)->data_read_event(); }


    // use for static sensitivity to data read event

    sc_event_finder& data_read() const
    {
	return *new sc_event_finder_t<out_if_type>(
	    *this, &out_if_type::data_read_event );
    }

    virtual const char* kind() const
        { return "sc_fifo_out"; }

private:

    // disabled
    sc_fifo_out( const this_type& );
    this_type& operator = ( const this_type& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

} // namespace sc_core

#endif

// Taf!
