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

#ifndef __MULTI_SOCKET_BASES_H__
#define __MULTI_SOCKET_BASES_H__

#include <systemc>

#if !(SYSTEMC_VERSION <= 20050714)
# include "sysc/kernel/sc_boost.h"
# include "sysc/kernel/sc_spawn.h"
# include "sysc/kernel/sc_spawn_options.h"
#endif
#include "tlm.h"

#include "boost/function.hpp"

template <typename TYPES>
class callback_binder_fw: public tlm::tlm_fw_transport_if<TYPES>{
  public:
    typedef typename TYPES::tlm_payload_type              transaction_type;
    typedef typename TYPES::tlm_phase_type                phase_type;  
    typedef tlm::tlm_sync_enum                            sync_enum_type;
  
    typedef boost::function<sync_enum_type (int i, transaction_type& txn, phase_type& ph, sc_core::sc_time& t)> nb_func_type;
    typedef boost::function<void (int i, transaction_type& txn, sc_core::sc_time& t)> b_func_type;
    typedef boost::function<unsigned int (int i, transaction_type& txn)> debug_func_type;
    typedef boost::function<bool (int i, transaction_type& txn, tlm::tlm_dmi& dmi)> dmi_func_type;

    callback_binder_fw(int id): m_id(id){
    }

    sync_enum_type nb_transport_fw(transaction_type& txn,
                                phase_type& p,
                                sc_core::sc_time& t){
      if (m_nb_f->empty()){
        std::cerr<<"No function registered"<<std::endl; //here we could do an automatic nb->b conversion
        exit(1);
      }
      else
        return (*m_nb_f)(m_id, txn, p, t); 
    }
    
    void b_transport(transaction_type& trans,sc_core::sc_time& t){
      if (m_b_f->empty()){
        std::cerr<<"No function registered"<<std::endl; //here we could do an automatic b->nb conversion
        exit(1);
      }
      else
        (*m_b_f)(m_id, trans,t); 
    }
    
    bool get_direct_mem_ptr(transaction_type& trans, tlm::tlm_dmi&  dmi_data){
      if (m_dmi_f->empty()){
        std::cerr<<"No function registered"<<std::endl; 
        exit(1);
      }
      else
        return (*m_dmi_f)(m_id, trans,dmi_data); 
    }
    
    unsigned int transport_dbg(transaction_type& trans){
      if (m_dbg_f->empty()){
        std::cerr<<"No function registered"<<std::endl; 
        exit(1);
      }
      else
        return (*m_dbg_f)(m_id, trans); 
    }
    
    
    void register_port(sc_core::sc_port_base& b, const char* name){
      m_caller_port=&b;
    }
    

      
    void set_callbacks(nb_func_type& cb1, b_func_type& cb2, dmi_func_type& cb3, debug_func_type& cb4){
      m_nb_f=&cb1;
      m_b_f=&cb2;
      m_dmi_f=&cb3;
      m_dbg_f=&cb4;
    }
    
    sc_core::sc_port_base* get_other_side(){return m_caller_port;}
    
  private:
    int m_id;
    nb_func_type* m_nb_f;
    b_func_type*  m_b_f;
    debug_func_type* m_dbg_f;
    dmi_func_type* m_dmi_f;
    sc_core::sc_port_base* m_caller_port;   
};

template <typename TYPES>
class callback_binder_bw: public tlm::tlm_bw_transport_if<TYPES>{
  public:
    typedef typename TYPES::tlm_payload_type              transaction_type;
    typedef typename TYPES::tlm_phase_type                phase_type;  
    typedef tlm::tlm_sync_enum                            sync_enum_type;
  
    typedef boost::function<sync_enum_type (int i, transaction_type& txn, phase_type& p, sc_core::sc_time& t)> nb_func_type;
    typedef boost::function<void (int i, sc_dt::uint64 l, sc_dt::uint64 u)> dmi_func_type;

    callback_binder_bw(int id): m_id(id){
    }

