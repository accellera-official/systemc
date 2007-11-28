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

#ifndef __TRANSACTOR_UT2LT_H__
#define __TRANSACTOR_UT2LT_H__

//#include <systemc>
#include "tlm.h"
#include "simple_initiator_socket.h"

class TransactorUT2LT :
  public virtual tlm::tlm_fw_b_transport_if<>,
  public sc_core::sc_module
{
public:
  typedef tlm::tlm_fw_b_transport_if<>   fw_interface_type;
  typedef tlm::tlm_bw_b_transport_if     bw_interface_type;
  typedef tlm::tlm_target_socket<32,
                                 fw_interface_type,
                                 bw_interface_type> target_socket_type;

  typedef tlm::tlm_generic_payload     transaction_type;
  typedef tlm::tlm_dmi_mode            dmi_mode_type;
  typedef tlm::tlm_phase               phase_type;
  typedef tlm::tlm_sync_enum           sync_enum_type;
  typedef SimpleInitiatorSocket<>      initiator_socket_type;


public:
  target_socket_type targetSocket;
  initiator_socket_type initiatorSocket;

public:
  SC_HAS_PROCESS(TransactorUT2LT);
  TransactorUT2LT(sc_core::sc_module_name name) :
    sc_core::sc_module(name),
    targetSocket("targetSocket"),
    initiatorSocket("initiatorSocket")
  {
    targetSocket(*this);
  }

  void b_transport(transaction_type& trans)
  {
    phase_type phase = tlm::BEGIN_REQ;
    sc_core::sc_time t = sc_core::SC_ZERO_TIME;

    switch (initiatorSocket->nb_transport(trans, phase, t)) {
    case tlm::TLM_COMPLETED:
      // Transaction Finished
      break;

    case tlm::TLM_ACCEPTED:
    case tlm::TLM_UPDATED:
      // Transaction not yet finished, wait for the end of it
      wait(initiatorSocket.getEndEvent());
      break;

    case tlm::TLM_REJECTED:
      // FIXME: Not supported (wait and retry same transaction)
    default:
      assert(0); exit(1);
    };
  }

  unsigned int transport_dbg(tlm::tlm_debug_payload& r)
  {
    return 0;
  }

  bool get_direct_mem_ptr(const sc_dt::uint64& address,
                          dmi_mode_type& dmi_mode,
                          tlm::tlm_dmi& dmi_data)
  {
    dmi_mode.type = tlm::tlm_dmi_mode::READ_WRITE;

    dmi_data.dmi_start_address = 0x0;
    dmi_data.dmi_end_address = (sc_dt::uint64)-1;
    return false;
  }

};

#endif
