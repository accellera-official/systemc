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
/*==============================================================================
  @file lt_initiator.h
  @brief This is the TLM AT initiator

  @Details
  This module implements three interfaces. The first implements the functionality
  required to access the tlm_master_socket. The second and third interfaces 
  implements the functionality required to allow other modules to pass generic 
  payloads via an sc_fifo.
=============================================================================*/
   
/*****************************************************************************
  Original Authors:
    Anna Keist, ESLX
    Bill Bunton, ESLX
*****************************************************************************/

#ifndef __LT_INITIATOR_H__
#define __LT_INITIATOR_H__

#include "tlm.h"                                    // TLM headers


class lt_initiator                                  // lt_initiator 
  :  public sc_core::sc_module               // module base class 
  , public virtual tlm::tlm_bw_transport_if<>
{
public:
//Constructor ==================================================================
  
    lt_initiator                                // constructor
    ( sc_core::sc_module_name name              // module name
    , const unsigned int ID                     // initiator ID
    , sc_core::sc_time end_rsp_delay            // delay
    );
     
// Method Declarations =========================================================
  private:
  void initiator_thread (void);                     // initiator thread
//=============================================================================
///     @brief Implementation of call from targets. 
//
///     @details
///             This is the ultimate destination of the nb_transport_bw call from
///             the targets after being routed trough a Bus
//
//===================================================================== 
   tlm::tlm_sync_enum nb_transport_bw(              // nb_transport
    tlm::tlm_generic_payload& transaction,          // transaction
    tlm::tlm_phase&           phase,                // transaction phase
    sc_core::sc_time&         time);                // elapsed time


//==============================================================================
// Required but not implemented member methods
//============================================================================== 
  void invalidate_direct_mem_ptr(                   // invalidate_direct_mem_ptr
    sc_dt::uint64 start_range,                      // start range
    sc_dt::uint64 end_range);                       // end range
  
  
//Variable and Object Declarations =============================================
public:
   typedef tlm::tlm_generic_payload  *gp_ptr;        // generic payload
   typedef tlm::tlm_fw_transport_if<>      fw_interface_type;
   typedef tlm::tlm_bw_transport_if<>      bw_interface_type;
   typedef tlm::tlm_initiator_socket<32,
                                     fw_interface_type,
                                     bw_interface_type> initiator_socket_type;

    sc_core::sc_port<sc_core::sc_fifo_in_if  <gp_ptr> > request_in_port;  
    sc_core::sc_port<sc_core::sc_fifo_out_if <gp_ptr> > response_out_port;
    
    initiator_socket_type                               initiator_socket; 

private:
  unsigned int            m_ID;                     // initiator ID
  sc_core::sc_time        m_end_rsp_delay;          // end response delay
  
}; 
 #endif /* __LT_INITIATOR_H__ */
