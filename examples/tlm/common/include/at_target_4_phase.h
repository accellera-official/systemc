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
/// @file at_target_4_phase.h
//
/// @brief Four phase AT target only
//
//=====================================================================
//  Original Authors:
//   Bill Bunton, ESLX
//   Anna Keist, ESLX
//   Charles Wilson, ESLX
//   Jack Donovan, ESLX
//=====================================================================

#ifndef __AT_TARGET_4_PHASE_H__ 
#define __AT_TARGET_4_PHASE_H__

#include "tlm.h"                          		        // TLM headers
#include "tlm_utils/peq_with_get.h"                   // Payload event queue FIFO
#include "memory.h"                                   // memory storage

class at_target_4_phase                               /// at_target_4_phase
:         public sc_core::sc_module           	      /// inherit from SC module base clase
, virtual public tlm::tlm_fw_transport_if<>   	      /// inherit from TLM "forward interface"
{
// Member Methods =====================================================
	
  public:	  
//=====================================================================
///	@fn at_initiator_4_phase
///
///	@brief Constructor for Four Phase AT target
///
///	@details
///		Generic Four Phase target used in several examples.
///		Constructor offers several parameters for customization	
///
//=====================================================================
  
  at_target_4_phase                           
  ( sc_core::sc_module_name   module_name           ///< SC module name
  , const unsigned int        ID                    ///< target ID
  , const char                *memory_socket        ///< socket name
  , sc_dt::uint64             memory_size           ///< memory size (bytes)
  , unsigned int              memory_width          ///< memory width (bytes)
  , const sc_core::sc_time    accept_delay          ///< accept delay (SC_TIME, SC_NS)
  , const sc_core::sc_time    read_response_delay   ///< read response delay (SC_TIME, SC_NS)
  , const sc_core::sc_time    write_response_delay  ///< write response delay (SC_TIME, SC_NS)
  );

//=====================================================================
///	@brief Implementation of call from Initiator. 
//
///	@details
///		This is the ultimate destination of the nb_transport_fw call from
///		the initiator after being routed trough a Bus
//
//===================================================================== 
  
  tlm::tlm_sync_enum                                // sync status
  nb_transport_fw                    
  ( tlm::tlm_generic_payload &gp                    ///< generic payoad pointer
  , tlm::tlm_phase           &phase                 ///< transaction phase
  , sc_core::sc_time         &delay_time            ///< time taken for transport
  );
 
  //=====================================================================
  ///  @fn at_target_4_phase::end_request_method
  /// 
  ///  @brief End Request Processing
  ///
  ///  @details
  ///                   ????
  //=====================================================================

   void
   end_request_method                             
   ( void
   );
   
  //=====================================================================
  ///  @fn at_target_4_phase::begin_response_method
  /// 
  ///  @brief Response Processing
  ///
  ///  @details
  ///    This routine takes transaction responses from the m_response_PEQ.
  ///    It contains the state machine to manage the communication path
  ///    back to the initiator. This method is registered as an SC_METHOD
  ///    with the SystemC kernal and is sensitive to m_response_PEQ.get_event() 
  //=====================================================================   
   
    void
   begin_response_method                             
   ( void
   );


  private:
	    
/// helper function for printing memory transactions
  void
  print_memory_transaction_helper
  ( const int 				  &ID
  , std::ostringstream  	  &partial_msg
  , tlm::tlm_generic_payload  &trans
  );
 
//==============================================================================
// Methods not Implemented for this Example
  
/// b_transport() - Blocking Transport
  void                                                // returns nothing 
  b_transport
  ( tlm::tlm_generic_payload  &payload                // ref to payload 
  , sc_core::sc_time          &delay_time             // delay time 
  );


/// Not implemented for this example but required by interface
  bool                                              // success / failure
  get_direct_mem_ptr                       
  ( tlm::tlm_generic_payload   &payload,            // address + extensions
    tlm::tlm_dmi               &dmi_data            // DMI data
  );

  
/// Not implemented for this example but required by interface
  unsigned int                                      // result
  transport_dbg                            
  ( tlm::tlm_generic_payload  &payload              // debug payload
  );

// Member Variables ===================================================
  
  public:  
	  
  typedef tlm::tlm_generic_payload  *gp_ptr;		///< generic payload pointer
  
  /// @todo Should this be the convenience scoket from utilities?
  /// for clarity and templated types with "con" in the title
  tlm::tlm_target_socket<>  m_memory_socket;        ///<  target socket

  private:
	  
  const unsigned int        m_ID;                   ///< target ID
        sc_dt::uint64       m_memory_size;          ///< memory size (bytes)
        unsigned int        m_memory_width;         ///< word size (bytes)
  const sc_core::sc_time    m_accept_delay;         ///< accept delay
  const sc_core::sc_time    m_read_response_delay;  ///< read response delay
  const sc_core::sc_time    m_write_response_delay; ///< write response delays
        bool                m_nb_trans_fw_prev_warning;
        bool                m_end_request_method_prev_warning;
        bool                m_begin_resp_method_prev_warning;
        bool                m_trans_dbg_prev_warning;
        bool                m_get_dm_ptr_prev_warning;
        tlm_utils::peq_with_get<tlm::tlm_generic_payload>
                            m_end_request_PEQ;      ///< response payload event queue
        sc_core::sc_event   m_end_resp_rcvd_event;
        tlm_utils::peq_with_get<tlm::tlm_generic_payload>
                            m_response_PEQ;         ///< response payload event queue
        memory              m_target_memory;
};

#endif /* __AT_TARGET_4_PHASE_H__ */
