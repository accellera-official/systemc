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

//=====================================================================
/// @file lt_synch_target.cpp
//
/// @brief Implements single phase AT target
//
//=====================================================================
//  Original Authors:
//    Jack Donovan, ESLX
//
//=====================================================================

#include "lt_synch_target.h"                        // our header
#include "reporting.h"                            // reporting macros

using namespace  std;

static const char *filename = "lt_synch_target.cpp"; ///< filename for reporting

///Constructor
lt_synch_target::lt_synch_target
( sc_core::sc_module_name module_name               // module name
, const unsigned int        ID                      // target ID
, const char                *memory_socket          // socket name
, sc_dt::uint64             memory_size             // memory size (bytes)
, unsigned int              memory_width            // memory width (bytes)
, const sc_core::sc_time    accept_delay            // accept delay (SC_TIME)
, const sc_core::sc_time    read_response_delay     // read response delay (SC_TIME)
, const sc_core::sc_time    write_response_delay    // write response delay (SC_TIME)
)
: sc_module               (module_name)             /// init module name
, m_memory_socket         (memory_socket)           /// init socket name
, m_ID                    (ID)                      /// init target ID
, m_memory_size           (memory_size)             /// init memory size (bytes)
, m_memory_width          (memory_width)            /// init memory width (bytes)
, m_accept_delay          (accept_delay)            /// init accept delay
, m_read_response_delay   (read_response_delay)     /// init read response delay
, m_write_response_delay  (write_response_delay)    /// init write response delay

, m_target_memory
  ( m_ID                          // initiator ID for messaging
  , m_read_response_delay         // delay for reads
  , m_write_response_delay        // delay for writes
  , m_memory_size                 // memory size (bytes)
  , m_memory_width                // memory width (bytes)
  )

{

  m_memory_socket.register_b_transport(this, &lt_synch_target::custom_b_transport);

}

//==============================================================================
//  b_transport implementation calls from initiators
//
//=============================================================================
void
lt_synch_target::custom_b_transport
( tlm::tlm_generic_payload  &payload                // ref to  Generic Payload
, sc_core::sc_time          &delay_time             // delay time
)
{

  std::ostringstream  msg;
  msg.str("");
  sc_core::sc_time      mem_op_time;

  m_target_memory.operation(payload, mem_op_time);

  delay_time = delay_time + m_accept_delay + mem_op_time;

  msg << "Target: " << m_ID
      << " Forcing a synch in a temporal decoupled initiator with wait( "
      << delay_time << "),";
  REPORT_INFO(filename,  __FUNCTION__, msg.str());

  wait(delay_time);

  delay_time = sc_core::SC_ZERO_TIME;

  msg.str("");
  msg << "Target: " << m_ID
      << " return from wait will return a delay of "
      << delay_time;
  REPORT_INFO(filename,  __FUNCTION__, msg.str());

  return;
}
