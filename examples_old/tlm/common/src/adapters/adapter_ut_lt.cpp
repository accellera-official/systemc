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
 @file adapter_ut_lt.cpp
 
 @brief UT-LT adapter class
 
  Original Authors:
    Charles Wilson, ESLX
    Bill Bunton, ESLX
    
--------------------------------------------------------------------------------------- */

#include "reporting.h"                                ///< Reporting convenience macros
#include "adapter_ut_lt.h"                            ///< Our header
#include "tlm.h"                                      ///< TLM headers

static const char *filename = "adapter_ut_lt.cpp";    ///< filename for reporting

/*==============================================================================
  @fn adapter_ut_lt::adapter_ut_lt
  
  @brief adapter_ut_lt class constructor 
  
  @details
    This is the UT to LT adapter class constructor.
    
  @param name module name
  
  @retal none
    
==============================================================================*/

adapter_ut_lt::adapter_ut_lt                          ///< constructor
( sc_core::sc_module_name name                        ///< module name
)
: sc_module         (name)                            ///< module name
, adapter_socket    ("UT_LT_adapter_socket")          ///< initiator socket (adapter)
, target_socket     ("UT_target_socket")              ///< target socket
{ 
  // bind sockets to instance
  adapter_socket  (*this);
  target_socket   (*this);
}

/*==============================================================================
  @fn adapter_ut_lt::~adapter_ut_lt
  
  @brief adapter_ut_lt class destructor 
  
  @details
    This is the UT to LT adapter class destructor.
    
  @param none
  
  @retal none
    
==============================================================================*/

adapter_ut_lt::~adapter_ut_lt                         ///< destructor
( void
)
{
}

/*==============================================================================
  @fn adapter_ut_lt::read
  
  @brief adapter_ut_lt read method 
  
  @details
    This routine handles UT to LT read commands.
    
  @param trans  GP transaction
  
  @retal bool success/failure
    
==============================================================================*/

bool                                                  ///< success/failure 
adapter_ut_lt::read                                   ///< read 
( tlm::tlm_generic_payload &trans                     ///< transaction
)
{                   
  bool  success = false;                              ///< return status

  // build TLM2 transaction payload

  tlm::tlm_generic_payload transaction;

  transaction.set_read            ();
  transaction.set_address         (trans.get_address ());  
  transaction.set_data_ptr        (trans.get_data_ptr ());
  transaction.set_data_length     (trans.get_data_length ());
  transaction.set_response_status (tlm::TLM_INCOMPLETE_RESPONSE);

  sc_core::sc_time delay  = sc_core::SC_ZERO_TIME;
  tlm::tlm_phase phase    = tlm::BEGIN_REQ;
  
  REPORT_INFO (filename, __FUNCTION__, "bridging UT read command to LT");

  if (adapter_socket->nb_transport  ( transaction
                                    , phase
                                    , delay ) == tlm::TLM_COMPLETED)
  {
    wait(delay);
    
    success = true;
  }
  else
  {
    REPORT_FATAL(filename,  __FUNCTION__,"All requests must return complete");
  }
  
  REPORT_INFO (filename, __FUNCTION__, "returning read status to UT initiator");
  
  return success;
}

/*==============================================================================
  @fn adapter_ut_lt::write
  
  @brief adapter_ut_lt write method 
  
  @details
    This routine handles UT to LT write commands.
    
  @param trans  GP transaction
  
  @retal bool success/failure
    
==============================================================================*/

bool                                                  ///< success/failure 
adapter_ut_lt::write                                  ///< write 
( tlm::tlm_generic_payload &trans                     ///< transaction
)
{
  bool success = false;                               ///< return status

  // build TLM2 transaction payload

  tlm::tlm_generic_payload transaction;               ///< GP payload

  transaction.set_write           ();
  transaction.set_address         (trans.get_address ());  
  transaction.set_data_ptr        (trans.get_data_ptr ());
  transaction.set_data_length     (trans.get_data_length ());
  transaction.set_response_status (tlm::TLM_INCOMPLETE_RESPONSE);
  
  sc_core::sc_time delay  = sc_core::SC_ZERO_TIME;
  tlm::tlm_phase phase    = tlm::BEGIN_REQ;

  REPORT_INFO (filename, __FUNCTION__, "bridging UT write command to LT");

  if (adapter_socket->nb_transport  ( transaction
                                    , phase
                                    , delay ) == tlm::TLM_COMPLETED)
  {
    wait(delay);
    
    success = true;
  }
  else
  {
    REPORT_FATAL(filename,  __FUNCTION__,"All requests must return complete");
  }
  
  REPORT_INFO (filename, __FUNCTION__, "returning write status to UT initiator");
  
  return success;
}

