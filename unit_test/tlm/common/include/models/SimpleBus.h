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

#ifndef SIMPLEBUS_H
#define SIMPLEBUS_H

//#include <systemc.h>
//#include "tlm.h"

#include "simple_slave_socket.h"
#include "simple_master_socket.h"
#include "simple_socket_utils.h"

#include "MyPEQ.h"

template <int NR_OF_MASTERS, int NR_OF_SLAVES>
class SimpleBus : public sc_module
{
public:
  typedef tlm::tlm_generic_payload transaction_type;
  typedef tlm::tlm_phase phase_type;
  typedef tlm::tlm_sync_enum sync_enum_type;
  typedef SimpleSlaveSocket<> slave_socket_type;
  typedef SimpleMasterSocket<> master_socket_type;

public:
  slave_socket_type slave_socket[NR_OF_MASTERS];
  master_socket_type master_socket[NR_OF_SLAVES];

public:
  SC_HAS_PROCESS(SimpleBus);
  SimpleBus(sc_module_name name) :
    sc_module(name),
    mAbstraction(TLM_LT),
    mRequestPEQ("requestPEQ"),
    mResponsePEQ("responsePEQ")
  {
     for (unsigned int i = 0; i < NR_OF_MASTERS; ++i) {
       REGISTER_SOCKETPROCESS_USER(slave_socket[i], masterNBTransport, i);
       REGISTER_SOCKETPROCESS_USER(slave_socket[i], transportDebug, i);
       REGISTER_SOCKETPROCESS_USER(slave_socket[i], getDMIPointer, i);
     }
     for (unsigned int i = 0; i < NR_OF_SLAVES; ++i) {
       REGISTER_SOCKETPROCESS(master_socket[i], slaveNBTransport);
       REGISTER_SOCKETPROCESS(master_socket[i], invalidateDMIPointers);
     }

     SC_THREAD(RequestThread);
     SC_THREAD(ResponseThread);
  }

  bool setLTMode()
  {
    if (mAbstraction == TLM_LT) {
      return true;
    }

    if (mPendingTransactions.empty()) {
      std::cout << name() << ": Switching to LT mode" << std::endl;
      mAbstraction = TLM_LT;
      return true;

    } else {
      //FIXME: Not safe to switch to LT mode while there are pending transactions
      return false;
    }
  }

  bool setATMode()
  {
    if (mAbstraction == TLM_AT) {
      return true;
    }

    // Switching from LT -> AT is always possible
    // (END_REQ may not be forwarded to the master correctly)
    std::cout << name() << ": Switching to AT mode" << std::endl;
    mAbstraction = TLM_AT;

    // Invalidate all DMI pointers
    invalidateDMIPointers(true, 0, 0);

    return true;
  }

  //
  // Dummy decoder:
  // - address[31-28]: portId
  // - address[27-0]: masked address
  //

  unsigned int getPortId(const sc_dt::uint64& address)
  {
    return (unsigned int)address >> 28;
  }

  sc_dt::uint64 getAddressOffset(unsigned int portId)
  {
    return portId << 28;
  }

  sc_dt::uint64 getAddressMask(unsigned int portId)
  {
    return 0xfffffff;
  }

  unsigned int decode(const sc_dt::uint64& address)
  {
    // decode address:
    // - return master socket id

    return getPortId(address);
  }

  //
  // LT protocol
  // - forward each nb_transport call to the target/initiator
  //

  sync_enum_type masterNBTransportLT(transaction_type& trans, phase_type& phase, sc_time& t)
  {
    master_socket_type* decodeSocket;

    if (phase == tlm::BEGIN_REQ) {
      // new transaction: decode
      unsigned int portId = decode(trans.get_address());
      assert(portId < NR_OF_SLAVES);
      decodeSocket = &master_socket[portId];
      trans.set_address(trans.get_address() & getAddressMask(portId));
      addPendingTransaction(trans, decodeSocket);

    } else if (phase == tlm::END_RESP) {
      // end of response phase
      PendingTransactionsIterator it = mPendingTransactions.find(&trans);
      assert(it != mPendingTransactions.end());

      decodeSocket = it->second.to;

    } else {
      std::cout << "ERROR: '" << name()
                << "': Illegal phase received from master: " << phase << std::endl;
      assert(false); exit(1);
    }

    // FIXME: No limitation on number of pending transactions
    //        All slaves (that return false) must support multiple transactions
    sync_enum_type r = (*decodeSocket)->nb_transport(trans, phase, t);
    if (r == tlm::TLM_COMPLETED) {
      // Transaction finished
      mPendingTransactions.erase(&trans);
    }

    return r;
  }

