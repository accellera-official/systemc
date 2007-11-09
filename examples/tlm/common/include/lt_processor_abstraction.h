/*****************************************************************************
 *
 *   The following code is derived, directly or indirectly, from the SystemC
 *   source code Copyright (c) 1996-2007 by all Contributors.
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

  @file lt_processor_abstraction 
  
  @brief Loosely Time (LT) Processor Abstraction, an abstract model of a processor 
  
  @Details
  The LT processor abstraction is not an instructions set simulator (ISS), it 
  is an abstraction of a processor used to model bus traffic typical of a 
  processor executing an operating system and applications.  This abstract 
  processor model is used by the TLM 2 examples to replace instructions set 
  simulators in multi-master bus models. The abstract processor model is composed 
  of two threads: a traffic generator thread and a direct execution thread. 
  The traffic generator thread models processor execution. The direct execution 
  thread can be used to model device drivers or to implement directed test. 
   
===============================================================================*/
#ifndef __LT_PROCESSOR_ABSTRACTION_H__
#define __LT_PROCESSOR_ABSTRACTION_H__

#include "lt_initiator.h"
#include "traffic_generator.h"
//#include "direct_execution.h"

class lt_processor_abstraction                     ///< lt_processor_abstraction 
  : public sc_core::sc_module                      ///< module base class
  , virtual public tlm::tlm_bw_nb_transport_if<>   ///< initiator socket
{
  public:
//==============================================================================
// TLM sockets
//==============================================================================
    tlm::tlm_initiator_socket<> processor_abstraction_socket; ///< processor socket

//==============================================================================
// Constructors & Destructor 
//==============================================================================
    lt_processor_abstraction(                 ///< constructor
      sc_core::sc_module_name name);          ///< name

    ~lt_processor_abstraction(void);          ///< destructor

//==============================================================================
// Private Member methods and variables
//==============================================================================
  private:
  lt_initiator                m_initiator;          ///< initiator
  traffic_generator_config    m_tg_config;          ///< traffic generator configuration
  traffic_generator           m_traffic_generator;  ///< traffic generator
//  direct_execution          m_direct_execution;   ///< direct execution

//==============================================================================
// manditory virtual implementations
//==============================================================================
  void  invalidate_direct_mem_ptr(sc_dt::uint64,sc_dt::uint64){}
  
  tlm::tlm_sync_enum
  nb_transport
  ( tlm::tlm_generic_payload  &payload
  , tlm::tlm_phase            &phase
  , sc_core::sc_time          &delta
  )
  {return tlm::TLM_REJECTED;}

};
#endif /* __LT_PROCESSOR_ABSTRACTION_H__ */
