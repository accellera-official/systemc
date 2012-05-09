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
//==============================================================================
/// @file at_initiator_explicit.h
//
/// @brief This is a TLM AT initiator
// 
//=============================================================================
//  Original Authors:
//    Bill Bunton, ESLX
//    Anna Keist, ESLX
//
//=============================================================================

#ifndef  __AT_INITIATOR_EXPLICIT_H__
#define  __AT_INITIATOR_EXPLICIT_H__

#include "tlm.h"                                      // TLM headers
#include <map>                                        // STL map
#include "tlm_utils/peq_with_get.h"                   // Payload event queue FIFO

class at_initiator_explicit                           /// TLM AT at_initiator_explicit 
  :         public sc_core::sc_module                 /// inherit from SC module base clase
  , virtual public tlm::tlm_bw_transport_if<>         /// inherit from TLM "backward interface"
{
  SC_HAS_PROCESS(at_initiator_explicit);

//==============================================================================
// Ports, exports and Sockets
//==============================================================================
  typedef tlm::tlm_generic_payload  *gp_ptr;        // generic payload
  public:
    sc_core::sc_port<sc_core::sc_fifo_in_if  <gp_ptr> > request_in_port;  
    sc_core::sc_port<sc_core::sc_fifo_out_if <gp_ptr> > response_out_port;
    tlm::tlm_initiator_socket<>                         initiator_socket; 


//=============================================================================
///	@fn at_initiator_explicit
///
///	@brief Constructor for AT Initiator
///
///	@details
///		Generic AT Initiator used in several examples.
///		Constructor offers several parameters for customization	
///
//=============================================================================

    at_initiator_explicit                                // constructor
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
    {Rcved_UPDATED_enum    	                      // Received TLM_UPDATED d 
    ,Rcved_ACCEPTED_enum    	                    // Received ACCEPTED
    ,Rcved_END_REQ_enum    	                      // Received TLM_BEGIN_RESP
    };

  typedef std::map<tlm::tlm_generic_payload *, previous_phase_enum> waiting_bw_path_map;
  
  waiting_bw_path_map     m_waiting_bw_path_map;    // Wait backward path map 
  sc_core::sc_event       m_enable_next_request_event; 
  tlm_utils::peq_with_get<tlm::tlm_generic_payload>
                          m_send_end_rsp_PEQ;       // send end response PEq
  unsigned int            m_ID;                     // initiator ID
  sc_core::sc_time        m_end_rsp_delay;          // end response delay
  bool                    m_nb_trans_fw_prev_warning;
}; 
 #endif /* __AT_INITIATOR_EXPLICIT_H__ */
