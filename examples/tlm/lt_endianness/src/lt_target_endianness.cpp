/*****************************************************************************
 *
 *   The following code is derived, directly or indirectly, from the SystemC
 *   source code Copyright (c) 1996-2008 by all Contributors.
 *   All Rights reserved.
 *
 *   The contents of this file are subject to the restrictions and limitations
 *   set forth in the SystemC Open Source License Version 3.0 (the "License");
 *   You may not use this file except in compliance with such restrictions and
 *   limitations. You may obtain instructions on how to receive a copy of the
 *   License at http://www.systemc.org/. Software distributed by Contributors
 *   under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 *   ANY KIND, either express or implied. See the License for the specific
 *   language governing rights and limitations under the License.
 *
 *   *****************************************************************************/

/* ---------------------------------------------------------------------------------------
 @file lt_target.cpp
 
 @brief LT target memory routines
 
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

#include "lt_target_endianness.h"                     ///< our headers
#include "reporting.h"                                ///< reporting headers
#include "lt_endian_structure.h"                      ///< LT endian structure
#include "get_host_endianness.h"                      ///< get_host_endianness helper routine
#include "tlm.h"                                      ///< TLM headers
                    
using namespace  std;

static const char *filename = "lt_target_endianness.cpp"; ///< filename for reporting

/*=============================================================================
  @fn lt_target::lt_target
  
  @brief lt_target constructor
  
  @details
    This routine initialized an lt_traget class instance.
    
  @param module_name          module name
  @param ID                   target ID
  @param endianness           target endianness
  @param memory_socket        socket name
  @param base_address         base address
  @param memory_size          memory size (bytes)
  @param memory_width         memory width (bytes)
  @param accept_delay         accept delay (SC_TIME)
  @param read_response_delay  read response delay (SC_TIME)
  @param write_response_delay write response delay (SC_TIME)
  
  @retval none
=============================================================================*/
lt_target::lt_target                                ///< constructor
( sc_core::sc_module_name module_name               ///< module name
, const unsigned int        ID                      ///< target ID
, const tlm::tlm_endianness endianness              ///< target endianness
, const char                *memory_socket          ///< socket name
, sc_dt::uint64             base_address            ///< base address
, sc_dt::uint64             memory_size             ///< memory size (bytes)
, unsigned int              memory_width            ///< memory width (bytes)
, const sc_core::sc_time    accept_delay            ///< accept delay (SC_TIME)
, const sc_core::sc_time    read_response_delay     ///< read response delay (SC_TIME)
, const sc_core::sc_time    write_response_delay    ///< write response delay (SC_TIME)
) : sc_module               (module_name)           ///< module name
  , m_ID                    (ID)                    ///< target ID
  , m_endianness            (endianness)            ///< target endianness
  , m_memory_socket         (memory_socket)         ///< socket name
  , m_base_address          (base_address)          ///< base address
  , m_memory_size           (memory_size)           ///< memory size (bytes)
  , m_memory_width          (memory_width)          ///< memory width (bytes)
  , m_accept_delay          (accept_delay)          ///< accept delay
  , m_read_response_delay   (read_response_delay)   ///< read response delay
  , m_write_response_delay  (write_response_delay)  ///< write response delay
  , m_DMI_granted           (false)                 ///< DMI pointer granted (bool)
{
  // get the host endianness
  m_host_endianness = get_host_endianness ();

  // Allocate an array for the target's memory
  m_memory = new unsigned char[size_t(m_memory_size)];

  // initialize the memory with a pattern
  for ( unsigned int i = 0; i < m_memory_size; i++ )
  {
    m_memory [ i ] = (unsigned char)(i % 256);
  }
      
  // Bind the socket's export to the interface
  m_memory_socket(*this);

  SC_METHOD(begin_response)

  sensitive << m_begin_response_event;
}

/*=============================================================================
  @fn lt_target::~lt_target
  
  @brief lt_target destructor
  
  @details
    This routine terminates an lt_target class instance.
    
  @param none
  
  @retval none
=============================================================================*/
lt_target::~lt_target                               ///< destructor
( void
)
{
  // Free up the target's memory array
  delete [] m_memory;
}

