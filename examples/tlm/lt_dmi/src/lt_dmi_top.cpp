/*******************************************************************************
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
 ******************************************************************************/

//==============================================================================
/// @file lt_dmi_top.cpp
//
/// @brief Very Basic Loosley Timed (blocking) example with DMI
//
/// @details This class instantiates components that compose the lt_dmi 
///        example system. A demonstration of the blocking interface for
//         TLM 2.0. The same (or similar) block diagram is created 
///        for additional examples, but with different components
//
//==============================================================================
//
//  Authors:
//    Jack Donovan, ESLX
//    Charls Wilson, ESLX
//    Anna Keist, ESLX
//
//==============================================================================
#include "lt_dmi_top.h"           	// example system top header

//==============================================================================
///  @fn lt_dmi_top::lt_dmi_top
//
///  @brief Constructor for the top module for an LT example with DMI
///  @details
///    The construcor method 
///       1. initializes the instantiated components
///       2. calls the bind methods to connect the example components 
//
//==============================================================================

SC_HAS_PROCESS(lt_dmi_top);

lt_dmi_top::lt_dmi_top  
( sc_core::sc_module_name name             
)
  : sc_core::sc_module                      /// Init SC base
    ( name                                 
    )
  , m_bus                                   /// Init Simple Bus
    ( "m_bus"                              
    )
  , m_simulation_limit(1000000, sc_core::SC_NS) ///< simulation time limit
  , m_lt_dmi_target_1                       /// Init target with support for DMI
    ( "m_lt_dmi_target_1"                   // instance name
    , 201                                   // 1st Target ID is 201
    , "memory_socket_1"                     // socket name
    , 4*1024                                // memory size (bytes)
    , 4                                     // memory width (bytes)
    , sc_core::sc_time(20, sc_core::SC_NS)  // accept delay
    , sc_core::sc_time(20, sc_core::SC_NS)  // read response delay
    , sc_core::sc_time(15, sc_core::SC_NS)  // write response delay
    )
  , m_lt_dmi_target_2                       /// Init Target with support for DMI
    ( "m_lt_dmi_target_2"                   // instance name
    , 202                                   // 2nd Target ID is 202
    , "memory_socket_2"                     // socket name
    , 4*1024                                // memory size (bytes)
    , 4                                     // memory width (bytes)
    , sc_core::sc_time(20, sc_core::SC_NS)  // accept delay
    , sc_core::sc_time(50, sc_core::SC_NS)  // read response delay
    , sc_core::sc_time(30, sc_core::SC_NS)  // write response delay
    )
  , m_initiator_1                           /// LT instance with DMI
    ( "m_initiator_1"                       // instance name
    , 101                                   // 1st Initiator ID is 101
    , 0x0000000000000000                    // fitst base address
    , 0x0000000010000000                    // second base address                       
    )
  , m_initiator_2                           /// LT instance with DMI
    ( "m_initiator_2"                       // instance name
    , 102                                   // 2nd Initiator ID is 102
    , 0x0000000000000000                    // fitst base address
    , 0x0000000010000000                    // second base address
    )
{
  // register the time limiter
  SC_THREAD(limit_thread);

  /// bind TLM2 initiators to TLM2 target sockets on SimpleBus
  m_initiator_1.top_initiator_socket(m_bus.target_socket[0]);
  m_initiator_2.top_initiator_socket(m_bus.target_socket[1]);

  /// bind TLM2 targets to TLM2 initiator sockets on SimpleBus
  m_bus.initiator_socket[0](m_lt_dmi_target_1.m_memory_socket);
  m_bus.initiator_socket[1](m_lt_dmi_target_2.m_memory_socket);
}

void
lt_dmi_top::limit_thread                    ///< limit_thread
( void
)
{
  sc_core::wait ( sc_core::SC_ZERO_TIME );  // Only after simulation initialized
  
  sc_core::wait ( m_simulation_limit );     // Limit simulation time
  
  sc_core::sc_stop ();                      // Ensure end_of_simulation called
}
