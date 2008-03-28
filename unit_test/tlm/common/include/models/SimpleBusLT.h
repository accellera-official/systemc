/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
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

#ifndef __SIMPLEBUSLT_H__
#define __SIMPLEBUSLT_H__

//#include <systemc>
#include "tlm.h"

#include "simple_target_socket.h"
#include "simple_initiator_socket.h"

#include "MyPEQ.h"

template <int NR_OF_INITIATORS, int NR_OF_TARGETS>
class SimpleBusLT : public sc_core::sc_module
{
public:
  typedef tlm::tlm_generic_payload                 transaction_type;
  typedef tlm::tlm_phase                           phase_type;
  typedef tlm::tlm_sync_enum                       sync_enum_type;
  typedef SimpleTargetSocketTagged<SimpleBusLT>    target_socket_type;
  typedef SimpleInitiatorSocketTagged<SimpleBusLT> initiator_socket_type;

public:
  target_socket_type target_socket[NR_OF_INITIATORS];
  initiator_socket_type initiator_socket[NR_OF_TARGETS];

public:
  SC_HAS_PROCESS(SimpleBusLT);
  SimpleBusLT(sc_core::sc_module_name name) :
    sc_core::sc_module(name)
  {
    for (unsigned int i = 0; i < NR_OF_INITIATORS; ++i) {
      target_socket[i].registerNBTransport(this, &SimpleBusLT::initiatorNBTransport, i);
      target_socket[i].registerBTransport(this, &SimpleBusLT::initiatorBTransport, i);
      target_socket[i].registerDebugTransport(this, &SimpleBusLT::transportDebug, i);
      target_socket[i].registerDMI(this, &SimpleBusLT::getDMIPointer, i);
    }
    for (unsigned int i = 0; i < NR_OF_TARGETS; ++i) {
      initiator_socket[i].registerNBTransport_bw(this, &SimpleBusLT::targetNBTransport, i);
      initiator_socket[i].registerInvalidateDMI(this, &SimpleBusLT::invalidateDMIPointers, i);
    }
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
    // - return initiator socket id

    return getPortId(address);
  }

  //
  // interface methods
  //

  //
  // LT protocol
  // - forward each call to the target/initiator
  //
  void initiatorBTransport(int SocketId,
                           transaction_type& trans,
                           sc_core::sc_time& t)
  {
    initiator_socket_type* decodeSocket;
    unsigned int portId = decode(trans.get_address());
    assert(portId < NR_OF_TARGETS);
    decodeSocket = &initiator_socket[portId];
    trans.set_address(trans.get_address() & getAddressMask(portId));

    (*decodeSocket)->b_transport(trans, t);
  }

  sync_enum_type initiatorNBTransport(int SocketId,
                                      transaction_type& trans,
                                      phase_type& phase,
                                        sc_core::sc_time& t)
  {
    initiator_socket_type* decodeSocket;

    if (phase == tlm::BEGIN_REQ) {
      // new transaction: decode
      unsigned int portId = decode(trans.get_address());
      assert(portId < NR_OF_TARGETS);
      decodeSocket = &initiator_socket[portId];
      trans.set_address(trans.get_address() & getAddressMask(portId));
      addPendingTransaction(trans, decodeSocket, SocketId);

    } else if (phase == tlm::END_RESP) {
      // end of response phase
      PendingTransactionsIterator it = mPendingTransactions.find(&trans);
      assert(it != mPendingTransactions.end());

      decodeSocket = it->second.to;

    } else {
      std::cout << "ERROR: '" << name()
                << "': Illegal phase received from initiator: " << phase << std::endl;
      assert(false); exit(1);
    }

    // FIXME: No limitation on number of pending transactions
    //        All targets (that return false) must support multiple
    //        transactions
    sync_enum_type r = (*decodeSocket)->nb_transport_fw(trans, phase, t);
    if (r == tlm::TLM_COMPLETED) {
      // Transaction finished
      mPendingTransactions.erase(&trans);
    }

    return r;
  }

