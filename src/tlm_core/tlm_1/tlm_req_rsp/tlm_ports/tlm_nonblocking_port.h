/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
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

#ifndef __TLM_NONBLOCKING_PORT_H__
#define __TLM_NONBLOCKING_PORT_H__

#include "tlm_core/tlm_1/tlm_req_rsp/tlm_1_interfaces/tlm_core_ifs.h"
#include "tlm_core/tlm_1/tlm_req_rsp/tlm_ports/tlm_event_finder.h"

namespace tlm {

template < typename T >
class tlm_nonblocking_get_port :
public sc_core::sc_port< tlm_nonblocking_get_if< T > , 1 >
{
public:
  typedef tlm_nonblocking_get_if<T> get_if_type;

  tlm_nonblocking_get_port( const char *port_name ) :
    sc_core::sc_port< tlm_nonblocking_get_if< T > , 1 >( port_name ) {}

  sc_core::sc_event_finder& ok_to_get() const {

    return *new tlm_event_finder_t< get_if_type , T >(
       *this,
       &get_if_type::ok_to_get );

  }

};

template < typename T >
class tlm_nonblocking_peek_port :
public sc_core::sc_port< tlm_nonblocking_peek_if< T > , 1 >
{
public:
  typedef tlm_nonblocking_peek_if<T> peek_if_type;

  tlm_nonblocking_peek_port( const char *port_name ) :
    sc_core::sc_port< tlm_nonblocking_peek_if< T > , 1 >( port_name ) {}

  sc_core::sc_event_finder& ok_to_peek() const {

    return *new tlm_event_finder_t< peek_if_type , T >(
       *this,
       &peek_if_type::ok_to_peek );

  }

};


template < typename T >
class tlm_nonblocking_put_port :
public sc_core::sc_port< tlm_nonblocking_put_if< T > , 1 >
{
public:
  typedef tlm_nonblocking_put_if<T> put_if_type;

  tlm_nonblocking_put_port( const char *port_name ) :
    sc_core::sc_port< tlm_nonblocking_put_if< T > , 1 >( port_name ) {}

  sc_core::sc_event_finder& ok_to_put() const {

    return *new tlm_event_finder_t< put_if_type , T >(
             *this,
       &put_if_type::ok_to_put );

  }

};

} // namespace tlm

#endif
