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
 
//=============================================================================
///  @file at_initiator_annotated.cpp
///  @Details Implements a AT non-blocking initiator 
//
//=============================================================================
//  Original Authors:
//    Bill Bunton, ESLX
//    Charles Wilson, ESLX
//    Anna Keist, ESLX
//=============================================================================

#include "reporting.h"                                // Reporting convenience macros
#include "at_initiator_annotated.h"                    // Our header
#include "tlm.h"                                      // TLM headers

using namespace sc_core;

static const char *filename = "at_initiator_annotated.cpp";  ///  filename for reporting

//=============================================================================
///Constructor

at_initiator_annotated::at_initiator_annotated        // constructor
( sc_module_name name                               // module name
, const unsigned int ID                             // initiator ID
, sc_core::sc_time end_rsp_delay                    // delay
)
: sc_module           (name)                        /// init module name
, initiator_socket    ("initiator_socket")          /// init socket name
, m_send_end_rsp_PEQ  ("send_end_rsp_PEQ")          /// init PEQ name  
, m_ID            (ID)                              /// init initiator ID
, m_end_rsp_delay (end_rsp_delay)                   /// init end response delay

{ 
  // bind initiator to the export
  initiator_socket (*this);                     

  // register thread process
  SC_THREAD(initiator_thread);                  

  // register method process
  SC_METHOD(send_end_rsp_method);
               
    sensitive << m_send_end_rsp_PEQ.get_event();
    dont_initialize();
}

//=============================================================================
//
//  Initiator thread
//
//=============================================================================
void at_initiator_annotated::initiator_thread(void)  // initiator thread
{  
  tlm::tlm_generic_payload *transaction_ptr;        // transaction pointer
  std::ostringstream       msg;                     // log message

  while (true) 
  {
//=============================================================================
// Read FIFO to Get new transaction GP from the traffic generator 
//=============================================================================
    transaction_ptr = request_in_port->read();  // get request from input fifo
  
    tlm::tlm_phase phase  = tlm::BEGIN_REQ;     // Create phase objects 
    sc_time delay         = SC_ZERO_TIME;       // Create delay objects 

    msg.str("");
    msg << "Initiator: " << m_ID 
        << " starting new transaction" 
        << endl << "      " 
        << "Initiator: " << m_ID               
        << " nb_transport_fw (GP, " 
        << report::print(phase) << ", "
        << delay << ")";
    REPORT_INFO(filename,  __FUNCTION__, msg.str());


//-----------------------------------------------------------------------------
// Make the non-blocking call and decode returned status (tlm_sync_enum) 
//-----------------------------------------------------------------------------
    tlm::tlm_sync_enum 
    return_value = initiator_socket->nb_transport_fw(*transaction_ptr, phase, delay);
    
    msg.str("");
    msg << "Initiator: " << m_ID
      << " " << report::print(return_value) <<  " (GP, "
      << report::print(phase) << ", "
      << delay << ")" << endl; 

    switch (return_value) {
//-----------------------------------------------------------------------------
//  The target returned COMPLETED this is a 1 phase transaction 
//    Wait the annotated delay
//    Return the transaction to the traffic generator 
//    Make the next request 
//-----------------------------------------------------------------------------
      case tlm::TLM_COMPLETED: 
      {
        wait(delay + m_end_rsp_delay);              // wait the annotated delay 

        msg << "      "
          << "Initiator: " << m_ID
          << " target returned COMPLETED with annotated time ";
        REPORT_INFO (filename, __FUNCTION__, msg.str() );

        response_out_port->write(transaction_ptr);  // return txn to traffic gen

        break;
      }// end case TLM_COMPLETED

//-----------------------------------------------------------------------------
// Target returned UPDATED  
//-----------------------------------------------------------------------------
      case tlm::TLM_UPDATED: 
      {
//-----------------------------------------------------------------------------
//  Put poiter in waiting backware path map
//    Wait the annotated delay
//    Make the next request 
//-----------------------------------------------------------------------------
      if( phase == tlm::END_REQ) {

          m_waiting_bw_path_map.insert(make_pair(transaction_ptr
                                                  ,Rcved_END_REQ_enum
                                                  )); 
         
          wait(delay);                    // wait the annotated delay 
            
          msg << "      "
              << "Initiator: " << m_ID
              << " transaction waiting begin-response on backward path"; 
          REPORT_INFO (filename, __FUNCTION__, msg.str() );
        }
        
//-----------------------------------------------------------------------------
//    Wait the annotated delay
//    Use payload event queue to schedule sending end response  
//    Make the next request 
//-----------------------------------------------------------------------------

        else if( phase == tlm::BEGIN_RESP) {
          msg << "      "
              << "Initiator: " << m_ID 
              << " transaction moved to send-end-response PEQ "
              << endl;

          wait(delay);                    // wait the annotated delay 
          
          m_send_end_rsp_PEQ.notify (*transaction_ptr, m_end_rsp_delay);

          msg << "      "
              << "Initiator: " << m_ID
              << " "  <<  " (GP, "
              << report::print(phase) << ", "
              << delay << ")" ; 
          REPORT_INFO (filename, __FUNCTION__, msg.str() )
        }
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
        else {
          msg << "      "
          << "Initiator: " << m_ID
          << " Unexpected phase for UPDATED return from target ";
          REPORT_FATAL (filename, __FUNCTION__, msg.str() );
        }
        break;
      } // end case TLM_UPDATED


//-----------------------------------------------------------------------------
//  Target returned ACCEPTED this an explicit response 
//    Add the transaction pointer to the waiting backward path map  
//    END REQUEST RULE  wait for the target to response
//-----------------------------------------------------------------------------
      case tlm::TLM_ACCEPTED:
      {
        msg << "      "
        << "Initiator: " << m_ID 
        << " transaction waiting end-request on backward-path ";
        REPORT_INFO (filename, __FUNCTION__, msg.str() );
        
        //  use map to track transaction including current state information 
        m_waiting_bw_path_map.insert (make_pair (transaction_ptr
                                                ,Rcved_ACCEPTED_enum
                                                ));  
                                                
        wait (m_enable_next_request_event);   // wait for the target to response                                  
      
        break;
      } // end case TLM_ACCEPTED


//-----------------------------------------------------------------------------
// All case covered default 
//-----------------------------------------------------------------------------
      default:
      {
        msg << "      "
        << "Initiator: " << m_ID 
        << " Unexpected response to BEGIN_REQ ";
        REPORT_FATAL (filename, __FUNCTION__, msg.str() );
        break;
      }
    } // end case
  } // end while true
} // end initiator_thread 



