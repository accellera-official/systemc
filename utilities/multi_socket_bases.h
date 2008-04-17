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

namespace tlm{

template <typename TYPES>
class callback_binder_fw: public tlm_fw_transport_if<TYPES>{
  public:
    typedef typename TYPES::tlm_payload_type              transaction_type;
    typedef typename TYPES::tlm_phase_type                phase_type;  
    typedef tlm::tlm_sync_enum                            sync_enum_type;
  
    typedef boost::function<sync_enum_type (int i, transaction_type& txn, phase_type& ph, sc_core::sc_time& t)> nbFuncType;
    typedef boost::function<void (int i, transaction_type& txn, sc_core::sc_time& t)> bFuncType;
    typedef boost::function<unsigned int (int i, transaction_type& txn)> debugFuncType;
    typedef boost::function<bool (int i, transaction_type& txn, tlm::tlm_dmi& dmi)> dmiFuncType;

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
    
    bool get_direct_mem_ptr(transaction_type& trans, tlm_dmi&  dmi_data){
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
    

      
    void setCallBacks(nbFuncType& cb1, bFuncType& cb2, dmiFuncType& cb3, debugFuncType& cb4){
      m_nb_f=&cb1;
      m_b_f=&cb2;
      m_dmi_f=&cb3;
      m_dbg_f=&cb4;
    }
    
    sc_core::sc_port_base* getOtherSide(){return m_caller_port;}
    
  private:
    int m_id;
    nbFuncType* m_nb_f;
    bFuncType*  m_b_f;
    debugFuncType* m_dbg_f;
    dmiFuncType* m_dmi_f;
    sc_core::sc_port_base* m_caller_port;   
};

template <typename TYPES>
class callback_binder_bw: public tlm_bw_transport_if<TYPES>{
  public:
    typedef typename TYPES::tlm_payload_type              transaction_type;
    typedef typename TYPES::tlm_phase_type                phase_type;  
    typedef tlm::tlm_sync_enum                            sync_enum_type;
  
    typedef boost::function<sync_enum_type (int i, transaction_type& txn, phase_type& p, sc_core::sc_time& t)> nbFuncType;
    typedef boost::function<void (int i, sc_dt::uint64 l, sc_dt::uint64 u)> dmiFuncType;

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

    void setCallBacks(nbFuncType& cb1, dmiFuncType& cb2){
      m_nb_f=&cb1;
      m_dmi_f=&cb2;
    }
    
  private:
    int m_id;
    nbFuncType* m_nb_f;
    dmiFuncType* m_dmi_f;
};

template <unsigned int BUSWIDTH = 32,
          typename TYPES = tlm::tlm_generic_payload_types,
          unsigned int N=0
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION <= 20050714)
          ,sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND
#endif
          >
class MultiInitBase: public tlm_initiator_socket<BUSWIDTH, 
                                                tlm_fw_transport_if<TYPES>,
                                                tlm_bw_transport_if<TYPES>,
                                                N
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION <= 20050714)
                                                ,POL
#endif
                                                >{
public:
  typedef tlm_initiator_socket<BUSWIDTH, 
                              tlm_fw_transport_if<TYPES>,
                              tlm_bw_transport_if<TYPES>,
                              N
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION <= 20050714)
                              ,POL
#endif
                              > base_type;
  
  typedef typename base_type::base_target_socket_type base_target_socket_type;
                                                
  virtual void disableBEOE()=0;
  virtual MultiInitBase* getHierarchBind()=0;
  virtual std::vector<callback_binder_bw<TYPES>* >& getBinders()=0;
  virtual std::vector<tlm_fw_transport_if<TYPES>*>&  getSockets()=0;
  virtual ~MultiInitBase(){}
  MultiInitBase(const char* name):base_type(name){}
};

template <unsigned int BUSWIDTH = 32,
          typename TYPES = tlm::tlm_generic_payload_types,
          unsigned int N=0
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION <= 20050714)
          ,sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND
#endif
          >
class MultiTargetBase: public tlm_target_socket<BUSWIDTH, 
                                                tlm_fw_transport_if<TYPES>,
                                                tlm_bw_transport_if<TYPES>,
                                                N
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION <= 20050714)                                                
                                                ,POL
#endif
                                                >{
public:
  typedef tlm_target_socket<BUSWIDTH, 
                              tlm_fw_transport_if<TYPES>,
                              tlm_bw_transport_if<TYPES>,
                              N
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION <= 20050714)
                              ,POL
#endif
                              > base_type;
  
  typedef typename base_type::base_initiator_socket_type base_initiator_socket_type;
  typedef typename base_type::initiator_socket_type initiator_socket_type;
                                                
  virtual MultiTargetBase* getHierarchBind()=0;
  virtual void setHierarchBind(MultiTargetBase*)=0;
  virtual std::vector<callback_binder_fw<TYPES>* >& getBinders()=0;
  virtual std::map<unsigned int, tlm_bw_transport_if<TYPES>*>&  getMultiBinds()=0;
  virtual ~MultiTargetBase(){}
  MultiTargetBase(const char* name):base_type(name){}
};

template <typename TYPES>
class MultiToMultiBindBase{
public:
  virtual ~MultiToMultiBindBase(){}
  virtual tlm_fw_transport_if<TYPES>* getLastBinder(tlm_bw_transport_if<TYPES>*)=0;
};

}

#endif