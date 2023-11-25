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

  sc_stub.h -- sc_stub, sc_unbound, sc_tie

  Original Authors: Martin Barnasconi, Ralph Goergen, NXP B.V. 2021/06/01

  CHANGE LOG IS AT THE END OF THE FILE
 *****************************************************************************/

#ifndef SC_STUB_H
#define SC_STUB_H

#include "sysc/communication/sc_signal_ifs.h"
#include "sysc/communication/sc_prim_channel.h"
#include "sysc/communication/sc_communication_ids.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/datatypes/bit/sc_logic.h"

namespace sc_core {


// ----------------------------------------------------------------------------
//  CLASS : sc_stub_registry (implementation-defined)
//
//  Registry for stubs. 
// ----------------------------------------------------------------------------

class sc_stub_registry
{
  friend class sc_simcontext;

public:
  void insert( sc_prim_channel* stub_ ) {
    if( sc_is_running() ) {
      SC_REPORT_ERROR( SC_ID_INSERT_STUB_, "simulation running" );
      return;
    }

    if( m_simc->elaboration_done() ) {
      SC_REPORT_ERROR( SC_ID_INSERT_STUB_, "elaboration done" );
      return;
    }

    // insert
    m_stub_vec.push_back( stub_ );
  }

  int size() const { 
    return static_cast<int>(m_stub_vec.size());
  }

  void clear() { 
    for (int i = 0; i < size(); i++) {
      delete m_stub_vec[i];
    }
  }

private:
  // constructor
  explicit sc_stub_registry( sc_simcontext& simc_ )
  :  m_stub_vec(),  m_simc( &simc_ ) { }

  // destructor
  ~sc_stub_registry() {
    clear();
  };

private:
    std::vector<sc_prim_channel*> m_stub_vec;
    sc_simcontext*                m_simc;

private:
    // disabled
    sc_stub_registry();
    sc_stub_registry( const sc_stub_registry& );
    sc_stub_registry& operator = ( const sc_stub_registry& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_stub (implementation-defined)
//
//  Class sc_stub shall define a predefined channel which acts as an stub.
//  The class shall be used for the definition of sc_unbound and sc_tie.
// ----------------------------------------------------------------------------

template < typename T >
class sc_stub : public sc_core::sc_signal_inout_if<T>, public sc_prim_channel
{
public:
  typedef sc_stub<T> this_type;

  explicit sc_stub( const char* nm )
  : sc_core::sc_prim_channel(nm) {}

  explicit sc_stub( const char* nm, const T& val )
  : sc_core::sc_prim_channel(nm), m_init_val(val) {}

  // reading a stub returns the initial value
  // note: The value read from the channel is implementation-defined
  const T& read() const { return m_init_val; }
  operator const T& () const { return read(); }

  // writing to a stub should do nothing.
  virtual void write( const T& ) { /* do nothing */ }
  this_type& operator = ( const T& ){ return *this; }
  this_type& operator = ( const sc_signal_in_if<T>& ) { return *this; }
  this_type& operator = ( const this_type& ) { return *this; }

  const char* kind() const { return "sc_stub"; }

  void print( ::std::ostream& os = ::std::cout ) const { os << "sc_stub"; }
  void dump( ::std::ostream& os = ::std::cout ) const { os << "sc_stub"; }

  // no event handling, since there are no value changes
  // TODO: generate warning message?
  const sc_event& default_event() const { return ev; }
  const sc_event& value_changed_event() const { return ev; }
  const sc_event& posedge_event() const { return ev; }
  const sc_event& negedge_event() const { return ev; }
  bool event() const { return false; }
  bool posedge() const { return false; }
  bool negedge() const { return false; }

  // get a reference to the current value (for tracing)
  const T& get_data_ref() const { return m_init_val; }

  ~sc_stub() {}

private:
  T m_init_val;
  sc_event ev;
}; // class sc_stub


// ----------------------------------------------------------------------------
//  CLASS : sc_unbound_impl (implementation-defined)
//
//  The class sc_unbound_impl is used to create object sc_unbound
// ----------------------------------------------------------------------------

struct sc_unbound_impl // implementation-defined
{
  // Only allowed to connect sc_unbound to interfaces of type output and inout
  template < typename T >
  operator sc_core::sc_signal_inout_if<T> & () const
  {
    sc_core::sc_stub<T>* stub = new sc_core::sc_stub<T>(sc_core::sc_gen_unique_name("sc_unbound"));
    sc_get_curr_simcontext()->get_stub_registry()->insert(dynamic_cast<sc_core::sc_prim_channel*>(stub));
    return *stub;
  }
}; // struct sc_unbound_impl


// ----------------------------------------------------------------------------
//  sc_unbound
//
//  The static object sc_unbound shall define a predefined channel which acts as
//  an open connection. Each time sc_unbound is used in an application, a new
//  predefined channel shall be created with a name with prefix "sc_unbound",
//  followed by an underscore and series of one of more decimal digits from
//  the character set 0-9.
//
//  It shall be error to bind sc_unbound to a port which is not of type
//  sc_port <sc_signal_inout_if<T> >. It shall be allowed to read and write
//  to this channel. The value read from the channel is implementation-defined.
//  Values written to the channel shall be ignored. The channel shall not
//  notify an event when values are written.
//
//  NOTE—An application cannot not use sc_unbound for input ports of type
//  sc_port <sc_signal_in_if<T> > because its value is undefined.
// ----------------------------------------------------------------------------

static sc_unbound_impl const /* implementation defined */  sc_unbound = {};

// ----------------------------------------------------------------------------
//  sc_tie
//
//  The function sc_tie::value shall return a predefined channel to tie
//  the port to the specified value of type T. The type of the value shall be
//  compatible with the type of the associated channel to which the port
//  is bound. Each time sc_tie is used in an application, a new
//  predefined channel shall be created with a name with prefix "sc_tie",
//  followed by an underscore and series of one of more decimal digits from
//  the character set 0-9.
//
//  It shall be error to bind sc_tie to a port which is not of type
//  sc_port<sc_signal_in_if<T> > or sc_port <sc_signal_inout_if<T> >.
//  It shall be allowed to read and write to this channel. The value read
//  from the channel shall correspond to the specified pre-defined types.
//  Values written to the channel shall be ignored. The channel shall not
//  notify an event when values are written.
// ----------------------------------------------------------------------------

namespace sc_tie {

  template < typename T >
  sc_core::sc_signal_in_if<T>& value(const T& val)
  {
    sc_core::sc_stub<T>* stub = new sc_core::sc_stub<T>(sc_core::sc_gen_unique_name("sc_tie::value"), val);
    sc_get_curr_simcontext()->get_stub_registry()->insert(dynamic_cast<sc_core::sc_prim_channel*>(stub));
    return *stub;
  }

} // namespace sc_tie

} // namespace sc_core


/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
//$Log: sc_stub.h,v $


#endif // SC_STUB_H

// Taf!
