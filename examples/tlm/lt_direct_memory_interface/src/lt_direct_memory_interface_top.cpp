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
 @file lt_direct_memory_interface_top.cpp
 
 @brief LT direct memory interface SystemC top module
 
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

#include "lt_direct_memory_interface_top.h"           ///< our header
#include "lt_target.h"                                ///< LT target
#include "lt_processor_abstraction.h"                 ///< LT processor abstraction
#include "SimpleBus.h"                                ///< simple bus
#include "tlm.h"                                      ///< TLM headers

/*=============================================================================
  @fn lt_direct_memory_interface_top::lt_direct_memory_interface_top
  
  @brief lt_direct_memory_interface_top constructor
  
  @details
    This routine initialized a lt_direct_memory_interface_top class instance.
    
  @param name module name
  
  @retval none
=============================================================================*/
lt_direct_memory_interface_top::lt_direct_memory_interface_top       
( sc_core::sc_module_name name                      ///< module name
)
  : sc_core::sc_module                              ///< SC base
    ( name                                          ///< module name
    )
    
  , m_bus                                           ///< bus
    ( "m_bus"                                       ///< module name
    )
    
  , m_lt_target_1                                   ///< target memory 1
    ( "m_lt_target_1"                               ///< module name
    , 1                                             ///< target ID
    , "memory_socket_1"                             ///< socket name
    , 0x0000000000000000                            ///< base address
    , 4*1024                                        ///< memory size (bytes)
    , 4                                             ///< memory width (bytes)
    , sc_core::sc_time(10, sc_core::SC_NS)          ///< accept delay
    , sc_core::sc_time(50, sc_core::SC_NS)          ///< read response delay
    , sc_core::sc_time(30, sc_core::SC_NS)          ///< write response delay
    )
    
  , m_lt_target_2                                   ///< target memory 2
    ( "m_lt_target_2"                               ///< module name
    , 2                                             ///< target ID
    , "memory_socket_2"                             ///< socket name
    , 0x0000000010000000                            ///< base address
    , 4*1024                                        ///< memory size (bytes)
    , 4                                             ///< memory width (bytes)
    , sc_core::sc_time(15, sc_core::SC_NS)          ///< accept delay
    , sc_core::sc_time(60, sc_core::SC_NS)          ///< read response delay
    , sc_core::sc_time(40, sc_core::SC_NS)          ///< write response delay
    )
      
  , m_lt_initiator_1                                ///< initiator 1 (processor abstraction)
    ( "m_lt_initiator_1"                            ///< module name
    , 1                                             ///< initiator ID
    , 0xAFEDBEEF                                    ///< random number generator seed
    )
      
  , m_lt_initiator_2                                ///< initiator 2 (processor abstraction)
    ( "m_lt_initiator_2"                            ///< module name
    , 2                                             ///< initiator ID
    , 0xCAFEFEED                                    ///< random number generator seed
    )
{
  // connect the initiators and targets to the switch
  m_lt_initiator_1.lt_processor_socket(m_bus.target_socket[0]);
  m_lt_initiator_2.lt_processor_socket(m_bus.target_socket[1]);

  m_bus.initiator_socket[0](m_lt_target_1.m_memory_socket);
  m_bus.initiator_socket[1](m_lt_target_2.m_memory_socket);
}
