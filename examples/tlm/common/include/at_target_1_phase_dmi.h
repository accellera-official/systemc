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
/// @file at_target_1_phase.h
//
/// @brief Single phase AT target
//
//=====================================================================
//  Original Authors:
//   Bill Bunton, ESLX
//   Charles Wilson, ESLX
//   Jack Donovan, ESLX
//=====================================================================

#ifndef __AT_TARGET_1_PHASE_H__ 
#define __AT_TARGET_1_PHASE_H__

#include "tlm.h"                          		        // TLM headers
#include "tlm_utils/peq_with_get.h"                   // Payload event queue FIFO
#include "memory.h"                                   // memory storage

class at_target_1_phase                               /// at_target_1_phase
:         public sc_core::sc_module           	      /// inherit from SC module base clase
, virtual public tlm::tlm_fw_transport_if<>   	      /// inherit from TLM "forward interface"
{
// Member Methods =====================================================
	
  public:	  
//=====================================================================
///	@fn at_initiator_1_phase
///
///	@brief Constructor for Single Phase AT target
///
///	@details
///		Generic Single Phase target used in several examples.
///		Constructor offers several parameters for customization	
///
//=====================================================================
  
  at_target_1_phase                           
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
  ///  @fn at_target_1_phase::begin_response_method
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
  
  tlm::tlm_target_socket<>  m_memory_socket;        ///<  target socket

  private:
	  
  const unsigned int        m_ID;                   ///< target ID
        sc_dt::uint64       m_memory_size;          ///< memory size (bytes)
        unsigned int        m_memory_width;         ///< word size (bytes)
  const sc_core::sc_time    m_accept_delay;         ///< accept delay
  const sc_core::sc_time    m_read_response_delay;  ///< read response delay
  const sc_core::sc_time    m_write_response_delay; ///< write response delays
        unsigned long       m_request_count;        ///< used to calc synch transactions  
        bool                m_nb_trans_fw_prev_warning;
        bool                m_begin_resp_method_prev_warning;
        bool                m_trans_dbg_prev_warning;
        bool                m_get_dm_ptr_prev_warning;
        tlm_utils::peq_with_get<tlm::tlm_generic_payload>
                            m_response_PEQ;         ///< response payload event queue
        memory              m_target_memory;
        sc_core::sc_event   m_end_resp_rcvd_event;
};

#endif /* __AT_TARGET_1_PHASE_H__ */
