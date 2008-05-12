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
    Bill Bunton, ESLX
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

#include "traffic_generator.h"                      ///< traffic generator
#include "lt_initiator_endianness.h"                ///< initiator
#include "lt_processor_abstraction.h"               ///< processor abstraction
#include "tlm.h"                                    ///< TLM headers

/*=============================================================================
  @fn lt_processor_abstraction::lt_processor_abstraction
  
  @brief lt_processor_abstraction constructor
  
  @details
    This routine initialized an lt_processor_abstraction class instance.
    
  @param name       module name
  @param ID         initiator ID
  @param endianness initiator endianness
  @param seed       random number generator seed
  
  @retval none
=============================================================================*/
lt_processor_abstraction::lt_processor_abstraction  ///< constructor
( sc_core::sc_module_name   name                    ///< module name
, const unsigned int        ID                      ///< initiator ID
, const tlm::tlm_endianness endianness              ///< initiator endianness
, const unsigned long       seed                    ///< random number generator seed
, const unsigned int        message_count           ///< number of messsages to generate
) 
  : sc_module           ( name)                     ///< module name
  , m_ID                ( ID)                       ///< initiator ID
  , m_endianness        ( endianness)               ///< initiator endianness
  , m_seed              ( seed)                     ///< random number generator seed
  , lt_processor_socket ("lt_processor_socket")     ///< TLM socket
  , m_initiator         ( "m_initiator"
                        , ID
                        , endianness
                        , seed
                        )                           ///< initiator instance
  , m_traffic_gen       ( "m_traffic_gen"
                        , ID
                        , seed
                        , message_count
                        )                           ///< traffic generator instance
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
