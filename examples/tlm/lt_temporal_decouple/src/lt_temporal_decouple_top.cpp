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

//==============================================================================
/// @file lt_temporal_decouple_top.cpp
//
/// @brief Very basic Loosely Timed (blocking) example
//
/// @details This class instantiates components that compose the lt 
/// example system with dmi. A demonstration of the blocking interface for
/// TLM 2.0. The same (or similar) block diagram is created 
/// for additional examples, but with different components.
/// See the constructore for companents instatntiated for this example.
//
//==============================================================================
//
//  Authors:
//    Jack Donovan, ESLX
//    Anna Keist, ESLX
//    Charles Wilson, ESLX
//
//==============================================================================
#include "lt_temporal_decouple_top.h"           	// example system top header

//=====================================================================
///  @fn lt_temporal_decouple_top::lt_temporal_decouple_top
//
///  @brief Constructor for the top module for a basic LT example
//
///  @details
///    The construcor method 
///       1. initializes the instantiated components
///       2. calls the bind methods to connect the example components 
//
//=====================================================================
lt_temporal_decouple_top::lt_temporal_decouple_top  
( sc_core::sc_module_name name             
)
  : sc_core::sc_module                      /// Init SC base
    ( name                                 
    )
  , m_bus                                   /// Init Simple Bus
    ( "m_bus"                              
    )
  , m_lt_synch_target_1                     /// "problem lt"/forces synch lt target
    ( "m_lt_synch_target_1"                 // module instance name
    , 201                                   // 1st Target ID is 201
    , "memory_socket_1"                     // socket name
    , 4*1024                                // memory size (bytes)
    , 4                                     // memory width (bytes)
    , sc_core::sc_time(20, sc_core::SC_NS)  // accept delay
    , sc_core::sc_time(100, sc_core::SC_NS) // read response delay
    , sc_core::sc_time(60, sc_core::SC_NS)  // write response delay
    )
  , m_lt_target_2                           /// "well behaved"/normal lt target
    ( "m_lt_target_2"                       // module instance name
    , 202                                   // 2nd Target ID is 202
    , "memory_socket_1"                     // socket name
    , 4*1024                                // memory size (bytes)
    , 4                                     // memory width (bytes)
    , sc_core::sc_time(10, sc_core::SC_NS)  // accept delay
    , sc_core::sc_time(50, sc_core::SC_NS)  // read response delay
    , sc_core::sc_time(30, sc_core::SC_NS)  // write response delay
    )
  , m_td_initiator_1                        /// temporal decouple initiator
    ( "m_td_initiator_1"                    // module instance name
    , 101                                   // 1st Initiator ID is 101
    , 0x0000000000000000                    // fitst base address
    , 0x0000000010000000                    // second base address
    )
  , m_initiator_2                           /// non-td initiator
    ( "m_initiator_2"                       // module instance name
    , 102                                   // 2nd Initiator ID is 102
    , 0x0000000000000000                    // fitst base address
    , 0x0000000010000000                    // second base address
    )
{
  /// bind TLM2 initiators to TLM2 target sockets on SimpleBus
  m_td_initiator_1.top_initiator_socket(m_bus.target_socket[0]);
  m_initiator_2.top_initiator_socket(m_bus.target_socket[1]);

  /// bind TLM2 targets to TLM2 initiator sockets on SimpleBus
  m_bus.initiator_socket[0](m_lt_synch_target_1.m_memory_socket);
  m_bus.initiator_socket[1](m_lt_target_2.m_memory_socket);
}
