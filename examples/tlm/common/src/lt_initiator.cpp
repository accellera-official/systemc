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
  @file lt_initiator.cpp

  @Details
    This module implements two interfaces. The first implements the functionality
    required to access the tlm_master_socket. The second implements the functionality
    required to allow other modules to read and write the socket by way of an export.

  Original Authors:
    Charles Wilson, ESLX
    Bill Bunton, ESLX

*****************************************************************************/

#include "examples_rw_if.h"                           ///< Convenience R/W interface 
#include "reporting.h"                                ///< Reporting convenience macros
#include "lt_initiator.h"                             ///< Our header
#include "tlm.h"                                      ///< TLM headers

static const char *filename = "/TLM2/Examples/lt_initiator";  ///< filename for reporting

//------------------------------------------------------------------------------
//  constructor and destructor
//------------------------------------------------------------------------------
lt_initiator::lt_initiator                            ///< constructor
( sc_core::sc_module_name name                        ///< module name
)
: sc_module         (name)                            ///< module name
, initiator_socket  ("lt_initiator_socket")           ///< socket
{ 
  // bind initiator to the export
  rw_export (*this);

  // bind initiator to the socket
  initiator_socket (*this);
}

lt_initiator::~lt_initiator                           ///< destructor
( void
)
{
}

//==============================================================================
//  Initiators read method 
//==============================================================================
bool                                                  ///< success/failure
lt_initiator::read                                    ///< read
( sc_dt::uint64 bus_address                           ///< bus address
, unsigned int  byte_count                            ///< data transfer size
, unsigned char *read_buffer                          ///< pointer to read buffer
)
{
  tlm::tlm_generic_payload  transaction;              ///< transaction
  bool                      success       = false;    ///< success flag
  std::ostringstream        msg;                      ///< log message

  // Build transaction 
  transaction.set_read            ();
  transaction.set_address         (bus_address);  
  transaction.set_data_ptr        (read_buffer);
  transaction.set_data_length     (byte_count);
  transaction.set_response_status (tlm::TLM_INCOMPLETE_RESPONSE);

  sc_core::sc_time delay  = sc_core::SC_ZERO_TIME;
  tlm::tlm_phase phase    = tlm::BEGIN_REQ;
    
  msg.str ("");
  msg << "R -"
      << " A: 0x" << internal << setw( sizeof(transaction.get_address ()) * 2 ) << setfill( '0' ) 
      << uppercase << hex << transaction.get_address ()
      << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << transaction.get_data_length ();

  REPORT_INFO (filename, __FUNCTION__, msg.str() );

  if (initiator_socket->nb_transport( transaction
                                    , phase
                                    , delay) != tlm::TLM_COMPLETED)
  {
    REPORT_FATAL(filename, __FUNCTION__, "All requests must return complete");
  }
  else if (transaction.get_response_status() != tlm::TLM_OK_RESPONSE)
  {
    REPORT_ERROR(filename, __FUNCTION__, "operation failed");
  }
  else
  {
    wait(delay);
    
    success = true;
  }
  
  return success;
}

//==============================================================================
//  Initiators write method 
//==============================================================================
bool                                                  ///< success/failure
lt_initiator::write                                   ///< write
( sc_dt::uint64 bus_address                           ///< bus address
, unsigned int  byte_count                            ///< data transfer size
, unsigned char *write_buffer                         ///< pointer to write buffer
)
{
  tlm::tlm_generic_payload  transaction;              ///< transaction
  bool                      success       = false;    ///< success flag
  std::ostringstream        msg;                      ///< log message

  transaction.set_write           ();
  transaction.set_address         (bus_address);  
  transaction.set_data_ptr        (write_buffer);
  transaction.set_data_length     (byte_count);
  transaction.set_response_status (tlm::TLM_INCOMPLETE_RESPONSE);

  sc_core::sc_time delay  = sc_core::SC_ZERO_TIME;
  tlm::tlm_phase phase    = tlm::BEGIN_REQ;
    
  msg.str ("");
  msg << "W -"
      << " A: 0x" << internal << setw( sizeof(transaction.get_address ()) * 2 ) << setfill( '0' ) 
      << uppercase << hex << transaction.get_address ()
      << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << transaction.get_data_length ();

  REPORT_INFO (filename, __FUNCTION__, msg.str() );

  if (initiator_socket->nb_transport( transaction
                                    , phase
                                    , delay) != tlm::TLM_COMPLETED)
  {
    REPORT_FATAL(filename, __FUNCTION__, "All requests must return complete");
  }
  else if (transaction.get_response_status() != tlm::TLM_OK_RESPONSE)
  {
    REPORT_ERROR(filename, __FUNCTION__, "operation failed");
  }
  else
  {
    wait(delay);
    
    success = true;
  }

  return success;
}

//------------------------------------------------------------------------------
//  Required implementation of pure virtual functions       
//------------------------------------------------------------------------------
tlm::tlm_sync_enum                                    /// synchronization state 
lt_initiator::nb_transport                            ///< nb_transport
( tlm::tlm_generic_payload  &transaction              ///< transaction
, tlm::tlm_phase            &phase                    ///< transaction phase
, sc_core::sc_time          &delay                    ///< elapsed time
)
{
  REPORT_FATAL(filename, __FUNCTION__, "not implemented");
  
  return tlm::TLM_COMPLETED;
}

void 
lt_initiator::invalidate_direct_mem_ptr               ///< invalidate_direct_mem_ptr
( sc_dt::uint64 start_range                           ///< start range
, sc_dt::uint64 end_range                             ///< end range
)
{
  REPORT_FATAL(filename, __FUNCTION__, "not implemented");
}
