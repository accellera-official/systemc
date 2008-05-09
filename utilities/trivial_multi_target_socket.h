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
#ifndef __SIMPLE_MULTI_TARGET_SOCKET_H__
#define __SIMPLE_MULTI_TARGET_SOCKET_H__

#include "multi_socket_bases.h"

template <typename MODULE,
          unsigned int BUSWIDTH = 32,
          typename TYPES = tlm::tlm_generic_payload_types,
          unsigned int N=0
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION <= 20050714)
          ,sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND
#endif
          >
class trivial_multi_target_socket: public multi_target_base< BUSWIDTH, 
                                                        TYPES,
                                                        N
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION <= 20050714)
                                                        ,POL
#endif
                                                        >
                              , public multi_to_multi_bind_base<TYPES>
{

public:

  //typedefs
  //  tlm 2.0 types for nb_transport
  typedef typename TYPES::tlm_payload_type              transaction_type;
  typedef typename TYPES::tlm_phase_type                phase_type;  
  typedef tlm::tlm_sync_enum                            sync_enum_type;
  
  //  typedefs to keep the fn ptr notations short
  typedef sync_enum_type (MODULE::*nb_cb)(int, transaction_type&, phase_type&, sc_core::sc_time&);
  typedef void (MODULE::*b_cb)(int, transaction_type&, sc_core::sc_time&);
  typedef unsigned int (MODULE::*dbg_cb)(int, transaction_type& txn);
  typedef bool (MODULE::*dmi_cb)(int, transaction_type& txn, tlm::tlm_dmi& dmi);
  
  typedef multi_target_base<BUSWIDTH, 
                        TYPES,
                        N
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION == 20050714)
                        ,POL
#endif
                        > base_type;

  typedef typename base_type::base_initiator_socket_type base_initiator_socket_type;
  typedef typename base_type::initiator_socket_type initiator_socket_type;
  
  //CTOR
  trivial_multi_target_socket(const char* name)
      : base_type((std::string(name)+std::string("_base")).c_str())
      , m_mod(0)
      , m_nb_cb(0)
      , m_b_cb(0)
      , m_dbg_cb(0)
      , m_dmi_cb(0)
      , m_hierarch_bind(0)
      , m_beoe_disabled(false)
      , m_dummy(42)
  {
  }

  ~trivial_multi_target_socket(){
    //clean up everything allocated by 'new'
    for (unsigned int i=0; i<m_binders.size(); i++) delete m_binders[i];
  }
  
  void display_warning(const std::string& text){
    std::stringstream s;
    s<<"WARNING in instance "<<base_type::name()<<": "<<text;
    SC_REPORT_WARNING("multi_socket", s.str().c_str());
  }

  void display_error(const std::string& text){
    std::stringstream s;
    s<<"ERROR in instance "<<base_type::name()<<": "<<text;
    SC_REPORT_ERROR("multi_socket", s.str().c_str());
  }

  //simply remember the callback function ptr
  void register_nb_transport_fw(MODULE* mod,
                              nb_cb cb)
  {
    if (!sc_core::sc_export<tlm::tlm_fw_transport_if<TYPES> >::get_interface()) //if our export hasn't been bound yet (due to a hierarch binding)
      sc_core::sc_export<tlm::tlm_fw_transport_if<TYPES> >::bind(m_dummy);      //  we bind it now      

    if (m_mod) assert(m_mod==mod);
    else m_mod=mod;
    
    if (m_nb_cb){
      display_warning("NBTransport_bw callback already registered.");
      return;
    }
    m_nb_cb=cb;
    m_nb_f=boost::bind<sync_enum_type>(m_nb_cb, m_mod, _1, _2, _3, _4);
  }

  //simply remember the callback function ptr
  void register_b_transport(MODULE* mod,
                              b_cb cb)
  {
    if (!sc_core::sc_export<tlm::tlm_fw_transport_if<TYPES> >::get_interface()) //if our export hasn't been bound yet (due to a hierarch binding)
      sc_core::sc_export<tlm::tlm_fw_transport_if<TYPES> >::bind(m_dummy);      //  we bind it now      

    if (m_mod) assert(m_mod==mod);
    else m_mod=mod;
    
    if (m_b_cb){
      display_warning("BTransport callback already registered.");
      return;
    }
    m_b_cb=cb;
    m_b_f=boost::bind<void>(m_b_cb, m_mod, _1, _2, _3);
  }

  void register_debug_transport(MODULE* mod,
                              dbg_cb cb)
  {
    if (!sc_core::sc_export<tlm::tlm_fw_transport_if<TYPES> >::get_interface()) //if our export hasn't been bound yet (due to a hierarch binding)
      sc_core::sc_export<tlm::tlm_fw_transport_if<TYPES> >::bind(m_dummy);      //  we bind it now      

    if (m_mod) assert(m_mod==mod);
    else m_mod=mod;
    
    if (m_dbg_cb){
      display_warning("DebugTransport callback already registered.");
      return;
    }
    m_dbg_cb=cb;
    m_dbg_f=boost::bind<unsigned int>(m_nb_cb, m_mod, _1, _2);
  }

  //simply remember the callback function ptr
  void register_DMI(MODULE* mod,
                   dmi_cb cb)
  {
    if (!sc_core::sc_export<tlm::tlm_fw_transport_if<TYPES> >::get_interface()) //if our export hasn't been bound yet (due to a hierarch binding)
      sc_core::sc_export<tlm::tlm_fw_transport_if<TYPES> >::bind(m_dummy);      //  we bind it now      

    if (m_mod) assert(m_mod==mod);
    else m_mod=mod;

    if (m_dmi_cb){
      display_warning("DMI callback already registered.");
      return;
    }
    m_dmi_cb=cb;
    m_nb_f=boost::bind<bool>(m_nb_cb, m_mod, _1, _2, _3);
  }

  virtual tlm::tlm_fw_transport_if<TYPES>& get_base_interface()
  {
    if (m_hierarch_bind) display_error("Socket already bound hierarchically.");
    if (!sc_core::sc_export<tlm::tlm_fw_transport_if<TYPES> >::get_interface()) //if our export hasn't been bound yet (due to a hierarch binding)
      sc_core::sc_export<tlm::tlm_fw_transport_if<TYPES> >::bind(m_dummy);      //  we bind it now      
    m_binders.push_back(new callback_binder_fw<TYPES>(m_binders.size()));
    return *m_binders[m_binders.size()-1];
  }

  virtual sc_core::sc_export<tlm::tlm_fw_transport_if<TYPES> >& get_base_export()
  {
    return *this;
  }
  
  void end_of_elaboration(){
    if (m_beoe_disabled) return;
    std::vector<callback_binder_fw<TYPES>* >& binders=get_hierarch_bind()->get_binders();
    std::map<unsigned int, tlm::tlm_bw_transport_if<TYPES>*>&  multiBinds=get_hierarch_bind()->get_multi_binds();
    for (unsigned int i=0; i<binders.size(); i++) {
      binders[i]->set_callbacks(m_nb_f, m_b_f, m_dmi_f, m_dbg_f);
      if (multiBinds.find(i)!=multiBinds.end()) //this connection is multi-multi
        m_sockets.push_back(multiBinds[i]);
      else{ //we are bound to a normal socket
        base_initiator_socket_type* test=dynamic_cast<base_initiator_socket_type*>(binders[i]->get_other_side());
        if (!test){display_error("Not bound to tlm_socket.");}
        m_sockets.push_back(&test->get_base_interface());
      }
    }
  }

  //
  // Bind multi target socket to multi target socket (hierarchical bind)
  //
  void bind(base_type& s)
  {
    if (m_beoe_disabled){
      display_warning("Socket already bound hierarchically. Bind attempt ignored.");
      return;
    }
    disable_BEOE();
    s.set_hierarch_bind((base_type*)this);    
    base_type::bind(s); //satisfy SystemC
  }

  //operator notation for hierarchical bind
  void operator() (base_type& s)
  {
    bind(s);
  }

  //get access to sub port
  tlm::tlm_bw_transport_if<TYPES>* operator[](int i){return m_sockets[i];}
  
  unsigned int size(){return m_sockets.size();}

