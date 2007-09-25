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

#ifndef SIMPLE_LT_SLAVE2_H
#define SIMPLE_LT_SLAVE2_H

//#include "tlm.h"
#include "simple_slave_socket.h"
//#include <systemc.h>
#include <cassert>
#include <vector>
//#include <iostream>

class SimpleLTSlave2 : public sc_module
{
public:
  typedef tlm::tlm_transaction transaction_type;
  typedef tlm::tlm_phase phase_type;
  typedef SimpleSlaveSocket<transaction_type> slave_socket_type;

public:
  slave_socket_type socket;

public:
  SC_HAS_PROCESS(SimpleLTSlave2);
  SimpleLTSlave2(sc_module_name name) :
    sc_module(name),
    socket("socket"),
    mMem(100, 0)
  {
    // register nb_transport method
    REGISTER_SOCKETPROCESS(socket, myNBTransport);
  }

  bool myNBTransport(transaction_type& trans, phase_type& phase, sc_time& t)
  {
    assert(phase == tlm::BEGIN_REQ);

    uint64_t address = trans.getAddress();
    assert(address < 100);

    unsigned int& data = *reinterpret_cast<unsigned int*>(trans.getDataPtr());
    if (trans.isWrite()) {
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

    trans.setResponse(true);

    // LT slave
    // - always return true
    // - not necessary to update phase (if true is returned)
    return true;
  }

private:
  std::vector<unsigned int> mMem;
};

#endif
