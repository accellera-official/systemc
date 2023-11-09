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
/// @file select_initiator.h
//
/// @brief This is a TLM AT initiator
//
//=============================================================================
//  Original Authors:
//    Bill Bunton, ESLX
//    Anna Keist, ESLX
//
//=============================================================================

#ifndef __SELECT_INITIATOR_H__
#define __SELECT_INITIATOR_H__

#include "tlm.h"                                    // TLM headers
#include "tlm_utils/peq_with_get.h"                 // Payload event queue FIFO
#include <map>                                      // STL map

class select_initiator                              /// TLM AT select_initiator
:         public sc_core::sc_module                 /// inherit from SC module base clase
, virtual public tlm::tlm_bw_transport_if<>         /// inherit from TLM "backward interface"
{

//==============================================================================
// Ports, exports and Sockets
//==============================================================================
  typedef tlm::tlm_generic_payload  *gp_ptr;        // generic payload
  public:
    sc_core::sc_port<sc_core::sc_fifo_in_if  <gp_ptr> > request_in_port;
    sc_core::sc_port<sc_core::sc_fifo_out_if <gp_ptr> > response_out_port;
    tlm::tlm_initiator_socket<>                         initiator_socket;

//=============================================================================
///	@fn select_initiator
///
///	@brief Constructor for AT Initiator
///
///	@details
///		Generic AT Initiator used in several examples.
///		Constructor offers several parameters for customization
///
//=============================================================================

    select_initiator                                // constructor
    ( sc_core::sc_module_name name                  // module name
    , const unsigned int ID                         // initiator ID
    , sc_core::sc_time end_rsp_delay                // delay
    );

//=============================================================================
///  @fn at_target_2_phase::initiator_thread
///
///  @brief Initiator thread
///
///  @details
///   This thread takes generic payloads (gp) from the request FIFO that connects
///   to the traffic generator and initiates. When the transaction completes the
///   gp is placed in the response FIFO to return it to the traffic generator.
//=============================================================================
  private:
  void initiator_thread (void);                     // initiator thread

//=============================================================================
///  @fn at_target_2_phase::send_end_rsp_method
///
///  @brief Send end response method
///
///  @details
///   This routine takes transaction responses from the m_send_end_rsp_PEQ.
///   It contains the state machine to manage the communication path to the
///   targets.  This method is registered as an SC_METHOD with the SystemC
///   kernel and is sensitive to m_send_end_rsp_PEQ.get_event()
//=============================================================================
  private:
  void send_end_rsp_method(void);                   // send end response method

//=============================================================================
///	@brief Implementation of call from targets.
//
///	@details
///		This is the ultimate destination of the nb_transport_bw call from
///		the targets after being routed trough a Bus
//
//=====================================================================
   tlm::tlm_sync_enum nb_transport_bw(              // nb_transport
    tlm::tlm_generic_payload& transaction,          // transaction
    tlm::tlm_phase&           phase,                // transaction phase
    sc_core::sc_time&         time);                // elapsed time

//==============================================================================
// Required but not implemented member methods
//==============================================================================
  void invalidate_direct_mem_ptr(                   // invalidate_direct_mem_ptr
    sc_dt::uint64 start_range,                      // start range
    sc_dt::uint64 end_range);                       // end range

//==============================================================================
// Private member variables and methods
//==============================================================================
private:

  enum previous_phase_enum
    {Rcved_UPDATED_enum    	                    // Received TLM_UPDATED d
    ,Rcved_ACCEPTED_enum                            // Received ACCEPTED
    ,Rcved_END_REQ_enum    	                    // Received TLM_BEGIN_RESP
    };

  typedef std::map<tlm::tlm_generic_payload *, previous_phase_enum>
                          waiting_bw_path_map;
  waiting_bw_path_map     m_waiting_bw_path_map;        // Wait backward path map
  sc_core::sc_event       m_enable_next_request_event;
  tlm_utils::peq_with_get<tlm::tlm_generic_payload>
                          m_send_end_rsp_PEQ;           // send end response PEQ
  const unsigned int      m_ID;                         // initiator ID
  const sc_core::sc_time  m_end_rsp_delay;              // end response delay
  bool                    m_enable_target_tracking;     // ? remove
};
 #endif /* __SELECT_INITIATOR_H__ */
