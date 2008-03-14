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
 @file lt_initiator_direct_memory_interface.h
 
 @brief lt_iniator_direct_memory_interface class header
 
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

#ifndef __LT_INITIATOR_DIRECT_MEMORY_INTERFACE_H__
#define __LT_INITIATOR_DIRECT_MEMORY_INTERFACE_H__

#include "tlm.h"                                      ///< TLM headers
#include <queue>                                      ///< standard queue

class lt_initiator                                  ///< lt_initiator
:         public sc_core::sc_module                 ///< SC module
, virtual public tlm::tlm_bw_nb_transport_if<>      ///< initiator socket
{
  SC_HAS_PROCESS(lt_initiator);

  // Ports, exports and Sockets

  typedef tlm::tlm_generic_payload  *gp_ptr; 
  
  public:
  
  sc_core::sc_port<sc_core::sc_fifo_in_if  <gp_ptr> > request_in_port;
  sc_core::sc_port<sc_core::sc_fifo_out_if <gp_ptr> > response_out_port; 

  // Constructors / Destructor
  
  lt_initiator                                      ///< constructor
  ( sc_core::sc_module_name   name                  ///< module name
  , const unsigned int        ID                    ///< initiator ID
  , const tlm::tlm_endianness endianness            ///< endianness
  , const unsigned long       seed                  ///< random number generator seed
  );

  ~lt_initiator                                     ///< destructor
  ( void
  ); 

  // main processing thread
  
  private:
  
  void
  initiator_thread                                  ///< thread
  ( void
  );

  // backward non-blocking transport 

  tlm::tlm_sync_enum                                ///< TLM phase
  nb_transport                                      ///< nb_transport
  ( tlm::tlm_generic_payload  &transaction          ///< transaction
  , tlm::tlm_phase            &phase                ///< transaction phase
  , sc_core::sc_time          &time                 ///< elapsed time
  );
  
  // invalidate previously acquired DMI pointer

  void
  invalidate_direct_mem_ptr                         ///< invalidate_direct_mem_ptr
  ( sc_dt::uint64 start_range                       ///< start range
  , sc_dt::uint64 end_range                         ///< end range
  );
  
  // Private member variables and methods

  private:

  tlm::tlm_endianness         m_host_endianness;    ///< host endianness (platform)
  tlm::tlm_dmi                m_DMI_read_write;     ///< DMI read/write block
  tlm::tlm_quantumkeeper      m_QuantumKeeper;      ///< quantum keeper
  std::queue<gp_ptr>          m_req_accepted_queue; ///< request accepted queue
  sc_core::sc_event           m_req_accepted_event; ///< request accepted event

  const unsigned int          m_ID;                 ///< initiator ID
  const tlm::tlm_endianness   m_endianness;         ///< endianness
  const unsigned long         m_seed;               ///< random number generator seed
  
  public:
  
  tlm::tlm_initiator_socket<> initiator_socket;     ///< initiator socket
  
  public:
  
  bool                        m_DMI_active;         ///< DMI in use
};

 #endif /* __LT_INITIATOR_DIRECT_MEMORY_INTERFACE_H__ */
