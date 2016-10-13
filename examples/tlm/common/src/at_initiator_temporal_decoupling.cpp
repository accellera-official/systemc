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

/* ---------------------------------------------------------------------------------------
 @file at_initiator_temporal_decoupling.h
 
 @brief LT initiator with temporal decoupling
 
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

//==============================================================================

// Note: order of headers is important
#include "tlm.h"                                    ///< TLM headers
#include "reporting.h"                              ///< report output
#include "at_initiator_temporal_decoupling.h"       ///< LT initiator with temporal decoupling

using namespace sc_core;
using namespace std;

const char *filename = "at_initiator_temporal_decoupling.cpp";

/*=============================================================================
  @fn at_initiator_temporal_decoupling::at_initiator_temporal_decoupling
  
  @brief at_initiator_temporal_decoupling constructor
  
  @details
    This routine initialized an at_initiator_temporal_decoupling class instance.
    
  @param name module name
  @param ID   initiator ID
  
  @retval none
=============================================================================*/
at_initiator_temporal_decoupling::at_initiator_temporal_decoupling  ///< constructor
( sc_module_name      name                          ///< module name
, const unsigned int  ID                            ///< initiator ID
, sc_core::sc_time end_rsp_delay                    ///< delay
) 
  :sc_module          (name)                        ///< module name
  ,m_ID               (ID)                          ///< initiator ID
  ,initiator_socket   ("initiator_socket")          ///< initiator socket
  ,m_end_rsp_delay    (end_rsp_delay)               ///< initialize delay
  { 
  initiator_socket (*this);                         // bind initiator to the socket

  SC_THREAD(initiator_thread);                      // register thread process 

  tlm::tlm_quantumkeeper::set_global_quantum(sc_core::sc_time(500, sc_core::SC_NS));
  m_QuantumKeeper.reset();
}

// ??????????????????   need to use m_end_rsp_delay

/*=============================================================================
  @fn at_initiator_temporal_decoupling::initiator_thread
  
  @brief outbound data processing
  
  @details
    This routine handles outbound data.
    
  @param none
  
  @retval none
=============================================================================*/

void
at_initiator_temporal_decoupling::initiator_thread  ///< initiator thread
( void
)
{  
  tlm::tlm_generic_payload *transaction_ptr;        ///< transaction pointer
  std::ostringstream msg;                           ///< log message

  while (true)
  {
    // get transaction GP from reques in FIFO 
    transaction_ptr = request_in_port->read();      // get request from input fifo 

    // setup request 
    tlm::tlm_phase phase  = tlm::BEGIN_REQ;         // Create phase objects
    
    msg.str ("");
    msg << m_ID << " "
        << ((transaction_ptr->get_command () == tlm::TLM_READ_COMMAND) ? "R" : "W")
        << " A: 0x" << internal << setw( sizeof(transaction_ptr->get_address ()) * 2 ) << setfill( '0' ) 
        << uppercase << hex << transaction_ptr->get_address ()
        << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << transaction_ptr->get_data_length ();

    REPORT_INFO (filename, __FUNCTION__, msg.str() );

    // make the non-blocking call and decode returned status (tlm_sync_enum) 
    switch (initiator_socket->nb_transport
            ( *transaction_ptr
            , phase
            , m_QuantumKeeper.get_local_time()))
    {
      case tlm::TLM_COMPLETED:            // LT taget response complete with annonated delay 
      {
        if (m_QuantumKeeper.need_sync())
        {
          msg.str ("");
          msg << m_ID << " - " << "Sync Local time to SC time";
          
          REPORT_INFO (filename, __FUNCTION__, msg.str() );
          
          m_QuantumKeeper.sync();
        }
        
        response_out_port->write(transaction_ptr);
        
        msg.str ("");
        msg << m_ID << " - Completed LT request" << endl;
        msg << "      SC time    = " << sc_core::sc_time_stamp() << endl;
        msg << "      local time = " << m_QuantumKeeper.get_current_time();
            
        REPORT_INFO (filename, __FUNCTION__, msg.str());

        break;
      }

      case tlm::TLM_ACCEPTED:                       // LT target accepted request
      case tlm::TLM_UPDATED:
      {
        m_req_accepted_queue.push(transaction_ptr); 
        wait (m_req_accepted_event); 

        msg.str ("");
        msg << m_ID << " - Completed LT request forced sync" << endl;
        msg << "      local time       = " << m_QuantumKeeper.get_local_time() << endl;

        m_QuantumKeeper.reset();

        msg << "      Reset local time = " << m_QuantumKeeper.get_local_time(); 

        REPORT_INFO (filename, __FUNCTION__, msg.str());

        break;
      }

      case tlm::TLM_REJECTED: 
      default:
      {
        msg.str ("");
        msg << m_ID << " - TLM_REJECTED invalid response";
        
        REPORT_FATAL(filename,  __FUNCTION__, msg.str());
        
        break;
      }
    }
  }
}

