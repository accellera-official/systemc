
/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2004 by all Contributors.
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

#ifndef BASIC_INTIATOR_PORT_HEADER
#define BASIC_INTIATOR_PORT_HEADER

//
// Supplies read and write functions to the user. These call transport.
//
// Single ( N = 1 ) and multi connection ( N != 1 ) versions of read and write
// are supplied
//

#include "basic_protocol.h"
#include "tlm.h"

using tlm::tlm_transport_if;

namespace basic_protocol
{

template < class ADDRESS , class DATA , int N = 1>
class basic_initiator_port :
  public sc_port< 
    tlm_transport_if < 
      basic_request< ADDRESS , DATA > ,
      basic_response< DATA >
    > , N
  > ,
  public virtual basic_if<ADDRESS , DATA >
{
public:

  typedef
  tlm_transport_if < 
      basic_request< ADDRESS , DATA > ,
      basic_response< DATA >
    > if_type;
  
  basic_initiator_port( const char *port_name ) :
    sc_port< if_type , N > ( port_name )
  {}

  virtual basic_status write( const ADDRESS &a , const DATA &d ) {

    basic_request<ADDRESS,DATA> req;
    basic_response<DATA> rsp;

    req.type = WRITE;
    req.a = a;
    req.d = d;

    rsp = (*this)->transport( req );

    return rsp.status;

  }

  virtual basic_status write( const ADDRESS &a , const DATA &d , int p ) {

    basic_request<ADDRESS,DATA> req;
    basic_response<DATA> rsp;

    req.type = WRITE;
    req.a = a;
    req.d = d;

    rsp = (*this)[p]->transport( req );

    return rsp.status;

  }

  virtual basic_status read( const ADDRESS &a , DATA &d ) {

    basic_request<ADDRESS,DATA> req;
    basic_response<DATA> rsp;

    req.type = READ;
    req.a = a;

    rsp = (*this)->transport( req );
    d = rsp.d;

    return rsp.status;

  }

  virtual basic_status read( const ADDRESS &a , DATA &d , int p ) {

    basic_request<ADDRESS,DATA> req;
    basic_response<DATA> rsp;

    req.type = READ;
    req.a = a;

    rsp = (*this)[p]->transport( req );
    d = rsp.d;

    return rsp.status;

  }

};

}
#endif
