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
/*******************************************************************************
  @file
   at_target_memory.cpp
   @Details
    This module implements a TLM2 Approximately Timed target. This module can
    be configured as:
    1. AT target with Timing Annotation; uses 2 phases:
       - begin request
       - begin respond
       with timing annotation
    2. AT target with full phases; uses 4 phases: 
       - begin request
       - end request
       - begin response
       - end response
       where timing is explicit

 ******************************************************************************/

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Anna Keist, ESLX
*****************************************************************************/

// Note: includes are order dependent

#include "tlm.h"                            ///< TLM headers
#include "at_target_memory.h"               ///< our class header
#include "reporting.h"                      ///< Reporting convenience macros

using namespace std;                        ///< allows access to std entities 
using namespace sc_core;                    ///< allows access to sc_core entities

static const char *filename = "at_target_memory.cpp"; // used for reporting

/*==============================================================================
  @fn at_target_memory

  @brief class constructor

  @details
    This is the class constructor.
    1. memory is initialized to 0

  @param module_name module name
  @param socket name
  @param base_address memory's base address
  @param memory_size memory's size in bytes
  @param memory_witch memory's width in bytes
  @param clock_period memory's command delay
  @param read_clocks memory's read delay
  @param write_clocks memory's write delay
  @param refresh_clocks memory's refresh delay
  @param refresh_rate memory's refresh frequency
  @param model_style (2 or 4 phase)

  @retval void

  @note

==============================================================================*/

at_target_memory::at_target_memory
( sc_core::sc_module_name module_name                 ///< module name
, const unsigned int            ID                    ///< target ID
, const char                	  *memory_socket        ///< socket name
, sc_dt::uint64             	  base_address          ///< base address
, sc_dt::uint64             	  memory_size           ///< memory size (bytes)
, unsigned int              	  memory_width          ///< memory width (bytes)
, const sc_core::sc_time    	  accept_delay          ///< accept delay
, const sc_core::sc_time    	  read_response_delay   ///< read response delay
, const sc_core::sc_time    	  write_response_delay  ///< write response delay
, const e_AT_target_model_style model_style           ///< model style (2 or 4 phase)
) : sc_module              	(module_name)             ///< initializing module name
  , m_ID                    (ID)                      ///< initializing target ID
  , m_memory_socket        	(memory_socket)           ///< initializing memory socket
  , m_base_address         	(base_address)            ///< initializing target base address
  , m_memory_size          	(memory_size)             ///< initializing target memory size
  , m_memory_width         	(memory_width)            ///< initializing target memory width
  , m_accept_delay         	(accept_delay)            ///< initializing accept delay
  , m_read_response_delay  	(read_response_delay)     ///< initializing read response delay
  , m_write_response_delay 	(write_response_delay)    ///< initializing write response delay
  , m_model_style          	(model_style)             ///< initializing the target model style
  , begin_response_QActive  (false)                   ///< initializing begin response queue state
  , end_request_QActive     (false)                   ///< initializing end request queue state                   
  {
    // Allocate an array for the target's memory
    m_memory = new unsigned char[size_t(m_memory_size)];
     
    // Bind the socket's export to the interface
    m_memory_socket(*this);

    SC_METHOD(end_request_method)

    SC_METHOD(begin_response_method)
  }

/*=============================================================================
  @fn at_target_memory::~at_target_memory
  
  @brief at_target_memory destructor
  
  @details
    This routine terminates an at_target_memory class instance.
    
  @param none
  
  @retval none
=============================================================================*/
at_target_memory::~at_target_memory(void)      ///< destructor
{
  // Free up the target's memory array
  delete [] m_memory;
}

