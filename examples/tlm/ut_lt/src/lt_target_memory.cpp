/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
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
 @file lt_target_memory.cpp
 
 @brief lt_target_memory class
 
  Original Authors:
    Charles Wilson, ESLX
    Bill Bunton, ESLX
    
--------------------------------------------------------------------------------------- */

#include "tlm.h"                                      ///< TLM headers
#include "lt_target_memory.h"                         ///< our class header
#include "reporting.h"                                ///< Reporting convenience macros

using namespace  std;

static const char *filename = "lt_target_memory";     ///< filename

/*==============================================================================
  @fn lt_target_memory::lt_target_memory
  
  @brief class constructor
  
  @details
    This is the class constructor.
    
  @param module_name    module name
  @param ID             initiator ID
  @param base_address   memory's base address
  @param memory_size    memory's size in bytes
  @param memory_witch   memory's width in bytes
  @param clock_period   memory's command delay
  @param read_clocks    memory's read delay
  @param write_clocks   memory's write delay
  @param refresh_clocks memory's refresh delay
  @param refresh_rate   memory's refresh frequency
  
  @retval void
  
  @note
    memory is initialized to 0
    
==============================================================================*/

lt_target_memory::lt_target_memory                    ///< constructor
(  sc_core::sc_module_name  module_name               ///< module name
, const unsigned int        ID                        ///< initiator ID
, const sc_dt::uint64       base_address              ///< memory base address 64-bit
, const sc_dt::uint64       memory_size               ///< memory size 64-bit
, const unsigned int        memory_width              ///< memory width (bytes)(2, 4, 8, 16)
, const sc_core::sc_time    clock_period              ///< clock period for delays
, const unsigned int        read_clocks               ///< number of clocks for read
, const unsigned int        write_clocks              ///< number of clocks for write
, const unsigned int        refresh_clocks            ///< number of clocks for refresh
, const unsigned int        refresh_rate              ///< frequency of refresh
)
: sc_module       (module_name)                       ///< module name
, m_memory_socket ("memory_socket")                   ///< socket
, m_ID            (ID)                                ///< target ID
, m_base_address  (base_address)                      ///< base address
, m_memory_size   (memory_size)                       ///< memory size
, m_memory_width  (memory_width)                      ///< memory width
, m_read_delay    (clock_period * read_clocks)        ///< read delay
, m_write_delay   (clock_period * write_clocks)       ///< write delay
, m_refresh_delay (clock_period * read_clocks)        ///< refresh delay
, m_refresh_rate  (clock_period * refresh_rate)       ///< refresh rate
{
  // Allocate an array for the target's memory
   m_memory = new unsigned char[size_t(memory_size)];
  
  // clear the memory
  memset(m_memory, 0, memory_width * size_t(memory_size));
  
  // Bind the socket's export to the interface
  m_memory_socket(*this);
}
     
/*==============================================================================
  @fn lt_target_memory::~lt_target_memory
  
  @brief class destructor
  
  @details
    This routine handles the class cleanup.
    
  @param void
  
  @retval void
==============================================================================*/

lt_target_memory::~lt_target_memory(void)      ///< destructor
{
  // Free up the target's memory array
  delete [] m_memory;
}