/*==============================================================================
  @fn adapter_ut_lt::nb_transport
  
  @brief non-blocking transport
  
  @param transaction  generic payload pointer
  @param phase        transaction phase
  @param delay        event processing time
  
  @retval tlm_sync_enum synchronization state
  
  @note
    1. write overruns are ignored silently
    2. reads are preinitialized to 0
    3. read overruns are ignored
    
==============================================================================*/

tlm::tlm_sync_enum                                    /// synchronization state 
adapter_ut_lt::nb_transport                           ///< nb_transport
( tlm::tlm_generic_payload  &transaction              ///< transaction
, tlm::tlm_phase            &phase                    ///< phase
, sc_core::sc_time          &delay                    ///< delay
)
{
  REPORT_ERROR(filename,  __FUNCTION__, "not implemented");
  
  return tlm::TLM_COMPLETED;
}

/*==============================================================================
  @fn adapter_ut_lt::invalidate_direct_mem_ptr
  
  @brief invalidate a direct memory pointer
  
  @param start_range  start of invalidation range
  @param end_range    end of invalidation range
  
  @retval none
  
==============================================================================*/

void 
adapter_ut_lt::invalidate_direct_mem_ptr              ///< invalidate_direct_mem_ptr
( sc_dt::uint64             start_range               ///< start range
, sc_dt::uint64             end_range                 ///< end range
)
{
  REPORT_ERROR(filename,  __FUNCTION__, "not implemented");
}

/*==============================================================================
  @fn adapter_ut_lt::b_transport
  
  @brief blocking transport
  
  @param transaction  generic payload pointer
  
  @retval none
  
==============================================================================*/

void
adapter_ut_lt::b_transport                            ///< b_transport
( tlm::tlm_generic_payload  &trans                    ///< transaction
)
{
  trans.set_response_status (tlm::TLM_GENERIC_ERROR_RESPONSE);
  
  switch (trans.get_command ())
  {
    case tlm::TLM_READ_COMMAND:
    {
      if (read (trans))
      {
        trans.set_response_status (tlm::TLM_OK_RESPONSE);
      }
      
      break;
    }
    
    case tlm::TLM_WRITE_COMMAND:
    {
      if (write (trans))
      {
        trans.set_response_status (tlm::TLM_OK_RESPONSE);
      }
      
      break;
    }
    
    default:
    {
      REPORT_ERROR(filename, __FUNCTION__, "invalid GP command");
      
      trans.set_response_status (tlm::TLM_COMMAND_ERROR_RESPONSE);
    }
  }
}

/*==============================================================================
  @fn adapter_ut_lt::transport_dbg
  
  @brief transport debug
  
  @param payload  debug transaction
  
  @retval unsigned int  byte count
  
  @note
    not implemented
    
==============================================================================*/

unsigned int                                          ///< bytes returned
adapter_ut_lt::transport_dbg                          ///< transport_dbg
( tlm::tlm_generic_payload  &payload                  ///< debug payload
)
{
  REPORT_ERROR(filename,  __FUNCTION__, "not implemented");

  return 0;
}

/*==============================================================================
  @fn adapter_ut_lt::get_direct_mem_ptr
  
  @brief get direct memory pointer
  
  @param address  DMI address
  @param dmi_data DMI data
  
  @retval bool  success/failure flag
  
==============================================================================*/

bool                                                  ///< success/failure
adapter_ut_lt::get_direct_mem_ptr                     ///< get_direct_mem_ptr
( tlm::tlm_generic_payload   &payload,                ///< address + extensions
        tlm::tlm_dmi        &dmi_data                 ///< DMI data
)
{
  bool  success = false;                              ///< success flag
  
  REPORT_INFO(filename,  __FUNCTION__, "bridging UT DMI pointer request to LT");
  
  // pass through the DMI pointer request  
  success = adapter_socket->get_direct_mem_ptr  ( payload
                                                , dmi_data );
  
//  REPORT_ERROR(filename,  __FUNCTION__, "not implemented");

  return success;
}
