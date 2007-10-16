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

#ifndef TRANSACTOR_NB2B_H
#define TRANSACTOR_NB2B_H

//#include "tlm.h"
#include "simple_slave_socket.h"
#include "tlm_request.h"
#include "tlm_response.h"
#include "MyPEQ.h"
//#include <systemc.h>

class TransactorNB2B : public sc_module
{
public:
  typedef tlm_request<unsigned long long, unsigned int, TLM_PASS_BY_POINTER> tlm_request_type;
  typedef tlm_response<unsigned int, TLM_PASS_BY_POINTER> tlm_response_type;
  typedef tlm::tlm_annotated_transport_if<tlm_request_type, tlm_response_type> tlm_transport_if;
  typedef sc_port<tlm_transport_if> master_port;

  typedef tlm::tlm_generic_payload transaction_type;
  typedef tlm::tlm_phase phase_type;
  typedef tlm::tlm_nonblocking_transport_if<transaction_type> interface_type;
  typedef SimpleSlaveSocket<transaction_type> slave_socket_type;

public:
  slave_socket_type socket;
  master_port port;

public:
  SC_HAS_PROCESS(TransactorNB2B);
  TransactorNB2B(sc_module_name name) :
    sc_module(name),
    socket("socket"),
    port("port"),
    mTransactionPEQ("transactionPEQ")
  {
    REGISTER_SOCKETPROCESS(socket, myNBTransport);

    SC_THREAD(run);
  }

  void run()
  {
    while (true) {
      wait(mTransactionPEQ.getEvent());

      transaction_type* trans;
      while ((trans = mTransactionPEQ.getNextTransaction())) {
        tlm_request_type req;
        req.set_address(trans->get_address());

        unsigned int data;
        req.set_data_array(1, &data);

        if (trans->get_command() == tlm::TLM_READ_COMMAND) {
          req.set_command(READ);

        } else {
          req.set_command(WRITE);
          data = *reinterpret_cast<unsigned int*>(trans->get_data_ptr());
        }

        tlm_response_type resp = port->transport(req);

        if (resp.get_status().is_ok()) {
          trans->set_response_status(tlm::TLM_OK_RESP);

        } else {
          trans->set_response_status(tlm::TLM_GENERIC_ERROR_RESP);
        }

        if (trans->get_command() == tlm::TLM_READ_COMMAND) {
          *reinterpret_cast<unsigned int*>(trans->get_data_ptr()) = resp.get_data();
        }

        phase_type phase = tlm::BEGIN_RESP;
        sc_time t = SC_ZERO_TIME;
        if (!socket->nb_transport(*trans, phase, t)) {
          wait(mEndTransactionEvent);
        }        
      }
    }
  }

  bool myNBTransport(transaction_type& trans, phase_type& phase, sc_time& t)
  {
    if (phase == tlm::BEGIN_REQ) {
      mTransactionPEQ.notify(trans, t);
    
    } else if (phase == tlm::END_RESP) {
      mEndTransactionEvent.notify(t);

    } else {
      assert(0); exit(1);
    }

    return false;
  }

private:
  MyPEQ mTransactionPEQ;
  sc_event mEndTransactionEvent;
};

#endif
