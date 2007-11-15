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

#ifndef __SIMPLE_LT_INITIATOR1_DMI_H__
#define __SIMPLE_LT_INITIATOR1_DMI_H__

#include "tlm.h"
#include <systemc>
#include <cassert>
#include <iostream>
#include <iomanip>

class SimpleLTInitiator1_dmi :
  public sc_core::sc_module,
  public virtual tlm::tlm_bw_nb_transport_if<>
{
public:
  typedef tlm::tlm_generic_payload      transaction_type;
  typedef tlm::tlm_dmi_mode             dmi_mode_type;
  typedef tlm::tlm_dmi                  dmi_type;
  typedef tlm::tlm_phase                phase_type;
  typedef tlm::tlm_sync_enum            sync_enum_type;
  typedef tlm::tlm_fw_nb_transport_if<> fw_interface_type;
  typedef tlm::tlm_bw_nb_transport_if<> bw_interface_type;
  typedef tlm::tlm_initiator_socket<>   initiator_socket_type;

public:
  initiator_socket_type socket;

public:
  SC_HAS_PROCESS(SimpleLTInitiator1_dmi);
  SimpleLTInitiator1_dmi(sc_core::sc_module_name name,
                         unsigned int nrOfTransactions = 0x5,
                         unsigned int baseAddress = 0x0) :
    sc_core::sc_module(name),
    socket("socket"),
    mNrOfTransactions(nrOfTransactions),
    mBaseAddress(baseAddress),
    mTransactionCount(0)
  {
    invalidate(mDMIData);

    // Bind this initiator's interface to the initiator socket
    socket(*this);

    // Initiator thread
    SC_THREAD(run);
  }

  bool initTransaction(transaction_type& trans)
  {
    // initialize DMI hint:
    trans.set_dmi_allowed(false);

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
      // We use the DMI hint to check if it makes sense to ask for
      // DMI pointers. The pattern is:
      // - if the address is covered by a DMI region do a DMI access
      // - otherwise do a normal transaction
      //   -> check if we get a DMI hint and acquire the DMI pointers if it is
      //      set
      ///////////////////////////////////////////////////////////

      // Check if the address is covered by our DMI region
      if ( (trans.get_address() >= mDMIData.dmi_start_address) &&
           (trans.get_address() <= mDMIData.dmi_end_address) ) {
          // We can handle the data here. As the logEndTransaction is assuming
          // something to happen in the data structure, we really need to
          // do this:
          trans.set_response_status(tlm::TLM_OK_RESPONSE);
          sc_dt::uint64 tmp = trans.get_address() - mDMIData.dmi_start_address;
          assert(hasHostEndianness(mDMIData.endianness));
          if (trans.get_command() == tlm::TLM_WRITE_COMMAND) {
              *(unsigned int*)&mDMIData.dmi_ptr[tmp] = mData;

          } else {
              mData = *(unsigned int*)&mDMIData.dmi_ptr[tmp];
          }
          
          // Do the wait immediately. Note that doing the wait here eats almost
          // all the performance anyway, so we only gain something if we're
          // using temporal decoupling.
          if (trans.get_command() == tlm::TLM_WRITE_COMMAND) {
            wait(mDMIData.write_latency);

          } else {
            wait(mDMIData.read_latency);
          }
          
      } else { // we need a full transaction
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
          // Acquire DMI pointer if we get the hint:
          if (trans.get_dmi_allowed())
          {
              dmi_type tmp;
              dmi_mode_type tmp_mode;
              tmp_mode.type = tlm::tlm_dmi_mode::WRITE;
              if ( socket->get_direct_mem_ptr(trans.get_address(),
                                              tmp_mode,
                                              tmp) &&
                   (tmp_mode.type != tlm::tlm_dmi_mode::READ))
              {
                  mDMIData = tmp;
              }
          }
      }
      logEndTransaction(trans);
    }
    sc_core::sc_stop();
    wait();
  }

  sync_enum_type nb_transport(transaction_type& trans, phase_type& phase, sc_core::sc_time& t)
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

  void invalidate(dmi_type& dmiData)
  {
    dmiData.dmi_start_address = 1;
    dmiData.dmi_end_address = 0;
  }

  // Invalidate DMI pointer(s)
  void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
                                 sc_dt::uint64 end_range)
  {
      // do the invalidation if there is an address range overlap
      if (start_range <= mDMIData.dmi_end_address &&
          end_range >= mDMIData.dmi_start_address) {
          std::cout <<  name() << ": got DMI pointer invalidation"
                    << " @ " << sc_core::sc_time_stamp() << std::endl;
          
          invalidate(mDMIData);
      } else {
          std::cout <<  name() << ": ignored DMI invalidation for addresses "
                    << std::hex << start_range << ", "
                    << end_range << std::dec
                    << " @ " << sc_core::sc_time_stamp() << std::endl;
      }
  }

  // Test for transport_dbg:
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
        std::cout << "ERROR: debug transaction didn't give data." << std::endl;
    }
    std::cout << std::dec << std::endl;  
  }
private:
  dmi_type mDMIData;

  sc_core::sc_event mEndEvent;
  unsigned int mNrOfTransactions;
  unsigned int mBaseAddress;
  unsigned int mTransactionCount;
  unsigned int mData;
};

#endif