  sync_enum_type slaveNBTransportLT(transaction_type& trans, phase_type& phase, sc_time& t)
  {
    if (phase != tlm::END_REQ && phase != tlm::BEGIN_RESP) {
      std::cout << "ERROR: '" << name()
                << "': Illegal phase received from slave:" << phase << std::endl;
      assert(false); exit(1);
    }

    PendingTransactionsIterator it = mPendingTransactions.find(&trans);
    assert(it != mPendingTransactions.end());

    sync_enum_type r = (*it->second.from)->nb_transport(trans, phase, t);
    if (r == tlm::TLM_COMPLETED) {
      // Transaction finished
      mPendingTransactions.erase(it);
    }

    return r;
  }

  //
  // AT protocol
  //

  void RequestThread()
  {
    while (true) {
      wait(mRequestPEQ.getEvent());

      transaction_type* trans;
      while ((trans = mRequestPEQ.getNextTransaction())) {
        unsigned int portId = decode(trans->get_address());
        assert(portId < NR_OF_SLAVES);
        master_socket_type* decodeSocket = &master_socket[portId];
        trans->set_address(trans->get_address() & getAddressMask(portId));

        // Fill in the destination port
        PendingTransactionsIterator it = mPendingTransactions.find(trans);
        assert(it != mPendingTransactions.end());
        it->second.to = decodeSocket;

        phase_type phase = tlm::BEGIN_REQ;
        sc_time t = SC_ZERO_TIME;

        // FIXME: No limitation on number of pending transactions
        //        All slaves (that return false) must support multiple transactions
        switch ((*decodeSocket)->nb_transport(*trans, phase, t)) {
        case tlm::TLM_SYNC:
        case tlm::TLM_SYNC_CONTINUE:
          // Transaction not yet finished
          if (phase == tlm::BEGIN_REQ) {
            // Request phase not yet finished
            wait(mEndRequestEvent);

          } else if (phase == tlm::END_REQ) {
            // Request phase finished, but response phase not yet started
            wait(t);

          } else if (phase == tlm::BEGIN_RESP) {
            // FIXME: Do we need to notify END_REQ to the initiator?
            mResponsePEQ.notify(*trans, t);
            continue;

          } else { // END_RESP
            assert(0); exit(1);
          }

          phase = tlm::END_REQ;
          t = SC_ZERO_TIME;
          (*it->second.from)->nb_transport(*trans, phase, t);
          break;

        case tlm::TLM_COMPLETED:
          // Transaction finished
          mResponsePEQ.notify(*trans, t);

          // reset to destination port (we must not send END_RESP to target)
          it->second.to = 0;

          wait(t);
          break;

        case tlm::TLM_REJECTED:
          // FIXME: Not supported (wait and retry same transaction)
        default:
          assert(0); exit(1);
        };
      }
    }
  }

  void ResponseThread()
  {
    while (true) {
      wait(mResponsePEQ.getEvent());

      transaction_type* trans;
      while ((trans = mResponsePEQ.getNextTransaction())) {
        PendingTransactionsIterator it = mPendingTransactions.find(trans);
        assert(it != mPendingTransactions.end());

        phase_type phase = tlm::BEGIN_RESP;
        sc_time t = SC_ZERO_TIME;

        switch ((*it->second.from)->nb_transport(*trans, phase, t)) {
        case tlm::TLM_COMPLETED:
          // Transaction finished

          // Transaction may already be deleted (or re-used)
          // --> immediately notify target
          if (it->second.to) {
            phase = tlm::END_RESP;
            bool r = (*it->second.to)->nb_transport(*trans, phase, t);
            assert(r);
          }
          mPendingTransactions.erase(it);
          wait(t);
          break;

        case tlm::TLM_SYNC:
        case tlm::TLM_SYNC_CONTINUE:
          // Transaction not yet finished
          wait(mEndResponseEvent);
        break;

        case tlm::TLM_REJECTED:
          // FIXME: Not supported (wait and retry same transaction)
        default:
          assert(0); exit(1);
        };
      }
    }
  }

  sync_enum_type masterNBTransportAT(transaction_type& trans, phase_type& phase, sc_time& t)
  {
    if (phase == tlm::BEGIN_REQ) {
      addPendingTransaction(trans, 0);

      mRequestPEQ.notify(trans, t);

    } else if (phase == tlm::END_RESP) {
      // after returning true the transaction can be deleted (or re-used)
      // --> immediately forward call to target
      PendingTransactionsIterator it = mPendingTransactions.find(&trans);
      assert(it != mPendingTransactions.end());

      if (it->second.to) {
        phase = tlm::END_RESP;
        sync_enum_type r = (*it->second.to)->nb_transport(trans, phase, t);
        assert(r == tlm::TLM_COMPLETED);
      }

      mPendingTransactions.erase(it);

      mEndResponseEvent.notify(t);
      return tlm::TLM_COMPLETED;

    } else {
      std::cout << "ERROR: '" << name()
                << "': Illegal phase received from master." << std::endl;
      assert(false); exit(1);
    }

    return tlm::TLM_SYNC;
  }

