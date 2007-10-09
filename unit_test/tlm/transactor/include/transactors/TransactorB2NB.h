/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
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

#ifndef TRANSACTOR_B2NB_H
#define TRANSACTOR_B2NB_H

//#include <systemc.h>
//#include "tlm.h"
#include "simple_master_socket.h"
#include "tlm_request.h"
#include "tlm_response.h"

class TransactorB2NB :
  public virtual tlm::tlm_annotated_transport_if<
    tlm_request<unsigned long long, unsigned int, TLM_PASS_BY_POINTER>,
    tlm_response<unsigned int, TLM_PASS_BY_POINTER> >,
  public sc_module
{
public:
  typedef tlm_request<unsigned long long, unsigned int, TLM_PASS_BY_POINTER> tlm_request_type;
  typedef tlm_response<unsigned int, TLM_PASS_BY_POINTER> tlm_response_type;
  typedef tlm::tlm_annotated_transport_if<tlm_request_type, tlm_response_type> tlm_transport_if;
  typedef sc_export<tlm_transport_if> slave_export;

  typedef tlm::tlm_transaction transaction_type;
  typedef tlm::tlm_phase phase_type;
  typedef tlm::tlm_nonblocking_transport_if<transaction_type> interface_type;
  typedef SimpleMasterSocket<transaction_type> master_socket_type;

public:
  slave_export port;
  master_socket_type socket;

public:
  SC_HAS_PROCESS(TransactorB2NB);
  TransactorB2NB(sc_module_name name) :
    sc_module(name),
    port("port"),
    socket("socket")
  {
    port(*this);
  }

  void transport(const tlm_request_type& req, tlm_response_type& resp, sc_time& t)
  {
    phase_type phase = tlm::BEGIN_REQ;

    transaction_type trans;
    trans.setAddress(req.get_address());
    
    unsigned int data;
    trans.setDataPtr(reinterpret_cast<char*>(&data));

    if (req.get_command() == READ) {
      trans.setIsRead();
    } else { // WRITE
      trans.setIsWrite();
      data = req.get_data();
    }

    socket->nb_transport(trans, phase, t);

    tlm_status status;
    if (trans.getResponse()) {
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
    sc_time t = SC_ZERO_TIME;
    tlm_response_type resp;

    transport(req, resp, t);
    wait(t);    

    return resp;
  }

};

#endif