//=============================================================================
///  @fn at_initiator_annotated::nb_transport_bw
//
///  @brief non-blocking transport from targets
//
//=============================================================================
tlm::tlm_sync_enum    
at_initiator_annotated::nb_transport_bw                  // inbound nb_transport_bw
( tlm::tlm_generic_payload&  transaction_ref            // generic payload
 , tlm::tlm_phase&            phase                     // tlm phase
 , sc_time&                   delay                     // delay
 )

{
  tlm::tlm_sync_enum        status = tlm::TLM_COMPLETED;  // return status reject by default
  std::ostringstream        msg;                          // log message
  
//=============================================================================
// Check waiting backward path map of valid transaction  
//=============================================================================
  waiting_bw_path_map::iterator transaction_inter;      // create interator for map

  transaction_inter  = m_waiting_bw_path_map.find(&transaction_ref); 
  
  if (transaction_inter == m_waiting_bw_path_map.end() ) {   
            
//=============================================================================
//  The transaction pointer used by the backward path call does not belong 
//  to this initiator, this is a major error 
//=============================================================================
    msg << "      "
    << "Initiator: " << m_ID 
    << " Received invalid transaction pointer";
    REPORT_FATAL (filename, __FUNCTION__, msg.str() );
  }

//=============================================================================
//  Normal operation  
//    Decode backward path phase 
//=============================================================================
    else { 
    msg.str ("");
    msg << "Initiator: " << m_ID               
        << " nb_transport_bw (GP, " 
        << report::print(phase) << ", "
        << delay << ")"
        << endl;

    switch (phase) {
//-----------------------------------------------------------------------------
// Target has responded with END_REQ 
//    notify enable next request event
//-----------------------------------------------------------------------------
    case tlm::END_REQ:  {
        msg << "      "
            << "Initiator: " << m_ID
            << " transaction waiting begin-response on backward path" 
            << endl;
              
        m_enable_next_request_event.notify(SC_ZERO_TIME); 
        
        transaction_inter->second = Rcved_END_REQ_enum;   
        status = tlm::TLM_ACCEPTED;
        
        msg << "      "
            << "Initiator: " << m_ID
            << " " << report::print(status) <<  " (GP, "
            << report::print(phase) << ", "
            << delay << ")"; 
        REPORT_INFO (filename, __FUNCTION__, msg.str() );

        break;
      }

//-----------------------------------------------------------------------------
//  Target has responded with BEGIN_RESP 
//    Use payload event queue to schedule sending end response  
//    If there was no END REQUEST this ends the request phase notify
//    enable next request event
//-----------------------------------------------------------------------------
    case tlm::BEGIN_RESP: {
 
      msg << "      "
          << "Initiator: " << m_ID 
          << " transaction moved to send-end-response PEQ "
          << endl;
   
      // check for a synthetic 3-phase transaction (BEGIN_RESP without END_REQ) ?????
      if (transaction_inter->second == Rcved_ACCEPTED_enum) {
        m_enable_next_request_event.notify(SC_ZERO_TIME); 
        }
        
      m_waiting_bw_path_map.erase(&transaction_ref);    // erase from map 

      phase   = tlm::END_RESP;                          // set appropriate return phase
      delay   = m_end_rsp_delay;                        // wait for the response delay
      status  = tlm::TLM_COMPLETED;                     // return status
      
      response_out_port->write(&transaction_ref);     // transaction to rsp fifo port

      msg << "      "
          << "Initiator: " << m_ID
          << " " << report::print(status) <<  " (GP, "
          << report::print(phase) << ", "
          << delay << ")" ; 
      REPORT_INFO (filename, __FUNCTION__, msg.str() );

      break;
    }  // end case BEGIN_RESP


//-----------------------------------------------------------------------------
// Invalid phase for backward path 
//-----------------------------------------------------------------------------
    case tlm::BEGIN_REQ: 
    case tlm::END_RESP: 
      {
        msg.str ("");
        msg << m_ID << " Illegal phase on backward path ";
        REPORT_FATAL(filename, __FUNCTION__, msg.str() );
        break;   
      }
//-----------------------------------------------------------------------------
// Unknown phase on backward path  
//-----------------------------------------------------------------------------
    default: 
      {                     
        msg.str ("");
        msg << m_ID << " Unknown phase on the backward path ";
        REPORT_WARNING (filename, __FUNCTION__, msg.str() );
        break;   
      }
    } // end switch (phase)
  }
    return status;
} // end backward nb transport 



