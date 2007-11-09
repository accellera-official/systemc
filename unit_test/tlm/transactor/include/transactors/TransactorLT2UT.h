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

#ifndef __TRANSACTOR_LT2UT_H__
#define __TRANSACTOR_LT2UT_H__

#include "tlm.h"
#include "simple_target_socket.h"
#include "MyPEQ.h"
//#include <systemc>

class TransactorLT2UT :
  public virtual tlm::tlm_bw_transport_if,
  public sc_core::sc_module
{
public:
  typedef tlm::tlm_fw_transport_if<> fw_interface_type;
  typedef tlm::tlm_bw_transport_if bw_interface_type;
  typedef tlm::tlm_initiator_socket<32, fw_interface_type, bw_interface_type> initiator_socket_type;

  typedef tlm::tlm_generic_payload transaction_type;
  typedef tlm::tlm_phase phase_type;
  typedef tlm::tlm_sync_enum sync_enum_type;
  typedef SimpleTargetSocket<> target_socket_type;

public:
  target_socket_type targetSocket;
  initiator_socket_type initiatorSocket;

public:
  SC_HAS_PROCESS(TransactorLT2UT);
  TransactorLT2UT(sc_core::sc_module_name name) :
    sc_core::sc_module(name),
    targetSocket("targetSocket"),
    initiatorSocket("initiatorSocket"),
    mTransactionPEQ("transactionPEQ")
  {
    REGISTER_NBTRANSPORT(targetSocket, myNBTransport);

    initiatorSocket(*this);

    SC_THREAD(run);
  }

  void run()
  {
    while (true) {
      wait(mTransactionPEQ.getEvent());

      transaction_type* trans;
      while ((trans = mTransactionPEQ.getNextTransaction())) {
        initiatorSocket->b_transport(*trans);

        phase_type phase = tlm::BEGIN_RESP;
        sc_core::sc_time t = sc_core::SC_ZERO_TIME;
        switch (targetSocket->nb_transport(*trans, phase, t)) {
        case tlm::TLM_COMPLETED:
          // Transaction Finished
          break;

        case tlm::TLM_SYNC:
        case tlm::TLM_SYNC_CONTINUE:
          // Transaction not yet finished, wait for the end of it
          wait(mEndTransactionEvent);
          break;

        case tlm::TLM_REJECTED:
          // FIXME: Not supported (wait and retry same transaction)
        default:
          assert(0); exit(1);
        };
      }
    }
  }

  sync_enum_type myNBTransport(transaction_type& trans, phase_type& phase, sc_core::sc_time& t)
  {
    if (phase == tlm::BEGIN_REQ) {
      mTransactionPEQ.notify(trans, t);
    
    } else if (phase == tlm::END_RESP) {
      mEndTransactionEvent.notify(t);

    } else {
      assert(0); exit(1);
    }

    return tlm::TLM_SYNC;
  }

  void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
                                 sc_dt::uint64 end_range)
  {
    // No DMI support: ignore
  }

private:
  MyPEQ mTransactionPEQ;
  sc_core::sc_event mEndTransactionEvent;
};

#endif