protected:
  //implementation of base class interface
  base_type* get_hierarch_bind(){if (m_hierarch_bind) return m_hierarch_bind->get_hierarch_bind(); else return this;}
  std::map<unsigned int, tlm::tlm_bw_transport_if<TYPES>*>&  get_multi_binds(){return m_multi_binds;}
  void set_hierarch_bind(base_type* h){m_hierarch_bind=h;}
  tlm::tlm_fw_transport_if<TYPES>* get_last_binder(tlm::tlm_bw_transport_if<TYPES>* other){
    m_multi_binds[m_binders.size()-1]=other;
    return m_binders[m_binders.size()-1];
  }
  
  std::map<unsigned int, tlm::tlm_bw_transport_if<TYPES>*> m_multi_binds;
    
  void disable_BEOE(){ m_beoe_disabled=true;}
  std::vector<callback_binder_fw<TYPES>* >& get_binders(){return m_binders;}
  //vector of connected sockets
  std::vector<tlm::tlm_bw_transport_if<TYPES>*> m_sockets;
  //vector of binders that convert untagged interface into tagged interface
  std::vector<callback_binder_fw<TYPES>*> m_binders;
  
  MODULE* m_mod; //the owning module
  nb_cb   m_nb_cb; //the nb callback of the owning module
  b_cb    m_b_cb;
  dbg_cb  m_dbg_cb;
  dmi_cb  m_dmi_cb; //the dmi callback of the owning module
  base_type*  m_hierarch_bind; //pointer to hierarchical bound multi port
  bool m_beoe_disabled;
  callback_binder_fw<TYPES> m_dummy;

  boost::function<sync_enum_type (int i, transaction_type& txn, phase_type& p, sc_core::sc_time& t)> m_nb_f;
  boost::function<void (int i, transaction_type& txn, sc_core::sc_time& t)> m_b_f;
  boost::function<unsigned int (int i, transaction_type& txn)> m_dbg_f;
  boost::function<bool (int i, transaction_type& txn, tlm::tlm_dmi& dmi)> m_dmi_f;

};

#endif
