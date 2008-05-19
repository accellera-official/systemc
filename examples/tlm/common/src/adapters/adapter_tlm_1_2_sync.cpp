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
 * @file
 *  adapter_tlm_1_2_sync.cpp
 *  $Id:$
 *     
 *  @Details
 *   
 *  This module implements two interfaces. The first implements the functionality
 *  required to access the tlm_master_socket. The second implements the functionality
 *  required to allow other modules to read and write the socket by way of an export.
 *
 ******************************************************************************/

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
*****************************************************************************/

#include "reporting.h"                                ///< Reporting convenience macros
#include "adapter_tlm_1_2_sync.h"                     ///< Our header
#include "tlm.h"                                      ///< TLM headers

static const char *msg_type = "/TLM2/Examples/adapter_tlm_1_2_sync";  ///< filename for reporting

//------------------------------------------------------------------------------
//  constructor and destructor
//------------------------------------------------------------------------------
adapter_tlm_1_2_sync::adapter_tlm_1_2_sync(         ///< constructor
    sc_core::sc_module_name name)                   ///< name
  : sc_module(name) 
  , initiator_socket("adapter_tlm_1_2_socket") 
  { 
    // bind initiator to the export
    tlm_1_export (*this);
    
    // bind initiator to the socket
    initiator_socket (*this);
  }

adapter_tlm_1_2_sync::~adapter_tlm_1_2_sync(void) {     ///< destructor
}

unsigned char *read_buffer    = new unsigned char[4];   ///< read buffer
unsigned char *write_buffer   = new unsigned char[4];   ///< write buffer
/*==============================================================================
  @fn read 
  
  @brief basic_slave_base read function implementation
  
  @details 
  These methods implement the slave read & write functions for the TLM 1 base 
  protocol. The request is translated to TLM2 generic payload and a nb_transport 
  request is sent to the socket. Then the nb_transport return status is checked: 
  When the status is COMPLETE the TLM request is completed with out delay. 
  When the status is ACCEPTED the method waits on an event that indicates 
  the request has completed. 

==============================================================================*/
basic_status                            ///< TLM 1 status ERROR or SUCCESS 
  adapter_tlm_1_2_sync::read(           ///< TLM 1 read 
    const ADDRESS_TYPE & bus_address    ///< TLM 1 address 
  ,       DATA_TYPE    & read_data      ///< TLM 1 data
  ) {                   
//----------------------------------------------------------
// build TLM2 transaction payload 

  basic_status  return_status = basic_protocol::SUCCESS;
  
  tlm::tlm_generic_payload transaction;

  transaction.set_read();
  transaction.set_address(bus_address);  
  transaction.set_data_ptr(read_buffer);
  transaction.set_data_length(4);
  transaction.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

  sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
  tlm::tlm_phase phase = tlm::BEGIN_REQ;

//----------------------------------------------------------
// Make nb_transport call and decode return status 
  switch (initiator_socket->nb_transport(transaction, phase, delay)) {
    case tlm::TLM_COMPLETED: 
      {
      wait(delay);  // Transaction Finished, wait for the returned delay
      read_data = (read_buffer[0] << 24) | (read_buffer[1] << 16)  
                | (read_buffer[2] << 8)  |  read_buffer[3];
      return_status =  basic_protocol::SUCCESS;   // tlm 1 complete 
      break;
      }

    case tlm::TLM_ACCEPTED:
    case tlm::TLM_UPDATED: 
      {
      wait(m_Response_event); // Transaction not yet finished, wait for event 
      read_data = (read_buffer[0] << 24) | (read_buffer[1] << 16)  
                | (read_buffer[2] << 8)  |  read_buffer[3];
      return_status =  basic_protocol::SUCCESS;    // tlm 1 complete  
      break;
      }

    case tlm::TLM_REJECTED:
      {
      REPORT_FATAL(msg_type, "tlm 1 base read" ,"TLM_REJECTED all requests must complete");
      return_status =  basic_protocol::ERROR;
      break;
      }

    default:
      {
      REPORT_FATAL(msg_type,  "tlm 1 base read","All requests must return complete");
      return_status =  basic_protocol::ERROR;
      break;
      }
  };
  
  return return_status;
}

