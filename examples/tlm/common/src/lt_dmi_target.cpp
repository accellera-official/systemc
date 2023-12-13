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
/// @file lt_dmi_target.cpp
//
/// @brief Implements single phase AT target
//
//=====================================================================
//  Original Authors:
//    Jack Donovan, ESLX
//
//=====================================================================

#include "lt_dmi_target.h"                              // our header
#include "reporting.h"                                  // reporting macros

using namespace  std;

static const char *filename = "lt_dmi_target.cpp"; ///< filename for reporting


///Constructor
lt_dmi_target::lt_dmi_target
( sc_core::sc_module_name module_name                   ///< module name
, const unsigned int        ID                          ///< target ID
, const char                *memory_socket              ///< socket name
, sc_dt::uint64             memory_size                 ///< memory size (bytes)
, unsigned int              memory_width                ///< memory width (bytes)
, const sc_core::sc_time    accept_delay                ///< accept delay (SC_TIME)
, const sc_core::sc_time    read_response_delay         ///< read response delay (SC_TIME)
, const sc_core::sc_time    write_response_delay        ///< write response delay (SC_TIME)
, const sc_core::sc_time    start_dmi_delay             ///< start of dmi transactions
, const sc_core::sc_time    dmi_duration                ///< duration of dmi transactions
)
: sc_module                 ( module_name           )   ///< init module name
, m_memory_socket           ( memory_socket         )   ///< init socket name
, m_ID                      ( ID                    )   ///< init target ID
, m_memory_size             ( memory_size           )   ///< init memory size (bytes)
, m_memory_width            ( memory_width          )   ///< init memory width (bytes)
, m_accept_delay            ( accept_delay          )   ///< init accept delay
, m_read_response_delay     ( read_response_delay   )   ///< init read response delay
, m_write_response_delay    ( write_response_delay  )   ///< init write response delay
, m_dmi_enabled             ( true                  )
, m_start_dmi_delay         ( start_dmi_delay       )
, m_dmi_duration            ( dmi_duration          )
, m_start_address           ( 0                     )
, m_end_address             ( memory_size           )
, m_toggle_count            ( 0                     )
, m_max_dmi_toggle_count    ( 3                     )

, m_target_memory           ( m_ID                      // initiator ID for messaging
                            , m_read_response_delay     // delay for reads
                            , m_write_response_delay    // delay for writes
                            , m_memory_size             // memory size (bytes)
                            , m_memory_width            // memory width (bytes)
                            )                           ///< target memory
{
  SC_METHOD(toggle_dmi_method);

  m_memory_socket.register_b_transport(this, &lt_dmi_target::custom_b_transport);
  m_memory_socket.register_get_direct_mem_ptr(this, &lt_dmi_target::get_direct_mem_ptr);
}

//==============================================================================
//  b_transport implementation calls from initiators
//
//=============================================================================
void
lt_dmi_target::custom_b_transport
( tlm::tlm_generic_payload  &payload                // ref to  Generic Payload
, sc_core::sc_time          &delay_time             // delay time
)
{
  sc_core::sc_time      mem_op_time;
  std::ostringstream    msg;

  msg.str("");

  m_target_memory.operation(payload, mem_op_time);

  if(m_dmi_enabled)
  {
    payload.set_dmi_allowed(true);
    msg << "Target: " << m_ID
        << " has set dmi_allowed "
        << endl << "      ";
  }
  else
  {
    payload.set_dmi_allowed(false);
  }

  msg << "Target: " << m_ID
      << " returned delay of " << delay_time
      << " + " << m_accept_delay << " + " << mem_op_time;

  delay_time = delay_time + m_accept_delay + mem_op_time;

  msg << " = " << delay_time;
  REPORT_INFO(filename,  __FUNCTION__, msg.str());

  return;
}

bool
lt_dmi_target::get_direct_mem_ptr
( tlm::tlm_generic_payload   &gp              ///< address + extensions
, tlm::tlm_dmi               &dmi_properties  ///< dmi data
)
{
  std::ostringstream  msg;
  msg.str("");
  sc_dt::uint64 address = gp.get_address();

  // First check to see if we are "open" to a dmi
  if(!m_dmi_enabled)
  {
    msg << "Target: " << m_ID
        << " DMI not enabled, not expecting call ";
    REPORT_INFO(filename, __FUNCTION__, msg.str());
  }
  else
  {                                  // dmi processing
    if (address < m_end_address+1)   // check that address is in our range
    {
      // set up dmi properties object ======================================
      dmi_properties.allow_read_write   (                                );
      dmi_properties.set_start_address  ( m_start_address                );
      dmi_properties.set_end_address    ( m_end_address                  );
      dmi_properties.set_dmi_ptr        ( m_target_memory.get_mem_ptr () );
      dmi_properties.set_read_latency   ( m_read_response_delay          );
      dmi_properties.set_write_latency  ( m_write_response_delay         );

      msg << "Target: " << m_ID
          << " passing DMI pointer back to initiator";
      REPORT_INFO(filename, __FUNCTION__, msg.str());
      return true;
    }
    else
    {
      msg << "Target: " << m_ID
          << " DMI pointer request for address= " << address
          << " max address for this target = " << m_end_address+1;
      REPORT_INFO(filename, __FUNCTION__, msg.str());
    } // end else
  } // end else

  return false;
} // end get_direct_mem_ptr

void
lt_dmi_target::toggle_dmi_method
(void
)
{
  std::ostringstream msg;
  msg.str("");
  m_toggle_count++;

  if (m_dmi_enabled)
  {
    m_dmi_enabled = false;

    msg << "Target: " << m_ID
        << " invalidate_direct_ptr "
        << "(" << m_start_address << ", " << m_end_address << ");";
    REPORT_INFO(filename, __FUNCTION__, msg.str());

    m_memory_socket->invalidate_direct_mem_ptr(m_start_address, m_end_address );

    next_trigger(m_start_dmi_delay);
  }
  else
  {
    m_dmi_enabled = true;
    next_trigger(m_dmi_duration);
    msg << "Target: " << m_ID
        << " DMI has been enabled in this target";
    REPORT_INFO(filename, __FUNCTION__, msg.str());
  }

  // Don't execute again
  if(m_toggle_count >= m_max_dmi_toggle_count)
  {
    next_trigger();
    m_dmi_enabled = false;
  }
}
