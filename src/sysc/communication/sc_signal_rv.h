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

  sc_signal_rv.h -- The resolved vector signal class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_SIGNAL_RV_H
#define SC_SIGNAL_RV_H

#include "sysc/communication/sc_signal.h"
#include "sysc/datatypes/bit/sc_lv.h"

namespace sc_core {

class sc_process_b;


// ----------------------------------------------------------------------------
//  CLASS sc_lv_resolve<W>
//
//  Resolution function for sc_dt::sc_lv<W>.
// ----------------------------------------------------------------------------

extern const sc_dt::sc_logic_value_t sc_logic_resolution_tbl[4][4];


template <int W>
class sc_lv_resolve
{
public:

    // resolves sc_dt::sc_lv<W> values and returns the resolved value
    static void resolve(sc_dt::sc_lv<W>&, const sc_pvector<sc_dt::sc_lv<W>*>&);
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// resolves sc_dt::sc_lv<W> values and returns the resolved value

template <int W>
inline
void
sc_lv_resolve<W>::resolve( sc_dt::sc_lv<W>& result_,
			   const sc_pvector<sc_dt::sc_lv<W>*>& values_ )
{
    int sz = values_.size();

    assert( sz != 0 );

    if( sz == 1 ) {
	result_ = *values_[0];
	return;
    }

    for( int j = result_.length() - 1; j >= 0; -- j ) {
	sc_dt::sc_logic_value_t res = (*values_[0])[j].value();
	for( int i = sz - 1; i > 0 && res != 3; -- i ) {
	    res = sc_logic_resolution_tbl[res][(*values_[i])[j].value()];
	}
	result_[j] = res;
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_signal_rv<W>
//
//  The resolved vector signal class.
// ----------------------------------------------------------------------------

template <int W>
class sc_signal_rv
: public sc_signal<sc_dt::sc_lv<W> >
{
public:

    // typedefs

    typedef sc_signal_rv<W>             this_type;
    typedef sc_signal<sc_dt::sc_lv<W> > base_type;
    typedef sc_dt::sc_lv<W>             data_type;

public:

    // constructors

    sc_signal_rv()
        : base_type( sc_gen_unique_name( "signal_rv" ) )
	{}

    explicit sc_signal_rv( const char* name_ )
        : base_type( name_ )
	{}


    // destructor
    virtual ~sc_signal_rv();


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
        { return "sc_signal_rv"; }

protected:

    virtual void update();

protected:

    sc_pvector<sc_process_b*> m_proc_vec; // processes writing to this signal
    sc_pvector<data_type*>    m_val_vec;  // new values written to this signal

private:

    // disabled
    sc_signal_rv( const this_type& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII


// destructor

template <int W>
inline
sc_signal_rv<W>::~sc_signal_rv()
{
    for( int i = m_val_vec.size() - 1; i >= 0; -- i ) {
	delete m_val_vec[i];
    }
}


// write the new value

template <int W>
inline
void
sc_signal_rv<W>::write( const data_type& value_ )
{
    sc_process_b* cur_proc = sc_get_curr_process_handle();

    bool value_changed = false;
    bool found = false;
    
    for( int i = m_proc_vec.size() - 1; i >= 0; -- i ) {
	if( cur_proc == m_proc_vec[i] ) {
	    if( value_ != *m_val_vec[i] ) {
		*m_val_vec[i] = value_;
		value_changed = true;
	    }
	    found = true;
	    break;
	}
    }
    
    if( ! found ) {
	m_proc_vec.push_back( cur_proc );
	m_val_vec.push_back( new data_type( value_ ) );
	value_changed = true;
    }
    
    if( value_changed ) {
	this->request_update();
    }
}


template <int W>
inline
void
sc_signal_rv<W>::update()
{
    sc_lv_resolve<W>::resolve( this->m_new_val, m_val_vec );
    base_type::update();
}

} // namespace sc_core

#endif

// Taf!