/*=============================================================================
  @fn lt_target::nb_transport
  
  @brief inbound non-blocking transport
  
  @details
    This routine handles inbound non-blocking transport requests.
    
  @param gp         generic payload reference
  @param phase      transaction phase reference
  @param delay_time transport delay
  
  @retval tlm::tlm_sync_enum synchronization state
  
  @note this switches from annotated to synchronized timing every 20th time
  
=============================================================================*/
tlm::tlm_sync_enum                                  ///< synchronization state
lt_target::nb_transport                             ///< non-blocking transport
( tlm::tlm_generic_payload &gp                      ///< generic payoad pointer
, tlm::tlm_phase           &phase                   ///< transaction phase
, sc_core::sc_time         &delay_time)             ///< time it should take for transport
{
  std::ostringstream  msg;                          ///< log message
  tlm::tlm_sync_enum  return_status = tlm::TLM_REJECTED;
  
  static unsigned long  request_count = 0;          ///< request counter
  
  switch (phase)
  {
    case tlm::BEGIN_REQ:
    {
      // see how we should handle this one
      if(request_count++ % 20)
      {
        // LT_Annotated
        msg.str ("");
        msg << m_ID << " * BEGIN_REQ LT_Annotated";
        
        REPORT_INFO(filename,  __FUNCTION__, msg.str());
        memory_operation(gp);

        phase = tlm::BEGIN_RESP;           // update phase 

        switch (gp.get_command())
        {
          case tlm::TLM_READ_COMMAND:
          {
            delay_time += m_accept_delay + m_read_response_delay;
            break;
          }
        
          case tlm::TLM_WRITE_COMMAND:
          {
            delay_time += m_accept_delay + m_write_response_delay;
            break;
          }
          
          default:
          {
            msg.str ("");
            msg << m_ID << " - Invalid GP request";
            
            REPORT_FATAL(filename, __FUNCTION__, msg.str());
            break;
          }
        }
        
        return_status = tlm::TLM_COMPLETED;
      }
      else
      {
        // LT_sync
        msg.str ("");
        msg << m_ID << " * BEGIN_REQ LT_Sync";
        
        REPORT_INFO(filename, __FUNCTION__, msg.str());
        
        if (m_response_queue.empty())
        {
          m_begin_response_event.notify(sc_core::SC_ZERO_TIME);
        }
        
        m_response_queue.push(&gp);
        
        return_status = tlm::TLM_ACCEPTED;
      }
      
      break;
    }

    case tlm::END_REQ:
    {
      msg.str ("");
      msg << m_ID << " - END_REQ invalid phase for Target";
      
      REPORT_FATAL(filename, __FUNCTION__, msg.str()); 
      break;
    }

    case tlm::BEGIN_RESP:
    {
      msg.str ("");
      msg << m_ID << " - BEGIN_RESP invalid phase for Target";
      
      REPORT_FATAL(filename, __FUNCTION__, msg.str()); 
      break;
    }

    case tlm::END_RESP:
    {
      msg.str ("");
      msg << m_ID << " - END_RESP invalid phase for LT Target";
      
      REPORT_FATAL(filename, __FUNCTION__, msg.str()); 
      break;
    }

    default:
    {
      msg.str ("");
      msg << m_ID << " - invalid phase for TLM2 GP";
      
      REPORT_FATAL(filename, __FUNCTION__, msg.str());
      break;
    }
  }
  return return_status;  
}

