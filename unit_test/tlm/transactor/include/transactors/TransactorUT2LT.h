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

#ifndef __TRANSACTOR_UT2LT_H__
#define __TRANSACTOR_UT2LT_H__

//#include <systemc>
#include "tlm.h"
#include "simple_master_socket.h"

class TransactorUT2LT :
  public virtual tlm::tlm_fw_transport_if<>,
  public sc_core::sc_module
{
public:
  typedef tlm::tlm_fw_transport_if<> fw_interface_type;
  typedef tlm::tlm_bw_transport_if bw_interface_type;
  typedef tlm::tlm_slave_socket<32, fw_interface_type, bw_interface_type> slave_socket_type;

  typedef tlm::tlm_generic_payload transaction_type;
  typedef tlm::tlm_phase phase_type;
  typedef tlm::tlm_sync_enum sync_enum_type;
  typedef SimpleMasterSocket<> master_socket_type;


public:
  slave_socket_type slaveSocket;
  master_socket_type masterSocket;

public:
  SC_HAS_PROCESS(TransactorUT2LT);
  TransactorUT2LT(sc_core::sc_module_name name) :
    sc_core::sc_module(name),
    slaveSocket("slaveSocket"),
    masterSocket("masterSocket")
  {
    slaveSocket(*this);
  }

  void b_transport(transaction_type& trans)
  {
    phase_type phase = tlm::BEGIN_REQ;
    sc_core::sc_time t = sc_core::SC_ZERO_TIME;

    switch (masterSocket->nb_transport(trans, phase, t)) {
    case tlm::TLM_COMPLETED:
      // Transaction Finished
      break;

    case tlm::TLM_SYNC:
    case tlm::TLM_SYNC_CONTINUE:
      // Transaction not yet finished, wait for the end of it
      wait(masterSocket.getEndEvent());
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
                          bool forReads,
                          tlm::tlm_dmi& dmi_data)
  {
    dmi_data.dmi_start_address = 0x0;
    dmi_data.dmi_end_address = (sc_dt::uint64)-1;
    dmi_data.type = tlm::tlm_dmi::READ_WRITE;
    return false;
  }

};

#endif
