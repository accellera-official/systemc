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

/* ---------------------------------------------------------------------------------------
 @file ut_lt_top.cpp
 
 @brief UT-LT top class
 
  Original Authors:
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

#include "tlm.h"

#include "ut_lt_top.h"                              ///< our header
#include "lt_target_memory.h"                       ///< memory target
#include "lt_initiator.h"                           ///< initiator
#include "SimpleBus.h"                              ///< simple bus

/*=============================================================================
  @fn ut_lt_top::ut_lt_top
  
  @brief ut_lt_top constructor
  
  @details
    This routine does the initialization grunt work. It allows us to create a
    'top' module for SystemC simulation environments that need a 'top' in order
    to work properly.
    
  @param name module name
  
  @retval void
=============================================================================*/
ut_lt_top::ut_lt_top                                ///< constructor
( sc_core::sc_module_name name                      ///< module name
)
  : sc_core::sc_module                              ///< SC base
  ( name                                            ///< module name
  )
    
  , m_bus                                           ///< bus
  ( "m_bus"                                         ///< module name
  )
    
  , m_target_1                                      ///< target 1
  ( "m_target_1"                                    ///< module name
  , 1                                               ///< target ID
  , 0x00000000                                      ///< base address
  , 16*1024                                         ///< memory size (bytes)
  , 1                                               ///< memory width (bytes)
  , sc_core::sc_time(10, sc_core::SC_NS)            ///< delay clock period
  , 6                                               ///< read clocks
  , 4                                               ///< write clocks
  , 0                                               ///< refresh clocks
  , 0                                               ///< refresh frequency
  )
      
  , m_initiator_1                                   ///< initiator 1
  ( "m_initiator_1"                                 ///< module name
  , 1                                               ///< initiator ID
  , 0xDEAFBEEF                                      ///< random number generator seed
  )

  , m_adapter_ut_lt                                 ///< UT initiator to LT adapter
  ( "m_adapter_ut_lt"                               ///< module name
  )

  , m_adapter_lt_ut                                 ///< LT adapter to UT target
  ( "m_adapter_lt_ut"                               ///< module name
  )

  , m_ut_initiator                                  ///< UT initiator 
  ( "m_ut_initiator"                                ///< module name
  , 2                                               ///< initiator ID
  , 0xABEEFBED                                      ///< random number generation seed
  )

  , m_ut_target                                     ///< UT target memory      
  ( "m_ut_target"                                    ///< module name
  , 2                                               ///< target ID
  , 0x00000000                                      ///< base address
  , 16*1024                                         ///< memory size (bytes)
  , 1                                               ///< memory width (bytes)
  , sc_core::sc_time(10, sc_core::SC_NS)            ///< delay clock period
  , 2                                               ///< read clocks
  , 5                                               ///< write clocks
  )
{
  // connect the sockets to the switch
  m_initiator_1.lt_processor_socket (m_bus.target_socket[0]);         // LT initiator   to bus [0]
  m_adapter_ut_lt.adapter_socket    (m_bus.target_socket[1]);         // UT-LT adapter  to bus [1]
  m_bus.initiator_socket[0]         (m_target_1.m_memory_socket);     // bus [0]        to LT target memory
  m_bus.initiator_socket[1]         (m_adapter_lt_ut.adapter_socket); // bus [1]        to LT-UT adapter

  // bind the UT initiator and target sockets to the adapters
  m_ut_initiator.socket             (m_adapter_ut_lt.target_socket);  // UT initiator   to UT-LT adapter
  m_adapter_lt_ut.initiator_socket  (m_ut_target.socket);             // LT-UT adapter  to UT target memory
}
