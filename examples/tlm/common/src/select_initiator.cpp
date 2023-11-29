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
///  @file select_initiator.cpp
///  @Details Implements a AT non blocking initiator
//
//=============================================================================
//  Original Authors:
//    Bill Bunton, ESLX
//    Charles Wilson, ESLX
//    Anna Keist, ESLX
//=============================================================================

#include "reporting.h"                                // Reporting convenience macros
#include "select_initiator.h"                         // Our header
#include "tlm.h"                                      // TLM headers

using namespace sc_core;

static const char *filename = "select_initiator.cpp";  ///  filename for reporting

//=============================================================================
///Constructor

select_initiator::select_initiator                  // constructor
( sc_module_name name                               // module name
, const unsigned int ID                             // initiator ID
, sc_core::sc_time end_rsp_delay                    // delay
)
: sc_module           (name)                        /// init module name
, initiator_socket    ("initiator_socket")          /// init socket name

, m_send_end_rsp_PEQ  ("send_end_rsp_PEQ")          /// init PEQ name
, m_ID            (ID)                              /// init initiator ID
, m_end_rsp_delay (end_rsp_delay)                   /// init end response delay
, m_enable_target_tracking (true)                   /// init tracking mode
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
void select_initiator::initiator_thread(void)   // initiator thread
{
  tlm::tlm_generic_payload *transaction_ptr;    // transaction pointer

  while (true)
  {
//=============================================================================
// Read FIFO to Get new transaction GP from the traffic generator
//=============================================================================
    transaction_ptr = request_in_port->read();  // get request from input fifo

    tlm::tlm_phase phase  = tlm::BEGIN_REQ;     // Create phase objects
    sc_time delay         = SC_ZERO_TIME;       // Create delay objects

    std::ostringstream  msg;                    // log message

    msg.str("");
    msg << "Initiator: " << m_ID
        << " starting new transaction"
        << " for Addr:0x" << hex << setw(8) << setfill('0') << uppercase
        << transaction_ptr->get_address()
        << endl << "      " ;
    msg << "Initiator: " << dec <<  m_ID
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

    switch (return_value)
    {
//-----------------------------------------------------------------------------
//  The target returned COMPLETED this is a single phase transaction
//    Wait the annotated delay
//    Return the transaction to the traffic generator
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
// Target returned UPDATED this will be 2 phase transaction
//    Add the transaction pointer to the waiting backward path map
//    Set tracking enum to Rcved_UPDATED
//    Wait the annotated delay
//-----------------------------------------------------------------------------
      case tlm::TLM_UPDATED:
      {
        if( phase == tlm::END_REQ) {

          if (m_enable_target_tracking) {
            m_waiting_bw_path_map.insert(make_pair(transaction_ptr
                                                  ,Rcved_UPDATED_enum
                                                  ));
          }
          else {
            m_waiting_bw_path_map.insert(make_pair(transaction_ptr
                                                  ,Rcved_END_REQ_enum
                                                  ));
          }
            wait(delay);                    // wait the annotated delay

            msg << "      "
                << "Initiator: " << m_ID
                << " transaction waiting begin-response on backward path";
            REPORT_INFO (filename, __FUNCTION__, msg.str() );

        }
        else if( phase == tlm::BEGIN_RESP) {
        // ?????
        }

        else {
          msg << "      "
              << "Initiator: " << m_ID
              << " Unexpected phase for UPDATED return from target ";
          REPORT_FATAL (filename, __FUNCTION__, msg.str() );
        }
        break;
      } // end case TLM_UPDATED


//-----------------------------------------------------------------------------
//  Target returned ACCEPTED this will be 4 phase transaction
//    Add the transaction pointer to the waiting backward path map
//    Set tracking enum to Rcved_END_REQ
//    When the END REQUEST RULE is active wait for the target to response
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

        wait (m_enable_next_request_event);

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
///  @fn select_initiator::nb_transport_bw
//
///  @brief non-blocking transport from targets
//
//=============================================================================
tlm::tlm_sync_enum
select_initiator::nb_transport_bw                       // inbound nb_transport_bw
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
  waiting_bw_path_map::iterator transaction_pair ;        // create interator for map

  transaction_pair  = m_waiting_bw_path_map.find(&transaction_ref);

  if (transaction_pair == m_waiting_bw_path_map.end() ) {

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
        << "from Addr:0x" << hex << setw(8) << setfill('0') << uppercase << transaction_ref.get_address()
        << dec << endl;

    switch (phase)
    {

//-----------------------------------------------------------------------------
// Target has responded with END_REQ this is a 4 phase transaction
//    Verify the previous current tracking enum is Rcved_ACCEPTED_enum
//    Set tracking enum to Rcved_END_REQ
//-----------------------------------------------------------------------------
    case tlm::END_REQ:
      {
        if (transaction_pair->second == Rcved_ACCEPTED_enum) {
          msg << "      "
              << "Initiator: " << m_ID
              << " transaction waiting begin-response on backward path"
              << endl;

          m_enable_next_request_event.notify(SC_ZERO_TIME);
          transaction_pair->second = Rcved_END_REQ_enum;
          status = tlm::TLM_ACCEPTED;

          msg << "      "
              << "Initiator: " << m_ID
              << " " << report::print(status) <<  " (GP, "
              << report::print(phase) << ", "
              << delay << ")";
          REPORT_INFO (filename, __FUNCTION__, msg.str() );

        }
        else {
          msg << "Initiator: " << m_ID
              << " Unexpected END_REQ";
          REPORT_FATAL (filename, __FUNCTION__, msg.str() );
        }
        break;
      }

//-----------------------------------------------------------------------------
//  Target has responded with BEGIN_RESP
//    The style could be 2,3 or 4 phase
//	  Decode the previous tracking enum
//-----------------------------------------------------------------------------
    case tlm::BEGIN_RESP:
      {
//-----------------------------------------------------------------------------
// Respond to begin-response - 2 phase style
//-----------------------------------------------------------------------------
        if( transaction_pair->second == Rcved_UPDATED_enum){

          msg << "      "
              << "Initiator: " << m_ID
              << " Target response to begin-request was UPDATED" << endl
              << "      "
              << "Initiator: " << m_ID
              << " indicating 2 phase return COMPLETED and annotate time" << endl;

          m_enable_next_request_event.notify(SC_ZERO_TIME); // release reqeuster thread
          m_waiting_bw_path_map.erase(&transaction_ref);    // erase from map

          phase   = tlm::END_RESP;                          // set appropriate return phase
          delay   = m_end_rsp_delay;                        // wait for the response delay
          status  = tlm::TLM_COMPLETED;                     // return status

          msg << "      "
              << "Initiator: " << m_ID
              << " " << report::print(status) <<  " (GP, "
              << report::print(phase) << ", "
              << delay << ")";
          REPORT_INFO (filename, __FUNCTION__, msg.str() );

          response_out_port->write(&transaction_ref);     // transaction to rsp fifo port
        }

//-----------------------------------------------------------------------------
//  Respond to begin-response when the target has omitted the
//  end-request timing-point - 3 phase style
//-----------------------------------------------------------------------------
        else if ( transaction_pair->second == Rcved_ACCEPTED_enum){

          msg << "      "
              << "Initiator: " << m_ID
              << " target omitted end-request timing-point returning ACCEPTED ";
          REPORT_INFO (filename, __FUNCTION__, msg.str() );

          m_waiting_bw_path_map.erase(&transaction_ref);    // erase from map

          // use payload event queue to schedule sending end response
          m_send_end_rsp_PEQ.notify (transaction_ref, m_end_rsp_delay);

          m_enable_next_request_event.notify(SC_ZERO_TIME);

          status = tlm::TLM_ACCEPTED;
        }

//-----------------------------------------------------------------------------
// Respond to begin-response - 4 phase style
//-----------------------------------------------------------------------------
        else if ( transaction_pair->second == Rcved_END_REQ_enum){
          msg << "      "
              << "Initiator: " << m_ID
              << " transaction moved to send-end-response PEQ "
              << endl;

          m_waiting_bw_path_map.erase(&transaction_ref);    // erase from map

          // use payload event queue to schedule sending end response
          m_send_end_rsp_PEQ.notify (transaction_ref, m_end_rsp_delay);

          status = tlm::TLM_ACCEPTED;

          msg << "      "
              << "Initiator: " << m_ID
              << " " << report::print(status) <<  " (GP, "
              << report::print(phase) << ", "
              << delay << ")" ;
          REPORT_INFO (filename, __FUNCTION__, msg.str() );
        }

//-----------------------------------------------------------------------------
        else {
          msg.str ("");
          msg << m_ID << " - ** unexpected BEGIN_RESP";
          REPORT_FATAL (filename, __FUNCTION__, msg.str() );
        }

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
        REPORT_FATAL (filename, __FUNCTION__, msg.str() );
        break;
      }
    } // end switch (phase)
  }
  return status;
} // end backward nb transport



//=============================================================================
///  @fn select_initiator::send_end_rsp_method
//
///  @brief send end response method
//
///  @details This method is scheduled to send the end-response timing point.
///           It is sensitive to the m_send_end_rsp_PEQ.get_event() event.
//
//=============================================================================
void select_initiator::send_end_rsp_method(void)    // send end response method
{
  tlm::tlm_generic_payload* transaction_ptr;
  std::ostringstream        msg;                    // log message

//-----------------------------------------------------------------------------
//  Process all transactions scheduled for current time a return value of NULL
//  indicates that the PEQ is empty at this time
//-----------------------------------------------------------------------------
  while ((transaction_ptr = m_send_end_rsp_PEQ.get_next_transaction()) != NULL)
  {
    tlm::tlm_phase phase  = tlm::END_RESP;          // set the appropriate phase
    sc_time delay         = SC_ZERO_TIME;

    msg.str("");
    msg << "Initiator: " << m_ID
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
      default:
      {
        msg << "      "
            << "Initiator: " << m_ID
            << report::print(return_value) << " Unknown return value for END_RESP ";
        REPORT_INFO(filename,  __FUNCTION__, msg.str());
        break;
      }
    } // end switch
  } // end while

  return;
} // end send_end_rsp_method


//=============================================================================
///  @fn select_initiator::invalidate_direct_mem_ptr
//
///  @brief invalidate direct memory pointer Not implemented
//=============================================================================

void select_initiator::invalidate_direct_mem_ptr  // invalidate_direct_mem_ptr
( sc_dt::uint64 /*start_range*/                   // start range
, sc_dt::uint64 /*end_range*/                     // end range
)
{
  std::ostringstream       msg;                   // log message

  msg.str ("");
  msg << m_ID << " invalidate_direct_mem_ptr: not implemented";
  REPORT_INFO(filename, __FUNCTION__, msg.str());
}
