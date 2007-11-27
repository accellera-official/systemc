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

#ifndef __SIMPLE_LT_INITIATOR2_H__
#define __SIMPLE_LT_INITIATOR2_H__

#include "tlm.h"
#include "simple_initiator_socket.h"
//#include <systemc>
#include <cassert>
//#include <iostream>

class SimpleLTInitiator2 : public sc_core::sc_module
{
public:
  typedef tlm::tlm_generic_payload        transaction_type;
  typedef tlm::tlm_dmi_mode               dmi_mode_type;
  typedef tlm::tlm_phase                  phase_type;
  typedef tlm::tlm_sync_enum              sync_enum_type;
  typedef SimpleInitiatorSocket<>         initiator_socket_type;

public:
  initiator_socket_type socket;

public:
  SC_HAS_PROCESS(SimpleLTInitiator2);
  SimpleLTInitiator2(sc_core::sc_module_name name,
                     unsigned int nrOfTransactions = 0x5,
                     unsigned int baseAddress = 0x0) :
    sc_core::sc_module(name),
    socket("socket"),
    mNrOfTransactions(nrOfTransactions),
    mBaseAddress(baseAddress),
    mTransactionCount(0)
  {
    // register nb_transport method
    REGISTER_NBTRANSPORT(socket, myNBTransport);

    // Initiator thread
    SC_THREAD(run);
  }

  bool initTransaction(transaction_type& trans)
  {
    if (mTransactionCount < mNrOfTransactions) {
      trans.set_address(mBaseAddress + 4*mTransactionCount);
      mData = mTransactionCount;
      trans.set_data_ptr(reinterpret_cast<unsigned char*>(&mData));
      trans.set_command(tlm::TLM_WRITE_COMMAND);

    } else if (mTransactionCount < 2 * mNrOfTransactions) {
      trans.set_address(mBaseAddress + 4*(mTransactionCount - mNrOfTransactions));
      mData = 0;
      trans.set_data_ptr(reinterpret_cast<unsigned char*>(&mData));
      trans.set_command(tlm::TLM_READ_COMMAND);

    } else {
      return false;
    }

    ++mTransactionCount;
    return true;
  }

  void logStartTransation(transaction_type& trans)
  {
    if (trans.get_command() == tlm::TLM_WRITE_COMMAND) {
      std::cout << name() << ": Send write request: A = 0x"
                << std::hex << (unsigned int)trans.get_address()
                << ", D = 0x" << mData << std::dec
                << " @ " << sc_core::sc_time_stamp() << std::endl;

    } else {
      std::cout << name() << ": Send read request: A = 0x"
                << std::hex << (unsigned int)trans.get_address() << std::dec
                << " @ " << sc_core::sc_time_stamp() << std::endl;
    }
  }

  void logEndTransaction(transaction_type& trans)
  {
    if (trans.get_response_status() != tlm::TLM_OK_RESPONSE) {
      std::cout << name() << ": Received error response @ "
                << sc_core::sc_time_stamp() << std::endl;

    } else {
      std::cout << name() <<  ": Received ok response";
      if (trans.get_command() == tlm::TLM_READ_COMMAND) {
        std::cout << ": D = 0x" << std::hex << mData << std::dec;
      }
      std::cout << " @ " << sc_core::sc_time_stamp() << std::endl;
    }
  }

  void run()
  {
    transaction_type trans;
    phase_type phase;
    sc_core::sc_time t;

    while (initTransaction(trans)) {
      // Create transaction and initialise phase and t
      phase = tlm::BEGIN_REQ;
      t = sc_core::SC_ZERO_TIME;

      logStartTransation(trans);

      switch (socket->nb_transport(trans, phase, t)) {
      case tlm::TLM_COMPLETED:
        // Transaction Finished, wait for the returned delay
        wait(t);
        break;

      case tlm::TLM_ACCEPTED:
      case tlm::TLM_UPDATED:
        // Transaction not yet finished, wait for the end of it
        wait(mEndEvent);
        break;

      case tlm::TLM_REJECTED:
        // FIXME: Not supported (wait and retry same transaction)
      default:
        assert(0); exit(1);
      };

      logEndTransaction(trans);
    }
    sc_core::sc_stop();
    wait();

  }

  sync_enum_type myNBTransport(transaction_type& trans, phase_type& phase, sc_core::sc_time& t)
  {
    switch (phase) {
    case tlm::END_REQ:
      // Request phase ended
      return tlm::TLM_ACCEPTED;

    case tlm::BEGIN_RESP:
      assert(t == sc_core::SC_ZERO_TIME); // FIXME: can t != 0?
      mEndEvent.notify(t);
      // Not needed to update the phase if true is returned
      return tlm::TLM_COMPLETED;

    case tlm::BEGIN_REQ: // fall-through
    case tlm::END_RESP: // fall-through
    default:
      // A target should never call nb_transport with these phases
      assert(0); exit(1);
      return tlm::TLM_REJECTED;
    };
  }


private:
  sc_core::sc_event mEndEvent;
  unsigned int mNrOfTransactions;
  unsigned int mBaseAddress;
  unsigned int mTransactionCount;
  unsigned int mData;
};

#endif
