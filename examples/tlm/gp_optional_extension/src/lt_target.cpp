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
    Charles Wilson, ESLX
    Bill Bunton, ESLX
    
--------------------------------------------------------------------------------------- */

#include "lt_target.h"                                ///< our headers
#include "reporting.h"                                ///< reporting convenience macros
#include "tlm.h"                                      ///< TLM headers
                    
using namespace  std;

static const char *filename = "lt_target.cpp";        ///< filename

/*=============================================================================
  @fn lt_target::lt_target
  
  @brief lt_target constructor
  
  @details
    This routine initialized an lt_traget class instance.
    
  @param module_name          module name
  @param ID                   target ID
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
, const char                *memory_socket          ///< socket name
, sc_dt::uint64             base_address            ///< base address
, sc_dt::uint64             memory_size             ///< memory size (bytes)
, unsigned int              memory_width            ///< memory width (bytes)
, const sc_core::sc_time    accept_delay            ///< accept delay (SC_TIME)
, const sc_core::sc_time    read_response_delay     ///< read response delay (SC_TIME)
, const sc_core::sc_time    write_response_delay    ///< write response delay (SC_TIME)
)
: sc_module               (module_name)             ///< module name
, m_ID                    (ID)                      ///< target ID
, m_memory_socket         (memory_socket)           ///< socket name
, m_base_address          (base_address)            ///< base address
, m_memory_size           (memory_size)             ///< memory size (bytes)
, m_memory_width          (memory_width)            ///< memory width (bytes)
, m_accept_delay          (accept_delay)            ///< accept delay
, m_read_response_delay   (read_response_delay)     ///< read response delay
, m_write_response_delay  (write_response_delay)    ///< write response delay
{
  // Allocate an array for the target's memory
  m_memory = new unsigned char[size_t(m_memory_size)];

  // clear the memory
  memset(m_memory, 0, size_t(m_memory_size));
      
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
  
  @note this switches from annotated to synchronized timing every 20 time
  
=============================================================================*/
tlm::tlm_sync_enum                                  ///< synchronization state
lt_target::nb_transport                             ///< non-blocking transport
( tlm::tlm_generic_payload &gp                      ///< generic payoad pointer
, tlm::tlm_phase           &phase                   ///< transaction phase
, sc_core::sc_time         &delay_time)             ///< time it should take for transport
{
  std::ostringstream  msg;                          ///< log message
  tlm::tlm_sync_enum  return_status = tlm::TLM_COMPLETED;
  
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

        if (gp.get_command() == tlm::TLM_READ_COMMAND)
        {
          delay_time = delay_time + m_accept_delay + m_read_response_delay;
        }
        else if (gp.get_command() == tlm::TLM_WRITE_COMMAND)
        {
          delay_time = delay_time + m_accept_delay + m_write_response_delay;
        }
        else
        {
          msg.str ("");
          msg << m_ID << " - Invalid GP request";
          
          REPORT_FATAL(filename, __FUNCTION__, msg.str());
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
  tlm::tlm_generic_payload  *transaction_ptr;

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
( tlm::tlm_generic_payload   &payload               ///< debug payload
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
    This routine returns a pointer to the memory. Unimplemented.
    
  @param address  memory address reference
  @param mode     read/write mode
  @param data     data
  
  @retval bool success/failure
=============================================================================*/
bool                                                ///< success / failure
lt_target::get_direct_mem_ptr                       ///< get direct memory pointer
  (tlm::tlm_generic_payload   &payload,             ///< address + extensions
   tlm::tlm_dmi               &data                 ///< dmi data
  )
{
    return false;
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
lt_target::memory_operation                           ///< memory_operation
( tlm::tlm_generic_payload  &gp                       ///< generic payload
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
      msg.str ("");
      msg << m_ID << " - W -";
      msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) 
          << uppercase << hex << address;
      msg << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << length;
      msg << " D: 0x";
      
      for (unsigned int i = 0; i < length; i++)
      {
        msg << internal << setw( 2 ) << setfill( '0' ) << uppercase << hex << (int)data[i];
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

          m_memory[address++] = data[i];
        }
      }
      
      REPORT_INFO(filename, __FUNCTION__, msg.str());
      
      break;
    }

    case tlm::TLM_READ_COMMAND:
    {
      msg.str ("");
      msg << m_ID << " - R -";
      msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) 
          << uppercase << hex << address;
      msg << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << length;

      // clear read buffer
      memset(data, 0, length);

      // global -> local address
      address -= m_base_address;

      if  ( (address < 0)
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

          data[i] = m_memory[address++];
          
          msg << internal << setw( 2 ) << setfill( '0' ) << uppercase << hex << (int)data[i];
        }
      }

      REPORT_INFO(filename, __FUNCTION__, msg.str());
      
      break;
    }
  }

  gp.set_response_status(response);
  
  return;
}
