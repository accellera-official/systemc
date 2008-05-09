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
#ifndef __SIMPLE_MULTI_INITIATOR_SOCKET_H__
#define __SIMPLE_MULTI_INITIATOR_SOCKET_H__

#include "multi_socket_bases.h"

template <typename MODULE,
          unsigned int BUSWIDTH = 32,
          typename TYPES = tlm::tlm_generic_payload_types,
          unsigned int N=0
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION <= 20050714)
          ,sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND
#endif
          >
class trivial_multi_initiator_socket: public multi_init_base< BUSWIDTH, 
                                                        TYPES,
                                                        N
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION <= 20050714)
                                                        ,POL
#endif
                                                        >
{

public:

  //typedefs
  //  tlm 2.0 types for nb_transport
  typedef typename TYPES::tlm_payload_type              transaction_type;
  typedef typename TYPES::tlm_phase_type                phase_type;  
  typedef tlm::tlm_sync_enum                            sync_enum_type;
  
  //  typedefs to keep the fn ptr notations short
  typedef sync_enum_type (MODULE::*nb_cb)(int,
                                         transaction_type&,
                                         phase_type&,
                                         sc_core::sc_time&);
  typedef void (MODULE::*dmi_cb)(int, sc_dt::uint64, sc_dt::uint64);

  typedef multi_init_base<BUSWIDTH, 
                        TYPES,
                        N
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION <= 20050714)
                        ,POL
#endif
                        > base_type;

  typedef typename base_type::base_target_socket_type base_target_socket_type;

  //CTOR
  trivial_multi_initiator_socket(const char* name)
      : base_type((std::string(name)+std::string("_base")).c_str())
      , m_mod(0)
      , m_nb_cb(0)
      , m_dmi_cb(0)
      , m_hierarch_bind(0)
      , m_beoe_disabled(false)
      , m_dummy(42)
  {
  }

  ~trivial_multi_initiator_socket(){
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
  void register_nb_transport_bw(MODULE* mod,
                              sync_enum_type (MODULE::*cb)(int,
                                                           transaction_type&,
                                                           phase_type&,
                                                           sc_core::sc_time&))
  {
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
  void register_invalidate_DMI(MODULE* mod,
                             void (MODULE::*cb)(int, sc_dt::uint64, sc_dt::uint64))
  {
    if (m_mod) assert(m_mod==mod);
    else m_mod=mod;

    if (m_dmi_cb){
      display_warning("InvalidateDMI callback already registered.");
      return;
    }
    m_dmi_cb=cb;
    m_dmi_f=boost::bind<void>(m_dmi_cb, m_mod, _1, _2, _3);
  }

  virtual tlm::tlm_bw_transport_if<TYPES>& get_base_interface()
  {
    m_binders.push_back(new callback_binder_bw<TYPES>(m_binders.size()));
    return *m_binders[m_binders.size()-1];
  }

  virtual sc_core::sc_export<tlm::tlm_bw_transport_if<TYPES> >& get_base_export()
  {
    if (!m_beoe_disabled) //we are not bound hierarchically
      base_type::mExport.bind(m_dummy);
    return base_type::mExport;
  }

  void bind(base_target_socket_type& s)
  {
    if (m_hierarch_bind)
      display_error("Already hierarchically bound.");      
    base_type::bind(s); //satisfy systemC, leads to a call to get_base_interface()
    sc_core::sc_export<tlm::tlm_fw_transport_if<TYPES> >* p_ex_s=dynamic_cast<sc_core::sc_export<tlm::tlm_fw_transport_if<TYPES> >*>(&s);
    if (!p_ex_s) display_error("Multi socket not bound to tlm_socket.");
    
    multi_to_multi_bind_base<TYPES>* test=dynamic_cast<multi_to_multi_bind_base<TYPES>*> (p_ex_s);
    if (test) //did we just do a multi-multi bind??
      m_sockets.push_back(test->get_last_binder(m_binders[m_binders.size()-1]));
    else{  // if not just bind normally
      sc_core::sc_export<tlm::tlm_fw_transport_if<TYPES> >& ex_s=*p_ex_s;
      m_sockets.push_back(&((tlm::tlm_fw_transport_if<TYPES>&)ex_s));
    }
  }

  //operator notation for direct bind
  void operator() (base_target_socket_type& s)
  {
    bind(s);
  }
  
  void before_end_of_elaboration(){
    if (!base_type::mExport.get_interface()){ //if our export hasn't been bound yet (due to a hierarch binding)
      base_type::mExport.bind(m_dummy);      //  we bind it now
    }
    if (m_beoe_disabled) return;
    std::vector<callback_binder_bw<TYPES>* >& binders=get_hierarch_bind()->get_binders();
    m_used_sockets=get_hierarch_bind()->get_sockets();
    for (unsigned int i=0; i<binders.size(); i++) {
      binders[i]->set_callbacks(m_nb_f, m_dmi_f);
    }
  }

  //
  // Bind multi initiator socket to multi initiator socket (hierarchical bind)
  //
  void bind(base_type& s)
  {
    if (m_binders.size()) //a multi socket is either bound hierarchically or directly
      display_error("Socket already directly bound.");
    if (m_hierarch_bind){
      display_warning("Socket already bound hierarchically. Bind attempt ignored.");
      return;
    }
    //remember to which socket we are hierarchically bound and disable it, so that it won't try to register callbacks itself
    s.disable_BEOE();
    m_hierarch_bind=&s;    
    base_type::bind(s); //satisfy SystemC
  }

  //operator notation for hierarchical bind
  void operator() (base_type& s)
  {
    bind(s);
  }

  //get access to sub port
  tlm::tlm_fw_transport_if<TYPES>* operator[](int i){return m_used_sockets[i];}
  unsigned int size() {return m_used_sockets.size();}

protected:
  //implementation of base class interface
  base_type* get_hierarch_bind(){if (m_hierarch_bind) return m_hierarch_bind->get_hierarch_bind(); else return this;}
  void disable_BEOE(){ m_beoe_disabled=true;}
  std::vector<callback_binder_bw<TYPES>* >& get_binders(){return m_binders;}
  std::vector<tlm::tlm_fw_transport_if<TYPES>*>& get_sockets(){return m_sockets;}
  //vector of connected sockets
  std::vector<tlm::tlm_fw_transport_if<TYPES>*> m_sockets;
  std::vector<tlm::tlm_fw_transport_if<TYPES>*> m_used_sockets;
  //vector of binders that convert untagged interface into tagged interface
  std::vector<callback_binder_bw<TYPES>*> m_binders;
  
  MODULE* m_mod; //the owning module
  nb_cb   m_nb_cb; //the nb callback of the owning module
  dmi_cb  m_dmi_cb; //the dmi callback of the owning module
  base_type*  m_hierarch_bind; //pointer to hierarchical bound multi port
  bool m_beoe_disabled;
  callback_binder_bw<TYPES> m_dummy;

  boost::function<sync_enum_type (int i, transaction_type& txn, phase_type& p, sc_core::sc_time& t)> m_nb_f;
  boost::function<void (int i, sc_dt::uint64 l, sc_dt::uint64 u)> m_dmi_f;
};

#endif
