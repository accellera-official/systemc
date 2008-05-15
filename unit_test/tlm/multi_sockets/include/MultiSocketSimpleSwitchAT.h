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

#ifndef __SIMPLESWITCHAT_H__
#define __SIMPLESWITCHAT_H__

#include "tlm.h"

#include "tlm_utils/multi_passthrough_initiator_socket.h"
#include "tlm_utils/multi_passthrough_target_socket.h"
#include "simpleAddressMap.h"
#include "extensionPool.h"
#include "tlm_utils/instance_specific_extensions.h"
#include "tlm_utils/peq_with_phase.h"


/*
This class is a simple crossbar switch through which an arbitrary number of initiators
may communicate in parallel as long as they do not talk to the same target.

If two masters address the same target at the same point of time, 
the choice who will be allowed to communicate
is done non-deterministically (based on the SystemC process exectution order).

This could be avoided by changing the fwPEQ into a priority PEQ of some kind.

The switch ensures that the end_req and end_resp rules are not violated when
many initiator talk to the same target.
*/
class MultiSocketSimpleSwitchAT : public sc_core::sc_module
{
public:
  typedef tlm::tlm_generic_payload                                 transaction_type;
  typedef tlm::tlm_phase                                           phase_type;
  typedef tlm::tlm_sync_enum                                       sync_enum_type;
  typedef tlm_utils::multi_passthrough_target_socket<MultiSocketSimpleSwitchAT>    target_socket_type;
  typedef tlm_utils::multi_passthrough_initiator_socket<MultiSocketSimpleSwitchAT> initiator_socket_type;

public:
  target_socket_type target_socket; //the target multi socket
  
private:
  initiator_socket_type initiator_socket; //the initiator multi socket (private to enforce use of bindTarget function)
  SimpleAddressMap m_addrMap; //a pretty simple address map
  std::vector<std::deque<transaction_type*> > m_pendingReqs; //list of pending reqs per target
  std::vector<std::deque<transaction_type*> > m_pendingResps; //list of pending resps per initiator
  std::vector<sc_dt::uint64> m_masks; //address masks for each target
  tlm_utils::instance_specific_extension_accessor accessMySpecificExtensions; //extension accessor to access private extensions
  tlm_utils::peq_with_phase<MultiSocketSimpleSwitchAT> m_bwPEQ; //PEQ in the fw direction
  tlm_utils::peq_with_phase<MultiSocketSimpleSwitchAT> m_fwPEQ; //PEQ in the bw direction


  //an instance specific extension that tells us whether we are in a wrapped b_transport or not
  class BTag : public tlm_utils::instance_specific_extension<BTag>{
  public:
    sc_core::sc_event event; //trigger this event when transaction is done
  };

  //an instance specific extension that holds information about source and sink of a txn
  // as well as information if the req still has to be cleared and if the txn is already
  //  complete on the target side
  class ConnectionInfo : public tlm_utils::instance_specific_extension<ConnectionInfo>{
    public:
    unsigned int  fwID; //socket number of sink
    unsigned int  bwID; //socket number of source
    bool clearReq;      //is the txn still in req phase?
    bool alreadyComplete; //has the txn already completed on the target side?
  };

  class internalPEQTypes{ //use the tpPEQ to delay connection infos
    public:
    typedef ConnectionInfo tlm_payload_type;
    typedef tlm::tlm_phase tlm_phase_type;
  };

  tlm_utils::peq_with_phase<MultiSocketSimpleSwitchAT, internalPEQTypes> m_clearPEQ; //PEQ to delay response clearing

  
  ExtensionPool<ConnectionInfo> m_connInfoPool; //our pool of extensions
  unsigned int m_target_count;  //number of connected targets (see bindTargetSocket for explanation)
  
public:
  SC_HAS_PROCESS(MultiSocketSimpleSwitchAT);
  MultiSocketSimpleSwitchAT(sc_core::sc_module_name name) :
    sc_core::sc_module(name),
    target_socket("target_socket"),
    initiator_socket("initiator_socket"),
    m_bwPEQ(this, &MultiSocketSimpleSwitchAT::bwPEQcb),
    m_fwPEQ(this, &MultiSocketSimpleSwitchAT::fwPEQcb),
    m_clearPEQ(this, &MultiSocketSimpleSwitchAT::clearPEQcb),
    m_connInfoPool(10),
    m_target_count(0)
  {
    target_socket.register_nb_transport_fw(this, &MultiSocketSimpleSwitchAT::initiatorNBTransport);
    target_socket.register_b_transport(this, &MultiSocketSimpleSwitchAT::b_transport);
    initiator_socket.register_nb_transport_bw(this, &MultiSocketSimpleSwitchAT::targetNBTransport);
  }

