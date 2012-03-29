/*****************************************************************************
 *
 *   The following code is derived, directly or indirectly, from the SystemC
 *   source code Copyright (c) 1996-2008 by all Contributors.
 *   All Rights reserved.
 *
 *   The contents of this file are subject to the restrictions and limitations
 *   set forth in the SystemC Open Source License Version 3.0 (the "License");
 *   You may not use this file except in compliance with such restrictions and
 *   limitations. You may obtain instructions on how to receive a copy of the
 *   License at http://www.systemc.org/. Software distributed by Contributors
 *   under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 *   ANY KIND, either express or implied. See the License for the specific
 *   language governing rights and limitations under the License.
 *
 *   *****************************************************************************/

/* ---------------------------------------------------------------------------------------
 @file at_initiator_temporal_decoupling.h
 
 @brief lt_iniator_temporal_decoupling class header
 
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

#ifndef __AT_INITIATOR_TEMPORAL_DECOUPLING_H__
#define __AT_INITIATOR_TEMPORAL_DECOUPLING_H__

#include "tlm.h"                                    ///< TLM headers
#include <queue>                                    ///< standard queue

class at_initiator_temporal_decoupling              ///< at_initiator_temporal_decoupling
:         public sc_core::sc_module                 ///< SC module
, virtual public tlm::tlm_bw_nb_transport_if<>      ///< initiator socket
{
  SC_HAS_PROCESS(at_initiator_temporal_decoupling);

  // Ports, exports and Sockets

  typedef tlm::tlm_generic_payload  *gp_ptr; 
  
  public:
  
  sc_core::sc_port<sc_core::sc_fifo_in_if  <gp_ptr> > request_in_port;
  sc_core::sc_port<sc_core::sc_fifo_out_if <gp_ptr> > response_out_port; 

  // Constructors / Destructor
  
  at_initiator_temporal_decoupling                  ///< constructor
  ( sc_core::sc_module_name name                    ///< module name
  , const unsigned int      ID                      ///< initiator ID
  , sc_core::sc_time end_rsp_delay                  ///< delay
  );

  // Threads and Methods
  
  private:
  
  void
  initiator_thread                                  ///< thread
  ( void
  );

  // backward non-blocking transport method 

  tlm::tlm_sync_enum                                ///< TLM phase
  nb_transport                                      ///< nb_transport
  ( tlm::tlm_generic_payload  &transaction          ///< transaction
  , tlm::tlm_phase            &phase                ///< transaction phase
  , sc_core::sc_time          &time                 ///< elapsed time
  );
  
  // Required but unimplemented member methods

  void
  invalidate_direct_mem_ptr                         ///< invalidate_direct_mem_ptr
  ( sc_dt::uint64 start_range                       ///< start range
  , sc_dt::uint64 end_range                         ///< end range
  );
  
  // Private member variables and methods

  private:

  tlm::tlm_quantumkeeper  m_QuantumKeeper;          ///< quantum keeper
  std::queue<gp_ptr>      m_req_accepted_queue;     ///< request accepted queue
  sc_core::sc_event       m_req_accepted_event;     ///< request accepted event

  const unsigned int      m_ID;                     ///< initiator ID
  sc_core::sc_time        m_end_rsp_delay;      ///< end response delay

  
  public:
  
  tlm::tlm_initiator_socket<> initiator_socket;     ///< initiator socket
};

 #endif /* __AT_INITIATOR_TEMPORAL_DECOUPLING_H__ */
