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

#ifndef SIMPLE_LT_MASTER2_H
#define SIMPLE_LT_MASTER2_H

//#include "tlm.h"
#include "simple_master_socket.h"
//#include <systemc.h>
#include <cassert>
//#include <iostream>

class SimpleLTMaster2 : public sc_module
{
public:
  typedef tlm::tlm_transaction transaction_type;
  typedef tlm::tlm_phase phase_type;
  typedef SimpleMasterSocket<transaction_type> master_socket_type;

public:
  master_socket_type socket;

public:
  SC_HAS_PROCESS(SimpleLTMaster2);
  SimpleLTMaster2(sc_module_name name,
                  unsigned int nrOfTransactions = 0x5,
                  unsigned int baseAddress = 0x0) :
    sc_module(name),
    socket("socket"),
    mNrOfTransactions(nrOfTransactions),
    mBaseAddress(baseAddress),
    mTransactionCount(0)
  {
    // register nb_transport method
    REGISTER_SOCKETPROCESS(socket, myNBTransport);

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
                << " @ " << sc_time_stamp() << std::endl;

    } else {
      std::cerr << name() << ": Send read request: A = "
                << (void*)(int)trans.getAddress()
                << " @ " << sc_time_stamp() << std::endl;
    }
  }

  void logEndTransaction(transaction_type& trans)
  {
    if (!trans.getResponse()) {
      std::cerr << name() << ": Received error response @ "
                << sc_time_stamp() << std::endl;

    } else {
      std::cerr << name() <<  ": Received ok response";
      if (trans.isRead()) {
        std::cerr << ": D = " << (void*)mData;
      }
      std::cerr << " @ " << sc_time_stamp() << std::endl;
    }
  }

  void run()
  {
    transaction_type trans;
    phase_type phase;
    sc_time t;

    while (initTransaction(trans)) {
      // Create transaction and initialise phase and t
      phase = tlm::BEGIN_REQ;
      t = SC_ZERO_TIME;

      logStartTransation(trans);

      if (socket->nb_transport(trans, phase, t)) {
        // Transaction Finished, wait for the returned delay
        wait(t);

      } else {
        // Transaction not yet finished, wait for the end of it
        wait(mEndEvent);
      }

      logEndTransaction(trans);
    }
  }

  bool myNBTransport(transaction_type& trans, phase_type& phase, sc_time& t)
  {
    switch (phase) {
    case tlm::END_REQ:
      // Request phase ended
      return false;
    case tlm::BEGIN_RESP:
      assert(t == SC_ZERO_TIME); // FIXME: can t != 0?
      mEndEvent.notify(t);
      // Not needed to update the phase if true is returned
      return true;
    case tlm::BEGIN_REQ: // fall-through
    case tlm::END_RESP: // fall-through
    default:
      // A slave should never call nb_transport with these phases
      assert(0); exit(1);
      return false;
    }
    return false;
  }


private:
  sc_event mEndEvent;
  unsigned int mNrOfTransactions;
  unsigned int mBaseAddress;
  unsigned int mTransactionCount;
  unsigned int mData;
};

#endif
