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
 @file lt_processor_abstraction.h
 
 @brief lt_processor_abstraction class header
 
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

#ifndef __LT_PROCESSOR_ABSTRACTION_H__
#define __LT_PROCESSOR_ABSTRACTION_H__

#include "tlm.h"                                    ///< TLM headers
#include "lt_initiator_endianness.h"                ///< LT initiator
#include "traffic_generator.h"                      ///< traffic generator

class lt_processor_abstraction                      ///< lt_processor_abstraction
  : public sc_core::sc_module                       ///< SC module
  , virtual public tlm::tlm_bw_nb_transport_if<>    ///< backward non-blocking interface
{
  public:
  
  // Constructors / Destructor 

  lt_processor_abstraction                          ///< constructor
  ( sc_core::sc_module_name   name                  ///< module name
  , const unsigned int        ID                    ///< initiator ID
  , const tlm::tlm_endianness endianness            ///< initiator endianness
  , const unsigned long       seed                  ///< random number generator seed
  , const unsigned int        message_count         ///< number of messsages to generate
  );

  ~lt_processor_abstraction                         ///< destructor
  ( void
  ); 

  // Private Member methods and variables

  private:
  
  typedef tlm::tlm_generic_payload  *gp_ptr;
  
  sc_core::sc_fifo <gp_ptr>   m_request_fifo;       ///< request SC FIFO
  sc_core::sc_fifo <gp_ptr>   m_response_fifo;      ///< response SC FIFO
  
  const unsigned int          m_ID;                 ///< initiator ID
  const tlm::tlm_endianness   m_endianness;         ///< initiator endianness
  const unsigned long         m_seed;               ///< random number generator seed
  
  public:
  
  tlm::tlm_initiator_socket<> lt_processor_socket;  ///< processor socket

  private:
  
  lt_initiator                m_initiator;          ///< initiator instance
  traffic_generator           m_traffic_gen;        ///< traffic generator instance

  // manditory virtual implementations

  void
  invalidate_direct_mem_ptr                         ///< invalidate_direct_mem_ptr
  ( sc_dt::uint64                                   ///< starting address
  , sc_dt::uint64                                   ///< ending address
  )
  {
  }
  
  tlm::tlm_sync_enum
  nb_transport
  ( tlm::tlm_generic_payload  &payload
  , tlm::tlm_phase            &phase
  , sc_core::sc_time          &delta
  )
  {
    return tlm::TLM_REJECTED;
  }
};

#endif /* __LT_PROCESSOR_ABSTRACTION_H__ */
