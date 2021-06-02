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

  sc_stub.h -- sc_stub, sc_open, sc_tie

  Original Authors: Martin Barnasconi, Ralph Goergen, NXP B.V. 2021/06/01

  CHANGE LOG IS AT THE END OF THE FILE
 *****************************************************************************/

#ifndef SC_STUB_H
#define SC_STUB_H

#include "sysc/communication/sc_signal_ifs.h"
#include "sysc/communication/sc_prim_channel.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/datatypes/bit/sc_logic.h"

namespace sc_core {


// ----------------------------------------------------------------------------
//  CLASS : sc_stub (implementation-defined)
//
//  Class sc_stub shall define a predefined channel which acts as an stub.
//  The class shall be used for the definition of sc_open and sc_tie.
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
//  CLASS : sc_open_impl (implementation-defined)
//
//  The class sc_open_impl is used to create object sc_open
// ----------------------------------------------------------------------------

struct sc_open_impl // implementation-defined
{
  // Only allowed to connect sc_open to interfaces of type output and inout
  template < typename T >
  operator sc_core::sc_signal_inout_if<T> & () const
  {
    return *(new sc_core::sc_stub<T>(sc_core::sc_gen_unique_name("sc_open")));
  }
}; // struct sc_open_impl


// ----------------------------------------------------------------------------
//  CLASS : sc_tie_high_impl (implementation-defined)
//
//  The class sc_tie_high_impl is used to create object sc_tie::high
// ----------------------------------------------------------------------------

class sc_tie_high_impl
{
public:
  operator sc_core::sc_signal_inout_if<sc_dt::sc_logic> & () const
  {
    return *(new sc_core::sc_stub<sc_dt::sc_logic>(sc_core::sc_gen_unique_name("sc_tie::high"), sc_dt::SC_LOGIC_1));
  }

  operator sc_core::sc_signal_inout_if<bool> & () const
  {
    return *(new sc_core::sc_stub<bool>(sc_core::sc_gen_unique_name("sc_tie::high"), true));
  }

  operator sc_core::sc_signal_inout_if<int> & () const
  {
    return *(new sc_core::sc_stub<int>(sc_core::sc_gen_unique_name("sc_tie::high"), 1));
  }

  operator sc_core::sc_signal_inout_if<double> & () const
  {
    return *(new sc_core::sc_stub<double>(sc_core::sc_gen_unique_name("sc_tie::high"), 1.0));
  }

  operator sc_core::sc_signal_in_if<sc_dt::sc_logic> & () const
  {
    return *(new sc_core::sc_stub<sc_dt::sc_logic>(sc_core::sc_gen_unique_name("sc_tie::high"), sc_dt::SC_LOGIC_1));
  }

  operator sc_core::sc_signal_in_if<bool> & () const
  {
    return *(new sc_core::sc_stub<bool>(sc_core::sc_gen_unique_name("sc_tie::high"), true));
  }

  operator sc_core::sc_signal_in_if<int> & () const
  {
    return *(new sc_core::sc_stub<int>(sc_core::sc_gen_unique_name("sc_tie::high"), 1));
  }

  operator sc_core::sc_signal_in_if<double> & () const
  {
    return *(new sc_core::sc_stub<double>(sc_core::sc_gen_unique_name("sc_tie::high"), 1.0));
  }
}; // class sc_tie_high_impl


// ----------------------------------------------------------------------------
//  CLASS : sc_tie_low_impl (implementation-defined)
//
//  The class sc_tie_low_impl is used to create object sc_tie::low
// ----------------------------------------------------------------------------

class sc_tie_low_impl
{
public:
  operator sc_core::sc_signal_inout_if<sc_dt::sc_logic> & () const
  {
    return *(new sc_core::sc_stub<sc_dt::sc_logic>(sc_core::sc_gen_unique_name("sc_tie::low"), sc_dt::SC_LOGIC_0));
  }

  operator sc_core::sc_signal_inout_if<bool> & () const
  {
    return *(new sc_core::sc_stub<bool>(sc_core::sc_gen_unique_name("sc_tie::low"), false));
  }

