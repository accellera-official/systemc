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

#ifndef EXPLICIT_LT_SLAVE_H
#define EXPLICIT_LT_SLAVE_H

//#include "tlm.h"
#include "simple_slave_socket.h"
//#include <systemc.h>
#include <cassert>
#include <vector>
#include <queue>
//#include <iostream>

class ExplicitLTSlave : public sc_module
{
public:
  typedef tlm::tlm_generic_payload transaction_type;
  typedef tlm::tlm_phase phase_type;
  typedef SimpleSlaveSocket<transaction_type> slave_socket_type;

public:
  slave_socket_type socket;

public:
  SC_HAS_PROCESS(ExplicitLTSlave);
  ExplicitLTSlave(sc_module_name name) :
    sc_module(name),
    socket("socket"),
    mMem(100, 0),
    mCurrentTransaction(0)
  {
    // register nb_transport method
    REGISTER_SOCKETPROCESS(socket, myNBTransport);

    SC_THREAD(beginResponse)
  }

  bool myNBTransport(transaction_type& trans, phase_type& phase, sc_time& t)
  {
    if (phase == tlm::BEGIN_REQ) {
      sc_dt::uint64 address = trans.get_address();
      assert(address < 100);

      // This slave only supports one transaction at a time
      // This will only work with LT masters
      assert(mCurrentTransaction == 0);

      unsigned int& data = *reinterpret_cast<unsigned int*>(trans.get_data_ptr());
      if (trans.get_command() == tlm::TLM_WRITE_COMMAND) {
        std::cerr << name() << ": Received write request: A = "
                << (void*)(int)address << ", D = " << (void*)data
                << " @ " << sc_time_stamp() << std::endl;

        mMem[address] = data;

        // Synchronization on demand (eg need to assert an interrupt)
        mResponseEvent.notify(t);
        mCurrentTransaction = &trans;

        // End request phase
        phase = tlm::END_REQ;
        return false;

      } else {
        std::cerr << name() << ": Received read request: A = "
                 << (void*)(int)address << " @ " << sc_time_stamp() << std::endl;

        data = mMem[address];

        // Finish transaction (use timing annotation)
        t += sc_time(100, SC_NS);
        return true;
      }

    } else if (phase == tlm::END_RESP) {

      // Transaction finished
      mCurrentTransaction = 0;
      return true;
    }

    // Not possible
    assert(0); exit(1);
    return false;
  }

  void beginResponse()
  {
    while (true) {
      // Wait for next synchronization request
      wait(mResponseEvent);

      assert(mCurrentTransaction);
      // start response phase
      phase_type phase = tlm::BEGIN_RESP;
      sc_time t = SC_ZERO_TIME;

      // Set response data
      mCurrentTransaction->set_response_status(tlm::TLM_OK_RESP);
      assert(mCurrentTransaction->get_command() == tlm::TLM_WRITE_COMMAND);

      sc_dt::uint64 address = mCurrentTransaction->get_address();
      assert(address < 100);
      *reinterpret_cast<unsigned int*>(mCurrentTransaction->get_data_ptr()) =
        mMem[address];

      // We are synchronized, we can read/write sc_signals, wait,...
      // Wait before sending the response
      wait(50, SC_NS);

      if (socket->nb_transport(*mCurrentTransaction, phase, t)) {
        mCurrentTransaction = 0;

      } else {
        // Master will call nb_transport(trans, END_RESP, t)
      }
    }
  }

private:
  std::vector<unsigned int> mMem;
  sc_event mResponseEvent;
  transaction_type* mCurrentTransaction;
};

#endif
