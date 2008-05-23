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

#ifndef __TRIVIAL_TARGET_SOCKET_H__
#define __TRIVIAL_TARGET_SOCKET_H__

#include "tlm.h"

template <typename MODULE,
          unsigned int BUSWIDTH = 32,
          typename TYPES = tlm::tlm_base_protocol_types>
class TrivialTargetSocket :
  public tlm::tlm_target_socket<BUSWIDTH,
                               tlm::tlm_fw_transport_if<TYPES>,
                               tlm::tlm_bw_transport_if<TYPES> >
{
public:
  typedef typename TYPES::tlm_payload_type              transaction_type;
  typedef typename TYPES::tlm_phase_type                phase_type;
  typedef tlm::tlm_sync_enum                            sync_enum_type;
  typedef tlm::tlm_fw_transport_if<TYPES>               fw_interface_type;
  typedef tlm::tlm_bw_transport_if<TYPES>               bw_interface_type;
  typedef tlm::tlm_target_socket<BUSWIDTH,
                                 fw_interface_type,
                                 bw_interface_type>     base_type;

public:
  explicit TrivialTargetSocket(const char* n = "TrivialTargetSocket") :
    base_type(sc_core::sc_gen_unique_name(n)),
    mProcess(this->name())
  {
    bind(mProcess);
  }

  // REGISTER_XXX
  void registerNBTransport(MODULE* mod, sync_enum_type (MODULE::*cb)(transaction_type&, phase_type&, sc_core::sc_time&))
  {
    mProcess.setNBTransportPtr(mod, cb);
  }

  void registerBTransport(MODULE* mod, void (MODULE::*cb)(transaction_type&, sc_core::sc_time&))
  {
    mProcess.setBTransportPtr(mod, cb);
  }

  void registerDebugTransport(MODULE* mod,
                              unsigned int (MODULE::*cb)(transaction_type&))
  {
    mProcess.setTransportDebugPtr(mod, cb);
  }

  void registerDMI(MODULE* mod, bool (MODULE::*cb)(transaction_type&,
                                                   tlm::tlm_dmi&))
  {
    mProcess.setGetDMIPtr(mod, cb);
  }

private:
  class Process : public tlm::tlm_fw_transport_if<TYPES>
  {
  public:
    typedef sync_enum_type (MODULE::*NBTransportPtr)(transaction_type&,
                                                     tlm::tlm_phase&,
                                                     sc_core::sc_time&);
    typedef void (MODULE::*BTransportPtr)(transaction_type&,
                                          sc_core::sc_time&);
    typedef unsigned int (MODULE::*TransportDebugPtr)(transaction_type&);
    typedef bool (MODULE::*GetDMIPtr)(transaction_type&,
                                                  tlm::tlm_dmi&);
      
    Process(const std::string& name) :
      mName(name),
      mMod(0),
      mNBTransportPtr(0),
      mBTransportPtr(0),
      mTransportDebugPtr(0),
      mGetDMIPtr(0)
    {
    }
  
    void setNBTransportPtr(MODULE* mod, NBTransportPtr p)
    {
      if (mNBTransportPtr) {
        std::cerr << mName << ": non-blocking callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mNBTransportPtr = p;
      }
    }

    void setBTransportPtr(MODULE* mod, BTransportPtr p)
    {
      if (mBTransportPtr) {
        std::cerr << mName << ": non-blocking callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mBTransportPtr = p;
      }
    }

    void setTransportDebugPtr(MODULE* mod, TransportDebugPtr p)
    {
      if (mTransportDebugPtr) {
        std::cerr << mName << ": debug callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mTransportDebugPtr = p;
      }
    }

    void setGetDMIPtr(MODULE* mod, GetDMIPtr p)
    {
      if (mGetDMIPtr) {
        std::cerr << mName << ": get DMI pointer callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mGetDMIPtr = p;
      }
    }

    sync_enum_type nb_transport_fw(transaction_type& trans,
                                   phase_type& phase,
                                   sc_core::sc_time& t)
    {
      if (mNBTransportPtr) {
        // forward call
        assert(mMod);
        return (mMod->*mNBTransportPtr)(trans, phase, t);

      } else {
        std::cerr << mName << ": no non-blocking callback registered" << std::endl;
        assert(0); exit(1);
//        return tlm::TLM_COMPLETED;   ///< unreachable code
      }
    }

    void b_transport(transaction_type& trans, sc_core::sc_time& t)
    {
      if (mBTransportPtr) {
        // forward call
        assert(mMod);
        return (mMod->*mBTransportPtr)(trans, t);

      } else {
        std::cerr << mName << ": no blocking callback registered" << std::endl;
        assert(0); exit(1);
//        return tlm::TLM_COMPLETED;   ///< unreachable code
      }
    }

    unsigned int transport_dbg(transaction_type& trans)
    {
      if (mTransportDebugPtr) {
        // forward call
        assert(mMod);
        return (mMod->*mTransportDebugPtr)(trans);

      } else {
        // No debug support
        return 0;
      }
    }

    bool get_direct_mem_ptr(transaction_type& trans,
                            tlm::tlm_dmi&  dmi_data)
    {
      if (mGetDMIPtr) {
        // forward call
        assert(mMod);
        return (mMod->*mGetDMIPtr)(trans, dmi_data);

      } else {
        // No DMI support
        dmi_data.allow_read_write();
        dmi_data.set_start_address(0x0);
        dmi_data.set_end_address((sc_dt::uint64)-1);
        return false;
      }
    }

  private:
    const std::string mName;
    MODULE* mMod;
    NBTransportPtr mNBTransportPtr;
    BTransportPtr mBTransportPtr;
    TransportDebugPtr mTransportDebugPtr;
    GetDMIPtr mGetDMIPtr;
  };

private:
  Process mProcess;
};

#endif