/*=============================================================================
  @fn at_initiator_temporal_decoupling::nb_transport
  
  @brief inbound non-blocking transport
  
  @details
    This routine handles inbound non-blocking transport.
    
  @param transaction_ref  transaction reference
  @param phase            transaction phase reference
  @param delay            transaction delay reference
  
  @retval phase synchronization phase
=============================================================================*/

tlm::tlm_sync_enum                                  ///< phase
at_initiator_temporal_decoupling::nb_transport      ///< nb_transport
( tlm::tlm_generic_payload  &transaction_ref        ///< transaction
, tlm::tlm_phase            &phase                  ///< phase
, sc_time                   &delay                  ///< delay
)
{
  std::ostringstream        msg;                    ///< log message
  tlm::tlm_sync_enum        return_status = tlm::TLM_REJECTED;
  
  tlm::tlm_generic_payload  *trans_ptr;

  switch (phase)
  {
    case tlm::BEGIN_RESP:
    { 
      trans_ptr = m_req_accepted_queue.front();
      
      if (trans_ptr == &transaction_ref)            // LT target completing request
      {
        m_req_accepted_queue.pop();
        m_req_accepted_event.notify(SC_ZERO_TIME);  // release reqeuster thread
        response_out_port->write(&transaction_ref);
        
        msg.str ("");
        msg << m_ID << " - LT target completing request";
        
        REPORT_INFO (filename,  __FUNCTION__, msg.str());
        
        return_status = tlm::TLM_COMPLETED;
      }
      else
      {
        msg.str ("");
        msg << m_ID << " - transaction ptr not valid BEGIN_RESP";
        
        REPORT_FATAL(filename, __FUNCTION__, msg.str());
      }
      break;
    } 

    case tlm::END_REQ: 
    case tlm::BEGIN_REQ: 
    case tlm::END_RESP:
    {
      msg.str ("");
      msg << m_ID << " - phase undefined";
        
      REPORT_FATAL(filename, __FUNCTION__, msg.str());
      
      break;
    }

    default:
    {                     
      msg.str ("");
      msg << m_ID << " - phase not implemented";
      
      REPORT_FATAL(filename, __FUNCTION__, msg.str());
      
      break;
    }
  }
  
  return return_status;
}

/*=============================================================================
  @fn at_initiator_temporal_decoupling::invalidate_direct_mem_ptr
  
  @brief invalidate DMI pointer
  
  @details
    This routine a DMI pointer.
    
  @param name module name
  
  @retval none
=============================================================================*/

void
at_initiator_temporal_decoupling::invalidate_direct_mem_ptr ///< invalidate_direct_mem_ptr
( sc_dt::uint64 start_range                         ///< start range
, sc_dt::uint64 end_range                           ///< end range
)
{
  std::ostringstream        msg;                    ///< log message

  msg.str ("");
  msg << m_ID << " - not implemented";
  
  REPORT_FATAL(filename, __FUNCTION__, msg.str());
}
