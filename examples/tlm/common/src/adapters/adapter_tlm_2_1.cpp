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
/*==============================================================================

  @file adapter_tlm_2_1.cpp
  
  @brief Implementation of adapter_tlm_2_1 class
  
  @Details
    Implements the adapter_tlm_2_1 class. This class does not support memory refresh or 
    DMI.

  @Note
    1. transport_dbg() not implemented
    2. get_direct_mem_ptr() not implemented
===============================================================================*/

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
*****************************************************************************/

#include "adapter_tlm_2_1.h"                          ///< our class header
#include "reporting.h"                                ///< Reporting convenience macros
#include "tlm.h"                                      ///< TLM headers

using namespace  std;

static const char *report_source = "adapter_tlm_2_1"; ///< filename for reporting

/*==============================================================================
  @fn adapter_tlm_2_1
  
  @brief class constructor destructor 
  
  @details
    This is the class constructor.
    
  @note
 
==============================================================================*/

adapter_tlm_2_1::adapter_tlm_2_1              ///< constructor
  (sc_core::sc_module_name  module_name       ///< module name 
   ) : sc_module        (module_name)
   , adapter_socket ("adapter_socket")
   , tlm_1_initiator_port ("tlm_1_initiator_port")
      
   {
      adapter_socket(*this); // Bind the socket's to interface
}
     

adapter_tlm_2_1::~adapter_tlm_2_1(void)      ///< destructor
  { 
  }

/*==============================================================================
  @fn nb_transport
  
  @brief non-blocking transport
  
  @details
  
  @param gp generic payload pointer
  @param phase transaction phase
  @param delay_time event processing time
  
  @retval synchronization state
  
  @note
    1. write overruns are ignored silently
    2. reads are preinitialized to 0
    3. read overruns are ignored
    
==============================================================================*/

tlm::tlm_sync_enum                              ///< synchronization state
adapter_tlm_2_1::nb_transport(                 ///< non-blocking transport
    tlm::tlm_generic_payload &gp                ///< generic payoad pointer
  , tlm::tlm_phase           &phase             ///< transaction phase
  , sc_core::sc_time         &delay_time)       ///< time it should take for transport
{
  // Access the required attributes from the payload
  
  sc_dt::uint64      address = gp.get_address();         ///< memory address
  tlm::tlm_command   command = gp.get_command();         ///< memory command
  unsigned char      *data   = gp.get_data_ptr();        ///< data pointer
  int                length  = gp.get_data_length();     ///< data length
  tlm::tlm_sync_enum result  = tlm::TLM_COMPLETED;       ///< routine result (always TLM_COMPLETED)
  std::ostringstream msg;                                ///< log message

  // Error checking
  if (phase != tlm::BEGIN_REQ) {
    REPORT_FATAL(report_source, __FUNCTION__, "phase not BEGIN_REQ");
  }

  // Perform the requested operation
  switch (command)
  {
//-----------------------------------------------------------------------------
     case tlm::TLM_WRITE_COMMAND: 
     {
      msg << "W -";
      msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) 
          << uppercase << hex << address;
      msg << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << length;
      msg << " D: 0x";

      REPORT_INFO(report_source, __FUNCTION__, msg.str());
//-----------------------------------------------------------------------------
// convert tlm 2 payload to tlm 1

  //unsigned int b3 = data[3];  // ?? debug code 
  //unsigned int b2 = data[2];
  //unsigned int b1 = data[1];
  //unsigned int b0 = data[0];

      unsigned int write_data = 
        (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];

      tlm_1_initiator_port.write( (unsigned int)(address), write_data);
      break;
      }


//-----------------------------------------------------------------------------     
    case tlm::TLM_READ_COMMAND:
    {
      msg << "R -";
      msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) 
          << uppercase << hex << address;
      msg << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << length;
      
      REPORT_INFO(report_source, __FUNCTION__, msg.str());

//-----------------------------------------------------------------------------
// convert tlm 2 payload to tlm 1
      unsigned int read_data;
      tlm_1_initiator_port.read( (unsigned int) (address) , read_data);

  // convert int data to char array 
      data[3] = (unsigned char)  (read_data        & 0x000000ff); 
      data[2] = (unsigned char) ((read_data >>  8) & 0x000000ff);
      data[1] = (unsigned char) ((read_data >> 16) & 0x000000ff);
      data[0] = (unsigned char) ((read_data >> 24) & 0x000000ff);

  //unsigned int rb3 = data[3];  // ?? debug code 
  //unsigned int rb2 = data[2];
  //unsigned int rb1 = data[1];
  //unsigned int rb0 = data[0];

      break;
    }
//-----------------------------------------------------------------------------
   default:
    {
      REPORT_FATAL(report_source, __FUNCTION__, "invalid command");
      break;
    }
  }
  // Set parameters to indicate all is well
  phase = tlm::BEGIN_RESP;
  gp.set_response_status(tlm::TLM_OK_RESPONSE);
  return result; 
 
}

/*==============================================================================
  @fn transport_dbg
  
  @brief transport debug
  
  @note
    1. not implemented
    2. currently generates a fatal report (OS: disabled)
    
==============================================================================*/

unsigned int                                ///< result
adapter_tlm_2_1::transport_dbg(             ///< transport debug
  tlm::tlm_generic_payload     &payload     ///< debug payload
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
adapter_tlm_2_1::get_direct_mem_ptr(        ///< get direct memory pointer
  tlm::tlm_generic_payload   &payload,      ///< address + extensions
  tlm::tlm_dmi               &dmi_data      ///< dmi data
)
{
    // It's perfectly allowed to not support DMI, no need to
    // throw an error:
    // REPORT_FATAL(report_source, __FUNCTION__, "routine not implemented");
    return false;
}
