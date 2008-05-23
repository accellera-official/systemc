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
/*
 *
 * SystemC TLM API Standard Proposal and Examples
 * Copyright Cadence Design Systems Inc 2004
 *
 */

#ifndef BASIC_SLAVE_BASE
#define BASIC_SLAVE_BASE

#include "basic_protocol.h"
#include "tlm.h"

using tlm::tlm_transport_if;
//using basic_protocol::basic_if< typename ADDRESS , typename DATA >::write;

namespace basic_protocol
{

template< class ADDRESS , class DATA >
class basic_slave_base :
  public virtual basic_if< ADDRESS , DATA > ,
  public virtual tlm_transport_if<
    basic_request< ADDRESS , DATA > ,
    basic_response< DATA >
  >
{
using basic_if<ADDRESS , DATA >::write;
using basic_if<ADDRESS , DATA >::read;            ///< correction for GCC 4.1 supplied by Bishnupriya Bhattacharya (Cadence)
public:
  typedef  tlm_transport_if<
    basic_request< ADDRESS , DATA > ,
    basic_response< DATA >
  > if_type;

  basic_response<DATA> transport( const basic_request<ADDRESS,DATA> &request ) {

    basic_response<DATA> response;

    switch( request.type ) {
    case READ :
      response.status = read( request.a , response.d );
      break; 
    case WRITE:
      //response.status = this->write( request.a , request.d );
      response.status = write( request.a , request.d );
      break;
    default :
      response.status = ERROR;
      break; 

    }

    return response;

  }


};

}

#endif