  void bindTargetSocket(initiator_socket_type::base_target_socket_type& target
                       ,sc_dt::uint64 low
                       ,sc_dt::uint64 high
                       ,sc_dt::uint64 mask = 0xffffffffffffffffULL){
    initiator_socket(target); //bind sockets
    //insert into address map and increase target count
    // (we have to count the targets manually, because target_socket.size() is only reliable during simulation
    //  as it gets evaluated during end_of_elaboration)
    m_addrMap.insert(low, high, m_target_count++); 
    m_masks.push_back(mask); //add the mask for this target
  }

  unsigned int decode(const sc_dt::uint64& address)
  {
    return m_addrMap.decode(address);
  }

  void start_of_simulation(){
    //initialize the lists of pending reqs and resps
    m_pendingReqs.resize(initiator_socket.size());
    m_pendingResps.resize(target_socket.size());
  }


  void b_transport(int initiator_id, transaction_type& trans, sc_core::sc_time& t){
    //first make sure that there is no BTag (just for debugging)
    BTag* btag;
    accessMySpecificExtensions(trans).get_extension(btag);
    assert(!btag);
    BTag tag; //now add our BTag
    accessMySpecificExtensions(trans).set_extension(&tag);
    phase_type phase=tlm::BEGIN_REQ; //then simply use our nb implementation (respects all the rules)
    initiatorNBTransport(initiator_id, trans, phase, t);
    wait(tag.event); //and wait for the event to be triggered
    accessMySpecificExtensions(trans).clear_extension(&tag); //don't forget to remove the extension
  }

  //do a fw transmission
  void initiatorNBTransport_core(transaction_type& trans,
                                 phase_type& phase,
                                 sc_core::sc_time& t,
                                 unsigned int tgtSocketNumber){
    switch (initiator_socket[tgtSocketNumber]->nb_transport_fw(trans, phase, t)) {
      case tlm::TLM_ACCEPTED:
      case tlm::TLM_UPDATED:
        // Transaction not yet finished
        if (phase != tlm::BEGIN_REQ)
        {
          assert(phase!=tlm::END_RESP);
          m_bwPEQ.notify(trans,phase,t);
        }
        break;
      case tlm::TLM_COMPLETED:
        // Transaction finished
        ConnectionInfo* connInfo;
        accessMySpecificExtensions(trans).get_extension(connInfo); 
        assert(connInfo);
        connInfo->alreadyComplete=true;
        phase=tlm::BEGIN_RESP;
        m_bwPEQ.notify(trans, phase, t);
        break;
      default:
        assert(0); exit(1);
    };
  }
  
  //nb_transport_fw
  sync_enum_type initiatorNBTransport(int initiator_id,
                                      transaction_type& trans,
                                      phase_type& phase,
                                      sc_core::sc_time& t)
  {
    //if we were allowed to put END_RESP into a PEQ, we would have an 
    // implementation beautifully symetrical to the bw path
    //  but since we are not allowed to do so
    //   we have to insert the private info from END_RESP into another PEQ
    ConnectionInfo* connInfo;
    accessMySpecificExtensions(trans).get_extension(connInfo);
    if (phase==tlm::BEGIN_REQ){
      //add our private information to the txn
      assert(!connInfo); 
      connInfo=m_connInfoPool.construct();
      connInfo->fwID=decode(trans.get_address());
      connInfo->bwID=initiator_id;
      connInfo->clearReq=true; 
      connInfo->alreadyComplete=false;
      accessMySpecificExtensions(trans).set_extension(connInfo);
      m_fwPEQ.notify(trans,phase,t);
    }
    else
    if (phase==tlm::END_RESP){
      assert(connInfo);
      accessMySpecificExtensions(trans).clear_extension(connInfo);
      if (!connInfo->alreadyComplete) initiator_socket[connInfo->fwID]->nb_transport_fw(trans, phase, t);
      m_clearPEQ.notify(*connInfo, phase, t);
      return tlm::TLM_COMPLETED;
    }
    else
      {assert(0); exit(1);}
    return tlm::TLM_ACCEPTED;

  }

  sync_enum_type targetNBTransport(int portId,
                                   transaction_type& trans,
                                   phase_type& phase,
                                   sc_core::sc_time& t)
  {
    if (phase != tlm::END_REQ && phase != tlm::BEGIN_RESP) {
      std::cout << "ERROR: '" << name()
                << "': Illegal phase received from target." << std::endl;
      assert(false); exit(1);
    }
    //simply stuff it into the bw PEQ
    m_bwPEQ.notify(trans,phase,t);
    return tlm::TLM_ACCEPTED;
  }
  
