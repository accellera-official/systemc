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

//=====================================================================
/// @file at_target_1_phase.cpp
//
/// @brief Implements single phase AT target
//
//=====================================================================
//  Original Authors:
//    Charles Wilson, ESLX
//    Bill Bunton, ESLX
//    Jack Donovan, ESLX
//=====================================================================

#include "at_target_1_phase.h"                        // our header
#include "reporting.h"                                // reporting macros
//#include <stdio.h>
                    
using namespace  std;

static const char *filename = "at_target_1_phase.cpp";	///< filename for reporting

///Constructor
at_target_1_phase::at_target_1_phase                      
( sc_core::sc_module_name module_name               // module name
, const unsigned int        ID                      // target ID
, const char                *memory_socket          // socket name
, sc_dt::uint64             memory_size             // memory size (bytes)
, unsigned int              memory_width            // memory width (bytes)
, const sc_core::sc_time    accept_delay            // accept delay (SC_TIME)
, const sc_core::sc_time    read_response_delay     // read response delay (SC_TIME)
, const sc_core::sc_time    write_response_delay    // write response delay (SC_TIME)
)
: sc_module               (module_name)             /// init module name
, m_memory_socket         (memory_socket)           /// init socket name
, m_ID                    (ID)                      /// init target ID
, m_memory_size           (memory_size)             /// init memory size (bytes)
, m_memory_width          (memory_width)            /// init memory width (bytes)
, m_accept_delay          (accept_delay)            /// init accept delay
, m_read_response_delay   (read_response_delay)     /// init read response delay
, m_write_response_delay  (write_response_delay)    /// init write response delay
, m_request_count         (1)                       /// init request count
, m_nb_trans_fw_prev_warning(false)
, m_begin_resp_method_prev_warning(false)
, m_trans_dbg_prev_warning(false)
, m_get_dm_ptr_prev_warning(false)
, m_response_PEQ          ("response_PEQ")
, m_target_memory                                   /// init target's memory 
  ( m_ID                          // initiator ID for messaging
  , m_read_response_delay         // delay for reads
  , m_write_response_delay        // delay for writes
  , m_memory_size                 // memory size (bytes)
  , m_memory_width                // memory width (bytes)      
  )
  
  
{
      
  /// Bind the socket's export to the interface
  m_memory_socket(*this);

  /// Register begin_reponse as an SC_METHOD
  /// Used to implement force synchronization multiple timing points
  SC_METHOD(begin_response_method);
  sensitive << m_response_PEQ.get_event();
  dont_initialize();
}

//==============================================================================
//  b_transport implementation calls from initiators 
//
//=============================================================================
void                                        
at_target_1_phase::b_transport
( tlm::tlm_generic_payload  &payload                // ref to  Generic Payload 
, sc_core::sc_time          &delay_time             // delay time 
)
{
 
  std::ostringstream  msg;                          
  msg.str("");
  sc_core::sc_time      mem_op_time;
  
  m_target_memory.operation(payload, mem_op_time);

  msg << "Target: " << m_ID               
      << " returned delay of " << delay_time 
      << " + " << m_accept_delay << " + " << mem_op_time;

  delay_time = delay_time + m_accept_delay + mem_op_time;
  
  msg << " = " << delay_time;
  REPORT_INFO(filename,  __FUNCTION__, msg.str());
  
  return;     
}


//=============================================================================
// nb_transport_fw implementation calls from initiators 
//
//=============================================================================
tlm::tlm_sync_enum                                  // synchronization state
at_target_1_phase::nb_transport_fw                  // non-blocking transport call through Bus
( tlm::tlm_generic_payload &gp                      // generic payoad pointer
, tlm::tlm_phase           &phase                   // transaction phase
, sc_core::sc_time         &delay_time)             // time it should take for transport
{
  std::ostringstream  msg;                          // log message
  
  msg.str("");
  msg << "Target: " << m_ID               
      << " nb_transport_fw (GP, " 
      << report::print(phase) << ", "
      << delay_time << ")";
 
  tlm::tlm_sync_enum  return_status = tlm::TLM_COMPLETED;
  
//-----------------------------------------------------------------------------
// decode phase argument 
//-----------------------------------------------------------------------------
  switch (phase)
  {
//=============================================================================
    case tlm::BEGIN_REQ: 	
      {
     
/// Generate frorced synchronization response every 20th request

    if(m_request_count++ % 20)
//-----------------------------------------------------------------------------
//  AT single timing point forced by returning TLM_COMPLETED 
//-----------------------------------------------------------------------------
      {
        REPORT_INFO(filename,  __FUNCTION__, msg.str());
        
        m_target_memory.operation(gp, delay_time); // perform memory operation now
        
        delay_time += m_accept_delay;
        return_status = tlm::TLM_COMPLETED;       // indicate end of transaction 
        
        msg.str("");
        msg << "Target: " << m_ID  
          << " " << report::print(return_status) <<  " (GP, "
          << report::print(phase) << ", "
          << delay_time << ")" ;
        REPORT_INFO(filename,  __FUNCTION__, msg.str());

        
      } // end AT single timing point 
      
//-----------------------------------------------------------------------------
// Force synchronization multiple timing points by returning TLM_ACCEPTED 
// use a payload event queue to schedule BEGIN_RESP timing point  
//-----------------------------------------------------------------------------
      else
      { 
        m_target_memory.get_delay(gp, delay_time);  // get memory operation delay
        
        delay_time += m_accept_delay;
        
        m_response_PEQ.notify(gp, delay_time);      // put transaction in the PEQ
        
        delay_time = m_accept_delay;
        phase = tlm::END_REQ;                       // advance txn state to end request     
        return_status = tlm::TLM_UPDATED;           // force synchronization 
         
        msg << endl << "      "
          << "Target: " << m_ID  
          << " " << report::print(return_status) <<  " (GP, "
          << report::print(phase) << ", "
          << delay_time << ")" ;
        REPORT_INFO(filename,  __FUNCTION__, msg.str());
 
      }       
      break;
    } // end BEGIN_REQ

//=============================================================================
    case tlm::END_RESP:
    {
      m_end_resp_rcvd_event.notify (sc_core::SC_ZERO_TIME);
      return_status = tlm::TLM_COMPLETED;         // indicate end of transaction     
      break;
      
    }
    
//=============================================================================
    case tlm::END_REQ:
    case tlm::BEGIN_RESP:
    { 
      msg << "Target: " << m_ID 
          << " Illegal phase received by target -- END_REQ or BEGIN_RESP";
      REPORT_FATAL(filename, __FUNCTION__, msg.str()); 
      return_status = tlm::TLM_ACCEPTED;
      break;
    }
   
//=============================================================================
    default:
    { 
      return_status = tlm::TLM_ACCEPTED; 
      if(!m_nb_trans_fw_prev_warning)
        {
        msg << "Target: " << m_ID 
            << " default phase encountered";
        REPORT_WARNING(filename, __FUNCTION__, msg.str()); 
        }
      else m_nb_trans_fw_prev_warning = true;
      break;
    }
  }
  
  return return_status;  
} //end nb_transport_fw


