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

/* ---------------------------------------------------------------------------------------
 @file adapter_lt_ut.cpp
 
 @brief LT-UT adapter class
 
  Original Authors:
    Charles Wilson, ESLX
    Bill Bunton, ESLX
    
--------------------------------------------------------------------------------------- */

#include "adapter_lt_ut.h"                            ///< our class header
#include "reporting.h"                                ///< Reporting convenience macros
#include "tlm.h"                                      ///< TLM headers

static const char *filename = "adapter_lt_ut.cpp";    ///< filename for reporting

/*==============================================================================
  @fn adapter_lt_ut::adapter_lt_ut
  
  @brief adapter_lt_ut class constructor 
  
  @details
    This is the LT to UT adapter class constructor.
    
  @param name module name
  
  @retval none
    
==============================================================================*/

adapter_lt_ut::adapter_lt_ut                          ///< constructor
(sc_core::sc_module_name  module_name                 ///< module name 
)
: sc_module             (module_name)                 ///< module name
, adapter_socket        ("LT_UT_adapter_socket")      ///< target socket (adapter)
, initiator_socket      ("UT_initiator_socket")       ///< initiator socket
{
  // bind sockets to instance
  adapter_socket    (*this);
  initiator_socket  (*this);
}
     
/*==============================================================================
  @fn adapter_lt_ut::~adapter_lt_ut
  
  @brief adapter_lt_ut class destructor 
  
  @details
    This is the LT to UT adapter class destructor.
    
  @param none
  
  @retval none
    
==============================================================================*/

adapter_lt_ut::~adapter_lt_ut                         ///< destructor
( void
)
{ 
}

/*==============================================================================
  @fn adapter_lt_ut::nb_transport
  
  @brief non-blocking transport
  
  @param gp generic payload pointer
  @param phase transaction phase
  @param delay_time event processing time
  
  @retval synchronization state
  
  @note
    1. write overruns are ignored silently
    2. reads are preinitialized to 0
    3. read overruns are ignored
    
==============================================================================*/

tlm::tlm_sync_enum                                    ///< synchronization state
adapter_lt_ut::nb_transport                           ///< non-blocking transport
( tlm::tlm_generic_payload &gp                        ///< generic payoad pointer
, tlm::tlm_phase           &phase                     ///< transaction phase
, sc_core::sc_time         &delay_time)               ///< time it should take for transport
{
  // Access the required attributes from the payload
  
  sc_dt::uint64      address = gp.get_address();      ///< memory address
  tlm::tlm_command   command = gp.get_command();      ///< memory command
  unsigned char      *data   = gp.get_data_ptr();     ///< data pointer
  int                length  = gp.get_data_length();  ///< data length
  tlm::tlm_sync_enum result  = tlm::TLM_COMPLETED;    ///< routine result (always TLM_COMPLETED)
  std::ostringstream msg;                             ///< log message

  // Error checking
  if (phase != tlm::BEGIN_REQ)
  {
    REPORT_FATAL(filename, __FUNCTION__, "phase not BEGIN_REQ");
  }

  // Perform the requested operation
  switch (command)
  {
    case tlm::TLM_WRITE_COMMAND: 
    {
      msg << "W -";
      msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) 
          << uppercase << hex << address;
      msg << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << length;
      msg << " D: 0x";

      REPORT_INFO(filename, __FUNCTION__, msg.str());
      
      tlm::tlm_generic_payload  transaction;          ///< transaction
      
      transaction.set_write           ();
      transaction.set_address         (address);  
      transaction.set_data_ptr        (data);
      transaction.set_data_length     (length);
      transaction.set_response_status (tlm::TLM_INCOMPLETE_RESPONSE);
      
      initiator_socket->b_transport ( transaction );

      break;
    }

    case tlm::TLM_READ_COMMAND:
    {
      msg << "R -";
      msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) 
          << uppercase << hex << address;
      msg << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << length;
      
      REPORT_INFO(filename, __FUNCTION__, msg.str());

      tlm::tlm_generic_payload  transaction;          ///< transaction
      
      transaction.set_read            ();
      transaction.set_address         (address);  
      transaction.set_data_ptr        (data);
      transaction.set_data_length     (length);
      transaction.set_response_status (tlm::TLM_INCOMPLETE_RESPONSE);
      
      initiator_socket->b_transport ( transaction );

      break;
    }

    default:
    {
      REPORT_FATAL(filename, __FUNCTION__, "invalid command");
      break;
    }
  }

  // Set parameters to indicate all is well
  phase = tlm::BEGIN_RESP;
  gp.set_response_status(tlm::TLM_OK_RESPONSE);

  return result; 
}

/*==============================================================================
  @fn adapter_lt_ut::transport_dbg
  
  @brief transport debug
  
  @param payload  debug transaction
  
  @retval unsigned int  byte count
  
  @note
    not implemented
    
==============================================================================*/

unsigned int                                          ///< result
adapter_lt_ut::transport_dbg                          ///< transport debug
(  tlm::tlm_debug_payload     &payload                ///< debug payload
)
{
  REPORT_ERROR(filename, __FUNCTION__, "not implemented");

  return 0;
}

/*==============================================================================
  @fn adapter_lt_ut::get_direct_mem_ptr
  
  @brief get direct memory pointer
  
  @param address  DMI address
  @param dmi_mode DMI read/write mode
  @param dmi_data DMI data
  
  @retval bool  success/failure flag
  
==============================================================================*/

bool                                                  ///< success / failure
adapter_lt_ut::get_direct_mem_ptr                     ///< get direct memory pointer
( const sc_dt::uint64     &address                    ///< DMI address
,       tlm::tlm_dmi_mode &dmi_mode                   ///< DMI read/write mode
,       tlm::tlm_dmi      &dmi_data                   ///< DMI data
)
{
  bool  success = false;                              ///< success flag

  REPORT_INFO(filename,  __FUNCTION__, "bridging LT DMI pointer request to UT");
  
  // pass through the DMI pointer request
  success = initiator_socket->get_direct_mem_ptr  ( address
                                                  , dmi_mode
                                                  , dmi_data );
  
  return success;
}

/*==============================================================================
  @fn adapter_lt_ut::invalidate_direct_mem_ptr
  
  @brief invalidate a direct memory pointer
  
  @param start_range  start of invalidation range
  @param end_range    end of invalidation range
  
  @retval none
  
==============================================================================*/

void
adapter_lt_ut::invalidate_direct_mem_ptr              ///< invalidate_direct_mem_ptr
( sc_dt::uint64             start_range               ///< start range
, sc_dt::uint64             end_range                 ///< end range
)
{
  REPORT_ERROR(filename, __FUNCTION__, "not implemented");
}

/*==============================================================================
  @fn adapter_lt_ut::b_transport
  
  @brief blocking transport
  
  @param trans  GP transaction
  
  @retval none
  
  @note
    not implemented
  
==============================================================================*/

void
adapter_lt_ut::b_transport                            //< b_transport
( tlm::tlm_generic_payload  &trans                    ///< transaction
)
{
  REPORT_ERROR(filename, __FUNCTION__, "not implemented");
} 
