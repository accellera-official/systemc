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

#ifndef __TLM_ADAPTERS_H__
#define __TLM_ADAPTERS_H__

#include "tlm_core/tlm_1/tlm_req_rsp/tlm_1_interfaces/tlm_master_slave_ifs.h"

namespace tlm {

template< typename REQ , typename RSP >
class tlm_transport_to_master :
  public sc_core::sc_module ,
  public virtual tlm_transport_if< REQ , RSP >
{
public:
  sc_core::sc_export< tlm_transport_if< REQ , RSP > > target_export;
  sc_core::sc_port< tlm_master_if< REQ , RSP > > master_port;

  tlm_transport_to_master( sc_core::sc_module_name nm ) :
    sc_core::sc_module( nm ) {

    target_export( *this );

  }

  tlm_transport_to_master() :
    sc_core::sc_module( sc_core::sc_module_name( sc_core::sc_gen_unique_name( "transport_to_master" ) ) ){

    target_export( *this );

  }

  RSP transport( const REQ &req ) {

    mutex.lock();

    master_port->put( req );
    rsp = master_port->get();

    mutex.unlock();
    return rsp;

  }

private:
  sc_core::sc_mutex mutex;
  RSP rsp;

};

template< typename REQ , typename RSP >
class tlm_slave_to_transport : public sc_core::sc_module
{
public:

  SC_HAS_PROCESS( tlm_slave_to_transport );

  sc_core::sc_port< tlm_slave_if< REQ , RSP > > slave_port;
  sc_core::sc_port< tlm_transport_if< REQ , RSP > > initiator_port;

  tlm_slave_to_transport( sc_core::sc_module_name nm ) : sc_core::sc_module( nm )
  {}

  tlm_slave_to_transport() :
    sc_core::sc_module( sc_core::sc_module_name( sc_core::sc_gen_unique_name("slave_to_transport") ) )
  {}

private:
  void run() {

    REQ req;
    RSP rsp;

    while( true ) {

     slave_port->get( req );
     rsp = initiator_port->transport( req );
     slave_port->put( rsp );

    }

  }

};

} // namespace tlm

#endif