  operator sc_core::sc_signal_inout_if<int> & () const
  {
    return *(new sc_core::sc_stub<int>(sc_core::sc_gen_unique_name("sc_tie::low"), 0));
  }

  operator sc_core::sc_signal_inout_if<double> & () const
  {
    return *(new sc_core::sc_stub<double>(sc_core::sc_gen_unique_name("sc_tie::low"), 0.0));
  }

  operator sc_core::sc_signal_in_if<sc_dt::sc_logic> & () const
  {
    return *(new sc_core::sc_stub<sc_dt::sc_logic>(sc_core::sc_gen_unique_name("sc_tie::low"), sc_dt::SC_LOGIC_0));
  }

  operator sc_core::sc_signal_in_if<bool> & () const
  {
    return *(new sc_core::sc_stub<bool>(sc_core::sc_gen_unique_name("sc_tie::low"), false));
  }

  operator sc_core::sc_signal_in_if<int> & () const
  {
    return *(new sc_core::sc_stub<int>(sc_core::sc_gen_unique_name("sc_tie::low"), 0));
  }

  operator sc_core::sc_signal_in_if<double> & () const
  {
    return *(new sc_core::sc_stub<double>(sc_core::sc_gen_unique_name("sc_tie::low"), 0.0));
  }
}; // class sc_tie_low_impl

// ----------------------------------------------------------------------------
//  sc_open
//
//  The static object sc_open shall define a predefined channel which acts as
//  an open connection. Each time sc_open is used in an application, a new
//  predefined channel shall be created with a name with prefix "sc_open",
//  followed by an underscore and series of one of more decimal digits from
//  the character set 0-9.
//
//  It shall be error to bind sc_open to a port which is not of type
//  sc_port <sc_signal_inout_if>. It shall be allowed to read and write
//  to this channel. The value read from the channel is implementation-defined.
//  Values written to the channel shall be ignored. The channel shall not
//  notify an event when values are written.
//
//  NOTE—An application cannot not use sc_open for input ports of type
//  sc_port <sc_signal_in_if> because its value is undefined.
// ----------------------------------------------------------------------------

static sc_open_impl const /* implementation defined */  sc_open = {};

// ----------------------------------------------------------------------------
//  sc_tie
//
//  The static object sc_tie shall define a predefined channel to tie the
//  port to a fixed high, low or predefined value. Each time sc_tie is used
//  in an application, a new predefined channel shall be created with a name
//  with prefix "sc_tie", followed by an underscore and series of one of more
//  decimal digits from the character set 0-9.
//
//  The following pre-defined types for sc_tie shall be supported:
//
//  * sc_tie::high, representing a channel tied to a logic 1. It shall support
//    the follow types: Boolean type ‘1’ or true, sc_dt::sc_logic type
//    sc_dt::SC_LOGIC_1, integer type with a value of 1, and double and
//    floating types with a value of 1.0.
//  * sc_tie::low, representing a channel tied to a logic 0. It shall support
//    the follow types: Boolean type ‘0’ or false, sc_dt::sc_logic type
//    sc_dt::SC_LOGIC_0, integer type with a value of 0, and double and
//    floating types with a value of 0.0.
//  * sc_tie::value, representing a channel tied to specified value of type T.
//    The type of the value shall be compatible with the type of the
//    associated channel and port to which the channel is bound.
//
//  It shall be error to bind sc_tie to a port which is not of type
//  sc_port<sc_signal_in_if> or sc_port <sc_signal_inout_if>.
//  It shall be allowed to read and write to this channel. The value read
//  from the channel shall correspond to the specified pre-defined types.
//  Values written to the channel shall be ignored. The channel shall not
//  notify an event when values are written.
// ----------------------------------------------------------------------------

namespace sc_tie {

  static sc_tie_high_impl const high = {};
  static sc_tie_low_impl const low = {};

  template < typename T >
  sc_core::sc_signal_in_if<T>& value(const T& val)
  {
    return *(new sc_core::sc_stub<T>(sc_core::sc_gen_unique_name("sc_tie::value"), val));
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