/*==============================================================================
  @fn lt_target_memory::nb_transport
  
  @brief non-blocking transport
  
  @param gp         generic payload pointer
  @param phase      transaction phase
  @param delay_time event processing time
  
  @retval tlm_sync_enum synchronization state
  
  @note
    1. write overruns are ignored silently
    2. reads are preinitialized to 0
    3. read overruns are ignored
    
==============================================================================*/
tlm::tlm_sync_enum                                    ///< synchronization state
lt_target_memory::nb_transport                        ///< non-blocking transport
( tlm::tlm_generic_payload &gp                        ///< generic payoad pointer
, tlm::tlm_phase           &phase                     ///< transaction phase
, sc_core::sc_time         &delay_time                ///< time it should take for transport
)
{
  // Access the required attributes from the payload
  
  sc_dt::uint64      address = gp.get_address();      ///< memory address
  tlm::tlm_command   command = gp.get_command();      ///< memory command
  unsigned char      *data   = gp.get_data_ptr();     ///< data pointer
  int                length  = gp.get_data_length();  ///< data length
  tlm::tlm_sync_enum result  = tlm::TLM_COMPLETED;    ///< routine result (always TLM_COMPLETED)
  std::ostringstream msg;                             ///< log message

  // Error checking
  if (phase != tlm::BEGIN_REQ) {
    REPORT_FATAL(filename, __FUNCTION__, "phase not BEGIN_REQ");
  }

  // Perform the requested operation
  switch (command)
  {
    default:
    {
      REPORT_FATAL(filename, __FUNCTION__, "invalid command");
      break;
    }
      
    case tlm::TLM_WRITE_COMMAND:
    {
      msg << "W - " << m_ID << " -";
      msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) << uppercase << hex << address;
      msg << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << length;
      msg << " D: 0x";
      for (int i = 0; i < length; i++)
      {
        msg << internal << setw( 2 ) << setfill( '0' ) << uppercase << hex << (int)data[i];
      }
      REPORT_INFO(filename, __FUNCTION__, msg.str());
      
      delay_time += m_write_delay;
      
      // global -> local address
      address -= m_base_address;

      if  (  (address < 0)
          || (address >= m_memory_size))
      {
        // ignore out-of-bounds writes
      }
      else
      {
        for (int i = 0; i < length; i++)
        {
          if ( address >= m_memory_size )
          {
            break;
          }
          
          m_memory[address++] = data[i];
        }
      }
      break;
    }
    
    case tlm::TLM_READ_COMMAND:
    {
      msg << "R - " << m_ID << " -";
      msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) << uppercase << hex << address;
      msg << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << length;

      // clear read buffer
      memset(data, 0, length);

      delay_time += m_read_delay;

      // global -> local address
      address -= m_base_address;
        
      if  (  (address < 0)
          || (address >= m_memory_size))
      {
        // out-of-bounds read
        msg << " address out-of-range, data zeroed";
      }
      else
      {
        msg << " D: 0x";
        for (int i = 0; i < length; i++)
        {
          if ( address >= m_memory_size )
          {
            // ignore out-of-bounds reads
            break;
          }
          
          data[i] = m_memory[address++];
          msg << internal << setw( 2 ) << setfill( '0' ) << uppercase << hex << (int)data[i];
        }
      }

      REPORT_INFO(filename, __FUNCTION__, msg.str());
      break;
    }
  }

  // Set parameters to indicate all is well
  phase = tlm::BEGIN_RESP;
  gp.set_response_status(tlm::TLM_OK_RESPONSE);
  return result; 
}

/*==============================================================================
  @fn lt_target_memory::transport_dbg
  
  @brief transport debug
  
  @param payload  debug payload
  
  @retval unsigned int  byte count
  
  @note
    not implemented
    
==============================================================================*/
unsigned int                                          ///< result
lt_target_memory::transport_dbg                       ///< transport debug
(  tlm::tlm_debug_payload     &payload                ///< debug payload
)
{
  return 0;
}

/*==============================================================================
  @fn lt_target_memory::get_direct_mem_ptr
  
  @brief get direct memory pointer
  
  @param address  memory address
  @param dmi_mode read/write flag
  @param dmi_data data reference
  
  @retval bool success / failure
  
  @note
    not implemented
    
==============================================================================*/
bool                                                  ///< success / failure
lt_target_memory::get_direct_mem_ptr                  ///< get direct memory pointer
( const sc_dt::uint64        &address                 ///< address
, tlm::tlm_dmi_mode          &dmi_mode                ///< dmi read/write mode
, tlm::tlm_dmi               &dmi_data                ///< dmi data
)
{
    return false;
}
