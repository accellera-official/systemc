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

#ifndef __SIMPLE_UT_SLAVE1_H__
#define __SIMPLE_UT_SLAVE1_H__

#include "tlm.h"
//#include <systemc>
#include <cassert>
#include <vector>
//#include <iostream>

class SimpleUTSlave1 :
  public sc_core::sc_module,
  public virtual tlm::tlm_fw_transport_if<>
{
public:
  typedef tlm::tlm_generic_payload transaction_type;
  typedef tlm::tlm_phase phase_type;
  typedef tlm::tlm_sync_enum sync_enum_type;
  typedef tlm::tlm_fw_transport_if<> fw_interface_type;
  typedef tlm::tlm_bw_transport_if bw_interface_type;
  typedef tlm::tlm_slave_socket<32, fw_interface_type, bw_interface_type> slave_socket_type;

public:
  slave_socket_type socket;

public:
  SimpleUTSlave1(sc_core::sc_module_name name) :
    sc_core::sc_module(name),
    socket("socket")
  {
    // Bind this slave's interface to the slave socket
    socket(*this);
  }

  void b_transport(transaction_type& trans)
  {
    sc_dt::uint64 address = trans.get_address();
    assert(address < 400);

    unsigned int& data = *reinterpret_cast<unsigned int*>(trans.get_data_ptr());
    if (trans.get_command() == tlm::TLM_WRITE_COMMAND) {
      std::cout << name() << ": Received write request: A = 0x"
                << std::hex << (unsigned int)address
                << ", D = 0x" << data << std::dec
                << " @ " << sc_core::sc_time_stamp() << std::endl;

      *reinterpret_cast<unsigned int*>(&mMem[address]) = data;
      wait(sc_core::sc_time(10, sc_core::SC_NS));

    } else {
      std::cout << name() << ": Received read request: A = 0x"
                << std::hex << (unsigned int)address << std::dec
                << " @ " << sc_core::sc_time_stamp() << std::endl;

      data = *reinterpret_cast<unsigned int*>(&mMem[address]);
      wait(sc_core::sc_time(100, sc_core::SC_NS));
    }

    trans.set_response_status(tlm::TLM_OK_RESPONSE);

    trans.set_dmi_allowed(true);
  }

  unsigned int transport_dbg(tlm::tlm_debug_payload& r)
  {
    if (r.address >= 400) return 0;

    unsigned int tmp = (int)r.address;
    unsigned int num_bytes;
    if (tmp + r.num_bytes >= 400) {
      num_bytes = 400 - tmp;

    } else {
      num_bytes = r.num_bytes;
    }
    if (r.do_read) {
      for (unsigned int i = 0; i < num_bytes; ++i) {
        r.data[i] = mMem[i + tmp];
      }

    } else {
      for (unsigned int i = 0; i < num_bytes; ++i) {
        mMem[i + tmp] = r.data[i];
      }
    }
    return num_bytes;
  }

  bool get_direct_mem_ptr(const sc_dt::uint64& address,
                          bool forReads,
                          tlm::tlm_dmi& dmi_data)
  {
    if (address < 400) {
      dmi_data.dmi_start_address = 0x0;
      dmi_data.dmi_end_address = 399;
      dmi_data.dmi_ptr = mMem;
      dmi_data.read_latency = sc_core::sc_time(100, sc_core::SC_NS);
      dmi_data.write_latency = sc_core::sc_time(10, sc_core::SC_NS);
      dmi_data.type = tlm::tlm_dmi::READ_WRITE;
      dmi_data.endianness =
        (tlm::hostHasLittleEndianness() ? tlm::TLM_LITTLE_ENDIAN :
                                          tlm::TLM_BIG_ENDIAN);
      return true;

    } else {
      // should not happen
      dmi_data.dmi_start_address = address;
      dmi_data.dmi_end_address = address;
      dmi_data.type = tlm::tlm_dmi::READ_WRITE;
      return false;
    }
  }

private:
  unsigned char mMem[400];
};

#endif
