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
 *  lt_initiator.cpp
 *  $Id:$
 *  Changed from mini-spec TBD by Bill     
 *  @Details
 *  Changed from mini-spec TBD by Bill     
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
#include "examples_rw_if.h"                        ///< Convenience R/W interface 
#include "reporting.h"                             ///< Reporting convenience macros
#include "lt_initiator.h"                          ///< Our header
static char *msg_type = "/TLM2/Examples/lt_initiator";

//------------------------------------------------------------------------------
//  constructor and destructor
//------------------------------------------------------------------------------
lt_initiator::lt_initiator(                        ///< constructor
    sc_core::sc_module_name name)                  ///< name
  : sc_module(name) 
  , initiator_socket("lt_initiator_socket") 
  { 
    // bind initiator to the export
    rw_export (*this);
    
    // bind initiator to the socket
    initiator_socket (*this);
  }

lt_initiator::~lt_initiator(void) {                ///< destructor
}

//==============================================================================
//  Initiators read method 
//==============================================================================
bool                                    ///< success/failure
lt_initiator::read(                     ///< read
    sc_dt::uint64   bus_address         ///< bus address
  , unsigned int    byte_count          ///< data transfer size
  , unsigned char*  read_buffer         ///< pointer to read buffer
  ) {                   

  tlm::tlm_generic_payload transaction;

  // Build transaction 
  transaction.set_read();
  transaction.set_address(bus_address);  
  transaction.set_data_ptr(read_buffer);
  transaction.set_data_length(byte_count);
  transaction.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

  sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
  tlm::tlm_phase phase = tlm::BEGIN_REQ;

  if (initiator_socket->nb_transport(transaction, phase, delay)
      == tlm::TLM_COMPLETED) {
    wait(delay);
    return true;
  }
  else {
    REPORT_FATAL(msg_type, "lt_initiator::read","All requests must return complete");
     return false;
  }
}
//==============================================================================
//  Initiators write method 
//==============================================================================
bool                                    ///< success/failure
lt_initiator::write(                    ///< write
    sc_dt::uint64   bus_address         ///< bus address
  , unsigned int    byte_count          ///< data transfer size
  , unsigned char*  write_buffer        ///< pointer to write buffer
  ) {
  tlm::tlm_generic_payload transaction;

  transaction.set_write();
  transaction.set_address(bus_address);  
  transaction.set_data_ptr(write_buffer);
  transaction.set_data_length(byte_count);
  transaction.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

  sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
  tlm::tlm_phase phase = tlm::BEGIN_REQ;

  if (initiator_socket->nb_transport(transaction, phase, delay)
      == tlm::TLM_COMPLETED) {
    wait(delay);
    return true;
  }
  else {
    REPORT_FATAL(msg_type, "lt_initiator::write","All requests must return complete");
    return false;
  }
}

//------------------------------------------------------------------------------
//  Required implementation of pure virtual functions       
//------------------------------------------------------------------------------
tlm::tlm_sync_enum                              /// result 
lt_initiator::nb_transport(                     ///< nb_transport
    tlm::tlm_generic_payload& transaction,      ///< transaction
    tlm::tlm_phase&           phase,            ///< transaction phase
    sc_core::sc_time&         time) {           ///< elapsed time
  REPORT_FATAL(msg_type, "lt_initiator::nb_transport","nb_transport: not implemented");
  return tlm::TLM_COMPLETED;
}

void 
lt_initiator::invalidate_direct_mem_ptr(        ///< invalidate_direct_mem_ptr
    sc_dt::uint64 start_range,                  ///< start range
    sc_dt::uint64 end_range) {                  ///< end range
  REPORT_FATAL(msg_type, "lt_initiator::invalidate_direct_mem_ptr", "invalidate_direct_mem_ptr: not implemented");
}
