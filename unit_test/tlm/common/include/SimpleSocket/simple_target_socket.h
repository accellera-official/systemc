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
                               tlm::tlm_fw_nb_transport_if<TYPES>,
                               tlm::tlm_bw_nb_transport_if<TYPES> >
{
public:
  typedef typename TYPES::tlm_payload_type              transaction_type;
  typedef typename TYPES::tlm_dmi_mode_type             dmi_mode_type;
  typedef typename TYPES::tlm_phase_type                phase_type;
  typedef tlm::tlm_sync_enum                            sync_enum_type;
  typedef tlm::tlm_fw_nb_transport_if<TYPES>            fw_interface_type;
  typedef tlm::tlm_bw_nb_transport_if<TYPES>            bw_interface_type;
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
    mProcess.setTransportPtr(mod, static_cast<typename Process::TransportPtr>(cb));
    mProcess.setTransportUserId(id);
  }

  template <typename MODULE>
  void registerDebugTransport(MODULE* mod,
                              unsigned int (MODULE::*cb)(tlm::tlm_debug_payload&),
                              int id)
  {
    mProcess.setTransportDebugPtr(mod, static_cast<typename Process::TransportDebugPtr>(cb));
    mProcess.setTransportDebugUserId(id);
  }

  template <typename MODULE>
  void registerDMI(MODULE* mod, bool (MODULE::*cb)(const sc_dt::uint64&,
                                                   dmi_mode_type& dmi_mode,
                                                   tlm::tlm_dmi&), int id)
  {
    mProcess.setGetDMIPtr(mod, static_cast<typename Process::GetDMIPtr>(cb));
    mProcess.setGetDMIUserId(id);
  }

private:
  class Process : public tlm::tlm_fw_nb_transport_if<TYPES>
  {
  public:
    typedef sync_enum_type (sc_core::sc_module::*TransportPtr)(transaction_type&,
                                                               tlm::tlm_phase&,
                                                               sc_core::sc_time&);
    typedef unsigned int (sc_core::sc_module::*TransportDebugPtr)(tlm::tlm_debug_payload&);
    typedef bool (sc_core::sc_module::*GetDMIPtr)(const sc_dt::uint64&,
                                                  dmi_mode_type& dmi_mode,
                                                  tlm::tlm_dmi&);
      
    Process(const std::string& name) :
      mName(name),
      mMod(0),
      mTransportPtr(0),
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
      if (mTransportPtr) {
        // forward call
        assert(mMod);
        simple_socket_utils::simple_socket_user::instance().set_user_id(mTransportUserId);
        return (mMod->*mTransportPtr)(trans, phase, t);

      } else {
        std::cerr << mName << ": no non-blocking callback registered" << std::endl;
        assert(0); exit(1);
        return tlm::TLM_REJECTED;
      }
    }

    unsigned int transport_dbg(tlm::tlm_debug_payload& trans)
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

    bool get_direct_mem_ptr(const sc_dt::uint64& address,
                            dmi_mode_type& dmi_mode,
                            tlm::tlm_dmi&  dmi_data)
    {
      if (mGetDMIPtr) {
        // forward call
        assert(mMod);
        simple_socket_utils::simple_socket_user::instance().set_user_id(mGetDMIUserId);
        return (mMod->*mGetDMIPtr)(address, dmi_mode, dmi_data);

      } else {
        // No DMI support
        dmi_mode.type = tlm::tlm_dmi_mode::READ_WRITE;
        dmi_data.dmi_start_address = 0x0;
        dmi_data.dmi_end_address = (sc_dt::uint64)-1;
        return false;
      }
    }

  private:
    const std::string mName;
    sc_core::sc_module* mMod;
    TransportPtr mTransportPtr;
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
