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
  typedef SimpleSlaveSocket<transaction_type> slave_socket_type;
  typedef SimpleMasterSocket<transaction_type> master_socket_type;

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
     }
     for (unsigned int i = 0; i < NR_OF_SLAVES; ++i) {
       REGISTER_SOCKETPROCESS(master_socket[i], slaveNBTransport);
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
      std::cerr << name() << ": Switching to LT mode" << std::endl;
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
    std::cerr << name() << ": Switching to AT mode" << std::endl;
    mAbstraction = TLM_AT;
    return true;
  }

  //
  // Dummy decoder
  //
  master_socket_type* decode(transaction_type& trans)
  {
    // decode address:
    // - substract base address
    // - return master socket

    const sc_dt::uint64 address = trans.get_address();

    unsigned int portId = (unsigned int)address >> 28;
    trans.set_address(address & 0xfffffff);

    return &master_socket[portId];
  }

  //
  // LT protocol
  // - forward each nb_transport call to the target/initiator
  //

  bool masterNBTransportLT(transaction_type& trans, phase_type& phase, sc_time& t)
  {
    master_socket_type* decodeSocket;

    if (phase == tlm::BEGIN_REQ) {
      // new transaction: decode
      decodeSocket = decode(trans);
      addPendingTransaction(trans, decodeSocket);

    } else if (phase == tlm::END_RESP) {
      // end of response phase
      PendingTransactionsIterator it = mPendingTransactions.find(&trans);
      assert(it != mPendingTransactions.end());

      decodeSocket = it->second.to;

    } else {
      std::cerr << "ERROR: '" << name()
                << "': Illegal phase received from master: " << phase << std::endl;
      assert(false); exit(1);
    }

    // FIXME: No limitation on number of pending transactions
    //        All slaves (that return false) must support multiple transactions
    if (!(*decodeSocket)->nb_transport(trans, phase, t)) {
      // Transaction not yet finished
      return false;

    } else {
      // Transaction finished
      mPendingTransactions.erase(&trans);
      return true;
    }
  }

  bool slaveNBTransportLT(transaction_type& trans, phase_type& phase, sc_time& t)
  {
    if (phase != tlm::END_REQ && phase != tlm::BEGIN_RESP) {
      std::cerr << "ERROR: '" << name()
                << "': Illegal phase received from slave:" << phase << std::endl;
      assert(false); exit(1);
    }

    PendingTransactionsIterator it = mPendingTransactions.find(&trans);
    assert(it != mPendingTransactions.end());

    if ((*it->second.from)->nb_transport(trans, phase, t)) {
      // Transaction finished
      mPendingTransactions.erase(it);
      return true;

    } else {
      // Transaction not yet finished
      return false;
    }
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
        master_socket_type* decodeSocket = decode(*trans);

        // Fill in the destination port
        PendingTransactionsIterator it = mPendingTransactions.find(trans);
        assert(it != mPendingTransactions.end());
        it->second.to = decodeSocket;

        phase_type phase = tlm::BEGIN_REQ;
        sc_time t = SC_ZERO_TIME;

        // FIXME: No limitation on number of pending transactions
        //        All slaves (that return false) must support multiple transactions
        if (!(*decodeSocket)->nb_transport(*trans, phase, t)) {
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

        } else {
          // Transaction finished
          mResponsePEQ.notify(*trans, t);

          // reset to destination port (we must not send END_RESP to target)
          it->second.to = 0;

          wait(t);
        }
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

        if ((*it->second.from)->nb_transport(*trans, phase, t)) {
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

        } else {
          // Transaction not yet finished
          wait(mEndResponseEvent);
        }
      }
    }
  }

  bool masterNBTransportAT(transaction_type& trans, phase_type& phase, sc_time& t)
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
        bool r = (*it->second.to)->nb_transport(trans, phase, t);
        assert(r);
      }

      mPendingTransactions.erase(it);

      mEndResponseEvent.notify(t);
      return true;

    } else {
      std::cerr << "ERROR: '" << name()
                << "': Illegal phase received from master." << std::endl;
      assert(false); exit(1);
    }

    return false;
  }

  bool slaveNBTransportAT(transaction_type& trans, phase_type& phase, sc_time& t)
  {
    if (phase != tlm::END_REQ && phase != tlm::BEGIN_RESP) {
      std::cerr << "ERROR: '" << name()
                << "': Illegal phase received from slave." << std::endl;
      assert(false); exit(1);
    }

    mEndRequestEvent.notify(t);

    if (phase == tlm::BEGIN_RESP) {
      mResponsePEQ.notify(trans, t);
    }

    return false;
  }

  //
  // interface methods
  //

  bool masterNBTransport(transaction_type& trans, phase_type& phase, sc_time& t)
  {
    if (mAbstraction == TLM_LT) {
      return masterNBTransportLT(trans, phase, t);

    } else { // TLM_AT
      return masterNBTransportAT(trans, phase, t);
    }
  }

  bool slaveNBTransport(transaction_type& trans, phase_type& phase, sc_time& t)
  {
    if (mAbstraction == TLM_LT) {
      return slaveNBTransportLT(trans, phase, t);

    } else { // TLM_AT
      return slaveNBTransportAT(trans, phase, t);
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