//=============================================================================
///  @fn at_initiator_annotated::send_end_rsp_method
//
///  @brief send end response method
//
///  @details This method is scheduled to send the end-response timing point.
///           It is sensitive to the m_send_end_rsp_PEQ.get_event() event. 
//
//=============================================================================
void at_initiator_annotated::send_end_rsp_method(void) // send end response method
{
  tlm::tlm_generic_payload* transaction_ptr;
  std::ostringstream        msg;                      // log message

//-----------------------------------------------------------------------------  
//  Process all transactions scheduled for current time a return value of NULL 
//  indicates that the PEQ is empty at this time
//----------------------------------------------------------------------------- 
  while ((transaction_ptr = m_send_end_rsp_PEQ.get_next_transaction()) != NULL)
  {
    tlm::tlm_phase phase  = tlm::END_RESP;          // set the appropriate phase
    sc_time delay         = SC_ZERO_TIME;

    msg.str("");
    msg   << "Initiator: " << m_ID
        << " starting send-end-response method"
        << endl << "      " 
        << "Initiator: " << m_ID               
        << " nb_transport_fw (GP, " 
        << report::print(phase) << ", "
        << delay << ")";
    REPORT_INFO(filename,  __FUNCTION__, msg.str());

    // call begin response and then decode return status
    tlm::tlm_sync_enum 
    return_value = initiator_socket->nb_transport_fw(*transaction_ptr, phase, delay);
    
    msg.str("");
    msg << "Initiator: " << m_ID
      << " " << report::print(return_value) <<  " (GP, "
      << report::print(phase) << ", "
      << delay << ")"; 

    switch (return_value)    
    {
      case tlm::TLM_COMPLETED:                        // transaction complete
      {
        response_out_port->write(transaction_ptr);    // send GP to output rsp fifo
        
        REPORT_INFO (filename, __FUNCTION__, msg.str() );
        break;
      }

      case tlm::TLM_ACCEPTED: 
      case tlm::TLM_UPDATED:   
      {
        msg << "      " 
            << "Initiator: " << m_ID
            << report::print(return_value) << " Invalid return value for END_RESP ";
        REPORT_FATAL(filename,  __FUNCTION__, msg.str()); 
        break;
      }
      
      default: 
      {
        msg << "      " 
            << "Initiator: " << m_ID
            << report::print(return_value) << " Unknown return value for END_RESP ";
        REPORT_FATAL(filename,  __FUNCTION__, msg.str()); 
        break;
      }
    } // end switch 
  } // end while 

    return;
} // end send_end_rsp_method


//=============================================================================
///  @fn at_initiator_annotated::invalidate_direct_mem_ptr
//
///  @brief invalidate direct memory pointer Not implemented 
//=============================================================================

void at_initiator_annotated::invalidate_direct_mem_ptr  // invalidate_direct_mem_ptr
( sc_dt::uint64 start_range                             // start range
, sc_dt::uint64 end_range                               // end range
) 
{
  std::ostringstream       msg;                         // log message

  msg.str ("");
  msg << m_ID << " invalidate_direct_mem_ptr: not implemented";
  REPORT_INFO(filename, __FUNCTION__, msg.str());
}

