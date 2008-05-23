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
/*==============================================================================

  @file adapter_tlm_2_1_sync.cpp
  
  @brief Implementation of adapter_tlm_2_1_sync class
  
===============================================================================*/

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
*****************************************************************************/

#include <systemc>
#include "adapter_tlm_2_1_sync.h"                     ///< our class header
#include "reporting.h"                                ///< Reporting convenience macros
#include "tlm.h"                                      ///< TLM headers

using namespace  std;

static const char *report_source = "adapter_tlm_2_1_sync";  ///< filenaem for reporting

/*==============================================================================
  @fn adapter_tlm_2_1_sync
  
  @brief class constructor destructor 
  
==============================================================================*/

adapter_tlm_2_1_sync::adapter_tlm_2_1_sync          ///< constructor
  (sc_core::sc_module_name  module_name             ///< module name 
   ) : sc_module        (module_name)
   , adapter_socket ("adapter_socket")
   , tlm_1_initiator_port ("tlm_1_initiator_port")
   {
    adapter_socket(*this); // Bind the socket's to interface

    SC_THREAD(tlm_1_requestor_thread);
  }

adapter_tlm_2_1_sync::~adapter_tlm_2_1_sync(void) {}  ///< destructor
 

/*==============================================================================
  @fn nb_transport forward 
  
  @brief nb_transport forward interface for request 
  
  @details
  Requests received from the nb_transport forward interface are buffered 
  and TLM_ACCECPTED status is returned. The new request event is notified to 
  schedule the request processing thread
  
==============================================================================*/

tlm::tlm_sync_enum                              ///< synchronization state
adapter_tlm_2_1_sync::nb_transport(             ///< non-blocking transport
    tlm::tlm_generic_payload &gp                ///< generic payoad pointer
  , tlm::tlm_phase           &phase             ///< transaction phase
  , sc_core::sc_time         &delay_time)       ///< time for transport 
  {
    if (phase != tlm::BEGIN_REQ) {    // Error checking
      REPORT_FATAL(report_source, __FUNCTION__, "phase not BEGIN_REQ");
    }
    m_new_request_queue.push(&gp);                      // push gp pointer queue
    m_new_request_event.notify(sc_core::SC_ZERO_TIME);  // sehedule thread  
    return tlm::TLM_ACCEPTED;                           // return request accepted 
}


/*==============================================================================
  @fn tlm_1_requestor_thread
  
  @brief thread to translate an issue buffered request to the TLM1 port.
  
  @details
  This thread takes to generic payloads pointers from the FIFO, translates the gp 
  to TLM1 and issues the requests. When the TLM1 request completes the response 
  is translated to TLM2 and the non blocking backward interface is called to 
  report completion of the TLM2 transaction 
    
==============================================================================*/

void 
adapter_tlm_2_1_sync::tlm_1_requestor_thread(void){
  while (true) {

    if (m_new_request_queue.empty()) {
      sc_core::wait(m_new_request_event);
    }
    
//-------------------------------------
// get gp form the queue 
    tlm::tlm_generic_payload* gp = m_new_request_queue.front();
    m_new_request_queue.pop();

//-------------------------------------
// get request details for TLM2 gp
    sc_dt::uint64      address = gp->get_address();        ///< memory address
    tlm::tlm_command   command = gp->get_command();        ///< memory command
    unsigned char      *data   = gp->get_data_ptr();       ///< data pointer
    int                length  = gp->get_data_length();    ///< data length

    std::ostringstream msg;                                ///< log message

//-------------------------------------   
// decode TLM 2 command  
    switch (command){

//==============================================================================
    case tlm::TLM_WRITE_COMMAND: { 
      msg << "W -";
      msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) 
          << uppercase << hex << address;
      msg << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << length;
      msg << " D: 0x";
      REPORT_INFO(report_source, __FUNCTION__, msg.str());

//--------------------------------------
// convert tlm 2 write to tlm 1 write
      unsigned int write_data = 
        (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];

//--------------------------------------
// Make TLM 1 blocking write request 
      tlm_1_initiator_port.write( (unsigned int)(address), write_data);
      break;
      }


//==============================================================================    
    case tlm::TLM_READ_COMMAND:{
      msg << "R -";
      msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) 
          << uppercase << hex << address;
      msg << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << length;
      REPORT_INFO(report_source, __FUNCTION__, msg.str());

//--------------------------------------
// convert tlm 2 payload to tlm 1
      unsigned int read_data;
      tlm_1_initiator_port.read( (unsigned int) (address) , read_data);

      // convert int data to char array 
      data[3] = (unsigned char)  (read_data        & 0x000000ff); 
      data[2] = (unsigned char) ((read_data >>  8) & 0x000000ff);
      data[1] = (unsigned char) ((read_data >> 16) & 0x000000ff);
      data[0] = (unsigned char) ((read_data >> 24) & 0x000000ff);
      break;
  }
//==============================================================================
    default: {
      REPORT_FATAL(report_source, __FUNCTION__, "invalid command");
      break;
      }
    }

//==============================================================================
// make nb transport call to complete the request 
//==============================================================================
    gp->set_response_status(tlm::TLM_OK_RESPONSE);

    sc_core::sc_time delay  = sc_core::SC_ZERO_TIME;
    tlm::tlm_phase phase    = tlm::BEGIN_RESP;
    adapter_socket->nb_transport(*gp, phase, delay);

  } // end while(true)
}


/*==============================================================================
  @fn transport_dbg
  
  @brief transport debug
  
  @note
    1. not implemented
    2. currently generates a fatal report (OS: disabled)
    
==============================================================================*/

unsigned int                                ///< result
adapter_tlm_2_1_sync::transport_dbg(        ///< transport debug
  tlm::tlm_generic_payload    &payload      ///< debug payload
)
{
    // No error needed, disabled
    //REPORT_FATAL(report_source, __FUNCTION__, "routine not implemented");
  return 0;
}

/*==============================================================================
  @fn get_direct_mem_ptr
  
  @brief get direct memory pointer
  
  @note
    1. not implemented
    2. currently generates a fatal report (OS: disabled)
    
==============================================================================*/

bool                                        ///< success / failure
adapter_tlm_2_1_sync::get_direct_mem_ptr(   ///< get direct memory pointer
  tlm::tlm_generic_payload   &payload,      ///< address + extensions
  tlm::tlm_dmi               &dmi_data      ///< dmi data
)
{
    // It's perfectly allowed to not support DMI, no need to
    // throw an error:
    // REPORT_FATAL(report_source, __FUNCTION__, "routine not implemented");
    return false;
}
