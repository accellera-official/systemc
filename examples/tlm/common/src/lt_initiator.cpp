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
//    Bill Bunton, ESLX
//    Charles Wilson, ESLX
//    Anna Keist, ESLX
//==============================================================================

#include "reporting.h"                             ///< Reporting convenience macros
#include "lt_initiator.h"                          ///< Our header
#include "tlm.h"                                   ///< TLM headers

using namespace sc_core;

static const char *filename = "lt_initiator.cpp";  ///< filename for reporting

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
, const unsigned int ID                           // initiator ID
, sc_core::sc_time end_rsp_delay                  // delay
)
: sc_module           (name)                      // initialize module name
, initiator_socket    ("initiator_socket")        // initiator socket
, m_ID                (ID)                        // initialize initiator ID

{ 
  // bind initiator to the export
  initiator_socket(*this);                     

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
    report::print(m_ID,*transaction_ptr);
  
    sc_time delay         = SC_ZERO_TIME;       // Create delay objects 

    std::ostringstream  msg;           
    msg.str("");
    
    msg << "Initiator: " << m_ID               
        << " b_transport(GP, " 
        << delay << ")"
        << endl << "      ";
    REPORT_INFO(filename,  __FUNCTION__, msg.str());

    initiator_socket->b_transport(*transaction_ptr, delay);
    
    /// @todo Need to check transaction status after blocking call
    msg.str("");
    msg << "Initiator: " << m_ID
        << " (GP, " << delay << ")" << endl; 
    REPORT_INFO(filename,  __FUNCTION__, msg.str());
    
    wait(delay);

  } // end while true
} // end initiator_thread 


//==============================================================================
///  @fn lt_initiator::invalidate_direct_mem_ptr
///
///  @brief invalidate direct memory pointer
///
///  @details
///
//==============================================================================

void lt_initiator::invalidate_direct_mem_ptr  // invalidate_direct_mem_ptr
( sc_dt::uint64 start_range                       // start range
, sc_dt::uint64 end_range                         // end range
) 
{
  std::ostringstream       msg;                     // log message

  msg.str ("");
  msg << m_ID << " - invalidate_direct_mem_ptr: not implemented";
  REPORT_INFO(filename, __FUNCTION__, msg.str());
}

//===================================================================== 
tlm::tlm_sync_enum 
lt_initiator::nb_transport_bw
(              
   tlm::tlm_generic_payload& transaction,          // transaction
   tlm::tlm_phase&           phase,                // transaction phase
   sc_core::sc_time&         time                  // elapsed time
)  
{
    std::ostringstream       msg;          // log message
    msg.str ("");
        
    msg << "Initiator: " << m_ID << " Not implemented for this example";
    REPORT_WARNING(filename, __FUNCTION__, msg.str());     
    /// @todo need to return an error status in GP
    return tlm::TLM_UPDATED;
}

