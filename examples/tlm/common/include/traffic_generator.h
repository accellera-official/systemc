/**********************************************************************
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
 *********************************************************************/

//=====================================================================
/// @file traffic_generator.h
//
/// @brief traffic_generator class header
//
//=====================================================================
//  Authors:
//    Bill Bunton, ESLX
//    Charles Wilson, ESLX
//    Jack Donovan, ESLX
//====================================================================

#ifndef __TRAFFIC_GENERATOR_H__
#define __TRAFFIC_GENERATOR_H__

#include "tlm.h"                              	// TLM headers
#include <queue>                                // queue header from std lib

class traffic_generator                       	// traffic_generator
: public sc_core::sc_module                    	// sc_module
{

// Member Methods  ==================================================== 

  public:
//=====================================================================
///  @fn traffic_generator
//  
///  @brief traffic_generator constructor
//  
///  @details
///    Initializes Traffice Generator
//    
//=====================================================================

  traffic_generator                                
  ( sc_core::sc_module_name name            	  ///< module name for SC
  , const unsigned int      ID               	  ///< initiator ID
  , sc_dt::uint64           base_address_1    	///< first base address
  , sc_dt::uint64           base_address_2    	///< second base address
  );

//=============================================================================
///  @fn traffic_generator
//  
///  @brief traffic_generator constructor
//  
///  @details
///    Initializes Traffice Generator iIncluding active transaction count  
//    
//=============================================================================

  traffic_generator                                
  ( sc_core::sc_module_name name            	///< module name for SC
  , const unsigned int  ID               	    ///< initiator ID
  , sc_dt::uint64       base_address_1    	  ///< first base address
  , sc_dt::uint64       base_address_2    	  ///< second base address
  , unsigned int        active_txn_count      ///< Max number of active transactions  
  );

  //=============================================================================
  ///  @fn traffic_generator_thread
  // 
  ///  @brief traffic_generator processing thread
  // 
  ///  @details
  ///    Method actually called by SC simulator to generate and 
  ///    check traffic. Generate Writes then Reads to check the 
  ///    Writes
  //    
  //=============================================================================

  void
  traffic_generator_thread            
  ( void
  ); 
  
//-----------------------------------------------------------------------------
//  Check Complete method
void check_complete (void); 




//-----------------------------------------------------------------------------
//  Check All Complete method

void check_all_complete (void);

//=============================================================================
// Member Variables 
	
  private:
	  
  typedef tlm::tlm_generic_payload  *gp_ptr;   	    // pointer to a generic payload
  
  const unsigned int  m_ID;                   	    // initiator ID

  sc_dt::uint64       m_base_address_1;      	      // first base address
  sc_dt::uint64       m_base_address_2;       	    // second base address
  
  std::queue<tlm::tlm_generic_payload*> m_txn_pool; // response queue
  unsigned int        m_active_txn_count;
  bool                m_check_all;
  
  public:

  /// Port for requests to the initiator
  sc_core::sc_port<sc_core::sc_fifo_out_if <gp_ptr> > request_out_port; 
  
  /// Port for responses from the initiator
  sc_core::sc_port<sc_core::sc_fifo_in_if  <gp_ptr> > response_in_port;

  
};
#endif /* __TRAFFIC_GENERATOR_H__ */
