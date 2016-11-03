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
/// @file td_initiator_top.h
//
///  @brief Top level for lt example with temporal decoupling
//
//==============================================================================
//
//  Original Authors:
//    Jack Donovan, ESLX
//    Charles Wilson, ESLX
//    Anna Keist, ESLX
//
//==============================================================================

#ifndef __TD_INITIATOR_TOP_H__
#define __TD_INITIATOR_TOP_H__

#include "tlm.h"                                // TLM headers
#include "lt_td_initiator.h"                    // LT initiator with temp decoup
#include "traffic_generator.h"                  // traffic generator

class td_initiator_top                          		       
  : public sc_core::sc_module    
  , virtual public tlm::tlm_bw_transport_if<>  // inherit from tlm interfaces
{
public:
	  
//=====================================================================
///  @fn td_initiator_top::td_initiator_top
//	    
///  @brief td_initiator_top constructor
//	    
///  @details
///    Initiator top module contains a traffic generator and an example 
///    unique initiator module
//
//===================================================================== 
  td_initiator_top 	                                   
  ( sc_core::sc_module_name name                 ///< module name
  , const unsigned int  ID                       ///< initiator ID
  , sc_dt::uint64       base_address_1           ///< first base address
  , sc_dt::uint64       base_address_2           ///< second base address
  );

/// Required for Hierachical Internconnect
  void                                                                                          
  invalidate_direct_mem_ptr                                      
  ( sc_dt::uint64      start_range                        
  , sc_dt::uint64      end_range                 
  );
  
  tlm::tlm_sync_enum       
  nb_transport_bw                                                          
  ( tlm::tlm_generic_payload  &payload
  , tlm::tlm_phase            &phase
  , sc_core::sc_time          &delta
  );
  
//Member Variables/Objects  ====================================================

public:
  
  tlm::tlm_initiator_socket< > top_initiator_socket;

private:
  
  typedef tlm::tlm_generic_payload  *gp_ptr;   ///< Generic Payload pointer
  
  sc_core::sc_fifo <gp_ptr>  m_request_fifo;   ///< request SC FIFO
  sc_core::sc_fifo <gp_ptr>  m_response_fifo;  ///< response SC FIFO
  
  const unsigned int         m_ID;             ///< initiator ID
    
  lt_td_initiator            m_lt_td_initiator;///< TLM initiator instance
  traffic_generator          m_traffic_gen;    ///< traffic generator instance

};

#endif /* __TD_INITIATOR_TOP_H__ */
