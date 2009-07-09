/*******************************************************************************
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
*******************************************************************************/
//==============================================================================
///  @file dmi_memory.cpp
//
///  @details
///     Isolates the dmi based memory functions from the rest of the
///     TLM initiator "shell"
//
//==============================================================================
//
//  Original Authors:
//    Jack Donovan, ESLX
//    Charles Wilson, ESLX
//
//==============================================================================

#include "reporting.h"                          // Reporting convenience macros
#include "dmi_memory.h"                         // Header for this class

using namespace sc_core;

static const char *filename = "dmi_memory.cpp"; ///< filename for reporting

/// Constructor

dmi_memory::dmi_memory
(
  const unsigned int ID                         // Target ID
)
: m_ID                 (ID),
  m_dmi_size           (0),
  m_dmi_base_address   (0)

{
} // end Constructor

//=====================================================================
///  @fn dmi_memory::operation
//
///  @brief DMI read and write processing
//
///  @details
///    This routine implements dmi based read and  write operations
//
//=====================================================================
void
dmi_memory::operation
( tlm::tlm_generic_payload  &gp
, sc_core::sc_time          &delay   ///< transaction delay
)
{
  std::ostringstream  msg;
  msg.str("");

  m_address   = gp.get_address();
  m_data      = gp.get_data_ptr();    ///< data pointer
  m_length    = gp.get_data_length(); ///< data length
  m_command   = gp.get_command();
  m_is_dmi_flag = false;
  gp.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);

  if (is_address_dmi(gp) == false )
  {
    msg << "Iniiator:" << m_ID
        << "A GP with an address not in the allowed DMI ranges has been received"
        << endl << "       "
        << "Use check_status before passing a gp to operation";
      REPORT_INFO(filename, __FUNCTION__, msg.str());
      gp.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE); // Need a different error
    }
 else
 {
  m_offset  = m_address - m_dmi_base_address;

  switch (m_command)
  {
    case tlm::TLM_WRITE_COMMAND:
    {
      for (unsigned int i = 0; i < m_length; i++)
       {
          m_dmi_ptr[m_offset + i] = m_data[i];
        }
      gp.set_response_status(tlm::TLM_OK_RESPONSE);

      delay = delay + m_dmi_write_latency;  // update time
      report::print(m_ID, gp, filename);

      break;
    }

    case tlm::TLM_READ_COMMAND:
    {
      // clear read buffer
      for (unsigned int i = 0; i < m_length; i++)
        {
        m_data[i] = m_dmi_ptr [m_offset + i];
        }
      gp.set_response_status(tlm::TLM_OK_RESPONSE);

      delay = delay +  m_dmi_read_latency; // update time
      report::print(m_ID, gp, filename);

      break;
    }

    default:
    {
      msg << "Target: " << m_ID
          << " Unknown GP command that could be ignorable";
      REPORT_WARNING(filename, __FUNCTION__, msg.str());
      gp.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
      break;
    }

    }//end switch
  }

  return;

//==============================================================================
} // end memory_operation
 void
dmi_memory::invalidate_dmi_ptr
( sc_dt::uint64     start_range
, sc_dt::uint64     end_range
)
{
  std::ostringstream  msg;
  msg.str("");

  if (  ( start_range <= end_range       )
     && ( start_range >= m_dmi_base_address )
     && ( end_range   <= (m_dmi_base_address-m_dmi_size)   ) )
    {
      msg << "Initiator:" << m_ID
          << " DMI Pointer invalidated for "
          << "(" << start_range << ", " << end_range << ")";

      m_start_address=1;
      m_end_address=0;
    }
  else
  {
    msg << "Initiator:" << m_ID
        << " DMI Pointer not invalidated for "
        << "(" << start_range << ", " << end_range << ")";
  }

  REPORT_INFO(filename, __FUNCTION__, msg.str());
  return;
}

//==============================================================================
void
dmi_memory::load_dmi_ptr
(
  tlm::tlm_dmi &dmi_properties
)
{
  report::print(m_ID,dmi_properties,filename);
//  m_dmi_properties = dmi_properties;
  m_dmi_ptr           = dmi_properties.get_dmi_ptr();
  m_dmi_read_latency  = dmi_properties.get_read_latency();
  m_dmi_write_latency = dmi_properties.get_write_latency();
  m_dmi_base_address       = dmi_properties.get_start_address();
  m_dmi_size          = dmi_properties.get_end_address()
                      - m_dmi_base_address;
  m_granted_access    = dmi_properties.get_granted_access();

  return;
}

//==============================================================================
bool
dmi_memory::is_address_dmi
(
  tlm::tlm_generic_payload  &gp
)
{
  m_start_address = gp.get_address();
  m_end_address   = m_start_address + gp.get_data_length();

  std::ostringstream  msg;
  msg.str("");
  msg << "Initiator:" << m_ID;
  bool                return_status = false;

  if ( (    ( m_start_address < ( m_dmi_base_address              ) )
         || ( m_end_address   > ( m_dmi_base_address + m_dmi_size ) )
        )
      )
  {
    // address is outside of the DMI boundaries
    msg << " address is not a dmi address";
//    msg << "m_start_address= " << m_start_address << " m_address= " << m_address
//        << endl << "       "
//        << "m_end_address= " << m_end_address << " m_offset= " << m_offset << " m_dmi_size= "<< m_dmi_size;
    REPORT_INFO(filename, __FUNCTION__, msg.str());
  }
  else if (        ( gp.get_command () == tlm::TLM_WRITE_COMMAND        )
          )
       {
        if (  (m_granted_access != tlm::tlm_dmi::DMI_ACCESS_READ )
                && (  m_granted_access != tlm::tlm_dmi::DMI_ACCESS_NONE)){
         msg << " correct address and appropriate access for a GP Write Command "<<endl<<"      ";
         REPORT_INFO(filename, __FUNCTION__, msg.str());
         return_status=true;
         }
       } //end if
  else if (        ( gp.get_command () == tlm::TLM_READ_COMMAND          )
          )
      {
        if ( (m_granted_access != tlm::tlm_dmi::DMI_ACCESS_WRITE  )
                && (  m_granted_access != tlm::tlm_dmi::DMI_ACCESS_NONE)){
        msg << " correct address and appropriate access for a GP Read Command "<<endl<<"      ";
        REPORT_INFO(filename, __FUNCTION__, msg.str());
        return_status=true;
        }
      } //end if
  else {
     // access permission does not match access required for operation
         msg << " Incompatible GP Command for DMI Access Granted ";
     //    msg << " Gp.getcommand()= " << gp.get_command();
         REPORT_INFO(filename, __FUNCTION__, msg.str());
  } //end else

  return return_status;
} // end check is dmi
