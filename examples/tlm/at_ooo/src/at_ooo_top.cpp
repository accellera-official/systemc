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

/*=============================================================================
  @file example_system_top.cpp
  
  @brief This class instantiates components that compose a TLM2 example system
         that demonstrates Out Of Order transaction processing

=============================================================================*/

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Anna Keist, ESLX
*****************************************************************************/

#include "at_ooo_top.h"                             // example system top header
#include "at_target_2_phase.h"                      // memory target
#include "at_target_ooo_2_phase.h"                  // memory target
#include "initiator_top.h"                          // initiator
#include "tlm.h"                                    // TLM header

/*=============================================================================
  @fn example_system_top::example_system_top
  
  @brief example_system_top constructor
  
  @details
    This class instantiates the example system components and call 
    the modules bind methods to logically connect the model components. 
    
  @param name module name
  
  @retval void
=============================================================================*/
example_system_top::example_system_top    ///< constructor
( sc_core::sc_module_name name              ///< module name
)
  : sc_core::sc_module                      ///< SC base
    ( name                                  ///< module name
    )

  , m_bus                                   ///< bus
    ( "m_bus"                               ///< module name
    )

  , m_at_target_2_phase_1                   ///< at_test_target
    ( "m_at_target_2_phase_1"               ///< module name
    , 201
    , "memory_socket_1"                     ///< socket name
    , 4*1024                                ///< memory size (bytes)
    , 4                                     ///< memory width (bytes)
    , sc_core::sc_time(10, sc_core::SC_NS)  ///< accept delay
    , sc_core::sc_time(50, sc_core::SC_NS)  ///< read response delay
    , sc_core::sc_time(30, sc_core::SC_NS)  ///< write response delay
    )

  , m_at_target_ooo_2_phase_1               ///< at_test_target
    ( "m_at_target_ooo_2_phase_1"           ///< module name
    , 202
    , "memory_socket_1"                     ///< socket name
    , 4*1024                                ///< memory size (bytes)
    , 4                                     ///< memory width (bytes)
    // force additional out of order execution by making on 
    // target longer than the other
    , sc_core::sc_time(20, sc_core::SC_NS)  ///< accept delay
    , sc_core::sc_time(100, sc_core::SC_NS)  ///< read response delay
    , sc_core::sc_time(60, sc_core::SC_NS)  ///< write response delay
    )

  , m_initiator_1                           ///< initiator 1
    ( "m_initiator_1"                       ///< module name
    , 101                                   ///< initiator ID
    , 0x0000000000000100                    ///< fitst base address
    , 0x0000000010000100                    ///< second base address
    , 2                                     // active transactions
    )

  , m_initiator_2                           ///< initiator 2
    ( "m_initiator_2"                       ///< module name
    , 102                                   ///< initiator ID
    , 0x0000000000000200                    ///< fitst base address
    , 0x0000000010000200                    ///< second base address
    , 2                                     // active transactions
    )

{
  // bind TLM2 initiator to TLM2 target
  m_initiator_1.initiator_socket(m_bus.target_socket[0]);
  m_initiator_2.initiator_socket(m_bus.target_socket[1]);

  m_bus.initiator_socket[0](m_at_target_2_phase_1.m_memory_socket);
  m_bus.initiator_socket[1](m_at_target_ooo_2_phase_1.m_memory_socket);

}