  sync_enum_type slaveNBTransportAT(transaction_type& trans, phase_type& phase, sc_time& t)
  {
    if (phase != tlm::END_REQ && phase != tlm::BEGIN_RESP) {
      std::cout << "ERROR: '" << name()
                << "': Illegal phase received from slave." << std::endl;
      assert(false); exit(1);
    }

    mEndRequestEvent.notify(t);

    if (phase == tlm::BEGIN_RESP) {
      mResponsePEQ.notify(trans, t);
    }

    return tlm::TLM_SYNC;
  }

  //
  // interface methods
  //

  sync_enum_type masterNBTransport(transaction_type& trans, phase_type& phase, sc_time& t)
  {
    if (mAbstraction == TLM_LT) {
      return masterNBTransportLT(trans, phase, t);

    } else { // TLM_AT
      return masterNBTransportAT(trans, phase, t);
    }
  }

  sync_enum_type slaveNBTransport(transaction_type& trans, phase_type& phase, sc_time& t)
  {
    if (mAbstraction == TLM_LT) {
      return slaveNBTransportLT(trans, phase, t);

    } else { // TLM_AT
      return slaveNBTransportAT(trans, phase, t);
    }
  }

  unsigned int transportDebug(tlm::tlm_debug_payload& trans)
  {
    unsigned int portId = decode(trans.address);
    assert(portId < NR_OF_SLAVES);
    master_socket_type* decodeSocket = &master_socket[portId];
    trans.address &= getAddressMask(portId);
    
    return (*decodeSocket)->transport_dbg(trans);
  }

  bool limitRange(unsigned int portId, sc_dt::uint64& low, sc_dt::uint64& high)
  {
    sc_dt::uint64 addressOffset = getAddressOffset(portId);
    sc_dt::uint64 addressMask = getAddressMask(portId);

    if (low > addressMask) {
      // Range does not overlap with addressrange for this slave
      return false;
    }

    low += addressOffset;
    if (high > addressMask) {
      high = addressOffset + addressMask;

    } else {
      high += addressOffset;
    }
    return true;
  }

  bool getDMIPointer(const sc_dt::uint64& address,
                     bool forReads,
                     tlm::tlm_dmi& dmi_data)
  {
    if (mAbstraction == TLM_AT) {
      // DMI not supported if the bus operates in AT mode
      dmi_data.dmi_start_address = 0x0;
      dmi_data.dmi_end_address = (sc_dt::uint64)-1;
    }

    unsigned int portId = decode(address);
    assert(portId < NR_OF_SLAVES);
    master_socket_type* decodeSocket = &master_socket[portId];
    sc_dt::uint64 maskedAddress = address & getAddressMask(portId);
    
    bool result =
      (*decodeSocket)->get_direct_mem_ptr(maskedAddress, forReads, dmi_data);
    
    // Range must contain address
    assert(dmi_data.dmi_start_address <= maskedAddress);
    assert(dmi_data.dmi_end_address >= maskedAddress);

    // Should always succeed
    limitRange(portId, dmi_data.dmi_start_address, dmi_data.dmi_end_address);

    return result;
  }

  void invalidateDMIPointers(bool invalidate_all,
                             sc_dt::uint64 start_range,
                             sc_dt::uint64 end_range)
  {
    // FIXME: probably faster to always invalidate everything?
    if (invalidate_all) {
      for (unsigned int i = 0; i < NR_OF_MASTERS; ++i) {
        (slave_socket[i])->invalidate_direct_mem_ptr(true, 0, 0);
      }

    } else {
      unsigned int portId = 
        simple_socket_utils::simple_socket_user::instance().get_user_id();

      if (!limitRange(portId, start_range, end_range)) {
        // Range does not fall into address range of slave
        return;
      }
      
      for (unsigned int i = 0; i < NR_OF_MASTERS; ++i) {
        (slave_socket[i])->invalidate_direct_mem_ptr(false, start_range, end_range);
      }
    }
  }

private:
  void addPendingTransaction(transaction_type& trans, master_socket_type* to)
  {
    int masterId =
      simple_socket_utils::simple_socket_user::instance().get_user_id();
    const ConnectionInfo info = { &slave_socket[masterId], to };
    assert(mPendingTransactions.find(&trans) == mPendingTransactions.end());
    mPendingTransactions[&trans] = info;
  }

private:
  struct ConnectionInfo {
    slave_socket_type* from;
    master_socket_type* to;
  };
  typedef std::map<transaction_type*, ConnectionInfo> PendingTransactions;
  typedef typename PendingTransactions::iterator PendingTransactionsIterator;
  typedef typename PendingTransactions::const_iterator PendingTransactionsConstIterator;

  enum tlm_abstraction { TLM_LT, TLM_AT };

private:
  tlm_abstraction mAbstraction;

  PendingTransactions mPendingTransactions;

  MyPEQ mRequestPEQ;
  sc_event mBeginRequestEvent;
  sc_event mEndRequestEvent;

  MyPEQ mResponsePEQ;
  sc_event mBeginResponseEvent;
  sc_event mEndResponseEvent;
};

#endif