/*=============================================================================
  @fn lt_target::begin_response
  
  @brief response processing
  
  @details
    This routine handles response processing.
    
  @param none
  
  @retval none
=============================================================================*/
void
lt_target::begin_response                           ///< begin_response
( void
)
{
  static bool begin_response_queue_active  = false;

  std::ostringstream        msg;                    ///< log message
  tlm::tlm_generic_payload  *transaction_ptr;       ///< GP payload

  if (begin_response_queue_active)
  {
    msg.str ("");
    msg << m_ID << " ** BEGIN_RESP for queued response";
    
    REPORT_INFO(filename,  __FUNCTION__, msg.str());

    transaction_ptr = m_response_queue.front();
    m_response_queue.pop();

    memory_operation(*transaction_ptr);

    transaction_ptr->set_response_status(tlm::TLM_OK_RESPONSE);
    
    tlm::tlm_phase phase    = tlm::BEGIN_RESP; 
    sc_core::sc_time delay  = sc_core::SC_ZERO_TIME;

    // Response call to nb_transport and decode return status

    switch (m_memory_socket->nb_transport(*transaction_ptr, phase, delay))
    {
      case tlm::TLM_COMPLETED:  // no acction required operation is complete 
      {                       
        break;
      }

      case tlm::TLM_ACCEPTED:   
      case tlm::TLM_UPDATED:   
      case tlm::TLM_REJECTED:   
      default: 
      {
        msg.str ("");
        msg << m_ID << " - Invalid reponse for LT BEGIN_REPONSE";
        
        REPORT_FATAL(filename, __FUNCTION__, msg.str());      
        
        break;
      }
    }
  }

  // check queue for another transaction

  if (m_response_queue.empty())
  {
    begin_response_queue_active = false;
  }
  else
  {
    begin_response_queue_active = true;
    
    msg.str ("");
    msg << m_ID << " ** BEGIN_RESP start delay";
    
    REPORT_INFO(filename,  __FUNCTION__, msg.str());

    transaction_ptr = m_response_queue.front();

    if (transaction_ptr->get_command() == tlm::TLM_READ_COMMAND)
    {
      m_begin_response_event.notify (m_accept_delay + m_read_response_delay);
    }
    else if (transaction_ptr->get_command() == tlm::TLM_WRITE_COMMAND)
    {
      m_begin_response_event.notify (m_accept_delay + m_write_response_delay);
    }
    else
    {
      msg.str ("");
      msg << m_ID << " - Invalid GP Command";
    
      REPORT_FATAL(filename, __FUNCTION__, msg.str());
    }

    sc_core::next_trigger(m_begin_response_event);
    return;
  }
}

/*=============================================================================
  @fn lt_target::transport_dbg
  
  @brief transport debug routine
  
  @details
    This routine transport debugging. Unimplemented.
    
  @param payload  debug payload reference
  
  @retval result 0
=============================================================================*/
unsigned int                                        ///< result
lt_target::transport_dbg                            ///< transport debug
( tlm::tlm_debug_payload     &payload               ///< debug payload
)
{
  // No error needed, disabled
//REPORT_FATAL(filename, __FUNCTION__, "routine not implemented");
  return 0;
}

