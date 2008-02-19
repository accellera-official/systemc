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
/*==============================================================================
  @file at_initiator.h
  @brief This is the TLM AT initiator

  @Details
  This module implements three interfaces. The first implements the functionality
  required to access the tlm_master_socket. The second and third interfaces 
  implements the functionality required to allow other modules to pass generic 
  payloads via an sc_fifo.
=============================================================================*/
   
/*****************************************************************************
  Original Authors:
    Anna Keist, ESLX
    Bill Bunton, ESLX
*****************************************************************************/

#ifndef __AT_INITIATOR_H__
#define __AT_INITIATOR_H__

#include "tlm.h"                                      ///< TLM headers
#include <queue>                                      ///< standard queue
#include <set>                                        ///< standard set

class at_initiator                                  ///< at_initiator 
  :         public sc_core::sc_module               ///< module base class 
  , virtual public tlm::tlm_bw_nb_transport_if<>    ///< initiator socket
{
  SC_HAS_PROCESS(at_initiator);

//==============================================================================
// Ports, exports and Sockets
//==============================================================================
  typedef tlm::tlm_generic_payload  *gp_ptr;        ///< generic payload
  public:
    sc_core::sc_port<sc_core::sc_fifo_in_if  <gp_ptr> > request_in_port;  
    sc_core::sc_port<sc_core::sc_fifo_out_if <gp_ptr> > response_out_port;
    tlm::tlm_initiator_socket<>                         initiator_socket; 

//==============================================================================
// Constructors & Destructor 
//==============================================================================
    at_initiator                                    ///< constructor
    ( sc_core::sc_module_name name                  ///< module name
    , const unsigned int ID                         ///< initiator ID
    , sc_core::sc_time end_rsp_delay                ///< delay
    );
     
    /// Destructor
    ~at_initiator(void);                            ///< destructor

//==============================================================================

//------------------------------------------------------------------------------
// Private member variable and methods
//------------------------------------------------------------------------------
  private:
  void initiator_thread (void);                     ///< initiator thread
  void m_send_end_rsp_method(void);                 ///< send end response method

//==============================================================================
// Backward nb transport method 
//============================================================================== 
  tlm::tlm_sync_enum nb_transport(                  ///< nb_transport
    tlm::tlm_generic_payload& transaction,          ///< transaction
    tlm::tlm_phase&           phase,                ///< transaction phase
    sc_core::sc_time&         time);                ///< elapsed time

//==============================================================================
// Required but not implemented member methods
//============================================================================== 
  void invalidate_direct_mem_ptr(                   ///< invalidate_direct_mem_ptr
    sc_dt::uint64 start_range,                      ///< start range
    sc_dt::uint64 end_range);                       ///< end range

//==============================================================================
// Private member variables and methods
//==============================================================================
private:
  std::queue<tlm::tlm_generic_payload *>  m_req_accepted_queue; ///< request accpeted queue
  sc_core::sc_event                       m_req_accepted_event; ///< request accpeted event
  std::set<tlm::tlm_generic_payload *>    m_wait_rsp_set;       ///< wait for response queue
  std::queue<tlm::tlm_generic_payload *>  m_send_end_rsp_queue; ///< send end response queue
  sc_core::sc_event                       m_send_end_rsp_event; ///< send end response event
  unsigned int                            m_ID;                 ///< initiator ID
  sc_core::sc_time                        m_end_rsp_delay;      ///< end response delay
  bool                                    detect_2_phase_target;///< determines whether AT target is 2 or 4phase 
}; 
 #endif /* __AT_INITIATOR_H__ */
