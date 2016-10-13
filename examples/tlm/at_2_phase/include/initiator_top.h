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

//=====================================================================
/// @file initiator_top.h
//
/// @brief Initiator top module contains a traffic generator and an
///        example initiator module unique for the example
//
//=====================================================================
//  Original Authors:
//    Bill Bunton, ESLX
//    Charles Wilson, ESLX
//    Jack Donovan, ESLX
//=====================================================================

#ifndef __INITIATOR_TOP_H__
#define __INITIATOR_TOP_H__

#include "tlm.h"                                // TLM headers
#include "select_initiator.h"                   // AT initiator
#include "traffic_generator.h"                  // traffic generator

class initiator_top
  : public sc_core::sc_module
  , virtual public tlm::tlm_bw_transport_if<>  // backward non-blocking interface
{
//Member Methods  =====================================================

  public:

//=====================================================================
///  @fn initiator_top::initiator_top
//
///  @brief initiator_top constructor
//
///  @details
///    Initiator top module contains a traffic generator and an example
///    unique initiator module
//
//=====================================================================
  initiator_top
  ( sc_core::sc_module_name name                ///< module name
  , const unsigned int  ID                      ///< initiator ID
  , sc_dt::uint64       base_address_1          ///< first base address
  , sc_dt::uint64       base_address_2          ///< second base address
  , unsigned int        active_txn_count        ///< Max number of active transactions
  );

private:

/// Not Implemented for this example but required by the initiator socket
  void
  invalidate_direct_mem_ptr
  ( sc_dt::uint64      start_range
  , sc_dt::uint64      end_range
  );

/// Not Implemented for this example but require by the initiator socket
  tlm::tlm_sync_enum
  nb_transport_bw
  ( tlm::tlm_generic_payload  &payload
  , tlm::tlm_phase            &phase
  , sc_core::sc_time          &delta
  );

//Member Variables/Objects  ===========================================

  public:

  tlm::tlm_initiator_socket<> initiator_socket;	///< processor socket

  private:

  typedef tlm::tlm_generic_payload  *gp_ptr;   ///< Generic Payload pointer

  sc_core::sc_fifo <gp_ptr>  m_request_fifo;   ///< request SC FIFO
  sc_core::sc_fifo <gp_ptr>  m_response_fifo;  ///< response SC FIFO

  const unsigned int         m_ID;             ///< initiator ID

  select_initiator           m_initiator;      ///< TLM initiator instance
  traffic_generator          m_traffic_gen;    ///< traffic generator instance

};

#endif /* __INITIATOR_TOP_H__ */
