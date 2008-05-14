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

#ifndef __TRIVIAL_TARGET_SOCKET_H__
#define __TRIVIAL_TARGET_SOCKET_H__

#include "tlm.h"

namespace tlm_utils {

template <typename MODULE,
          unsigned int BUSWIDTH = 32,
          typename TYPES = tlm::tlm_generic_payload_types>
class trivial_target_socket :
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
  explicit trivial_target_socket(const char* n = "trivial_target_socket") :
    base_type(sc_core::sc_gen_unique_name(n)),
    m_process(this->name())
  {
    bind(m_process);
  }

  // REGISTER_XXX
  void register_nb_transport_fw(MODULE* mod,
                                sync_enum_type (MODULE::*cb)(transaction_type&,
                                                             phase_type&,
                                                             sc_core::sc_time&))
  {
    m_process.set_nb_transport_ptr(mod, cb);
  }

  void register_b_transport(MODULE* mod,
                            void (MODULE::*cb)(transaction_type&,
                                               sc_core::sc_time&))
  {
    m_process.set_b_transport_ptr(mod, cb);
  }

  void register_transport_dbg(MODULE* mod,
                              unsigned int (MODULE::*cb)(transaction_type&))
  {
    m_process.set_transport_dbg_ptr(mod, cb);
  }

  void register_get_direct_mem_ptr(MODULE* mod,
                                   bool (MODULE::*cb)(transaction_type&,
                                                      tlm::tlm_dmi&))
  {
    m_process.set_get_direct_mem_ptr(mod, cb);
  }

private:
  class process : public tlm::tlm_fw_transport_if<TYPES>
  {
  public:
    typedef sync_enum_type (MODULE::*NBTransportPtr)(transaction_type&,
                                                       tlm::tlm_phase&,
                                                       sc_core::sc_time&);
    typedef void (MODULE::*BTransportPtr)(transaction_type&,
                                            sc_core::sc_time&);
    typedef unsigned int (MODULE::*TransportDbgPtr)(transaction_type&);
    typedef bool (MODULE::*GetDirectMem_ptr)(transaction_type&,
                                               tlm::tlm_dmi&);
      
    process(const std::string& name) :
      m_name(name),
      m_mod(0),
      m_nb_transport_ptr(0),
      m_b_transport_ptr(0),
      m_transport_dbg_ptr(0),
      m_get_direct_mem_ptr(0)
    {
    }
  
    void set_nb_transport_ptr(MODULE* mod, NBTransportPtr p)
    {
      if (m_nb_transport_ptr) {
        std::cerr << m_name << ": non-blocking callback allready registered" << std::endl;

      } else {
        assert(!m_mod || m_mod == mod);
        m_mod = mod;
        m_nb_transport_ptr = p;
      }
    }

    void set_b_transport_ptr(MODULE* mod, BTransportPtr p)
    {
      if (m_b_transport_ptr) {
        std::cerr << m_name << ": non-blocking callback allready registered" << std::endl;

      } else {
        assert(!m_mod || m_mod == mod);
        m_mod = mod;
        m_b_transport_ptr = p;
      }
    }

    void set_transport_dbg_ptr(MODULE* mod, TransportDbgPtr p)
    {
      if (m_transport_dbg_ptr) {
        std::cerr << m_name << ": debug callback allready registered" << std::endl;

      } else {
        assert(!m_mod || m_mod == mod);
        m_mod = mod;
        m_transport_dbg_ptr = p;
      }
    }

    void set_get_direct_mem_ptr(MODULE* mod, GetDirectMem_ptr p)
    {
      if (m_get_direct_mem_ptr) {
        std::cerr << m_name << ": get DMI pointer callback allready registered" << std::endl;

      } else {
        assert(!m_mod || m_mod == mod);
        m_mod = mod;
        m_get_direct_mem_ptr = p;
      }
    }

    sync_enum_type nb_transport_fw(transaction_type& trans,
                                   phase_type& phase,
                                   sc_core::sc_time& t)
    {
      if (m_nb_transport_ptr) {
        // forward call
        assert(m_mod);
        return (m_mod->*m_nb_transport_ptr)(trans, phase, t);

      } else {
        std::cerr << m_name << ": no non-blocking callback registered" << std::endl;
        assert(0); exit(1);
//        return tlm::TLM_COMPLETED;   ///< unreachable code
      }
    }

    void b_transport(transaction_type& trans, sc_core::sc_time& t)
    {
      if (m_b_transport_ptr) {
        // forward call
        assert(m_mod);
        return (m_mod->*m_b_transport_ptr)(trans, t);

      } else {
        std::cerr << m_name << ": no blocking callback registered" << std::endl;
        assert(0); exit(1);
//        return tlm::TLM_COMPLETED;   ///< unreachable code
      }
    }

    unsigned int transport_dbg(transaction_type& trans)
    {
      if (m_transport_dbg_ptr) {
        // forward call
        assert(m_mod);
        return (m_mod->*m_transport_dbg_ptr)(trans);

      } else {
        // No debug support
        return 0;
      }
    }

    bool get_direct_mem_ptr(transaction_type& trans,
                            tlm::tlm_dmi&  dmi_data)
    {
      if (m_get_direct_mem_ptr) {
        // forward call
        assert(m_mod);
        return (m_mod->*m_get_direct_mem_ptr)(trans, dmi_data);

      } else {
        // No DMI support
        dmi_data.allow_read_write();
        dmi_data.set_start_address(0x0);
        dmi_data.set_end_address((sc_dt::uint64)-1);
        return false;
      }
    }

  private:
    const std::string m_name;
    MODULE* m_mod;
    NBTransportPtr m_nb_transport_ptr;
    BTransportPtr m_b_transport_ptr;
    TransportDbgPtr m_transport_dbg_ptr;
    GetDirectMem_ptr m_get_direct_mem_ptr;
  };

private:
  process m_process;
};

}

#endif