/*=============================================================================
  @fn at_target_memory::nb_transport
  
  @brief inbound non-blocking transport
  
  @details
    This routine handles inbound non-blocking transport requests.
    
  @param gp         generic payload reference
  @param phase      transaction phase reference
  @param delay_time transport delay
  
  @retval tlm::tlm_sync_enum synchronization state
=============================================================================*/
tlm::tlm_sync_enum                            ///< synchronization state
at_target_memory::nb_transport                ///< non-blocking transport
( tlm::tlm_generic_payload& transaction_ref   ///< generic payoad pointer
, tlm::tlm_phase&           phase             ///< transaction phase
, sc_time&                  delay)            ///< time it should take for transport
{
  std::ostringstream       msg;               // log message
  tlm::tlm_sync_enum       return_status = tlm::TLM_REJECTED;

  // non-supported feature check
  // return error for byte enable; this target does not support
  if (transaction_ref.get_byte_enable_ptr()){
    transaction_ref.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
  }
  else if (transaction_ref.get_streaming_width()) {
    transaction_ref.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
  }
  else {
    switch (phase) 
    {
      case tlm::BEGIN_REQ: 
      {
        if (m_model_style == AT_2_Phase_Annotated_e)      // ## AT_2_Phase_Annotated
        { 
          msg.str ("");
    	      msg << m_ID << " - ** BEGIN_REQ AT 2 Phase Annotated";
          REPORT_INFO(filename,  __FUNCTION__, msg.str());

          if (m_response_queue.empty())
          {
            m_begin_response_event.notify(SC_ZERO_TIME);
          }
          m_response_queue.push(&transaction_ref);

          phase       = tlm::END_REQ;   // update phase and time  
          delay       = m_accept_delay;  
          return_status =  tlm::TLM_UPDATED;
        }
        else 
        {
          msg.str ("");
          msg << m_ID << " - ** BEGIN_REQ AT 4 Phase";
          REPORT_INFO(filename,  __FUNCTION__, msg.str());

          if (m_end_request_queue.empty()){
            m_end_request_event.notify(SC_ZERO_TIME);
          }
          m_end_request_queue.push(&transaction_ref);

          return_status = tlm::TLM_ACCEPTED;
        }
        break;
      }

      // declare a transaction pointer to store the value we get back from queue
      tlm::tlm_generic_payload *transaction_ptr;  
      case tlm::END_RESP: 
      {
        if (m_model_style == AT_4_Phase_e)
        {
          transaction_ptr = m_end_response_queue.front();
          if (transaction_ptr != &transaction_ref)
          {    
            msg.str ("");
    	        msg << m_ID << " - Unexpected END_RESP";
            REPORT_FATAL(filename,  __FUNCTION__, msg.str());
          }
          m_end_response_queue.pop();
          m_end_response_event.notify(SC_ZERO_TIME);
          msg.str ("");
    	      msg << m_ID << " - ** END_RESP received AT 4 Phase ";
          REPORT_INFO(filename, __FUNCTION__, msg.str());
          return_status = tlm::TLM_COMPLETED;
        }
        else 
        {
          msg.str ("");
    	      msg << m_ID << " - END_RESP is invalid";
          REPORT_FATAL(filename, __FUNCTION__, msg.str());
          return_status = tlm::TLM_REJECTED; 
        }
        break;
      }

      case tlm::BEGIN_RESP: 
      {
        msg.str ("");
        msg << m_ID << " - ** BEGIN_RESP is invalid phase for Target";
        REPORT_FATAL(filename, __FUNCTION__, msg.str());
        return_status = tlm::TLM_REJECTED; 
        break;
      }

      case tlm::END_REQ: 
      {
        msg.str ("");
        msg << m_ID << " - ** END_REQ is invalid phase for Target";
        REPORT_FATAL(filename, __FUNCTION__, msg.str());
        return_status = tlm::TLM_REJECTED; 
        break;
      }

      default: 
      {
        msg.str ("");
        msg << m_ID << " - invalid phase for TLM2 GP";
        REPORT_FATAL(filename, __FUNCTION__, msg.str());
        return_status = tlm::TLM_REJECTED; 
        break;
      }
    } //  end phase switch 
  } // end if
  return return_status;
} // end inboud (forwared) nb_transport 


