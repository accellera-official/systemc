/*****************************************************************************

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

 *****************************************************************************/
/*******************************************************************************
  @file at_initiator.cpp
  @Details
   This module implements three interfaces. The first implements the functionality
   required to access the tlm_master_socket. The second and third interfaces 
   implements the functionality required to allow other modules to pass generic 
   payloads via an sc_fifo.
  
   This AT initiator is capable of interfacing with either LT or AT targets:
     1. LT - with timing annotation
     2. LT - with sync
     3. AT - with timing annotation (2 phase)
     4. AT - full 4 phases
   
   An LT with timing annotation requires a single nb_transport call and process
     the return status and phase from the target.  

 ******************************************************************************/

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
    Anna Keist, ESLX
*****************************************************************************/

// Note: order of headers is important
#include "tlm.h"                              // TLM headers
#include "reporting.h"                        // Reporting convenience macros
#include "at_initiator.h"                     // Our header

using namespace sc_core;

const char *filename = "at_initiator.cpp";

/*==============================================================================
  @fn at_initiator::at_initiator

  @brief class constructor

  @details
    This is the class constructor.

  @param name module name
  @param ID   initiator ID

  @retval void

==============================================================================*/
at_initiator::at_initiator                    ///< constructor
( sc_module_name name                         ///< module name
, const unsigned int ID                       ///< initiator ID
, sc_core::sc_time end_rsp_delay              ///< delay
)
: sc_module       (name)                      ///< initialize module name
, initiator_socket("initiator_socket")        ///< initiator socket
, m_ID            (ID)                        ///< initialize initiator ID
, m_end_rsp_delay (end_rsp_delay)             ///< initialize delay
, detect_2_phase_target(false)                ///< initiatlize AT target detection
{ 
  // bind initiator to the export
  initiator_socket (*this);                     

  // register thread process
  SC_THREAD(initiator_thread);                  

  // register method process
  SC_METHOD(m_send_end_rsp_method)              
    sensitive << m_send_end_rsp_event;
}

/*==============================================================================
  @fn at_initiator::~at_initiator

  @brief class destructor

  @details
    This routine handles the class cleanup.

  @param void

  @retval void
==============================================================================*/
at_initiator::~at_initiator(void) {}            ///< destructor

/*==============================================================================
  @fn at_initiator::initiator_thread

  @brief initiates non-blocking transport

  @details
    Initiator thread (TLM 2 Transport forwards  (outbound))
    Takes requests from input request queue and initiates a TLM2 non-blocking
    transfer via the initiator socket.  Process the return status from target
    and determine target type:
    1. LT - with timing annotation
    2. LT - with sync
    3. AT - with timing annotation (2 phase)
    4. AT - full 4 phases

  @param void

  @retval void

  @note

==============================================================================*/
void at_initiator::initiator_thread(void)     ///< initiator thread
{  
  tlm::tlm_generic_payload *transaction_ptr;  ///< transaction pointer
  std::ostringstream       msg;               ///< log message

  while (true) 
  {
    // Get Transaction GP from request in FIFO 
    transaction_ptr = request_in_port->read();  // get request from input fifo 

    // Setup to make request 
    tlm::tlm_phase phase  = tlm::BEGIN_REQ;     // Create phase objects 
    sc_time delay         = SC_ZERO_TIME;       // Create delay objects 
    
    msg.str ("");
    msg << m_ID << " "
        << ((transaction_ptr->get_command () == tlm::TLM_READ_COMMAND) ? "R" : "W")
        << " A: 0x" << internal << setw( sizeof(transaction_ptr->get_address ()) * 2 ) << setfill( '0' ) 
        << uppercase << hex << transaction_ptr->get_address ()
        << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << transaction_ptr->get_data_length ();

    REPORT_INFO (filename, __FUNCTION__, msg.str() );

    // Make the non-blocking call and decode returned status (tlm_sync_enum) 
    switch (initiator_socket->nb_transport(*transaction_ptr, phase, delay)) 
    {
      // LT taget response complete with annonated delay  
      case tlm::TLM_COMPLETED: 
      {
        wait(delay);  
        response_out_port->write(transaction_ptr);
        msg.str ("");
        msg << m_ID << " - Completed LT annotated request";
        REPORT_INFO (filename, __FUNCTION__, msg.str() );
        break;
      }

      // LT or AT target accepted request 
      // next phase could be AT 4 phase END_REQ or LT BEGIN_RESPONSE
      case tlm::TLM_ACCEPTED:
      {
        m_req_accepted_queue.push(transaction_ptr); 
        wait (m_req_accepted_event);        // AT must wait for END_REQ 
        msg.str ("");
        msg << m_ID << " - Target accepted LT or AT request";
        REPORT_INFO (filename, __FUNCTION__, msg.str() );
        break;
      }

      // Only AT 2 phase targets return update state decode phase
      case tlm::TLM_UPDATED: 
      {
        switch (phase)               // decide returned phase 
        {   
          case tlm::END_REQ:         // AT 2 phase
          {
            msg.str ("");
            msg << m_ID << " - * END_REQ AT 2 phase annotated";
            REPORT_INFO (filename, __FUNCTION__, msg.str() );
	    detect_2_phase_target = true;
	    m_wait_rsp_set.insert(transaction_ptr);
            wait (delay);            // wait delay specified by target
            break;
          }

          case tlm::BEGIN_RESP: 
          case tlm::BEGIN_REQ: 
          case tlm::END_RESP:
          default: 
          {
            msg.str ("");
            msg << m_ID << " - Invalid response to Initiator BEGIN_REQ";
            REPORT_FATAL (filename, __FUNCTION__, msg.str() );
            break;
          }
        } // end switch (phase)
        break;
      } // end case TLM_UPDATED

      case tlm::TLM_REJECTED: 
      default:
      {
        msg.str ("");
        msg << m_ID << " - TLM_REJECTED invalid response";
        REPORT_FATAL (filename, __FUNCTION__, msg.str() );
        break;
      }
    } // end case
  } // end while true
} // end initiator_thread 