/*==============================================================================
  @fn write

  @brief basic_slave_base write function implementation
  
  @details 
  These methods implement the slave read & write functions for the TLM 1 base 
  protocol. The request is translated to TLM2 generic payload and a nb_transport 
  request is sent to the socket. Then the nb_transport return status is checked: 
  When the status is COMPLETE the TLM request is completed with out delay. 
  When the status is ACCEPTED the method waits on an event that indicates 
  the request has completed. 


==============================================================================*/
basic_status                            ///< TLM 1 status ERROR or SUCCESS 
  adapter_tlm_1_2_sync::write(          ///< TLM 1 read 
    const ADDRESS_TYPE & bus_address    ///< TLM 1 address 
  , const DATA_TYPE    & write_data     ///< TLM 1 data
  ) {       
//-------------------------------------
// move data 

  write_buffer[3] = (unsigned char) (write_data        & 0x000000ff); // convert int data to char array 
  write_buffer[2] = (unsigned char)((write_data >>  8) & 0x000000ff);
  write_buffer[1] = (unsigned char)((write_data >> 16) & 0x000000ff);
  write_buffer[0] = (unsigned char)((write_data >> 24) & 0x000000ff);


//-------------------------------------
// build TLM2 transaction payload
  
  basic_status  return_status = basic_protocol::SUCCESS;

  tlm::tlm_generic_payload transaction;

  transaction.set_write();
  transaction.set_address(bus_address);  
  transaction.set_data_ptr(write_buffer);
  transaction.set_data_length(4);
  transaction.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
  
  sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
  tlm::tlm_phase phase = tlm::BEGIN_REQ;
 
  switch (initiator_socket->nb_transport(transaction, phase, delay)) {
    case tlm::TLM_COMPLETED: 
      {
      wait(delay);  // Transaction Finished, wait for the returned delay
      return_status = basic_protocol::SUCCESS;
      break;
      }

    case tlm::TLM_ACCEPTED:
    case tlm::TLM_UPDATED: 
      {
      wait(m_Response_event); // Transaction not yet finished, wait for event
      return_status = basic_protocol::SUCCESS;
      break;
      }

    case tlm::TLM_REJECTED:
      {
      REPORT_FATAL(msg_type, "tlm 1 base write", "TLM_REJECTED all requests must complete");
      return_status = basic_protocol::ERROR;
      break;
      }

    default:
      {
      REPORT_FATAL(msg_type,  "tlm 1 base write","All requests must return complete");
      return_status = basic_protocol::ERROR;
      break;
      }
  };
  
  return return_status;
}


/*==============================================================================
  @fn nb_transport 
  
  @brief Implementation of the tlm_bw_nb_transport_if
  
  @details
  This method implements the nb_transport backward interface to receive status. 
  When COMPLETE status is received the response event is notified. 
  
==============================================================================*/

tlm::tlm_sync_enum                              ///< synchronization state
adapter_tlm_1_2_sync::nb_transport(             ///< non-blocking transport
    tlm::tlm_generic_payload &gp                ///< generic payoad pointer
  , tlm::tlm_phase           &phase             ///< transaction phase
  , sc_core::sc_time         &delay_time)       ///< time it should take for transport
{
 
    switch (phase) {
    case tlm::END_REQ:  
      {
      return tlm::TLM_ACCEPTED;
      }
    case tlm::BEGIN_RESP:  
      {
      m_Response_event.notify(delay_time);   
      return tlm::TLM_COMPLETED;
      }
    case tlm::BEGIN_REQ: 
    case tlm::END_RESP: 
    default:                  // A target should never BEGIN_REQ or END_RESP
      {
      REPORT_FATAL(msg_type, "backward nb interface", "Unexpected status");
      return tlm::TLM_REJECTED;
      }
    };
  }

//------------------------------------------------------------------------------
//  Required implementation of pure virtual functions       
//------------------------------------------------------------------------------
void 
adapter_tlm_1_2_sync::invalidate_direct_mem_ptr(     ///< invalidate_direct_mem_ptr
    sc_dt::uint64 start_range,                  ///< start range
    sc_dt::uint64 end_range) {                  ///< end range
  REPORT_FATAL(msg_type, "backward nb interface", "invalidate_direct_mem_ptr: not implemented");
}

