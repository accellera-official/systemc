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
//==============================================================================
///  @file lt_initiator.cpp
///  @Details
///  
///
//==============================================================================
//  Original Authors:
//    Jack Donovan, ESLX
//    Charles Wilson, ESLX
//    Anna Keist, ESLX
//==============================================================================

#include "reporting.h"                             ///< Reporting convenience macros
#include "lt_initiator.h"                          ///< Our header
#include "tlm.h"                                   ///< TLM headers

using namespace sc_core;
static const char *filename = "lt_initiator.cpp"; ///< filename for reporting

//==============================================================================
///  @fn lt_initiator::lt_initiator
///
///  @brief class constructor
///
///  @details
///    This is the class constructor.
///
//==============================================================================
SC_HAS_PROCESS(lt_initiator);
lt_initiator::lt_initiator                        // constructor
( sc_module_name name                             // module name
, const unsigned int  ID                          // initiator ID
)
: sc_module           (name)                      // initialize module name
, initiator_socket    ("initiator_socket")        // initiator socket
, m_ID                (ID)                        // initialize initiator ID

{                

  // register thread process
  SC_THREAD(initiator_thread);                  
}

/*==============================================================================
///  @fn lt_initiator::initiator_thread
///
///  @brief initiates non-blocking transport
///
///  @details
/// 
==============================================================================*/
void lt_initiator::initiator_thread(void)   ///< initiator thread
{  
  tlm::tlm_generic_payload *transaction_ptr;    ///< transaction pointer
  std::ostringstream       msg;                 ///< log message

  while (true) 
  {
//=============================================================================
// Read FIFO to Get new transaction GP from the traffic generator 
//=============================================================================
    transaction_ptr = request_in_port->read();  // get request from input fifo
  
    sc_time delay         = SC_ZERO_TIME;       // Create delay objects 

    std::ostringstream  msg;           
    msg.str("");
    
    msg << "Initiator: " << m_ID               
        << " b_transport(GP, " 
        << delay << ")";
    REPORT_INFO(filename,  __FUNCTION__, msg.str());

    initiator_socket->b_transport(*transaction_ptr, delay);
    
    gp_status = transaction_ptr->get_response_status();
    
    if(gp_status == tlm::TLM_OK_RESPONSE)
    {
       msg.str("");
       msg << "Initiator: " << m_ID               
          << " b_transport returned delay = " 
          << delay;
       REPORT_INFO(filename,  __FUNCTION__, msg.str());
       wait(delay);
    }
    else
    {
      msg << "Initiator: " << m_ID               
          << "Bad GP status returned = " << gp_status;
       REPORT_WARNING(filename,  __FUNCTION__, msg.str());
    }
    
    response_out_port->write(transaction_ptr);  // return txn to traffic gen
  } // end while true
} // end initiator_thread 



