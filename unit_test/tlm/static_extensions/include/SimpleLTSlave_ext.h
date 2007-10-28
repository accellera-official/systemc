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

#ifndef __SIMPLE_LT_SLAVE2_H__
#define __SIMPLE_LT_SLAVE2_H__

#include "tlm.h"
#include "simple_slave_socket.h"
#include "my_extension.h"

//#include <systemc>
#include <cassert>
#include <vector>
//#include <iostream>

class SimpleLTSlave_ext : public sc_core::sc_module
{
public:
  typedef my_extended_payload transaction_type;
  typedef tlm::tlm_phase phase_type;
  typedef tlm::tlm_sync_enum sync_enum_type;
  typedef SimpleSlaveSocket<32, transaction_type> slave_socket_type;

public:
  slave_socket_type socket;

public:
  SC_HAS_PROCESS(SimpleLTSlave_ext);
  SimpleLTSlave_ext(sc_core::sc_module_name name,
                    sc_core::sc_time invalidate_dmi_time = sc_core::sc_time(25, sc_core::SC_NS)) :
    sc_core::sc_module(name),
    socket("socket")
  {
    // register nb_transport method
    REGISTER_NBTRANSPORT(socket, myNBTransport);
    REGISTER_DMI(socket, myGetDMIPtr);

    REGISTER_DEBUGTRANSPORT(socket, transport_dbg);
    
    SC_METHOD(invalidate_dmi_method);
    sensitive << m_invalidate_dmi_event;
    dont_initialize();
    m_invalidate_dmi_time = invalidate_dmi_time;
  }

  sync_enum_type myNBTransport(transaction_type& trans, phase_type& phase, sc_core::sc_time& t)
  {
    assert(phase == tlm::BEGIN_REQ);

    my_extension* tmp_ext;
    trans.get_extension(tmp_ext);
    if (!tmp_ext)
    {
        std::cout << name() << ": ERROR, extension not present" << std::endl;
    }
    else
    {
        std::cout << name() << ": OK, extension data = "
                  << tmp_ext->m_data << std::endl;
    }
    sc_dt::uint64 address = trans.get_address();
    assert(address < 400);

    unsigned int& data = *reinterpret_cast<unsigned int*>(trans.get_data_ptr());
    if (trans.get_command() == tlm::TLM_WRITE_COMMAND) {
      std::cout << name() << ": Received write request: A = 0x"
                << std::hex << (unsigned int)address
                << ", D = 0x" << data << std::dec
                << " @ " << sc_core::sc_time_stamp() << std::endl;

      *reinterpret_cast<unsigned int*>(&mMem[address]) = data;
      t += sc_core::sc_time(10, sc_core::SC_NS);

    } else {
      std::cout << name() << ": Received read request: A = 0x"
                << std::hex << (unsigned int)address << std::dec
                << " @ " << sc_core::sc_time_stamp() << std::endl;

      data = *reinterpret_cast<unsigned int*>(&mMem[address]);
      t += sc_core::sc_time(100, sc_core::SC_NS);
    }

    trans.set_response_status(tlm::TLM_OK_RESP);

    trans.set_dmi_allowed(true);

    // LT slave
    // - always return true
    // - not necessary to update phase (if true is returned)
    return tlm::TLM_COMPLETED;
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

  bool myGetDMIPtr(const sc_dt::uint64& address,
                   bool for_reads,
                   tlm::tlm_dmi& dmi_data)
  {
      // notify DMI invalidation, just to check if this reaches the
      // initiators properly
      m_invalidate_dmi_event.notify(m_invalidate_dmi_time);
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

  void invalidate_dmi_method()
  {
      sc_dt::uint64 start_address = 0x0;
      sc_dt::uint64 end_address = 399;
      socket->invalidate_direct_mem_ptr(start_address, end_address);
  }
private:
  unsigned char mMem[400];
  sc_core::sc_event m_invalidate_dmi_event;
  sc_core::sc_time  m_invalidate_dmi_time;
};

#endif
