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

  // memory manager (queue)
  
  static const unsigned int  m_txn_data_size = 4;         // transaction size
  
  class tg_queue_c                                        /// memory managed queue class
  : public tlm::tlm_mm_interface                          /// implements memory management IF
  {
    public:
    
    tg_queue_c                                            /// tg_queue_c constructor
    ( void
    )
    {
    }
    
    void
    enqueue                                                /// enqueue entry (create)
    ( void
    )
    { 
      tlm::tlm_generic_payload  *transaction_ptr  = new tlm::tlm_generic_payload ( this ); /// transaction pointer
      unsigned char             *data_buffer_ptr  = new unsigned char [ m_txn_data_size ];  /// data buffer pointer
      
      transaction_ptr->set_data_ptr ( data_buffer_ptr );
      
      m_queue.push ( transaction_ptr );
    }
    
    tlm::tlm_generic_payload *                            /// transaction pointer
    dequeue                                               /// dequeue entry
    ( void
    )
    {
      tlm::tlm_generic_payload *transaction_ptr = m_queue.front ();
      
      m_queue.pop();
      
      return transaction_ptr;
    }
    
    void
    release                                               /// release entry
    ( tlm::tlm_generic_payload *transaction_ptr           /// transaction pointer
    )
    {
      transaction_ptr->release ();
    }
    
    bool                                                  /// true / false
    is_empty                                              /// queue empty
    ( void
    )
    {
      return m_queue.empty ();
    }
    
    size_t                                                /// queue size
    size                                                  /// queue size
    ( void
    )
    {
      return m_queue.size ();
    }
    
    void
    free                                                  /// return transaction
    ( tlm::tlm_generic_payload *transaction_ptr           /// to the pool
    )
    {
      transaction_ptr->reset();
      m_queue.push ( transaction_ptr );
    }
    
    private:
    
    std::queue<tlm::tlm_generic_payload*> m_queue;        /// queue
  };

//=============================================================================
// Member Variables 
	
  private:
	  
  typedef tlm::tlm_generic_payload  *gp_ptr;   	    // pointer to a generic payload
  
  const unsigned int  m_ID;                   	    // initiator ID

  sc_dt::uint64       m_base_address_1;      	    // first base address
  sc_dt::uint64       m_base_address_2;       	    // second base address
  
  tg_queue_c          m_transaction_queue;          // transaction queue
  
  unsigned int        m_active_txn_count;           // active transaction count
  bool                m_check_all;
  
  public:

  /// Port for requests to the initiator
  sc_core::sc_port<sc_core::sc_fifo_out_if <gp_ptr> > request_out_port; 
  
  /// Port for responses from the initiator
  sc_core::sc_port<sc_core::sc_fifo_in_if  <gp_ptr> > response_in_port;
};
#endif /* __TRAFFIC_GENERATOR_H__ */