/*==============================================================================
  @fn at_initiator::nb_transport

  @brief non-blocking transport inbound

  @details
    nb_transport (TLM 2 Transport backwards  (inbound))
    Process inbound TLM2 transports.
    Supported phases include:
    1. END_REQ
    2. BEGIN_RESP

  @param gp generic payload pointer
  @param phase transaction phase
  @param delay_time event processing time

  @retval result code

  @note

==============================================================================*/
tlm::tlm_sync_enum    
at_initiator::nb_transport                                // inbound nb_transport
( tlm::tlm_generic_payload&  transaction_ref              // generic payload
, tlm::tlm_phase&            phase                        // tlm phase
, sc_time&                   delay                        // delay
)
{
  tlm::tlm_sync_enum        status = tlm::TLM_REJECTED;   // return status reject by default
  tlm::tlm_generic_payload *trans_ptr;
  std::ostringstream       msg;               // log message

  switch (phase) 
  {
    // AT target reported end request phase (4 phase AT handshake)
    // new reques are blocked untill received 
    case tlm::END_REQ:  
    {
      // get the next expected end_req from queue
      trans_ptr = m_req_accepted_queue.front(); 
      m_req_accepted_queue.pop();
      
      // compare the expected to the incoming
      if (trans_ptr != &transaction_ref)        // check trans_ptr == transaction_ptr
      {
        msg.str ("");
        msg << m_ID << " - ** transaction ptr not valid END_REQ";
        REPORT_FATAL (filename, __FUNCTION__, msg.str() );
      }
      // if we have the right transaction, put it on the wait response queue
      // and send back the appropriate status
	    m_wait_rsp_set.insert(&transaction_ref);
      m_req_accepted_event.notify(SC_ZERO_TIME);      // release reqeuster thread
      status = tlm::TLM_ACCEPTED;
      break;
    }

    // Either LT target completing request, or
    //        AT target beginning response phase	
    case tlm::BEGIN_RESP: 
    { 
      // check AT 2 phase and 4 phase queues; if this is an AT 2-phase target, return 
	    // TLM_COMPLETED status and END_RESP phase.  No further phases are required.
	    // If this is an AT 4-phase target, return TLM_ACCEPTED status.  An additional
      // handshake phase is required.
      // The following block handles out of order response from a target, and
      // relative out of order responses of a target over another target (multiple
      // target system)
      if (!m_wait_rsp_set.empty())
      {
        // check to see if incoming transaction matches one from 2 phase queue
        std::set<tlm::tlm_generic_payload *>::iterator set_iterator;
        set_iterator = m_wait_rsp_set.find(&transaction_ref);
	      if (set_iterator != m_wait_rsp_set.end())
        {
	        m_wait_rsp_set.erase(set_iterator);
          // phase 2
		      if (detect_2_phase_target) 
          {
            response_out_port->write(&transaction_ref); // put the GP on the rsp fifo port
            phase = tlm::END_RESP;                      // set appropriate return phase
            delay = m_end_rsp_delay;                    // wait for the response delay
            msg.str("");
            msg << m_ID << " - * END_RESP AT 2 phase annotated";
            REPORT_INFO (filename, __FUNCTION__, msg.str() );
            status = tlm::TLM_COMPLETED;                // return status
		      }
          // 4 phase
		      else 
          {
		        if (m_send_end_rsp_queue.empty()) 
		        {
		          m_send_end_rsp_event.notify(m_end_rsp_delay);
		        }
            m_send_end_rsp_queue.push(&transaction_ref);// put transaction onto rsp queue
            msg.str("");
            msg << m_ID << " - AT target starting response";
            REPORT_INFO (filename, __FUNCTION__, msg.str() );
            status = tlm::TLM_ACCEPTED;
          }
          //break;
        }
      }
    //}
      // check LT req_accepted_queue; if this is an LT target, return TLM_COMPLETED
      // status.  No further phases are required.
      //else if(!m_req_accepted_queue.empty())
    if(!m_req_accepted_queue.empty() && (status == tlm::TLM_REJECTED))
      {
        trans_ptr = m_req_accepted_queue.front();     
        if (trans_ptr == &transaction_ref)            // LT target completing request
        {
          m_req_accepted_queue.pop();
          m_req_accepted_event.notify(SC_ZERO_TIME);  // release reqeuster thread
          response_out_port->write(&transaction_ref); // put the GP on the rsp fifo port
          msg.str("");
          msg << m_ID << " - LT target completing request";
          REPORT_INFO (filename, __FUNCTION__, msg.str() );
          status = tlm::TLM_COMPLETED;                // return status
          break;
        }
      }
      // incoming transaction does not match any response queues
      else 
      {
        msg.str("");
        msg << m_ID << " - ** transaction ptr not valid BEGIN_RESP";
        REPORT_INFO (filename, __FUNCTION__, msg.str() );
        break;                 // return not reachable fatal error 
      }
      break;
    } // end case begin response 

    // initiator does not support the following incoming phases
    case tlm::BEGIN_REQ: 
    case tlm::END_RESP: 
    {
      msg.str ("");
      msg << m_ID << " - unsupported phase";
      REPORT_FATAL(filename, __FUNCTION__, msg.str() );
      break;   
    }
 
    default: {                     
      msg.str ("");
      msg << m_ID << " - nb_transport: phase not implemented";
      REPORT_FATAL (filename, __FUNCTION__, msg.str() );
      break;   
    }
  }
  return status;
}

