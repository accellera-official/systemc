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
 ******************************************************************************/
//==============================================================================
/*==============================================================================

  @file at_processor_abstraction 
  
  @brief Loosely Time (AT) Processor Abstraction, an abstract model of a processor 
  
  @Details
  The AT processor abstraction is not an instructions set simulator (ISS), it 
  is an abstraction of a processor used to model bus traffic typical of a 
  processor executing an operating system and applications.  This abstract 
  processor model is used by the TLM 2 examples to replace instructions set 
  simulators in multi-master bus models. The abstract processor model is composed 
  of two threads: a traffic generator thread and a direct execution thread. 
  The traffic generator thread models processor execution. 

==============================================================================*/

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Anna Keist, ESLX
*****************************************************************************/

#ifndef __AT_PROCESSOR_ABSTRACTION_H__
#define __AT_PROCESSOR_ABSTRACTION_H__

#include "tlm.h"                                  ///< TLM header
#include "at_initiator.h"                         ///< AT initiator
#include "traffic_generator.h"                    ///< TLM traffic generator

class at_processor_abstraction                    ///< at_processor_abstraction
  : public sc_core::sc_module                     ///< module base class
  , virtual public tlm::tlm_bw_transport_if<>     ///< initiator socket interface
{
  public:
//==============================================================================
// TLM socket
//==============================================================================
  tlm::tlm_initiator_socket<> at_processor_abstraction_socket; ///< processor socket

  at_processor_abstraction(                     ///< constructor
    sc_core::sc_module_name name                ///< name
  , const unsigned int      ID                  ///< initiator ID
  , const unsigned int      seed                ///< random number generator seed
  , const unsigned int      message_count       ///< number of messages to generate
  );

  ~at_processor_abstraction(void);              ///< destructor

//==============================================================================
// Private Member methods and variables
//==============================================================================
  private:
  typedef tlm::tlm_generic_payload  *gp_ptr;      ///< generic payload

  unsigned int                m_ID;               ///< initiator ID
  unsigned long               m_seed;             ///< random number generator
  at_initiator                m_initiator;        ///< initiator
  traffic_generator           m_traffic_generator;///< traffic generator
  sc_core::sc_fifo <gp_ptr>   m_request_fifo;     ///< request fifo
  sc_core::sc_fifo <gp_ptr>   m_response_fifo;    ///< response fifo

/*==============================================================================
 mandatory virtual implementations  
 @note
  Both functions, invalidate_direct_mem_ptr and nb_transport are required
  for our socket interface, tlm::tlm_bw_transport_if<>.  The actual
  implementation is found in the initiator.

==============================================================================*/

  void  invalidate_direct_mem_ptr(sc_dt::uint64,sc_dt::uint64){}
  
  tlm::tlm_sync_enum
  nb_transport
  ( tlm::tlm_generic_payload  &payload
  , tlm::tlm_phase            &phase
  , sc_core::sc_time          &delta
  )
  {return tlm::TLM_COMPLETED;}        ///< return a valid TLM sync value

};
#endif /* __AT_PROCESSOR_ABSTRACTION_H__ */
