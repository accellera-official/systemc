/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/
//==============================================================================
///  @file lt_td_initiator.h
///  @brief This is Loosley Timed Initiator
///
///  @Details
///  This module implements only implements the blocking interface.
///  The  nb_transport_bw and invalidate_direct_memory_ptr are implemented
///  within the covenience socket
//
//==============================================================================
   
//==============================================================================
//  Original Authors:
//    Jack Donovan, ESLX
//    Anna Keist, ESLX
//==============================================================================

#ifndef __LT_TD_INITIATOR_H__
#define __LT_TD_INITIATOR_H__

#include "tlm.h"      // TLM headers
#include "tlm_utils/tlm_quantumkeeper.h"
#include "tlm_utils/simple_initiator_socket.h"

class lt_td_initiator                                 // lt_td_initiator 
  :  public sc_core::sc_module                        // module base class 
{
public:
// Constructor ================================================================= 
    lt_td_initiator                                   // constructor
    ( sc_core::sc_module_name name                    // module name
    , const unsigned int  ID                          ///< initiator ID
    );
     
// Method Declarations =========================================================
    
//==============================================================================
///     @brief SC_THREAD that performs blocking call (lt call)
//
///     @details
///        This SC_THREAD takes transactions from traffic generator via the 
///        sc_fifo attached to the request_in_port. Performs the blocking call.
///        After completing the blocking call the transactions are returned to
///        the traffic generator for checking via the response_out_port
//
//============================================================================== 
  void initiator_thread (void);                    
  
  
// Variable and Object Declarations ============================================
public:
  
   typedef tlm::tlm_generic_payload  *gp_ptr;        // generic payload
   tlm_utils::simple_initiator_socket<lt_td_initiator> initiator_socket;
 
   sc_core::sc_port<sc_core::sc_fifo_in_if  <gp_ptr> > request_in_port;  
   sc_core::sc_port<sc_core::sc_fifo_out_if <gp_ptr> > response_out_port;
   
   sc_core::sc_time      m_delay;

private:
  tlm::tlm_response_status gp_status;
  unsigned int            m_ID;                     // initiator ID
  sc_core::sc_time        m_end_rsp_delay;          // end response delay
  tlm_utils::tlm_quantumkeeper m_quantum_keeper;
  
}; 
 #endif /* __LT_TD_INITIATOR_H__ */