/*=============================================================================
  @fn at_target_memory::end_request_method
  
  @brief end request processing
  
  @details
    This routine handles end requests phase. The method checks whether there
    are items in the end request queue.  If so, send a delayed notification 
    to start the end request phase.  After the delay, method wakes up again
    to send the end request.
    
  @param none
  
  @retval none
=============================================================================*/
void at_target_memory::end_request_method(void)  ///< end request processing
{
  std::ostringstream       msg;                  ///< log message
  tlm::tlm_generic_payload*  transaction_ptr;    ///< transaction pointer

  if (end_request_QActive)
  {
    transaction_ptr = m_end_request_queue.front();
    m_end_request_queue.pop();

    if (m_response_queue.empty())                // start response queue
    {
      m_begin_response_event.notify(SC_ZERO_TIME);
    }
    m_response_queue.push(transaction_ptr);      // move to response queue

    tlm::tlm_phase phase  = tlm::END_REQ; 
    sc_time delay         = SC_ZERO_TIME;

    switch (m_memory_socket->nb_transport(*transaction_ptr, phase, delay)) 
    {
      case tlm::TLM_ACCEPTED: 
      { 
        break;
      }
      default: 
      {
        msg.str ("");
	      msg << m_ID << " - invalid response for END_REQ";
        REPORT_FATAL(filename,  __FUNCTION__, msg.str());
        break;
      }
    } // end switch 
  } // end if
 
  if (m_end_request_queue.empty())
  {
    end_request_QActive = false;
  }
  else 
  {
    end_request_QActive = true;
    m_end_request_event.notify (m_accept_delay);
  }

  next_trigger(m_end_request_event);
  return;
}


/*=============================================================================
  @fn at_target_memory::begin_response
  
  @brief response processing
  
  @details
    This routine handles response processing.  The method checks whether there
    are items in the response queue.  If so, send a delayed notification (read/
    write delay) to start the begin response phase.  After the delay, method 
    wakes up again to send the begin response.
   
  @param none
  
  @retval none
=============================================================================*/
void at_target_memory::begin_response_method(void)
{
  std::ostringstream         msg;               // log message
  tlm::tlm_generic_payload*  transaction_ptr;

  if (begin_response_QActive)
  { 
    msg.str ("");
    msg << m_ID << " - ** BEGIN_RESP for queued response";
    REPORT_INFO(filename,  __FUNCTION__, msg.str());

    transaction_ptr = m_response_queue.front();
    m_response_queue.pop();

    memory_operation(*transaction_ptr);

    transaction_ptr->set_response_status(tlm::TLM_OK_RESPONSE);
    tlm::tlm_phase phase        = tlm::BEGIN_RESP; 
    sc_time end_response_delay  = SC_ZERO_TIME;

    // call begin response and then decode return status
    switch (m_memory_socket->nb_transport(*transaction_ptr, phase, end_response_delay))
    {
      case tlm::TLM_COMPLETED:            // model_style is AT_2_Phase_Annotated
      {
        begin_response_QActive = false;
        next_trigger(end_response_delay);
        break;
      }

      case tlm::TLM_ACCEPTED:             // model_style is AT_4_Phase
      {
        m_end_response_queue.push(transaction_ptr);
        begin_response_QActive = false;
        next_trigger(m_end_response_event);
        break;
      }

      case tlm::TLM_UPDATED:   
      case tlm::TLM_REJECTED:   
      default: 
      {
        msg.str ("");
	      msg << m_ID << " - Invalid response for BEGIN_RESPONSE";
        REPORT_FATAL(filename,  __FUNCTION__, msg.str());
        break;
      }
    } // end switch 
  } // end if begin_response_QActive


  // check queue for another transaction
  if (m_response_queue.empty())
  {
    begin_response_QActive = false;
    next_trigger(m_begin_response_event);
  }
  else 
  {
    begin_response_QActive = true;
    msg.str ("");
    msg << m_ID << " - ** start BEGIN_RESP delay";
    REPORT_INFO(filename,  __FUNCTION__, msg.str());

    transaction_ptr = m_response_queue.front();

    if (transaction_ptr->get_command() == tlm::TLM_READ_COMMAND) 
    {
      m_begin_response_event.notify (m_read_response_delay);
      next_trigger(m_begin_response_event);
    }
    else if (transaction_ptr->get_command() == tlm::TLM_WRITE_COMMAND) 
    {
      m_begin_response_event.notify (m_write_response_delay);
      next_trigger(m_begin_response_event);
    }
    else 
    {
      msg.str ("");
      msg << m_ID << " - Invalid GP Command";
      REPORT_FATAL(filename, __FUNCTION__, msg.str());
    }
    return;
  }
} // end Begin_Response_method