  sync_enum_type targetNBTransport(int target_index,
                                   transaction_type& trans,
                                   phase_type& phase,
                                   sc_core::sc_time& t)
  {
    if (phase != tlm::END_REQ && phase != tlm::BEGIN_RESP) {
      std::cout << "ERROR: '" << name()
                << "': Illegal phase received from target:" << phase
                << std::endl;
      assert(false); exit(1);
    }

    PendingTransactionsIterator it = mPendingTransactions.find(&trans);
    assert(it != mPendingTransactions.end());

    sync_enum_type r = (*it->second.from)->nb_transport_bw(trans, phase, t);
    if (r == tlm::TLM_COMPLETED) {
      // Transaction finished
      mPendingTransactions.erase(it);
    }

    return r;
  }

  unsigned int transportDebug(int SocketId,
                              transaction_type& trans)
  {
    unsigned int portId = decode(trans.get_address());
    assert(portId < NR_OF_TARGETS);
    initiator_socket_type* decodeSocket = &initiator_socket[portId];
    trans.set_address( trans.get_address() & getAddressMask(portId) );
    
    return (*decodeSocket)->transport_dbg(trans);
  }

  bool limitRange(unsigned int portId, sc_dt::uint64& low, sc_dt::uint64& high)
  {
    sc_dt::uint64 addressOffset = getAddressOffset(portId);
    sc_dt::uint64 addressMask = getAddressMask(portId);

    if (low > addressMask) {
      // Range does not overlap with addressrange for this target
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

  bool getDMIPointer(int SocketId,
                     transaction_type& trans,
                     tlm::tlm_dmi&  dmi_data)
  {
    sc_dt::uint64 address = trans.get_address();

    unsigned int portId = decode(address);
    assert(portId < NR_OF_TARGETS);
    initiator_socket_type* decodeSocket = &initiator_socket[portId];
    sc_dt::uint64 maskedAddress = address & getAddressMask(portId);

    trans.set_address(maskedAddress);

    bool result =
      (*decodeSocket)->get_direct_mem_ptr(trans, dmi_data);
    
    if (result)
    {
      // Range must contain address
      assert(dmi_data.get_start_address() <= maskedAddress);
      assert(dmi_data.get_end_address() >= maskedAddress);
    }
    
    // Should always succeed
	sc_dt::uint64 start, end;
	start = dmi_data.get_start_address();
	end = dmi_data.get_end_address();
	
	limitRange(portId, start, end);
	
	dmi_data.set_start_address(start);
	dmi_data.set_end_address(end);

    return result;
  }

  void invalidateDMIPointers(int port_id,
                             sc_dt::uint64 start_range,
                             sc_dt::uint64 end_range)
  {
    // FIXME: probably faster to always invalidate everything?

    if (!limitRange(port_id, start_range, end_range)) {
      // Range does not fall into address range of target
      return;
    }
    
    for (unsigned int i = 0; i < NR_OF_INITIATORS; ++i) {
      (target_socket[i])->invalidate_direct_mem_ptr(start_range, end_range);
    }
  }

private:
  void addPendingTransaction(transaction_type& trans,
                             initiator_socket_type* to,
                             int initiatorId)
  {
    const ConnectionInfo info = { &target_socket[initiatorId], to };
    assert(mPendingTransactions.find(&trans) == mPendingTransactions.end());
    mPendingTransactions[&trans] = info;
  }

private:
  struct ConnectionInfo {
    target_socket_type* from;
    initiator_socket_type* to;
  };
  typedef std::map<transaction_type*, ConnectionInfo> PendingTransactions;
  typedef typename PendingTransactions::iterator PendingTransactionsIterator;
  typedef typename PendingTransactions::const_iterator PendingTransactionsConstIterator;

  enum tlm_abstraction { TLM_LT, TLM_AT };

private:
  PendingTransactions mPendingTransactions;
};

#endif
