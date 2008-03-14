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

#ifndef __SIMPLE_INITIATOR_SOCKET_H__
#define __SIMPLE_INITIATOR_SOCKET_H__

#include "simple_socket_utils.h"
//#include "tlm.h"

template <unsigned int BUSWIDTH = 32,
          typename TYPES = tlm::tlm_generic_payload_types>
class SimpleInitiatorSocket :
  public tlm::tlm_initiator_socket<BUSWIDTH,
                                   tlm::tlm_fw_nb_transport_if<TYPES>,
                                   tlm::tlm_bw_nb_transport_if<TYPES> >
{
public:
  typedef typename TYPES::tlm_payload_type              transaction_type;
  typedef typename TYPES::tlm_phase_type                phase_type;
  typedef tlm::tlm_sync_enum                            sync_enum_type;
  typedef tlm::tlm_fw_nb_transport_if<TYPES>            fw_interface_type;
  typedef tlm::tlm_bw_nb_transport_if<TYPES>            bw_interface_type;
  typedef tlm::tlm_initiator_socket<BUSWIDTH,
                                    fw_interface_type,
                                    bw_interface_type>  base_type;

public:
  explicit SimpleInitiatorSocket(const char* n = "") :
    base_type(n),
    mProcess(this->name(), mEndEvent)
  {
    mExport(mProcess);
  }

  sc_core::sc_event& getEndEvent()
  {
    return mEndEvent;
  }

  // REGISTER_XXX
  template <typename MODULE>
  void registerNBTransport(MODULE* mod,
                           sync_enum_type (MODULE::*cb)(transaction_type&,
                                                        phase_type&,
                                                        sc_core::sc_time&),
                           int id)
  {
    mProcess.setTransportPtr(mod, static_cast<typename Process::TransportPtr>(cb));
    mProcess.setTransportUserId(id);
  }

  template <typename MODULE>
  void registerInvalidateDMI(MODULE* mod,
                             void (MODULE::*cb)(sc_dt::uint64, sc_dt::uint64),
                             int id)
  {
    mProcess.setInvalidateDMIPtr(mod, static_cast<typename Process::InvalidateDMIPtr>(cb));
    mProcess.setInvalidateDMIUserId(id);
  }

private:
  class Process : public tlm::tlm_bw_nb_transport_if<TYPES>
  {
  public:
    typedef sync_enum_type (sc_core::sc_module::*TransportPtr)(transaction_type&,
                                                               tlm::tlm_phase&,
                                                               sc_core::sc_time&);
    typedef void (sc_core::sc_module::*InvalidateDMIPtr)(sc_dt::uint64,
                                                         sc_dt::uint64);
      
    Process(const std::string& name, sc_core::sc_event& endEvent) :
      mName(name),
      mMod(0),
      mTransportPtr(0),
      mInvalidateDMIPtr(0),
      mTransportUserId(0),
      mInvalidateDMIUserId(0),
      mEndEvent(endEvent)
    {
    }
  
    void setTransportUserId(int id) { mTransportUserId = id; }
    void setInvalidateDMIUserId(int id) { mInvalidateDMIUserId = id; }

    void setTransportPtr(sc_core::sc_module* mod, TransportPtr p)
    {
      if (mTransportPtr) {
	std::cerr << mName << ": non-blocking callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mTransportPtr = p;
      }
    }

    void setInvalidateDMIPtr(sc_core::sc_module* mod, InvalidateDMIPtr p)
    {
      if (mInvalidateDMIPtr) {
	std::cerr << mName << ": invalidate DMI callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mInvalidateDMIPtr = p;
      }
    }

    sync_enum_type nb_transport(transaction_type& trans, phase_type& phase, sc_core::sc_time& t)
    {
      if (mTransportPtr) {
        // forward call
        assert(mMod);
        simple_socket_utils::simple_socket_user::instance().set_user_id(mTransportUserId);
        return (mMod->*mTransportPtr)(trans, phase, t);

      } else {
        // No callback registered, handle protocol and notify event
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
//          return tlm::TLM_REJECTED;   ///< unreachable code
        };
      }
    }

    void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
                                   sc_dt::uint64 end_range)
    {
      if (mInvalidateDMIPtr) {
        // forward call
        assert(mMod);
        (mMod->*mInvalidateDMIPtr)(start_range, end_range);
      }
    }

  private:
    const std::string mName;
    sc_core::sc_module* mMod;
    TransportPtr mTransportPtr;
    InvalidateDMIPtr mInvalidateDMIPtr;
    int mTransportUserId;
    int mInvalidateDMIUserId;
    sc_core::sc_event& mEndEvent;
  };

private:
  Process mProcess;
  sc_core::sc_event mEndEvent;
};

#endif
