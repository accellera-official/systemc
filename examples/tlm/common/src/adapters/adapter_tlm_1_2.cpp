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
 *  adapter_tlm_1_2.cpp
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

// Note: order of headers is important
#include "tlm.h"                                   ///< TLM headers
#include "reporting.h"                             ///< Reporting convenience macros
#include "adapter_tlm_1_2.h"                       ///< Our header
static char *msg_type = "/TLM2/Examples/adapter_tlm_1_2";

//------------------------------------------------------------------------------
//  constructor and destructor
//------------------------------------------------------------------------------
adapter_tlm_1_2::adapter_tlm_1_2(                  ///< constructor
    sc_core::sc_module_name name)                  ///< name
  : sc_module(name) 
  , initiator_socket("adapter_tlm_1_2_socket") 
  { 
    // bind initiator to the export
    tlm_1_export (*this);
    
    // bind initiator to the socket
    initiator_socket (*this);
  }

adapter_tlm_1_2::~adapter_tlm_1_2(void) {                ///< destructor
}

unsigned char *read_buffer    = new unsigned char[4];   ///< read buffer
unsigned char *write_buffer   = new unsigned char[4];   ///< write buffer

//==============================================================================
//  Initiators read method 
//==============================================================================
basic_status                            ///TLM 1 status ERROR or SUCCESS 
  adapter_tlm_1_2::read(                ///< TLM 1 read 
    const ADDRESS_TYPE & bus_address    ///< TLM 1 address 
  ,       DATA_TYPE    & read_data      ///< TLM 1 data
  ) {                   


//----------------------------------------------------------
// build TLM2 transaction payload

  tlm::tlm_generic_payload transaction;

  transaction.set_read();
  transaction.set_address(bus_address);  
  transaction.set_data_ptr(read_buffer);
  transaction.set_data_length(4);
  transaction.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

  sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
  tlm::tlm_phase phase = tlm::BEGIN_REQ;

  if (initiator_socket->nb_transport(transaction, phase, delay)
      == tlm::TLM_COMPLETED) {
    wait(delay);
    read_data = (read_buffer[0] << 24) | (read_buffer[1] << 16) | (read_buffer[2] << 8) | read_buffer[3];
    return basic_protocol::SUCCESS;
  }
  else {
    REPORT_FATAL(msg_type, "adapter_tlm_1_2::read","All requests must return complete");
     return basic_protocol::ERROR;
  }
}
//==============================================================================
//  Initiators write method 
//==============================================================================
basic_status                            ///< TLM 1 status ERROR or SUCCESS 
  adapter_tlm_1_2::write(               ///< TLM 1 read 
    const ADDRESS_TYPE & bus_address    ///< TLM 1 address 
  , const DATA_TYPE    & write_data     ///< TLM 1 data
  ) {       
//-------------------------------------

  write_buffer[3] = (unsigned char) (write_data        & 0x000000ff); // convert int data to char array 
  write_buffer[2] = (unsigned char)((write_data >>  8) & 0x000000ff);
  write_buffer[1] = (unsigned char)((write_data >> 16) & 0x000000ff);
  write_buffer[0] = (unsigned char)((write_data >> 24) & 0x000000ff);

// *reinterpret_cast<unsigned int*>(&write_buffer) = write_data;

  //unsigned int b3 = write_buffer[3];  // ?? debug code 
  //unsigned int b2 = write_buffer[2];
  //unsigned int b1 = write_buffer[1];
  //unsigned int b0 = write_buffer[0];


//----------------------------------------------------------
// build TLM2 transaction payload

  tlm::tlm_generic_payload transaction;

  transaction.set_write();
  transaction.set_address(bus_address);  
  transaction.set_data_ptr(write_buffer);
  transaction.set_data_length(4);
  transaction.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
  
  sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
  tlm::tlm_phase phase = tlm::BEGIN_REQ;

  if (initiator_socket->nb_transport(transaction, phase, delay)
      == tlm::TLM_COMPLETED) {
    wait(delay);
    return basic_protocol::SUCCESS;
  }
  else {
    REPORT_FATAL(msg_type, "adapter_tlm_1_2::write","All requests must return complete");
    return basic_protocol::ERROR;
  }
}

//------------------------------------------------------------------------------
//  Required implementation of pure virtual functions       
//------------------------------------------------------------------------------
tlm::tlm_sync_enum                              /// result 
adapter_tlm_1_2::nb_transport(                  ///< nb_transport
    tlm::tlm_generic_payload& transaction,      ///< transaction
    tlm::tlm_phase&           phase,            ///< transaction phase
    sc_core::sc_time&         time) {           ///< elapsed time
  REPORT_FATAL(msg_type, "adapter_tlm_1_2::nb_transport","nb_transport: not implemented");
  return tlm::TLM_COMPLETED;
}

void 
adapter_tlm_1_2::invalidate_direct_mem_ptr(        ///< invalidate_direct_mem_ptr
    sc_dt::uint64 start_range,                  ///< start range
    sc_dt::uint64 end_range) {                  ///< end range
  REPORT_FATAL(msg_type, "adapter_tlm_1_2::invalidate_direct_mem_ptr", "invalidate_direct_mem_ptr: not implemented");
}
