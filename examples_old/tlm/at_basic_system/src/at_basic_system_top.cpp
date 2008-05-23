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
  @file at_basic_system_top.cpp
  
  @brief AT example top module

=============================================================================*/

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Anna Keist, ESLX
*****************************************************************************/

#include "at_basic_system_top.h"                      ///< our header
#include "at_target_4_phase.h"                        ///< memory target
#include "at_processor_abstraction.h"                 ///< initiator
#include "SimpleBus.h"                                ///< simple bus
#include "tlm.h"                                      ///< TLM header

/*=============================================================================
  @fn at_basic_system_top::at_basic_system_top
  
  @brief _at_basic_system_top constructor
  
  @details
    This routine does the initialization grunt work. It allows us to create a
    'top' module for SystemC simulation environments that need a 'top' in order
    to work properly.
    
  @param name module name
  
  @retval void
=============================================================================*/
at_basic_system_top::at_basic_system_top    ///< constructor
( sc_core::sc_module_name name              ///< module name
)
  : sc_core::sc_module                      ///< SC base
    ( name                                  ///< module name
    )

  , m_bus                                   ///< bus
    ( "m_bus"                               ///< module name
    )

  , m_at_target_4_phase_1                   ///< at_test_target
    ( "m_at_target_4_phase_1"               ///< module name
    , 1
    , "memory_socket_1"                     ///< socket name
    , 0x0000000000000000                    ///< base address
    , 4*1024                                ///< memory size (bytes)
    , 4                                     ///< memory width (bytes)
    , sc_core::sc_time(10, sc_core::SC_NS)  ///< accept delay
    , sc_core::sc_time(50, sc_core::SC_NS)  ///< read response delay
    , sc_core::sc_time(30, sc_core::SC_NS)  ///< write response delay
    )

  , m_at_target_4_phase_2                   ///< at_test_target
    ( "m_at_target_4_phase_2"               ///< module name
    , 2
    , "memory_socket_1"                     ///< socket name
    , 0x0000000000000000                    ///< base address
    , 4*1024                                ///< memory size (bytes)
    , 4                                     ///< memory width (bytes)
    , sc_core::sc_time(10, sc_core::SC_NS)  ///< accept delay
    , sc_core::sc_time(50, sc_core::SC_NS)  ///< read response delay
    , sc_core::sc_time(30, sc_core::SC_NS)  ///< write response delay
    )

  , m_at_initiator_1                        ///< initiator 1
    ( "m_at_initiator_4_phase_1"            ///< module name
    , 1                                     ///< initiator ID
    , 0xAFEDBEEF                            ///< random number generator seed
    , 32                                    ///< number of messages to generate
    )

  , m_at_initiator_2                        ///< initiator 2
    ( "m_at_initiator_4_phase_2"            ///< module name
    , 2                                     ///< initiator ID
    , 0xCAFEFEED                            ///< random number generator seed
    , 32                                    ///< number of messages to generate
    )

{
  // bind TLM2 initiator to TLM2 target
  m_at_initiator_1.at_processor_abstraction_socket(m_bus.target_socket[0]);
  m_at_initiator_2.at_processor_abstraction_socket(m_bus.target_socket[1]);

  m_bus.initiator_socket[0](m_at_target_4_phase_1.m_memory_socket);
  m_bus.initiator_socket[1](m_at_target_4_phase_2.m_memory_socket);

}

