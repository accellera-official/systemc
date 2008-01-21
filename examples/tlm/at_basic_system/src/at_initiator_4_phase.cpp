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
  @file at_initiator_4_phase.cpp
  @Details
   This module implements three interfaces. The first implements the functionality
   required to access the tlm_master_socket. The second and third interfaces 
   implements the functionality required to allow other modules to pass generic 
   payloads via an sc_fifo.
  
   This AT initiator is capable of interfacing with AT 4-phase targets

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
#include "at_initiator_4_phase.h"             // Our header

using namespace sc_core;

const char *filename = "at_initiator_4_phase.cpp";

/*==============================================================================
  @fn at_initiator_4_phase::at_initiator_4_phase

  @brief class constructor

  @details
    This is the class constructor.

  @param name module name
  @param ID   initiator ID

  @retval void

==============================================================================*/
at_initiator_4_phase::at_initiator_4_phase    ///< constructor
( sc_module_name name                         ///< module name
, const unsigned int ID                       ///< initiator ID
, sc_core::sc_time end_rsp_delay              ///< delay
)
: sc_module       (name)                      ///< initialize module name
, initiator_socket("initiator_socket")        ///< initiator socket
, m_ID            (ID)                        ///< initialize initiator ID
, m_end_rsp_delay (end_rsp_delay)             ///< initialize delay
{ 
  // bind initiator to the export
  initiator_socket (*this);                     

  // register thread process
  SC_THREAD(initiator_thread);                  

  // register method process
  SC_METHOD(m_send_end_rsp_method)              
}

/*==============================================================================
  @fn at_initiator_4_phase::~at_initiator_4_phase

  @brief class destructor

  @details
    This routine handles the class cleanup.

  @param void

  @retval void
==============================================================================*/
at_initiator_4_phase::~at_initiator_4_phase(void) {}        ///< destructor

/*==============================================================================
  @fn at_initiator_4_phase::initiator_thread

  @brief initiates non-blocking transport

  @details
    Initiator thread (TLM 2 Transport forwards  (outbound))
    Takes requests from input request queue and initiates a TLM2 non-blocking
    transfer via the initiator socket.  Process the return status from target
    and determine target type:

  @param void

  @retval void

  @note

==============================================================================*/
void at_initiator_4_phase::initiator_thread(void)     ///< initiator thread
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

    // Make the non-blocking call and decode returned status (tlm_sync_enum) 
    switch (initiator_socket->nb_transport(*transaction_ptr, phase, delay)) 
    {
      // AT target accepted request 
      // next phase could be AT 4 phase END_REQ
      case tlm::TLM_ACCEPTED:
      {
        m_req_accepted_queue.push(transaction_ptr); 
        wait (m_req_accepted_event);        // AT must wait for END_REQ 
        msg.str ("");
        msg << m_ID << " - Target accepted AT request";
        REPORT_INFO (filename, __FUNCTION__, msg.str() );
        break;
      }

      case tlm::TLM_UPDATED: 
      case tlm::TLM_REJECTED: 
      case tlm::TLM_COMPLETED: 
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
  @fn at_initiator_4_phase::nb_transport

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
at_initiator_4_phase::nb_transport                        // inbound nb_transport
( tlm::tlm_generic_payload&  transaction_ref              // generic payload
, tlm::tlm_phase&            phase                        // tlm phase
, sc_time&                   delay                        // delay
)
{
  tlm::tlm_sync_enum        status = tlm::TLM_REJECTED;   // return status reject by default
  tlm::tlm_generic_payload *trans_ptr;
  std::ostringstream       msg;                           // log message

  switch (phase) 
  {
    // AT target reported end request phase (4 phase AT handshake)
    // new requests are blocked untill received 
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

    // AT target beginning response phase	
    case tlm::BEGIN_RESP: 
    { 
      // check AT 4 phase queues; If this is an AT 4-phase target, return TLM_ACCEPTED 
      // status.  An additional handshake phase is required.  The following block 
      // handles out of order response from a target, and relative out of order 
      // responses of a target over another target (multiple target system)
      if (!m_wait_rsp_set.empty())
      {
        std::set<tlm::tlm_generic_payload *>::iterator set_iterator;
        set_iterator = m_wait_rsp_set.find(&transaction_ref);
	      if (set_iterator != m_wait_rsp_set.end())     // 4 phase target response
        {
	        m_wait_rsp_set.erase(set_iterator);
          m_send_end_rsp_queue.push(&transaction_ref);// put transaction onto rsp queue
          m_send_end_rsp_event.notify(SC_ZERO_TIME);  // notify handshake phase
          msg.str("");
          msg << m_ID << " - AT target starting response";
          REPORT_INFO (filename, __FUNCTION__, msg.str() );
          status = tlm::TLM_ACCEPTED;
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
  @fn at_initiator_4_phase::m_send_end_rsp_method

  @brief send end response method

  @details
    Send end response method 
    Last phase in 4-phase AT target handshake;
    Requires target to send back TLM_COMPLETED return status

  @param void

  @retval void

  @note

==============================================================================*/
void at_initiator_4_phase::m_send_end_rsp_method(void)  ///< send end response method
{
  static bool m_send_end_rsp_active = false;    // set state of end rsp queue
  tlm::tlm_generic_payload* transaction_ptr;
  std::ostringstream        msg;                // log message

  // if there are items in the end response queue, start processing
  if (m_send_end_rsp_active) 
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
        msg << m_ID << " - Invalid reponse for END_RESP";
        REPORT_INFO (filename, __FUNCTION__, msg.str() );
        break;
      }
    } // end switch 
  } // end if m_send_end_rsp_active

  // check queue for another transaction
  if (m_send_end_rsp_queue.empty()){
    m_send_end_rsp_active = false;
    next_trigger(m_send_end_rsp_event);
  }
  else {
    m_send_end_rsp_active = true;
    msg.str ("");
    msg << m_ID << " - ** start END_RESP delay" << endl;
    REPORT_INFO (filename, __FUNCTION__, msg.str() );

    m_send_end_rsp_event.notify (m_end_rsp_delay);
    next_trigger(m_send_end_rsp_event);
  }

    return;
} // end End_Response_method

/*==============================================================================
  @fn at_initiator_4_phase::invalidate_direct_mem_ptr

  @brief invalidate direct memory pointer

  @details

  @param address start range to invalidate
  @param address end range to invalidate

  @retval void

  @note
    not implemented

==============================================================================*/

void at_initiator_4_phase::invalidate_direct_mem_ptr   // invalidate_direct_mem_ptr
( sc_dt::uint64 start_range                    // start range
, sc_dt::uint64 end_range                      // end range
) 
{
  std::ostringstream       msg;                // log message

  msg.str ("");
  msg << m_ID << " - invalidate_direct_mem_ptr: not implemented";
  REPORT_FATAL(filename, __FUNCTION__, msg.str());
}

