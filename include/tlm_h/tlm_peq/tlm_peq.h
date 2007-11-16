/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

#ifndef __TLM_PEQ_H__
#define __TLM_PEQ_H__

#include <map>
#include "tlm_h/tlm_peq/tlm_write_if.h"

namespace tlm {

//
// No lt_sc_time operator required, since we have operator< defined for
// sc_time ?
//
// struct lt_sc_time {
//  bool operator<( const sc_core::sc_time &a , const sc_core::sc_time &b ) {
//    return a < b;
//  }
// };
//
//
//

//
// This peq implements a delayed write and has an write port
//
// If you post many transactions to the same time slot, this will result in
// many transactions coming out of the write port in a single delta
//
// If you want event driven semantics, stuff the output of the write port
// into a fifo ( if you want to guarantee no losses ) or
// buffer ( if you don't mind losses but you can't be bothered or are
// not able to clear the fifo out ).
//
// For example, you can arrive at a blocking get interface by doing get on
// a fifo attached to the write port shown below
//
// We could even design a tlm_peq_fifo and/or tlm_peq_buffer with these
// channels built in
//

template< typename T>
class tlm_peq :
  public sc_core::sc_module ,
  public virtual tlm_delayed_write_if< T >
{
 public:
  sc_core::sc_export< tlm_delayed_write_if< T > > delayed_write_export;
  sc_core::sc_port< tlm_write_if< T > > write_port;

  SC_HAS_PROCESS( tlm_peq );

  tlm_peq( sc_core::sc_module_name nm ) :
    sc_core::sc_module( nm ) , write_port("write_port") {

    delayed_write_export( *this );

    SC_METHOD( wake_up_method );
    dont_initialize();
    sensitive << m_wake_up;

  }

  int size() const { return m_map.size(); }

  int posted_before( const sc_core::sc_time &time ) const {

    int i = 0;

    for( typename std::multimap< sc_core::sc_time , T>::const_iterator iter = m_map.begin();
	 iter != m_map.end();
	 ++iter ) {

      if( (*iter).first < time ) {
	i++;
      }

    }

    return i;

  }

  int posted_at( const sc_core::sc_time &time ) const {
    return m_map.count( time );
  }

  void write( const T &transaction , const sc_core::sc_time &time ) {

    m_map.insert( pair_type( time + sc_core::sc_time_stamp() , transaction ) );
    m_wake_up.notify( time );

  }

 private:
  typedef std::pair<sc_core::sc_time,T> pair_type;

  void wake_up_method() {

    pair_type p;
    sc_core::sc_time now = sc_core::sc_time_stamp();

    // must be something there, and it must be scheduled for now

    assert( m_map.size() > 0 );
    assert( (*(m_map.begin())).first == now );

    for( p = *(m_map.begin());
	 p.first == now;
	 p = *(m_map.begin()) )
    {

      write_port.write( p.second );
      m_map.erase( m_map.begin() );

      if( m_map.size() == 0 ) {
	return;
      }

      p = *(m_map.begin());

    }

    m_wake_up.notify( p.first - now );

  }

  sc_core::sc_event m_wake_up;
  std::multimap< sc_core::sc_time , T> m_map;

};

} // namespace tlm

#endif
