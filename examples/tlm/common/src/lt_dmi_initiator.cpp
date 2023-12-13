/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/
//==============================================================================
///  @file lt_dmi_initiator.cpp
///  @Details
///  
///
//==============================================================================
//  Original Authors:
//    Jack Donovan, ESLX
//    Charles Wilson, ESLX
//    Anna Keist, ESLX
//==============================================================================

//====================================================================
//  Nov 06, 2008
//
//  Updated by:
//    Xiaopeng Qiu, JEDA Technologies, Inc
//    Email:  qiuxp@jedatechnologies.net
//
//  To fix violations of TLM2.0 rules, which are detected by JEDA 
//  TLM2.0 checker.
//
//====================================================================

#include "reporting.h"                             ///< Reporting convenience macros
#include "lt_dmi_initiator.h"                          ///< Our header
#include "tlm.h"                                   ///< TLM headers

using namespace sc_core;
static const char *filename = "lt_dmi_initiator.cpp"; ///< filename for reporting

//==============================================================================
///  @fn lt_dmi_initiator::lt_dmi_initiator
///
///  @brief class constructor
///
///  @details
///    This is the class constructor.
///
//==============================================================================
lt_dmi_initiator::lt_dmi_initiator                // constructor
( sc_module_name name                             // module name
, const unsigned int  ID                          // initiator ID
)
: sc_module           (name)                      // module instance name
, initiator_socket    ("initiator_socket")        // initiator socket
, m_ID                (ID)                        // initialize initiator ID
, m_dmi_memory        (ID)
{                
  // bind initiator to the export
//  initiator_socket (*this);                
   initiator_socket.register_invalidate_direct_mem_ptr
     ( this
     , &lt_dmi_initiator::custom_invalidate_dmi_ptr);
  // register thread process
  SC_THREAD(initiator_thread);                  
}

/*==============================================================================
///  @fn lt_dmi_initiator::initiator_thread
///
///  @brief initiates non-blocking transport
///
///  @details
/// 
==============================================================================*/
void lt_dmi_initiator::initiator_thread(void)   ///< initiator thread
{  
  tlm::tlm_generic_payload *gp_ptr;    ///< transaction pointer
  std::ostringstream       msg;                 ///< log message

  while (true) 
  {
// Read transaction from input port
    gp_ptr = request_in_port->read();  // get request from input fifo
    msg.str("");
   
    sc_time delay         = SC_ZERO_TIME;       // Create delay objects 
       
    msg << "Initiator: " << m_ID               
        << " b_transport with delay of " << delay << ");";
    REPORT_INFO(filename,  __FUNCTION__, msg.str());
    
    if(!(m_dmi_memory.is_address_dmi(*gp_ptr)))
      {
      // Not a dmi transaction =================================================
        m_address = gp_ptr->get_address();
        gp_ptr->set_dmi_allowed(false);
        initiator_socket->b_transport(*gp_ptr, delay);
        
 //       report::print(m_ID, *gp_ptr);
        
    
        m_gp_status = gp_ptr->get_response_status();
        
        if(m_gp_status == tlm::TLM_OK_RESPONSE)
        {
           msg.str("");
           msg << "Initiator: " << m_ID               
              << " b_transport returned a delay of " << delay;
           REPORT_INFO(filename,  __FUNCTION__, msg.str());
           
           wait(delay);
           
           msg.str("");
           msg << "Initiator: " << m_ID               
              << " gp_tpr was returned with dmi_allowed set to " << gp_ptr->is_dmi_allowed();
           REPORT_INFO(filename,  __FUNCTION__, msg.str());
           
        }
        else
        {
          msg << "Initiator: " << m_ID               
              << "Bad GP status returned = " << m_gp_status;
           REPORT_WARNING(filename,  __FUNCTION__, msg.str());
        }
        // Set up for potential dmi utilization for next transaction
        if(gp_ptr->is_dmi_allowed())
          {
            msg.str(""); 
            msg << "Initiator: " << m_ID           
                << " set up dmi for next access to this target"
                << endl << "      ";
            // reset address in case it was mutated
            gp_ptr->set_address(m_address);
            
            msg << "Initiator: " << m_ID  
                << " requesting DMI pointer";
            REPORT_INFO(filename,  __FUNCTION__, msg.str());
            m_dmi_properties.init();
            if(initiator_socket->get_direct_mem_ptr(*gp_ptr,m_dmi_properties))
              {
                m_dmi_memory.load_dmi_ptr(m_dmi_properties);
              }
            else
              {
                msg.str("");
                msg << "Initiator: " << m_ID               
                   << " DMI access refused even though dmi_allowed was set";
                REPORT_INFO(filename,  __FUNCTION__, msg.str());
              }//end else
          
          }// end if dmi_allowed           
      }//end if
    else // dmi transaction ====================================================
      {   
        m_dmi_memory.operation(*gp_ptr, delay);
         
        m_gp_status = gp_ptr->get_response_status();
        
        if(m_gp_status == tlm::TLM_OK_RESPONSE)
        {
           msg.str("");
           msg << "Initiator: " << m_ID               
              << " dmi based transaction returned a delay of " << delay;
           REPORT_INFO(filename,  __FUNCTION__, msg.str());
           wait(delay);
        }
        else
        {
          msg << "Initiator: " << m_ID               
              << "Unknown GP status "
              << report::print(m_gp_status) 
              << " returned from DMI-based transaction ";
          REPORT_WARNING(filename,  __FUNCTION__, msg.str());
        }
      }//end else   
    
    msg.str("");
    msg << "Initiator: " << m_ID               
        << " End of processing";
    REPORT_INFO(filename,  __FUNCTION__, msg.str());
    
    response_out_port->write(gp_ptr);  // return txn to traffic gen
  } // end while true
} // end initiator_thread 

void 
lt_dmi_initiator::custom_invalidate_dmi_ptr      // invalidate_direct_mem_ptr
     ( sc_dt::uint64 start_range    // start range
     , sc_dt::uint64 end_range      // end range
     )
  {
//    std::ostringstream  msg;           
//    msg.str("");
    
//    msg << "Initiator: " << m_ID               
//        << " invalidate_direct_mem_ptr (" 
//        << start_range << " , " << end_range  << ");";
    
//    REPORT_INFO(filename,  __FUNCTION__, msg.str());
    
    m_dmi_memory.invalidate_dmi_ptr(start_range, end_range);
    return;
  }
