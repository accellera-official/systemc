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
//=============================================================================
///  @file memory.cpp
//
///  @brief Implement memory functionality
//
///  @details
///     This class implements the memory functionality (read, write, etc.)
///     and is used by all of the targets in the examples
//
//==============================================================================
//
//  Original Authors:
//    Jack Donovan, ESLX
//
//==============================================================================

#include "reporting.h"                      // Reporting convenience macros
#include "memory.h"                         // Header for this class

using namespace sc_core;

static const char *filename = "memory.cpp"; ///< filename for reporting

/// Constructor

memory::memory
(
  const unsigned int ID                  // Target ID
, sc_core::sc_time   read_delay          // read delay
, sc_core::sc_time   write_delay         // write delay
, sc_dt::uint64      memory_size         // memory size (bytes)
, unsigned int       memory_width        // memory width (bytes)
)
: m_ID              (ID)
, m_read_delay      (read_delay)
, m_write_delay     (write_delay)
, m_memory_size     (memory_size)
, m_memory_width    (memory_width)
, m_previous_warning(false)
{
/// Allocate and initalize an array for the target's memory
  m_memory = new unsigned char[size_t(m_memory_size)];

/// clear memory
  memset(m_memory, 0, size_t(m_memory_size));

  sc_assert(m_memory_width > 0);
  sc_assert(m_memory_size % m_memory_width == 0);

  std::ostringstream  msg;
  msg.str("");
  if ( m_memory_width > m_memory_size )
  {
    msg << "Target: " << m_ID
        <<" memory width is bigger than memory size";
    REPORT_FATAL(filename, __FUNCTION__, msg.str());
  }
} // end Constructor

//==============================================================================
///  @fn memory::operation
//
///  @brief performs read and write
//
///  @details
///    This routine implements the read and  write operations. including
///    checking for byte_enable and streaming that are not implemented
//
//==============================================================================
void
memory::operation
( tlm::tlm_generic_payload  &gp
, sc_core::sc_time          &delay_time   ///< transaction delay
)
{
  /// Access the required attributes from the payload
  sc_dt::uint64    address   = gp.get_address();     // memory address
  tlm::tlm_command command   = gp.get_command();     // memory command
  unsigned char    *data     = gp.get_data_ptr();    // data pointer
  unsigned  int     length   = gp.get_data_length(); // data length

  std::ostringstream  msg;
  msg.str("");
  tlm::tlm_response_status response_status = check_address(gp);

  if (gp.get_byte_enable_ptr())
  {
    gp.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
  }
  else if (gp.get_streaming_width() != gp.get_data_length())
  {
    gp.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
  }

  switch (command)
  {
    default:
    {
      if (m_previous_warning == false)
      {
        msg << "Target: " << m_ID
            << " Unsupported Command Extension";
        REPORT_INFO(filename, __FUNCTION__, msg.str());
        gp.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
        delay_time = sc_core::SC_ZERO_TIME;
        m_previous_warning = true;
      }
      break;
    }

    /// Setup a TLM_WRITE_COMMAND Informational Message and Write the Data from
    /// the Generic Payload Data pointer to Memory
    ///
    case tlm::TLM_WRITE_COMMAND:
    {
      if (response_status == tlm::TLM_OK_RESPONSE)
        {
          for (unsigned int i = 0; i < length; i++)
          {
            m_memory[address++] = data[i];     // move the data to memory
          }
          delay_time = delay_time + m_write_delay;
          report::print(m_ID, gp, filename);
        }
      break;
    }

    case tlm::TLM_READ_COMMAND:
    {
      if (response_status == tlm::TLM_OK_RESPONSE)
      {
        for (unsigned int i = 0; i < length; i++)
        {
          data[i] = m_memory[address++];         // move the data to memory
        }
        delay_time = delay_time + m_read_delay;
        report::print(m_ID, gp, filename);
      }
      break;
    }
  } // end switch

  gp.set_response_status(response_status);

  return;
} // end memory_operation

//==============================================================================
///  @fn memory::get_mem_ptr
//
///  @brief Method to return pointer to memory in this object
//
///  @details
///    This routine used during dmi example
//
//==============================================================================
unsigned char*
memory::get_mem_ptr(void)
{
  return m_memory;
}

//==============================================================================
///  @fn memory::get_delay
//
///  @brief Method to "pull" appropriate delay from the gp
//
///  @details
///    This routine used during several at examples
//
//==============================================================================
void
memory::get_delay
( tlm::tlm_generic_payload  &gp           ///< TLM2 GP reference
, sc_core::sc_time          &delay_time   ///< time to be updated
)
{
  /// Access the required attributes from the payload
  tlm::tlm_command command   = gp.get_command();     // memory command

  std::ostringstream  msg;
  msg.str("");

  switch (command)
  {
    default:
    {
      if (m_previous_warning == false)
      {
        msg << "Target: " << m_ID
            << " Unsupport GP command extension";
        REPORT_WARNING(filename, __FUNCTION__, msg.str());
        m_previous_warning = true;
      }
      break;
    }

    /// Setup a TLM_WRITE_COMMAND Informational Message and Write the Data from
    /// the Generic Payload Data pointer to Memory
    ///
    case tlm::TLM_WRITE_COMMAND:
    {
      delay_time = delay_time + m_write_delay;
      break;
    }

    case tlm::TLM_READ_COMMAND:
    {
      delay_time = delay_time + m_read_delay;
      break;
    }
  } // end switch
  return;
}

//==============================================================================
///  @fn memory::check_address
//
///  @brief Method to check if the gp is in the address range of this memory
//
///  @details
///    This routine used to check for errors in address space
//
//==============================================================================
tlm::tlm_response_status
memory::check_address
( tlm::tlm_generic_payload  &gp
)
{
  sc_dt::uint64    address   = gp.get_address();     // memory address
  unsigned  int     length   = gp.get_data_length(); // data length

  std::ostringstream  msg;
  msg.str("");

  if ( address >= m_memory_size )
  {
    msg << "Target: " << m_ID
        <<" address out-of-range";
    REPORT_WARNING(filename, __FUNCTION__, msg.str());

    return tlm::TLM_ADDRESS_ERROR_RESPONSE; // operation response
  }
  else
  {
    if ( (address + length) >= m_memory_size )
    {
      msg << "Target: " << m_ID
          << " address will go out of bounds";
      REPORT_WARNING(filename, __FUNCTION__, msg.str());

      return tlm::TLM_ADDRESS_ERROR_RESPONSE; // operation response
    }

  return tlm::TLM_OK_RESPONSE;
  }
} // end check address