/*=============================================================================
  @fn lt_target::get_direct_mem_ptr
  
  @brief get pointer to memory
  
  @details
    This routine returns a pointer to the memory if the address is in half our
     address range. 
    
  @param address  memory address reference
  @param mode     read/write mode
  @param data     data
  
  @retval bool success/failure
  
  @note
    We must always set the start and end address according to 4.3.5.f
    
=============================================================================*/
bool                                                ///< success / failure
lt_target::get_direct_mem_ptr                       ///< get direct memory pointer
  (const sc_dt::uint64        &address              ///< address
  ,tlm::tlm_dmi_mode          &mode                 ///< dmi read/write mode
  ,tlm::tlm_dmi               &data                 ///< dmi data
  )
{
  std::ostringstream  msg;                          ///< log message
  bool                success       = false;        ///< return status
  
  const sc_dt::uint64 block_size    = m_memory_size / 2;              ///< DMI block size
  const sc_dt::uint64 address_start = block_size * (m_ID - 1);        ///< starting DMI address
  const sc_dt::uint64 address_end   = address_start + block_size - 1; ///< ending DMI address
  
  msg.str ("");
  msg << m_ID << " - DMI pointer request A: 0x"
      << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) 
      << uppercase << hex << address;
  
  REPORT_INFO ( filename, __FUNCTION__, msg.str() );
  
  // set the start and end addresses (TLM 2.0 s4.3.5.f)
  data.dmi_start_address  = address_start;
  data.dmi_end_address    = address_end;

  // range check the address
  if (    ( address < address_start )
      ||  ( address > address_end   ) )
  {
    msg.str ("");
    msg << m_ID << " - DMI pointer request out of range";
    
    REPORT_INFO ( filename, __FUNCTION__, msg.str() );
  }
  else
  {
    if ( m_DMI_granted )
    {
      // invalidate the pointer
      msg.str ("");
      msg << m_ID << " - DMI invalidating pointer";
      
      REPORT_INFO (filename, __FUNCTION__, msg.str());

      m_memory_socket->invalidate_direct_mem_ptr  ( address_start
                                                  , address_end);
      
      m_DMI_granted = false;
    }
    
    if ( m_DMI_granted )
    {
      // invalidate not acknowledged
      msg.str ("");
      msg << m_ID << " - DMI invalidate not acknowledged";
      
      REPORT_INFO (filename, __FUNCTION__, msg.str());
    }
    else
    {
      // fulfill the request
      
      msg.str ("");
      msg << m_ID << " - granting DMI pointer request";
      
      REPORT_INFO ( filename, __FUNCTION__, msg.str() );
  
      switch ( mode.type )
      {
        case tlm::tlm_dmi_mode::READ:
        case tlm::tlm_dmi_mode::WRITE:
        case tlm::tlm_dmi_mode::READ_WRITE:
        {
          // upgrade DMI access mode
          mode.type = tlm::tlm_dmi_mode::READ_WRITE;
        }
      }
      
      // fill out the remainder of the DMI data block
      data.dmi_ptr        = m_memory;
      data.read_latency   = m_accept_delay + m_read_response_delay;
      data.write_latency  = m_accept_delay + m_write_response_delay;
      data.endianness     = m_endianness;
      
      m_DMI_granted       = true;
      success             = true;
    }
  }
  
  return success;
}