  void bwPEQcb(transaction_type& trans, const phase_type& phase){
    //first get our private info from the txn
    ConnectionInfo* connInfo;
    accessMySpecificExtensions(trans).get_extension(connInfo); 
    assert(connInfo); 
    phase_type p=phase;
    sc_core::sc_time t=sc_core::SC_ZERO_TIME;
    BTag* btag;
    accessMySpecificExtensions(trans).get_extension(btag);
    bool doCall=btag==NULL; //we only will do a bw call if we are not in a wrapped b_transport
    if (phase==tlm::END_REQ | connInfo->clearReq){ //in case the target left out end_req clearReq reminds us to unlock the req port
      assert(m_pendingReqs[connInfo->fwID].front()==&trans);
      m_pendingReqs[connInfo->fwID].pop_front(); //allow another req to start at this target
      if (m_pendingReqs[connInfo->fwID].size()){ //there was a pending req
        phase_type ph=tlm::BEGIN_REQ;
        initiatorNBTransport_core(*m_pendingReqs[connInfo->fwID].front(), ph, t,connInfo->fwID);
      }
      connInfo->clearReq=false;
    } 
    //no else here, since we might clear the req AND begin a resp
    if (phase==tlm::BEGIN_RESP){
      if (&trans!=m_pendingResps[connInfo->bwID].front()) //obviously this is a new response (we can't get the same txn twice before the first resp ends
        m_pendingResps[connInfo->bwID].push_back(&trans);
      doCall=m_pendingResps[connInfo->bwID].size()==1; //do a call in case the response socket was free
    }

    if (doCall){ //we have to do a call on the bw path
      if (btag){ //only possible if BEGIN_RESP and resp socket was free
          phase_type ph=tlm::END_RESP;
          if (!connInfo->alreadyComplete)
            initiator_socket[connInfo->fwID]->nb_transport_fw(trans, ph, t);
          m_clearPEQ.notify(*connInfo, ph,t); //schedule reset of response socket lock
          accessMySpecificExtensions(trans).clear_extension(connInfo); //remove the extension
          btag->event.notify(t); //release b_transport
      }
      else
        switch (target_socket[connInfo->bwID]->nb_transport_bw(trans, p, t)){
          case tlm::TLM_ACCEPTED:
          case tlm::TLM_UPDATED:
            break;
          case tlm::TLM_COMPLETED:{
            //covers a piggy bagged END_RESP to START_RESP
            phase_type ph=tlm::END_RESP;
            if (!connInfo->alreadyComplete)
              initiator_socket[connInfo->fwID]->nb_transport_fw(trans, ph, t);
            m_clearPEQ.notify(*connInfo, ph,t); //schedule reset of response socket lock
            accessMySpecificExtensions(trans).clear_extension(connInfo);  //remove extension
            }
            break;
          default:
            assert(0); exit(1);
            
        };
    }
  }

  //the following two functions (fwPEQcb and clearPEQcb) could be one, if we were allowed
  // to stick END_RESP into a PEQ
  void fwPEQcb(transaction_type& trans, const phase_type& phase){
    //phase is always BEGIN_REQ
    ConnectionInfo* connInfo;
    accessMySpecificExtensions(trans).get_extension(connInfo); 
    assert(connInfo); 
    trans.set_address(trans.get_address()&m_masks[connInfo->fwID]); //mask address
    m_pendingReqs[connInfo->fwID].push_back(&trans);
    if (m_pendingReqs[connInfo->fwID].size()==1){ //the socket is free
      phase_type ph=phase;
      sc_core::sc_time t=sc_core::SC_ZERO_TIME;
      initiatorNBTransport_core(trans, ph, t, connInfo->fwID);
    }
  }
  
  void clearPEQcb(ConnectionInfo& connInfo, const phase_type& phase){
    //phase is always END_RESP
    m_pendingResps[connInfo.bwID].pop_front(); //remove current response
    if (m_pendingResps[connInfo.bwID].size()){ //if there was one pending
      phase_type p=tlm::BEGIN_RESP; //schedule its transmission
      sc_core::sc_time t=sc_core::SC_ZERO_TIME;
      m_bwPEQ.notify(*m_pendingResps[connInfo.bwID].front(),p,t);
    }
    m_connInfoPool.free(&connInfo); //release connInfo
  }
  
  void dump_status(){
    std::cout<<"At "<<sc_core::sc_time_stamp()<<" status of "<<name()<<" is "<<std::endl
             <<"  Number of connected initiators: "<<target_socket.size()<<std::endl
             <<"  Number of connected targets: "<<initiator_socket.size()<<std::endl
             <<"  Pending requests:"<<std::endl;
    for (unsigned int i=0; i<m_pendingReqs.size(); i++)
      std::cout<<"    "<<m_pendingReqs[i].size()<<" pending requests for target number "<<i<<std::endl;
    std::cout<<"  Pending responses:"<<std::endl;
    for (unsigned int i=0; i<m_pendingResps.size(); i++)
      std::cout<<"    "<<m_pendingResps[i].size()<<" pending responses for initiator number "<<i<<std::endl;
    std::cout<<"  The address map is:"<<std::endl;
    m_addrMap.dumpMap();
    
  }
};

#endif
