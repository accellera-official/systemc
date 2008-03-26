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

  sc_buffer.h -- The sc_buffer<T> primitive channel class.
                 Like sc_signal<T>, but *every* write causes an event.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_BUFFER_H
#define SC_BUFFER_H


#include "sysc/communication/sc_signal.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_buffer<T>
//
//  The sc_buffer<T> primitive channel class.
// ----------------------------------------------------------------------------

template <class T>
class sc_buffer
: public sc_signal<T>
{
public:

    // typedefs

    typedef sc_buffer<T> this_type;
    typedef sc_signal<T> base_type;

public:

    // constructors

    sc_buffer()
	: base_type( sc_gen_unique_name( "buffer" ) )
	{}

    explicit sc_buffer( const char* name_ )
	: base_type( name_ )
	{}


    // interface methods

    // write the new value
    virtual void write( const T& );


    // other methods

    sc_buffer<T>& operator = ( const T& a )
	{ write( a ); return *this; }

    sc_buffer<T>& operator = ( const base_type& a )
	{ write( a.read() ); return *this; }

    sc_buffer<T>& operator = ( const this_type& a )
	{ write( a.read() ); return *this; }

    virtual const char* kind() const
        { return "sc_buffer"; }

protected:

    virtual void update();

private:

    // disabled
    sc_buffer( const sc_buffer<T>& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// write the new value

template <class T>
inline
void
sc_buffer<T>::write( const T& value_ )
{
#ifdef DEBUG_SYSTEMC
    this->check_writer();
#endif
    this->m_new_val = value_;
    this->request_update();
}


template <class T>
inline
void
sc_buffer<T>::update()
{
    this->m_cur_val = this->m_new_val;
    this->m_value_changed_event.notify_delayed();
    this->m_delta = this->simcontext()->delta_count();
}

} // namespace sc_core

#endif

// Taf!