/*==============================================================================
  @fn at_initiator::m_send_end_rsp_method

  @brief send end response method

  @details
    Send end response method 
    Last phase in 4-phase AT target handshake;
    Requires target to send back TLM_COMPLETED return status

  @param void

  @retval void

  @note

==============================================================================*/
void at_initiator::m_send_end_rsp_method(void)  ///< send end response method
{
  tlm::tlm_generic_payload* transaction_ptr;
  std::ostringstream        msg;                // log message

  // if there are items in the end response queue, start processing
  if (!m_send_end_rsp_queue.empty())
  {
    transaction_ptr = m_send_end_rsp_queue.front();
    m_send_end_rsp_queue.pop();

    tlm::tlm_phase phase  = tlm::END_RESP;      // set the appropriate phase
    sc_time delay         = SC_ZERO_TIME;

    // call begin response and then decode return status
    switch (initiator_socket->nb_transport(*transaction_ptr, phase, delay))
    {
      case tlm::TLM_COMPLETED:                         // transaction complete
      {
        response_out_port->write(transaction_ptr);     // send GP to output rsp fifo
        msg.str ("");
        msg << m_ID << " - ** AT initiator send end response";
        REPORT_INFO (filename, __FUNCTION__, msg.str() );
        break;
      }

      case tlm::TLM_ACCEPTED: 
      case tlm::TLM_UPDATED:   
      case tlm::TLM_REJECTED:   
      default: 
      {
        msg.str ("");
        msg << m_ID << " - Invalid response for END_RESP";
        REPORT_INFO (filename, __FUNCTION__, msg.str() );
        break;
      }
    } // end switch 
  } // end if m_send_end_rsp_active

  // check queue for another transaction
  if (!m_send_end_rsp_queue.empty())
  {
    m_send_end_rsp_event.notify (m_end_rsp_delay);
  }
    return;
} // end End_Response_method

/*==============================================================================
  @fn at_initiator::invalidate_direct_mem_ptr

  @brief invalidate direct memory pointer

  @details

  @param address start range to invalidate
  @param address end range to invalidate

  @retval void

  @note
    not implemented

==============================================================================*/

void at_initiator::invalidate_direct_mem_ptr   // invalidate_direct_mem_ptr
( sc_dt::uint64 start_range                    // start range
, sc_dt::uint64 end_range                      // end range
) 
{
  std::ostringstream       msg;                // log message

  msg.str ("");
  msg << m_ID << " - invalidate_direct_mem_ptr: not implemented";
  REPORT_INFO(filename, __FUNCTION__, msg.str());
}