    sync_enum_type nb_transport_bw(transaction_type& txn,
                                phase_type& p,
                                sc_core::sc_time& t){
      if (m_nb_f->empty()){
        std::cerr<<"No function registered"<<std::endl; //here we could do an automatic nb->b conversion
        exit(1);
      }
      else
        return (*m_nb_f)(m_id, txn, p, t); 
    }
    
    void invalidate_direct_mem_ptr(sc_dt::uint64 l, sc_dt::uint64 u){
      if (m_nb_f->empty()){
        std::cerr<<"No function registered"<<std::endl;
        exit(1);
      }
      else
        (*m_dmi_f)(m_id,l,u);
    }

    void set_callbacks(nb_func_type& cb1, dmi_func_type& cb2){
      m_nb_f=&cb1;
      m_dmi_f=&cb2;
    }
    
  private:
    int m_id;
    nb_func_type* m_nb_f;
    dmi_func_type* m_dmi_f;
};

template <unsigned int BUSWIDTH = 32,
          typename TYPES = tlm::tlm_generic_payload_types,
          unsigned int N=0
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION <= 20050714)
          ,sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND
#endif
          >
class multi_init_base: public tlm::tlm_initiator_socket<BUSWIDTH, 
                                                tlm::tlm_fw_transport_if<TYPES>,
                                                tlm::tlm_bw_transport_if<TYPES>,
                                                N
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION <= 20050714)
                                                ,POL
#endif
                                                >{
public:
  typedef tlm::tlm_initiator_socket<BUSWIDTH, 
                              tlm::tlm_fw_transport_if<TYPES>,
                              tlm::tlm_bw_transport_if<TYPES>,
                              N
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION <= 20050714)
                              ,POL
#endif
                              > base_type;
  
  typedef typename base_type::base_target_socket_type base_target_socket_type;
                                                
  virtual void disable_BEOE()=0;
  virtual multi_init_base* get_hierarch_bind()=0;
  virtual std::vector<callback_binder_bw<TYPES>* >& get_binders()=0;
  virtual std::vector<tlm::tlm_fw_transport_if<TYPES>*>&  get_sockets()=0;
  virtual ~multi_init_base(){}
  multi_init_base(const char* name):base_type(name){}
};

template <unsigned int BUSWIDTH = 32,
          typename TYPES = tlm::tlm_generic_payload_types,
          unsigned int N=0
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION <= 20050714)
          ,sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND
#endif
          >
class multi_target_base: public tlm::tlm_target_socket<BUSWIDTH, 
                                                tlm::tlm_fw_transport_if<TYPES>,
                                                tlm::tlm_bw_transport_if<TYPES>,
                                                N
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION <= 20050714)                                                
                                                ,POL
#endif
                                                >{
public:
  typedef tlm::tlm_target_socket<BUSWIDTH, 
                              tlm::tlm_fw_transport_if<TYPES>,
                              tlm::tlm_bw_transport_if<TYPES>,
                              N
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION <= 20050714)
                              ,POL
#endif
                              > base_type;
  
  typedef typename base_type::base_initiator_socket_type base_initiator_socket_type;
  typedef typename base_type::initiator_socket_type initiator_socket_type;
                                                
  virtual multi_target_base* get_hierarch_bind()=0;
  virtual void set_hierarch_bind(multi_target_base*)=0;
  virtual std::vector<callback_binder_fw<TYPES>* >& get_binders()=0;
  virtual std::map<unsigned int, tlm::tlm_bw_transport_if<TYPES>*>&  get_multi_binds()=0;
  virtual ~multi_target_base(){}
  multi_target_base(const char* name):base_type(name){}
};

template <typename TYPES>
class multi_to_multi_bind_base{
public:
  virtual ~multi_to_multi_bind_base(){}
  virtual tlm::tlm_fw_transport_if<TYPES>* get_last_binder(tlm::tlm_bw_transport_if<TYPES>*)=0;
};

#endif