/*=============================================================================
  @fn lt_target::memory_operation
  
  @brief read / write processing
  
  @details
    This routine implements the read / write operations.
    
  @param gp generic payload reference
  
  @retval none
=============================================================================*/
void 
lt_target::memory_operation                         ///< memory_operation
( tlm::tlm_generic_payload  &gp                     ///< generic payload
)    
{
  // Perform the requested operation
  // Access the required attributes from the payload

  sc_dt::uint64             address   = gp.get_address();     ///< memory address
  tlm::tlm_command          command   = gp.get_command();     ///< memory command
  unsigned char             *data     = gp.get_data_ptr();    ///< data pointer
  unsigned int              length    = gp.get_data_length(); ///< data length
  
  tlm::tlm_response_status  response  = tlm::TLM_OK_RESPONSE; ///< operation response
  std::ostringstream        msg;                              ///< log message
  transaction_structure     trans_data;                       ///< transaction data

  switch (command)
  {
    default:
    {
      msg.str ("");
      msg << m_ID << " - invalid command";
    
      REPORT_FATAL(filename, __FUNCTION__, msg.str());
      break;
    }

    case tlm::TLM_WRITE_COMMAND:
    {
      // copy the write buffer to the structure
      memcpy  ( trans_data.buffer, data, length );
      
      msg.str ("");
      msg << m_ID << " " << (tlm::hostHasLittleEndianness()? "L" : "B") << "E" << " - W -";
      msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) 
          << uppercase << hex << address;
      msg << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << length;
      msg << " D: 0x";
      
      for (unsigned int i = 0; i < length; i++)
      {
        msg << internal << setw( 2 ) << setfill( '0' ) << uppercase << hex << (unsigned int)trans_data.buffer[i];
      }
      
      REPORT_INFO(filename, __FUNCTION__, msg.str());

      // adjust data to target endianness
      if ( ! tlm::hasHostEndianness(m_endianness) )
      {
        // adjust the data
        
        trans_data.data.d_int64u = tlm::swap_bytes ( trans_data.data.d_int64u );
        trans_data.data.d_int32u = tlm::swap_bytes ( trans_data.data.d_int32u );
        trans_data.data.d_int16u = tlm::swap_bytes ( trans_data.data.d_int16u );
            
        msg.str ("");
        msg << m_ID << " - adjusting to target endianness";
        msg << " (" << ((m_endianness == tlm::TLM_LITTLE_ENDIAN) ? "L" : "B") << "E)";
        msg << " D: 0x";
         
        for (unsigned int i = 0; i < length; i++)
        {
          msg << internal << setw( 2 ) << setfill( '0' ) << uppercase << hex << (unsigned int)data[i];
        }
      }
      
      // global -> local address
      address -= m_base_address;

      if  (  (address < 0)
          || (address >= m_memory_size))
      {
        // address out-of-bounds
        msg << " address out-of-range";
        
        // set error response (9.9.d)
        response = tlm::TLM_ADDRESS_ERROR_RESPONSE;
      }
      else
      {
        for (unsigned int i = 0; i < length; i++)
        {
          if ( address >= m_memory_size )
          {
            msg << " address went out of bounds";
            
            // set error response (9.9.d)
            response = tlm::TLM_ADDRESS_ERROR_RESPONSE;
            
            break;
          }
          
          m_memory[address++] = trans_data.buffer[i];
        }
      }
      
      REPORT_INFO (filename, __FUNCTION__, msg.str());
      
      break;
    }

    case tlm::TLM_READ_COMMAND:
    {
      msg.str ("");
      msg << m_ID << " " << ((m_endianness == tlm::TLM_LITTLE_ENDIAN)? "L" : "B") << "E" << " - R -";
      msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) 
          << uppercase << hex << address;
      msg << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << length;

      // clear read buffer and structure
      memset(trans_data.buffer, 0, length);
      memset(data,              0, length);

      msg << " D: 0x";
      
      // global -> local address
      address -= m_base_address;
        
      if  (  (address < 0)
          || (address >= m_memory_size))
      {
        // address out-of-bounds
        msg << " address out-of-range, data zeroed";
        
        // set error response (9.9.d)
        response = tlm::TLM_ADDRESS_ERROR_RESPONSE;
      }
      else
      {
        msg << " D: 0x";
        for (unsigned int i = 0; i < length; i++)
        {
          if ( address >= m_memory_size )
          {
            // out-of-bounds reads
            msg << " address went out of bounds";
            
            // set error response (9.9.d)
            response = tlm::TLM_ADDRESS_ERROR_RESPONSE;
            
            break;
          }
          
          trans_data.buffer[i] = m_memory[address++];
          
          msg << internal << setw( 2 ) << setfill( '0' ) << uppercase << hex << (unsigned int)trans_data.buffer[i];
        }
      }

      REPORT_INFO(filename, __FUNCTION__, msg.str());
      
      // adjust data to target endianness
      if ( ! tlm::hasHostEndianness(m_endianness) )
      {
        // adjust the data
        
        trans_data.data.d_int64u = tlm::swap_bytes ( trans_data.data.d_int64u );
        trans_data.data.d_int32u = tlm::swap_bytes ( trans_data.data.d_int32u );
        trans_data.data.d_int16u = tlm::swap_bytes ( trans_data.data.d_int16u );
        
        msg.str ("");
        msg << m_ID << " - adjusting to host endianness";
        msg << " (" << (tlm::hostHasLittleEndianness() ? "L" : "B") << "E)";
        msg << " D: 0x";
        
        for (unsigned int i = 0; i < length; i++)
        {
          msg << internal << setw( 2 ) << setfill( '0' ) << uppercase << hex << (unsigned int)trans_data.buffer[i];
        }

        REPORT_INFO(filename, __FUNCTION__, msg.str());
      }
      
      // copy the structure to the read buffer
      memcpy ( data, trans_data.buffer, length );
      
      break;
    }
  }

  gp.set_response_status(response);
  
  return;
}
