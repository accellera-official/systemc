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

#include "tlm.h"

template <typename MODULE,
          unsigned int BUSWIDTH = 32,
          typename TYPES = tlm::tlm_generic_payload_types>
class SimpleInitiatorSocket :
  public tlm::tlm_initiator_socket<BUSWIDTH,
                                   tlm::tlm_fw_transport_if<TYPES>,
                                   tlm::tlm_bw_transport_if<TYPES> >
{
public:
  typedef typename TYPES::tlm_payload_type              transaction_type;
  typedef typename TYPES::tlm_phase_type                phase_type;
  typedef tlm::tlm_sync_enum                            sync_enum_type;
  typedef tlm::tlm_fw_transport_if<TYPES>               fw_interface_type;
  typedef tlm::tlm_bw_transport_if<TYPES>               bw_interface_type;
  typedef tlm::tlm_initiator_socket<BUSWIDTH,
                                    fw_interface_type,
                                    bw_interface_type>  base_type;

public:
  explicit SimpleInitiatorSocket(const char* n = "SimpleInitiatorSocket") :
    base_type(sc_core::sc_gen_unique_name(n)),
    mProcess(this->name())
  {
    this->mExport.bind(mProcess);
  }

  void registerNBTransport_bw(MODULE* mod,
                              sync_enum_type (MODULE::*cb)(transaction_type&,
                                                           phase_type&,
                                                           sc_core::sc_time&))
  {
    mProcess.setTransportPtr(mod, cb);
  }

  void registerInvalidateDMI(MODULE* mod,
                             void (MODULE::*cb)(sc_dt::uint64, sc_dt::uint64))
  {
    mProcess.setInvalidateDMIPtr(mod, cb);
  }

private:
  class Process : public tlm::tlm_bw_transport_if<TYPES>
  {
  public:
    typedef sync_enum_type (MODULE::*TransportPtr)(transaction_type&,
                                                   tlm::tlm_phase&,
                                                   sc_core::sc_time&);
    typedef void (MODULE::*InvalidateDMIPtr)(sc_dt::uint64,
                                             sc_dt::uint64);
      
    Process(const std::string& name) :
      mName(name),
      mMod(0),
      mTransportPtr(0),
      mInvalidateDMIPtr(0)
    {
    }
  
    void setTransportPtr(MODULE* mod, TransportPtr p)
    {
      if (mTransportPtr) {
        std::cerr << mName << ": non-blocking callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mTransportPtr = p;
      }
    }

    void setInvalidateDMIPtr(MODULE* mod, InvalidateDMIPtr p)
    {
      if (mInvalidateDMIPtr) {
        std::cerr << mName << ": invalidate DMI callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mInvalidateDMIPtr = p;
      }
    }

    sync_enum_type nb_transport_bw(transaction_type& trans, phase_type& phase, sc_core::sc_time& t)
    {
      if (mTransportPtr) {
        // forward call
        assert(mMod);
        return (mMod->*mTransportPtr)(trans, phase, t);

      } else {
        std::cerr << mName << ": no transport callback registered" << std::endl;
        assert(0); exit(1);
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
    MODULE* mMod;
    TransportPtr mTransportPtr;
    InvalidateDMIPtr mInvalidateDMIPtr;
  };

private:
  Process mProcess;
};

// Tagged version

template <typename MODULE,
          unsigned int BUSWIDTH = 32,
          typename TYPES = tlm::tlm_generic_payload_types>
class SimpleInitiatorSocketTagged :
  public tlm::tlm_initiator_socket<BUSWIDTH,
                                   tlm::tlm_fw_transport_if<TYPES>,
                                   tlm::tlm_bw_transport_if<TYPES> >
{
public:
  typedef typename TYPES::tlm_payload_type              transaction_type;
  typedef typename TYPES::tlm_phase_type                phase_type;
  typedef tlm::tlm_sync_enum                            sync_enum_type;
  typedef tlm::tlm_fw_transport_if<TYPES>               fw_interface_type;
  typedef tlm::tlm_bw_transport_if<TYPES>               bw_interface_type;
  typedef tlm::tlm_initiator_socket<BUSWIDTH,
                                    fw_interface_type,
                                    bw_interface_type>  base_type;

public:
  explicit SimpleInitiatorSocketTagged(const char* n = "SimpleInitiatorSocketTagged") :
    base_type(sc_core::sc_gen_unique_name(n)),
    mProcess(this->name())
  {
    this->mExport.bind(mProcess);
  }

  void registerNBTransport_bw(MODULE* mod,
                              sync_enum_type (MODULE::*cb)(int,
                                                           transaction_type&,
                                                           phase_type&,
                                                           sc_core::sc_time&),
                              int id)
  {
    mProcess.setTransportPtr(mod, cb);
    mProcess.setTransportUserId(id);
  }

  void registerInvalidateDMI(MODULE* mod,
                             void (MODULE::*cb)(int, sc_dt::uint64, sc_dt::uint64),
                             int id)
  {
    mProcess.setInvalidateDMIPtr(mod, cb);
    mProcess.setInvalidateDMIUserId(id);
  }

private:
  class Process : public tlm::tlm_bw_transport_if<TYPES>
  {
  public:
    typedef sync_enum_type (MODULE::*TransportPtr)(int,
                                                   transaction_type&,
                                                   tlm::tlm_phase&,
                                                   sc_core::sc_time&);
    typedef void (MODULE::*InvalidateDMIPtr)(int,
                                             sc_dt::uint64,
                                             sc_dt::uint64);
      
    Process(const std::string& name) :
      mName(name),
      mMod(0),
      mTransportPtr(0),
      mInvalidateDMIPtr(0),
      mTransportUserId(0),
      mInvalidateDMIUserId(0)
    {
    }
  
    void setTransportUserId(int id) { mTransportUserId = id; }
    void setInvalidateDMIUserId(int id) { mInvalidateDMIUserId = id; }

    void setTransportPtr(MODULE* mod, TransportPtr p)
    {
      if (mTransportPtr) {
        std::cerr << mName << ": non-blocking callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mTransportPtr = p;
      }
    }

    void setInvalidateDMIPtr(MODULE* mod, InvalidateDMIPtr p)
    {
      if (mInvalidateDMIPtr) {
        std::cerr << mName << ": invalidate DMI callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mInvalidateDMIPtr = p;
      }
    }

    sync_enum_type nb_transport_bw(transaction_type& trans, phase_type& phase, sc_core::sc_time& t)
    {
      if (mTransportPtr) {
        // forward call
        assert(mMod);
        return (mMod->*mTransportPtr)(mTransportUserId, trans, phase, t);

      } else {
        std::cerr << mName << ": no transport callback registered" << std::endl;
        assert(0); exit(1);
      }
    }

    void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
                                   sc_dt::uint64 end_range)
    {
      if (mInvalidateDMIPtr) {
        // forward call
        assert(mMod);
        (mMod->*mInvalidateDMIPtr)(mInvalidateDMIUserId, start_range, end_range);
      }
    }

  private:
    const std::string mName;
    MODULE* mMod;
    TransportPtr mTransportPtr;
    InvalidateDMIPtr mInvalidateDMIPtr;
    int mTransportUserId;
    int mInvalidateDMIUserId;
  };

private:
  Process mProcess;
};
#endif