/*==============================================================================
  @fn at_target_memory::transport_dbg
  
  @brief transport debug routine
  
  @details
    This routine transport debugging. Unimplemented.
    
  @param payload  debug payload reference
  
  @retval result 0
==============================================================================*/

unsigned int                                ///< result
at_target_memory::transport_dbg             ///< transport debug
( tlm::tlm_debug_payload     &payload       ///< debug payload
)
{
  return 0;
} // end transport_dbg

/*==============================================================================
  @fn lat_target_memory::get_direct_mem_ptr
  
  @brief get pointer to memory
  
  @details
    This routine returns a pointer to the memory. Unimplemented.
    
  @param address  memory address reference
  @param mode     read/write mode
  @param data     data
  
  @retval bool success/failure
==============================================================================*/
bool                                          ///< success / failure
at_target_memory::get_direct_mem_ptr          ///< get direct memory pointer
(const sc_dt::uint64        &address          ///< address
,tlm::tlm_dmi_mode          &dmi_mode         ///< dmi read/write mode
,tlm::tlm_dmi               &dmi_data         ///< dmi data
)
{
    return false;
} // end get_direct_mem_ptr

/*==============================================================================
  @fn at_target_memory::memory_operation
  
  @brief memory read and write
  
  @details
    This routine handles the actual array access for memory read and write.
    
  @param gp generic payload pointer
  
  @retval bool success/failure
==============================================================================*/
void at_target_memory::memory_operation
( tlm::tlm_generic_payload  &transaction_ref  
)    
{
  std::ostringstream       msg;               // log message

  // Perform the requested operation
  // Access the required attributes from the payload
  sc_dt::uint64      address = transaction_ref.get_address();    ///< memory address
  tlm::tlm_command   command = transaction_ref.get_command();    ///< memory command
  unsigned char      *data   = transaction_ref.get_data_ptr();   ///< data pointer
  int                length  = transaction_ref.get_data_length();///< data length

  switch (command)
  {
    default:
    {
      msg.str("");
      msg << m_ID << " - invalid command";
      REPORT_FATAL(filename, __FUNCTION__, msg.str());
      break;
    }
      
    case tlm::TLM_WRITE_COMMAND:
    {
      msg << endl;
      msg << "      W -";
      msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) 
          << uppercase << hex << address;
      msg << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << length;
      msg << " D: 0x";
      for (int i = 0; i < length; i++)
      {
        msg << internal << setw( 2 ) << setfill( '0' ) << uppercase << hex << (int)data[i];
      }
      REPORT_INFO(filename, __FUNCTION__, msg.str());
          
      // global -> local address
      address -= m_base_address;

      if  (  (address < m_base_address)
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
      msg << "R -";
      msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) 
          << uppercase << hex << address;
      msg << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << length;

      // global -> local address
      address -= m_base_address;

      // clear read buffer
      memset(data, 0, length);
        
      if  (  (address < m_base_address)
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

  transaction_ref.set_response_status(tlm::TLM_OK_RESPONSE);
  return;

} // end memory_operation


