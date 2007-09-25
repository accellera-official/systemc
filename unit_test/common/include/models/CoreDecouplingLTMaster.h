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

#ifndef CORE_DECOUPLING_LT_MASTER_H
#define CORE_DECOUPLING_LT_MASTER_H

//#include "tlm.h"
#include "simple_master_socket.h"
//#include <systemc.h>
#include <cassert>
//#include <iostream>

class CoreDecouplingLTMaster : public sc_module
{
public:
  typedef tlm::tlm_transaction transaction_type;
  typedef tlm::tlm_phase phase_type;
  typedef SimpleMasterSocket<transaction_type> master_socket_type;

public:
  master_socket_type socket;

public:
  SC_HAS_PROCESS(CoreDecouplingLTMaster);
  CoreDecouplingLTMaster(sc_module_name name,
                         unsigned int nrOfTransactions = 0x5,
                         unsigned int baseAddress = 0) :
    sc_module(name),
    socket("socket"),
    mNrOfTransactions(nrOfTransactions),
    mBaseAddress(baseAddress),
    mTransactionCount(0)
  {
    tlm::tlm_quantumkeeper::setGlobalQuantum(sc_time(500, SC_NS));
    mQuantumKeeper.reset();

    // Master thread
    SC_THREAD(run);
  }

  bool initTransaction(transaction_type& trans)
  {
    if (mTransactionCount < mNrOfTransactions) {
      trans.setAddress(mBaseAddress + mTransactionCount);
      mData = mTransactionCount;
      trans.setDataPtr(reinterpret_cast<char*>(&mData));
      trans.setIsWrite();

    } else if (mTransactionCount < 2 * mNrOfTransactions) {
      trans.setAddress(mBaseAddress + mTransactionCount - mNrOfTransactions);
      mData = 0;
      trans.setDataPtr(reinterpret_cast<char*>(&mData));
      trans.setIsRead();

    } else {
      return false;
    }

    ++mTransactionCount;
    return true;
  }

  void logStartTransation(transaction_type& trans)
  {
    if (trans.isWrite()) {
      std::cerr << name() << ": Send write request: A = "
                << (void*)(int)trans.getAddress() << ", D = " << (void*)mData
                << " @ " << mQuantumKeeper.getCurrentTime()
                << " (" << sc_time_stamp() << " + "
                << mQuantumKeeper.getLocalTime() << ")"
                << std::endl;
      
    } else {
      std::cerr << name() << ": Send read request: A = "
                << (void*)(int)trans.getAddress()
                << " @ " << mQuantumKeeper.getCurrentTime()
                << " (" << sc_time_stamp() << " + "
                << mQuantumKeeper.getLocalTime() << ")"
                << std::endl;
    }
  }

  void logEndTransaction(transaction_type& trans)
  {
    if (!trans.getResponse()) {
      std::cerr << name() << ": Received error response @ "
                << mQuantumKeeper.getCurrentTime()
                << " (" << sc_time_stamp() << " + "
                << mQuantumKeeper.getLocalTime() << ")"
                << std::endl;

    } else {
      std::cerr << name() <<  ": Received ok response";
      if (trans.isRead()) {
        std::cerr << ": D = " << (void*)mData;
      }
      std::cerr << " @ " << mQuantumKeeper.getCurrentTime()
                << " (" << sc_time_stamp() << " + "
                << mQuantumKeeper.getLocalTime() << ")"
                << std::endl;
    }
  }

  void run()
  {
    transaction_type trans;
    phase_type phase;

    while (initTransaction(trans)) {
      // Create transaction and initialise phase and t
      phase = tlm::BEGIN_REQ;

      logStartTransation(trans);

      if (socket->nb_transport(trans, phase, mQuantumKeeper.getLocalTime())) {
        // Transaction finished
        // Slave may have added a delay to the quantum -> sync if needed
        if (mQuantumKeeper.needSync()) {
          std::cerr << "Sync'ing..." << std::endl;
          mQuantumKeeper.sync();
        }

      } else {
        // Transaction not yet finished, wait for the end of it
        wait(socket.getEndEvent());
        mQuantumKeeper.reset();
      }

      logEndTransaction(trans);
    }
  }

private:
  sc_event mEndEvent;
  unsigned int mNrOfTransactions;
  unsigned int mBaseAddress;
  unsigned int mTransactionCount;
  unsigned int mData;
  tlm::tlm_quantumkeeper mQuantumKeeper;
};

#endif
