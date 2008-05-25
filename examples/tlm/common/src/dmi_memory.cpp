/**********************************************************************
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
 *********************************************************************/
//=====================================================================
///  @file dmi_memory.cpp
//
///  @Details
///  
//
//=====================================================================
//  Original Authors:
//    Jack Donovan, ESLX
//=====================================================================

#include "reporting.h"                      // Reporting convenience macros
#include "dmi_memory.h"                         // Our header

using namespace sc_core;

static const char *filename = "dmi_memory.cpp"; ///< filename for reporting

/// Constructor

dmi_memory::dmi_memory    
(
  const unsigned int ID                  // Target ID
)
: m_ID                 (ID)   
{ 

} // end Constructor

//=====================================================================
///  @fn memory::memory_operation
//  
///  @brief helper function for read and write processing
// 
///  @details
///    This routine implements the read and  write operations
//   
//=====================================================================
void 
dmi_memory::operation     
( 
    tlm::tlm_generic_payload  &gp     
  , sc_core::sc_time          &delay_time   ///< transaction delay 
)    
{
  std::ostringstream  msg;   
  msg.str("");
  
  m_address   = gp.get_address(); 
  m_data      = gp.get_data_ptr();    ///< data pointer
  m_length    = gp.get_data_length(); ///< data length
  m_command   = gp.get_command();
  bool m_is_dmi_flag = false;
   
  
  if (address_is_dmi(gp))
    {
      m_dmi_ptr           = m_dmi_properties.get_dmi_ptr();
      m_dmi_read_latency  = m_dmi_properties.get_read_latency();
      m_dmi_write_latency = m_dmi_properties.get_write_latency();
      m_dmi_size          = m_dmi_properties.get_end_address()
                          - m_dmi_properties.get_start_address();
      m_offset            = m_address
                          - m_dmi_properties.get_start_address();
      m_is_dmi_flag       =  true;
     }
 
  
  if(!m_is_dmi_flag) 
  {
    msg << "Iniiator:" << m_ID 
        << "A GP with an address not in the allowed DMI ranges has been received" 
        << endl << "       "
        << "Use check_status before passing a gp to operation";
    REPORT_FATAL(filename, __FUNCTION__, msg.str());
  }
 
 
    
  switch (m_command)
  {
    case tlm::TLM_WRITE_COMMAND:
    {
      for (unsigned int i = 0; i < m_length; i++)
       {
          m_dmi_ptr[m_offset + i] = m_data[i];
        }
          
      // update time
      wait(m_dmi_write_latency);
        
      break;     
    }
      
    case tlm::TLM_READ_COMMAND:
    {
      // clear read buffer
      memset(m_data, 0x00, m_length);
          
      for (unsigned int i = 0; i < m_length; i++)
        {   
        // Need a print message
        m_data[i] = m_dmi_ptr [m_offset + i];
        }  
          
      // update time
      wait (m_dmi_read_latency);
    
      break;
    }
      
    default:
    {
      msg << "Target: " << m_ID 
          << " Unknown GP command that could be ignorable";
      REPORT_WARNING(filename, __FUNCTION__, msg.str());
      break;
    }
     
    }//end switch

  
} // end memory_operation
 void
dmi_memory::invalidate_dmi_ptr
( sc_dt::uint64     start_range
, sc_dt::uint64     end_range
)
{
  m_dmi_properties.set_start_address(1);
  m_dmi_properties.set_end_address(0);
}
 
void
dmi_memory::load_dmi_ptr
( tlm::tlm_dmi &dmi_properties
)
{ 
  report::print(m_ID,m_dmi_properties);
  m_dmi_properties = dmi_properties;
}
 
bool
dmi_memory::address_is_dmi
(
  tlm::tlm_generic_payload  &gp        
)
{
  m_start_address = gp.get_address();
  m_end_address   = m_start_address + gp.get_data_length();
  
  std::ostringstream  msg;   
  msg.str("");
  bool                return_status = true;
  
  if(!( (m_start_address >= m_dmi_properties.get_start_address()) &&
        (m_end_address   <= m_dmi_properties.get_end_address())        ))
    {  
      // print debug message?
      return_status = false;
    }
  
  if( ((gp.get_command()==tlm::TLM_WRITE_COMMAND) && 
       m_dmi_properties.is_read_allowed()              ) 
      ||
      (gp.get_command()!=tlm::TLM_READ_COMMAND &&
       gp.get_command()!=tlm::TLM_WRITE_COMMAND   )
    )
    {
      //print debug message?
      return_status = false;
    } //end if

  return return_status;
} // end check is dmi


 
