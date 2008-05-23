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

/*=============================================================================
  @file tlm_1_integration_top.cpp
  
  @brief TLM 1 - 2 integration example top module
=============================================================================*/

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
*****************************************************************************/

#include "tlm_1_integration_top.h"                    ///< our header
#include "lt_target_memory.h"                         ///< memory target
#include "lt_initiator.h"                             ///< initiator
#include "SimpleBus.h"                                ///< simple bus
#include "tlm.h"                                      ///< TLM headers

/*=============================================================================
  @fn lt_top::lt_top
  
  @brief lt_top constructor
  
  @details
    This routine does the initialization grunt work. It allows us to create a
    'top' module for SystemC simulation environments that need a 'top' in order
    to work properly.
    
  @param name module name
  
  @retval void
=============================================================================*/
lt_top::lt_top                                ///< constructor
( sc_core::sc_module_name name                ///< module name
)
  : sc_core::sc_module                        ///< SC base
    ( name                                    ///< module name
    )
    
  , m_bus                                     ///< bus
    ( "m_bus"                                 ///< module name
    )
    
  , m_target_1                                ///< target 1
    ( "m_target_1"                            ///< module name
    , 0x00000000                              ///< base address
    , 16*1024                                 ///< memory size (bytes)
    , 1                                       ///< memory width (bytes)
    , sc_core::sc_time(10, sc_core::SC_NS)    ///< delay clock period
    , 6                                       ///< read clocks
    , 4                                       ///< write clocks
    , 0                                       ///< refresh clocks
    , 0                                       ///< refresh frequency
    )
      
  , m_initiator_1                             ///< initiator 1
    ( "m_initiator_1"                         ///< module name
    )

  , m_adapter_tlm_1_2                         ///< adapter 1 to 2
    ("m_adapter_tlm_1_2"
     )

  , m_adapter_tlm_2_1                         ///< adapter 2 to 1 
    ("m_adapter_tlm_2_1"
     )

  , m_tlm_1_master                            ///< tlm1 master 
    ("m_tlm_1_master"
     )

  , m_tlm_1_mem_slave                         ///< tlm1 memory slave      
    ("m_tlm_1_mem_slave"  
    )
{
//------------------------------------------------------------------------------
// connect the sockets to the switch

  // bind TLM2 initiator to TLM2 switch socket
  m_initiator_1.processor_abstraction_socket(m_bus.target_socket[0]);

  // bind TLM1-to-TLM2 adapter to TLM2 switch socket
  m_adapter_tlm_1_2.initiator_socket(m_bus.target_socket[1]);

  // bind TLM2 switch socket to TLM2 memory 
  m_bus.initiator_socket[0](m_target_1.m_memory_socket);

  // bind TLM2 switch to TLM2-to-TLM1 adapter socket
  m_bus.initiator_socket[1](m_adapter_tlm_2_1.adapter_socket);

  // bind TLM1 to TLM1-to-TLM2 adapter
  m_tlm_1_master.initiator_port(m_adapter_tlm_1_2.tlm_1_export);

  // bind TLM2-to-TLM1 adapter to TLM1 memory 
  m_adapter_tlm_2_1.tlm_1_initiator_port(m_tlm_1_mem_slave.target_port);










}

