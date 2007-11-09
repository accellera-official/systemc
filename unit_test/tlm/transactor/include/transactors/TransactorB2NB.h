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

#ifndef __TRANSACTOR_B2NB_H__
#define __TRANSACTOR_B2NB_H__

//#include <systemc>
#include "tlm.h"
#include "simple_initiator_socket.h"
#include "tlm_request.h"
#include "tlm_response.h"

class TransactorB2NB :
  public virtual tlm::tlm_annotated_transport_if<
    tlm_request<unsigned long long, unsigned int, TLM_PASS_BY_POINTER>,
    tlm_response<unsigned int, TLM_PASS_BY_POINTER> >,
    public sc_core::sc_module
{
public:
  typedef tlm_request<unsigned long long, unsigned int, TLM_PASS_BY_POINTER> tlm_request_type;
  typedef tlm_response<unsigned int, TLM_PASS_BY_POINTER> tlm_response_type;
  typedef tlm::tlm_annotated_transport_if<tlm_request_type, tlm_response_type> tlm_transport_if;
  typedef sc_core::sc_export<tlm_transport_if> slave_export;

  typedef tlm::tlm_generic_payload transaction_type;
  typedef tlm::tlm_phase phase_type;
  typedef tlm::tlm_sync_enum sync_enum_type;
  typedef SimpleInitiatorSocket<> initiator_socket_type;

public:
  slave_export port;
  initiator_socket_type socket;

public:
  SC_HAS_PROCESS(TransactorB2NB);
  TransactorB2NB(sc_core::sc_module_name name) :
    sc_core::sc_module(name),
    port("port"),
    socket("socket")
  {
    port(*this);
  }

  void transport(const tlm_request_type& req, tlm_response_type& resp, sc_core::sc_time& t)
  {
    phase_type phase = tlm::BEGIN_REQ;

    transaction_type trans;
    trans.set_address(req.get_address());
    
    unsigned int data;
    trans.set_data_ptr(reinterpret_cast<unsigned char*>(&data));

    if (req.get_command() == READ) {
      trans.set_command(tlm::TLM_READ_COMMAND);
    } else { // WRITE
      trans.set_command(tlm::TLM_WRITE_COMMAND);
      data = req.get_data();
    }

    switch (socket->nb_transport(trans, phase, t)) {
    case tlm::TLM_COMPLETED:
      // Transaction Finished
      break;

    case tlm::TLM_SYNC:
    case tlm::TLM_SYNC_CONTINUE:
      // Transaction not yet finished, wait for the end of it
      wait(socket.getEndEvent());
      break;

    case tlm::TLM_REJECTED:
      // FIXME: Not supported (wait and retry same transaction)
    default:
      assert(0); exit(1);
    };

    tlm_status status;
    if (trans.get_response_status() == tlm::TLM_OK_RESPONSE) {
      status.set_ok();

    } else {
      status.set_error();
    }
    resp.set_status(status);

    if (req.get_command() == READ) {
      resp.set_data_array(1, req.get_data_ptr());
      resp.set_data(data);
    }
  }

  tlm_response_type transport(const tlm_request_type& req)
  {
    sc_core::sc_time t = sc_core::SC_ZERO_TIME;
    tlm_response_type resp;

    transport(req, resp, t);
    wait(t);    

    return resp;
  }

};

#endif
