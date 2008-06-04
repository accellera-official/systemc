/*******************************************************************************
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
*******************************************************************************/
//==============================================================================
/// @file initiator_top.cpp
//
/// @brief Instantiates initiator and traffic_generator
//
/// @details
///    This module performs:
///      1. Instantiation of the traffic_generator and the lt_initiator
///         and the interconnecting sc_fifo's
///      2. Binding of the Interconnect for the components
//
//==============================================================================
//
//  Original Authors:
//    Jack Donovan, ESLX
//    Charles Wilson, ESLX
//    Anna Keist, ESLX
//
//==============================================================================


#include "initiator_top.h"                         // this header file
#include "reporting.h"                             // reporting macro helpers

static const char *filename = "initiator_top.cpp"; ///< filename for reporting

/// Constructor

initiator_top::initiator_top    		       
( sc_core::sc_module_name name                    
, const unsigned int    ID                        
, sc_dt::uint64         base_address_1            
, sc_dt::uint64         base_address_2
) 
  :sc_module           (name) 	         // module instance name
  ,top_initiator_socket                  // Init the socket
      ("top_initiator_socket")             
  ,m_ID                (ID)              // initiator ID
  ,m_initiator                           // Init initiator
    ("m_initiator"                                            
    ,ID                                  // ID for reporting                                        
    )
  ,m_traffic_gen                         // Init traffic Generator
    ("m_traffic_gen"                              
    ,ID                                  // ID for reporting
    ,base_address_1                      // first base address
    ,base_address_2                      // second base address
    ,4
    )

{
  /// Bind ports to m_request_fifo between m_initiator and m_traffic_gen
  m_traffic_gen.request_out_port  (m_request_fifo);
  m_initiator.request_in_port     (m_request_fifo);
  
  /// Bind ports to m_response_fifo between m_initiator and m_traffic_gen
  m_initiator.response_out_port   (m_response_fifo);
  m_traffic_gen.response_in_port  (m_response_fifo);

  /// Bind initiator-socket to initiator-socket hierarchical connection 
  m_initiator.initiator_socket(top_initiator_socket);
}

//==============================================================================
///  @fn initiator_top::invalidate_direct_mem_ptr
//         
///  @brief Mandatory virtual implementation
//  
///  @details
///    Unused but required when using hierarchical connectivity with simple_socket
///    Not Used or Implemented in this example
//
//============================================================================== 
void                                                                                          
initiator_top::invalidate_direct_mem_ptr                                      
( sc_dt::uint64      start_range                        
, sc_dt::uint64      end_range                 
)
{  
  std::ostringstream       msg;          // log message
  msg.str ("");
        
  msg << "Initiator: " << m_ID << " Not implemented";
  REPORT_ERROR(filename, __FUNCTION__, msg.str());      
} // end invalidate_direct_mem_ptr
  
//==============================================================================
///  @fn initiator_top::nb_transport_bw
//         
///  @brief Mandatory virtual implementation
//                
///  @details
///    Unused but required when using hierarchical connectivity with simple_socket
///    Not Used or Implemented in this example
//
//============================================================================== 
tlm::tlm_sync_enum       
initiator_top::nb_transport_bw                                                          
( tlm::tlm_generic_payload  &payload
, tlm::tlm_phase            &phase
, sc_core::sc_time          &delta
)
{
  std::ostringstream       msg;                 // log message
  msg.str ("");
          
  msg << "Initiator: " << m_ID 
      << " Not implemented, for hierachical connection of initiator socket";
  REPORT_ERROR(filename, __FUNCTION__, msg.str());
          
  return tlm::TLM_COMPLETED;  

} // end nb_transport_bw


