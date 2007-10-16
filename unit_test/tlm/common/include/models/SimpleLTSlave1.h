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

#ifndef SIMPLE_LT_SLAVE1_H
#define SIMPLE_LT_SLAVE1_H

//#include "tlm.h"
//#include <systemc.h>
#include <cassert>
#include <vector>
//#include <iostream>

class SimpleLTSlave1 :
  public sc_module,
  public virtual tlm::tlm_nonblocking_transport_if<tlm::tlm_generic_payload>
{
public:
  typedef tlm::tlm_generic_payload transaction_type;
  typedef tlm::tlm_phase phase_type;
  typedef tlm::tlm_nonblocking_transport_if<transaction_type> interface_type;
  typedef tlm::tlm_slave_socket<interface_type> slave_socket_type;

public:
  slave_socket_type socket;

public:
  SimpleLTSlave1(sc_module_name name) :
    sc_module(name),
    socket("socket"),
    mMem(100, 0)
  {
    // Bind this slave's interface to the slave socket
    socket(*this);
  }

  bool nb_transport(transaction_type& trans, phase_type& phase, sc_time& t)
  {
    assert(phase == tlm::BEGIN_REQ);

    sc_dt::uint64 address = trans.get_address();
    assert(address < 100);

    unsigned int& data = *reinterpret_cast<unsigned int*>(trans.get_data_ptr());
    if (trans.get_command() == tlm::TLM_WRITE_COMMAND) {
      std::cerr << name() << ": Received write request: A = "
                << (void*)(int)address << ", D = " << (void*)data
                << " @ " << sc_time_stamp() << std::endl;

      mMem[address] = data;
      t += sc_time(10, SC_NS);

    } else {
      std::cerr << name() << ": Received read request: A = "
                << (void*)(int)address << " @ " << sc_time_stamp() << std::endl;

      data = mMem[address];
      t += sc_time(100, SC_NS);
    }

    trans.set_response_status(tlm::TLM_OK_RESP);

    // LT slave
    // - always return true
    // - not necessary to update phase (if true is returned)
    return true;
  }

private:
  std::vector<unsigned int> mMem;
};

#endif
