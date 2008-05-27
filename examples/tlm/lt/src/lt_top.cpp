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
/// @file lt_top.cpp
//
/// @brief Very basic Loosely Timed (blocking) example
//
/// @details This class instantiates components that compose the lt 
/// example system. A demonstration of the blocking interface for
/// TLM 2.0. The same (or similar) block diagram is created 
/// for additional examples, but with different components.
/// See the constructore for companents instatntiated for this example.
//
//==============================================================================
//
//  Authors:
//    Jack Donovan, ESLX
//    Charles Wilson, ESLX
//    Anna Keist, ESLX
//
//==============================================================================
#include "lt_top.h"           	            // example system top header

//==============================================================================
///  @fn lt_top::lt_top
//
///  @brief Constructor for the top module for a basic LT example
//
///  @details
///    The construcor method 
///       1. initializes the instantiated components
///       2. calls the bind methods to connect the example components 
//
//==============================================================================
lt_top::lt_top  
( sc_core::sc_module_name name             
)
  : sc_core::sc_module                      /// Init SC base
    ( name                                 
    )
  , m_bus                                   /// Init Simple Bus
    ( "m_bus"                              
    )
  , m_at_and_lt_target_1                    /// Initiatlize at/lt target
    ( "m_at_and_lt_target_1"                //  module instance name
    , 201                                   // 1st Target ID is 201
    , "memory_socket_1"                     // socket name
    , 4*1024                                // memory size (bytes)
    , 4                                     // memory width (bytes)
    , sc_core::sc_time(20, sc_core::SC_NS)  // accept delay
    , sc_core::sc_time(100, sc_core::SC_NS) // read response delay
    , sc_core::sc_time(60, sc_core::SC_NS)  // write response delay
    )
  , m_lt_target_2                           /// initialize lt only target
    ( "m_lt_target_2"                       // module instance name
    , 202                                   // 2nd Target ID is 202
    , "memory_socket_2"                     // socket name
    , 4*1024                                // memory size (bytes)
    , 4                                     // memory width (bytes)
    , sc_core::sc_time(10, sc_core::SC_NS)  // accept delay
    , sc_core::sc_time(50, sc_core::SC_NS)  // read response delay
    , sc_core::sc_time(30, sc_core::SC_NS)  // write response delay
    )
  , m_initiator_1                           /// Init Instance 1 of LT initiator
    ( "m_initiator_1"                       // module instance name
    , 101                                   // 1st Initiator ID is 101
    , 0x0000000000000000                    // fitst base address
    , 0x0000000010000000                    // second base address
    )
  , m_initiator_2                           /// Init initiator 2
    ( "m_initiator_2"                       // module instance name
    , 102                                   // 2nd Initiator ID is 102
    , 0x0000000000000000                    // fitst base address
    , 0x0000000010000000                    // second base address
    )
{
  /// bind TLM2 initiators to TLM2 target sockets on SimpleBus
  m_initiator_1.top_initiator_socket(m_bus.target_socket[0]);
  m_initiator_2.top_initiator_socket(m_bus.target_socket[1]);

  /// bind TLM2 targets to TLM2 initiator sockets on SimpleBus
  m_bus.initiator_socket[0](m_at_and_lt_target_1.m_memory_socket);
  m_bus.initiator_socket[1](m_lt_target_2.m_memory_socket);
}
