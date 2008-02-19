/*****************************************************************************
 *
 *   The following code is derived, directly or indirectly, from the SystemC
 *   source code Copyright (c) 1996-2008 by all Contributors.
 *   All Rights reserved.
 *
 *   The contents of this file are subject to the restrictions and limitations
 *   set forth in the SystemC Open Source License Version 3.0 (the "License");
 *   You may not use this file except in compliance with such restrictions and
 *   limitations. You may obtain instructions on how to receive a copy of the
 *   License at http://www.systemc.org/. Software distributed by Contributors
 *   under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 *   ANY KIND, either express or implied. See the License for the specific
 *   language governing rights and limitations under the License.
 *
 *   *****************************************************************************/

/* ---------------------------------------------------------------------------------------
 @file lt_processor_abstraction.cpp
 
 @brief LT processor abstraction routines
 
  Original Authors:
    Charles Wilson, ESLX
    Bill Bunton, ESLX
    
--------------------------------------------------------------------------------------- */

#include "traffic_generator.h"                        ///< traffic generator
#include "lt_initiator.h"                             ///< initiator
#include "lt_processor_abstraction.h"                 ///< processor abstraction
#include "tlm.h"                                      ///< TLM headers

/*=============================================================================
  @fn lt_processor_abstraction::lt_processor_abstraction
  
  @brief lt_processor_abstraction constructor
  
  @details
    This routine initialized an lt_processor_abstraction class instance.
    
  @param name module name
  @param ID   initiator ID
  @param seed random number generator seed
  
  @retval none
=============================================================================*/
lt_processor_abstraction::lt_processor_abstraction  ///< constructor
( sc_core::sc_module_name name                      ///< module name
, const unsigned int      ID                        ///< initiator ID
, const unsigned long     seed                      ///< random number generator seed
) 
  : sc_module           (name)                      ///< module name
  , m_ID                (ID)                        ///< initiator ID
  , m_seed              (seed)                      ///< random number generator seed
  , lt_processor_socket ("lt_processor_socket")     ///< TLM socket
  , m_initiator         ("m_initiator", ID, seed)   ///< initiator instance
  , m_traffic_gen       ("m_traffic_gen", ID, seed) ///< traffic generator instance
{
  // Bind internal ports 
  m_traffic_gen.request_out_port  (m_request_fifo);
  m_initiator.request_in_port     (m_request_fifo);

  m_initiator.response_out_port   (m_response_fifo);
  m_traffic_gen.response_in_port  (m_response_fifo);

  // Do initiator-socket to initiator-socket hierarchical binding 
  m_initiator.initiator_socket(lt_processor_socket);
}

/*=============================================================================
  @fn lt_processor_abstraction::~lt_processor_abstraction
  
  @brief lt_processor_abstraction destructor
  
  @details
    This routine terminates an lt_processor_abstraction class instance.
    
  @param none
  
  @retval none
=============================================================================*/
lt_processor_abstraction::~lt_processor_abstraction   ///< destructor
( void
)
{
}

/*=============================================================================
  @fn lt_processor_abstraction::invalidate_direct_mem_ptr
  
  @brief handle invalidate DMI pointer
  
  @param start_range  start of invalidation range
  @param end_range    end of invalidation range
  
  @note
    not supported
  
  @retval none
=============================================================================*/
void
lt_processor_abstraction::invalidate_direct_mem_ptr   ///< invalidate_direct_mem_ptr
( sc_dt::uint64 start_range                           ///< starting address
, sc_dt::uint64 end_range                             ///< ending address
)
{
}

/*=============================================================================
  @fn lt_processor_abstraction::nb_transport
  
  @brief non-blocking transport
  
  @param transaction  GP transaction
  @param phase        phase
  @param delay        delay
  
  @note
    not supported
  
  @retval tlm_sync_enum synchronization state
=============================================================================*/
tlm::tlm_sync_enum                                  ///< synchronization state
lt_processor_abstraction::nb_transport              ///< nb_transport
( tlm::tlm_generic_payload  &transaction            ///< transaction
, tlm::tlm_phase            &phase                  ///< phase
, sc_core::sc_time          &delay                  ///< delay
)
{
  return tlm::TLM_REJECTED;
}
