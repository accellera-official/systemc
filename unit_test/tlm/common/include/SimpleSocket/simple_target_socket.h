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

#ifndef __SIMPLE_TARGET_SOCKET_H__
#define __SIMPLE_TARGET_SOCKET_H__

#include "simple_socket_utils.h"
//#include "tlm.h"

template <unsigned int BUSWIDTH = 32,
          typename TYPES = tlm::tlm_generic_payload_types>
class SimpleTargetSocket :
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
  explicit SimpleTargetSocket(const char* n = "") :
    base_type(n),
    mProcess(this->name())
  {
    (*this)(mProcess);
  }

  // REGISTER_XXX
  template <typename MODULE>
  void registerNBTransport(MODULE* mod, sync_enum_type (MODULE::*cb)(transaction_type&, phase_type&, sc_core::sc_time&), int id)
  {
    mProcess.setNBTransportPtr(mod, static_cast<typename Process::NBTransportPtr>(cb));
    mProcess.setTransportUserId(id);
  }

  template <typename MODULE>
  void registerBTransport(MODULE* mod, void (MODULE::*cb)(transaction_type&, sc_core::sc_time&), int id)
  {
    mProcess.setBTransportPtr(mod, static_cast<typename Process::BTransportPtr>(cb));
    mProcess.setTransportUserId(id);
  }

  template <typename MODULE>
  void registerDebugTransport(MODULE* mod,
                              unsigned int (MODULE::*cb)(transaction_type&),
                              int id)
  {
    mProcess.setTransportDebugPtr(mod, static_cast<typename Process::TransportDebugPtr>(cb));
    mProcess.setTransportDebugUserId(id);
  }

  template <typename MODULE>
  void registerDMI(MODULE* mod, bool (MODULE::*cb)(transaction_type&,
                                                   tlm::tlm_dmi&), int id)
  {
    mProcess.setGetDMIPtr(mod, static_cast<typename Process::GetDMIPtr>(cb));
    mProcess.setGetDMIUserId(id);
  }

private:
  class Process : public tlm::tlm_fw_transport_if<TYPES>
  {
  public:
    typedef sync_enum_type (sc_core::sc_module::*NBTransportPtr)(transaction_type&,
                                                                 tlm::tlm_phase&,
                                                                 sc_core::sc_time&);
    typedef void (sc_core::sc_module::*BTransportPtr)(transaction_type&,
                                                       sc_core::sc_time&);
    typedef unsigned int (sc_core::sc_module::*TransportDebugPtr)(transaction_type&);
    typedef bool (sc_core::sc_module::*GetDMIPtr)(transaction_type&,
                                                  tlm::tlm_dmi&);
      
    Process(const std::string& name) :
      mName(name),
      mMod(0),
      mNBTransportPtr(0),
      mBTransportPtr(0),
      mTransportDebugPtr(0),
      mGetDMIPtr(0),
      mTransportUserId(0),
      mTransportDebugUserId(0),
      mGetDMIUserId(0)
    {
    }
  
    void setTransportUserId(int id) { mTransportUserId = id; }
    void setTransportDebugUserId(int id) { mTransportDebugUserId = id; }
    void setGetDMIUserId(int id) { mGetDMIUserId = id; }

    void setNBTransportPtr(sc_core::sc_module* mod, NBTransportPtr p)
    {
      if (mNBTransportPtr) {
        std::cerr << mName << ": non-blocking callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mNBTransportPtr = p;
      }
    }

    void setNBTransportPtr(sc_core::sc_module* mod, BTransportPtr p)
    {
      if (mBTransportPtr) {
        std::cerr << mName << ": non-blocking callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mBTransportPtr = p;
      }
    }

    void setTransportDebugPtr(sc_core::sc_module* mod, TransportDebugPtr p)
    {
      if (mTransportDebugPtr) {
        std::cerr << mName << ": debug callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mTransportDebugPtr = p;
      }
    }

    void setGetDMIPtr(sc_core::sc_module* mod, GetDMIPtr p)
    {
      if (mGetDMIPtr) {
        std::cerr << mName << ": get DMI pointer callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mGetDMIPtr = p;
      }
    }

    sync_enum_type nb_transport(transaction_type& trans,
                                phase_type& phase,
                                sc_core::sc_time& t)
    {
      if (mNBTransportPtr) {
        // forward call
        assert(mMod);
        simple_socket_utils::simple_socket_user::instance().set_user_id(mTransportUserId);
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
        simple_socket_utils::simple_socket_user::instance().set_user_id(mTransportUserId);
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
        simple_socket_utils::simple_socket_user::instance().set_user_id(mTransportDebugUserId);
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
        simple_socket_utils::simple_socket_user::instance().set_user_id(mGetDMIUserId);
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
    sc_core::sc_module* mMod;
    NBTransportPtr mNBTransportPtr;
    BTransportPtr mBTransportPtr;
    TransportDebugPtr mTransportDebugPtr;
    GetDMIPtr mGetDMIPtr;
    int mTransportUserId;
    int mTransportDebugUserId;
    int mGetDMIUserId;
  };

private:
  Process mProcess;
};

#endif
