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

#ifndef __SIMPLE_LT_INITIATOR2_DMI_H__
#define __SIMPLE_LT_INITIATOR2_DMI_H__

#include "tlm.h"
#include "simple_initiator_socket.h"
#include <systemc>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <map>

class SimpleLTInitiator2_dmi : public sc_core::sc_module
{
public:
  typedef tlm::tlm_generic_payload transaction_type;
  typedef tlm::tlm_dmi dmi_type;
  typedef tlm::tlm_phase phase_type;
  typedef tlm::tlm_sync_enum sync_enum_type;
  typedef SimpleInitiatorSocket<> initiator_socket_type;

public:
  initiator_socket_type socket;

public:
  SC_HAS_PROCESS(SimpleLTInitiator2_dmi);
  SimpleLTInitiator2_dmi(sc_core::sc_module_name name,
                  unsigned int nrOfTransactions = 0x5,
                  unsigned int baseAddress = 0x0) :
    sc_core::sc_module(name),
    socket("socket"),
    mNrOfTransactions(nrOfTransactions),
    mBaseAddress(baseAddress),
    mTransactionCount(0)
  {
    mDMIDataReads.first.dmi_start_address = 1;
    mDMIDataReads.first.dmi_end_address = 0;
    mDMIDataWrites.first.dmi_start_address = 1;
    mDMIDataWrites.first.dmi_end_address = 0;

    // register nb_transport method
    REGISTER_NBTRANSPORT(socket, myNBTransport);
    REGISTER_INVALIDATEDMI(socket, invalidate_direct_mem_ptr);

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
      trans.set_address(mBaseAddress + 4*(mTransactionCount-mNrOfTransactions));
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

  std::pair<dmi_type, bool>& getDMIData(const transaction_type& trans)
  {
    if (trans.get_command() == tlm::TLM_READ_COMMAND) {
      return mDMIDataReads;

    } else { // WRITE
      return mDMIDataWrites;
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

      ///////////////////////////////////////////////////////////
      // DMI handling:
      // We do *not* use the DMI hint to check if it makes sense to ask for
      // DMI pointers. So the pattern is:
      // - if the address is not covered by a DMI region try to acquire DMI
      //   pointers
      // - if we have a DMI pointer, do the DMI "transaction"
      // - otherwise fall back to a normal transaction
      ///////////////////////////////////////////////////////////

      std::pair<dmi_type, bool>& dmi_data = getDMIData(trans);

      // Check if we need to acquire a DMI pointer
      if((trans.get_address() < dmi_data.first.dmi_start_address) ||
         (trans.get_address() > dmi_data.first.dmi_end_address) )
      {
          dmi_data.second =
            socket->get_direct_mem_ptr(trans.get_address(),
                                       trans.get_command() == tlm::TLM_READ_COMMAND,
                                       dmi_data.first);
      }
      // Do DMI "transaction" if we have a valid region
      if (dmi_data.second &&
          (trans.get_address() >= dmi_data.first.dmi_start_address) &&
          (trans.get_address() <= dmi_data.first.dmi_end_address) )
      {
          // We can handle the data here. As the logEndTransaction is assuming
          // something to happen in the data structure, we really need to
          // do this:
          trans.set_response_status(tlm::TLM_OK_RESPONSE);
          sc_dt::uint64 tmp = trans.get_address() - dmi_data.first.dmi_start_address;
          assert(hasHostEndianness(dmi_data.first.endianness));
          if (trans.get_command() == tlm::TLM_WRITE_COMMAND)
          {
              *(unsigned int*)&dmi_data.first.dmi_ptr[tmp] = mData;
          }
          else
          {
              mData = *(unsigned int*)&dmi_data.first.dmi_ptr[tmp];
          }
          
          // Do the wait immediately. Note that doing the wait here eats almost
          // all the performance anyway, so we only gain something if we're
          // using temporal decoupling.
          if (trans.get_command() == tlm::TLM_WRITE_COMMAND) {
            wait(dmi_data.first.write_latency);

          } else {
            wait(dmi_data.first.read_latency);
          }
      }
      else // we need a full transaction
      {
          switch (socket->nb_transport(trans, phase, t)) {
          case tlm::TLM_COMPLETED:
              // Transaction Finished, wait for the returned delay
              wait(t);
              break;
              
          case tlm::TLM_SYNC:
          case tlm::TLM_SYNC_CONTINUE:
              // Transaction not yet finished, wait for the end of it
              wait(mEndEvent);
              break;

          case tlm::TLM_REJECTED:
            // FIXME: Not supported (wait and retry same transaction)
          default:
            assert(0); exit(1);
          };
      }
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
      return tlm::TLM_SYNC;

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

  // Invalidate DMI pointer(s)
  void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
                                 sc_dt::uint64 end_range)
  {
    // FIXME: probably faster to always invalidate everything?
    if (start_range <= mDMIDataReads.first.dmi_end_address &&
        end_range >= mDMIDataReads.first.dmi_start_address) {
        mDMIDataReads.second = false;
    }
    if (start_range <= mDMIDataWrites.first.dmi_end_address &&
        end_range >= mDMIDataWrites.first.dmi_start_address) {
      mDMIDataWrites.second = false;
    }
  }

  // Test for transport_dbg, this one should fail in bus_dmi as we address
  // a target that doesn't support transport_dbg:
  // FIXME: use a configurable address
  void end_of_simulation()
  {
    std::cout <<  name() << ", <<SimpleLTInitiator1>>:" << std::endl
              << std::endl;
    unsigned char data[32];

    tlm::tlm_debug_payload trans;
    trans.address = mBaseAddress;
    trans.num_bytes = 32;
    trans.data = data;
    trans.do_read = true;

    unsigned int n = socket->transport_dbg(trans);
    
    std::cout << "Mem @" << std::hex << mBaseAddress << std::endl;
    unsigned int j = 0;
        
    if (n > 0)
    {
        // always align endianness, so that we don't get a diff when
        // printing the raw data
        int e_start = 0;
        int e_end = 4;
        int e_increment = 1;
        if (!tlm::hostHasLittleEndianness())
        {
            e_start = 3;
            e_end = -1;
            e_increment = -1;
        }
        
        for (unsigned int i=0; i<n; i+=4)
        {
            for (int k=e_start; k!=e_end; k+=e_increment)
            {
                std::cout << std::setw(2) << std::setfill('0')
                          << (int)data[i+k];
                j++;
                if (j==16) {
                    j=0;
                    std::cout << std::endl;
                } else {
                    std::cout << " ";
                }
            }
        }
    }
    else
    {
        std::cout << "OK: debug transaction didn't give data." << std::endl;
    }
    std::cout << std::dec << std::endl;  
  }
private:
  std::pair<dmi_type, bool> mDMIDataReads;
  std::pair<dmi_type, bool> mDMIDataWrites;

  sc_core::sc_event mEndEvent;
  unsigned int mNrOfTransactions;
  unsigned int mBaseAddress;
  unsigned int mTransactionCount;
  unsigned int mData;
};

#endif