//=============================================================================
/// begin_response method function implementation
//
// This method is statically sensitive to m_response_PEQ.get_event 
//
//=============================================================================
void at_target_1_phase::begin_response_method (void)
{
  std::ostringstream        msg;                    // log message
  tlm::tlm_generic_payload  *transaction_ptr;       // generic payload pointer
  msg.str("");
  tlm::tlm_sync_enum        status = tlm::TLM_COMPLETED;

//-----------------------------------------------------------------------------  
//  Process all transactions scheduled for current time a return value of NULL 
//  indicates that the PEQ is empty at this time
//----------------------------------------------------------------------------- 

  while ((transaction_ptr = m_response_PEQ.get_next_transaction()) != NULL)
  {
 
    sc_core::sc_time dummy_t  = sc_core::SC_ZERO_TIME;
    /// perform memory operation now
    m_target_memory.operation(*transaction_ptr, dummy_t); 

    tlm::tlm_phase phase    = tlm::BEGIN_RESP; 
    sc_core::sc_time delay  = sc_core::SC_ZERO_TIME;

    msg << "Target: " << m_ID 
      << " nb_transport_bw(GP, BEGIN_RESP, SC_ZERO_TIME)"
      << endl << "      ";
    REPORT_INFO(filename,  __FUNCTION__, msg.str());


//-----------------------------------------------------------------------------
// Call nb_transport_bw with phase BEGIN_RESP check the returned status 
//-----------------------------------------------------------------------------
    status = m_memory_socket->nb_transport_bw(*transaction_ptr, phase, delay);
    
    msg << "Target: " << m_ID
        << " " << report::print(status) << "(GP, "
        << report::print(phase) << ", "
        << delay << ")"; 
    REPORT_INFO(filename,  __FUNCTION__, msg.str());

    switch (status)
    { 
    
//=============================================================================
    case tlm::TLM_COMPLETED:    
      {          
        next_trigger (delay);     // add comments ??? 
        return; 
        break;
      }
      
//=============================================================================
    case tlm::TLM_ACCEPTED:
      {     
        next_trigger (m_end_resp_rcvd_event); // add comments ??? 
        return; 
        break;
      }

//=============================================================================
    case tlm::TLM_UPDATED:   
      {
      if(!m_begin_resp_method_prev_warning)
        {
          msg << "Target: " << m_ID 
              << " TLM_UPDATED invalid response to BEGIN_RESP";
          REPORT_WARNING(filename, __FUNCTION__, msg.str()); 
        }
      else m_begin_resp_method_prev_warning = true;
      break;
      }
 
//=============================================================================
    default:                  
      {
        if(!m_begin_resp_method_prev_warning)
          {
            msg << "Target: " << m_ID 
                << " undefined return status ";
           REPORT_WARNING(filename, __FUNCTION__, msg.str()); 
          }
        else m_begin_resp_method_prev_warning = true;
        break;
      }
    }// end switch
      
  } // end while
  
  next_trigger (m_response_PEQ.get_event()); 

} //end begin_response_queue_active

//==============================================================================
// Methods Required by Target Interface but not Implemented for this Example


// Not implemented for this example but required by interface
bool                                            
at_target_1_phase::get_direct_mem_ptr    
  (tlm::tlm_generic_payload & /*payload*/,          ///< address + extensions
   tlm::tlm_dmi &             /*data*/              ///< dmi data
  )
{
  // THis is not a fatal, print first as warning
  std::ostringstream  msg;
  msg.str("");
  if(!m_get_dm_ptr_prev_warning)
    {
      msg << "Target: " << m_ID 
          << " DMI not implemented for this example";
      REPORT_WARNING(filename, __FUNCTION__, msg.str()); 
     }
  else m_get_dm_ptr_prev_warning = true;
	      
  return false;
}

// Not implemented for this example but required by interface
unsigned int                                        // result
at_target_1_phase::transport_dbg            
( tlm::tlm_generic_payload & /*payload*/            ///< debug payload
)
{
  std::ostringstream  msg; 
  msg.str("");
  if(!m_trans_dbg_prev_warning)
  {
    msg << "Target: " << m_ID 
        << " DBG(debug) not implemented for this example";
    REPORT_WARNING(filename, __FUNCTION__, msg.str()); 
  }
  else m_trans_dbg_prev_warning = true;
              
  return false;
}



