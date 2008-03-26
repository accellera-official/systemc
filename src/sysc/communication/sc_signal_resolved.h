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

  sc_signal_resolved.h -- The resolved signal class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_SIGNAL_RESOLVED_H
#define SC_SIGNAL_RESOLVED_H


#include "sysc/communication/sc_signal.h"

namespace sc_core {

class sc_process_b;

extern const sc_dt::sc_logic_value_t sc_logic_resolution_tbl[4][4];


// ----------------------------------------------------------------------------
//  CLASS : sc_logic_resolve
//
//  Resolution function for sc_dt::sc_logic.
// ----------------------------------------------------------------------------

class sc_logic_resolve
{
public:

    // resolves sc_dt::sc_logic values and returns the resolved value
    static void resolve(sc_dt::sc_logic&, const sc_pvector<sc_dt::sc_logic*>&);
};


// ----------------------------------------------------------------------------
//  CLASS : sc_signal_resolved
//
//  The resolved signal class.
// ----------------------------------------------------------------------------

class sc_signal_resolved
: public sc_signal<sc_dt::sc_logic>
{
public:

    // typedefs

    typedef sc_signal_resolved         this_type;
    typedef sc_signal<sc_dt::sc_logic> base_type;
    typedef sc_dt::sc_logic            data_type;

public:

    // constructors

    sc_signal_resolved()
        : base_type( sc_gen_unique_name( "signal_resolved" ) )
	{}

    explicit sc_signal_resolved( const char* name_ )
        : base_type( name_ )
	{}


    // destructor
    virtual ~sc_signal_resolved();


    // interface methods

    virtual void register_port( sc_port_base&, const char* )
	{}


    // write the new value
    virtual void write( const data_type& );


    // other methods

    this_type& operator = ( const data_type& a )
        { write( a ); return *this; }

    this_type& operator = ( const this_type& a )
        { write( a.read() ); return *this; }

    virtual const char* kind() const
        { return "sc_signal_resolved"; }

protected:

    virtual void update();

protected:

    sc_pvector<sc_process_b*> m_proc_vec; // processes writing to this signal
    sc_pvector<data_type*>    m_val_vec;  // new values written to this signal

private:

    // disabled
    sc_signal_resolved( const this_type& );
};

} // namespace sc_core

#endif

// Taf!
