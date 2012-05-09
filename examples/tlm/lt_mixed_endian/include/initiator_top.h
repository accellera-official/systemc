/**********************************************************************
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
 *********************************************************************/

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

#include "tlm.h"                                    // TLM headers
//#include "simple_initiator_socket.h"
#include "lt_initiator.h"                   // AT initiator
#include "me_traffic_generator.h"                      // traffic generator

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
  ( sc_core::sc_module_name name                 ///< module name
  , const unsigned int  ID                       ///< initiator ID
  , sc_dt::uint64       base_address_1           ///< first base address
  , sc_dt::uint64       base_address_2           ///< second base address
  );

  void
  invalidate_direct_mem_ptr
  ( sc_dt::uint64      start_range
  , sc_dt::uint64      end_range
  );

  tlm::tlm_sync_enum
  nb_transport_bw
  ( tlm::tlm_generic_payload  &payload
  , tlm::tlm_phase            &phase
  , sc_core::sc_time          &delta
  );

private:


//Member Variables/Objects  ===========================================

public:

//  tlm_utils::simple_initiator_socket<initiator_top> top_initiator_socket;
//  typedef tlm::tlm_fw_transport_if<>      fw_interface_type;
//  typedef tlm::tlm_bw_transport_if<>      bw_interface_type;
//  typedef tlm::tlm_initiator_socket<32,
//                                    fw_interface_type,
//                                    bw_interface_type> initiator_socket_type;
//  initiator_socket_type top_initiator_socket;

  tlm::tlm_initiator_socket< > top_initiator_socket;

private:

  typedef tlm::tlm_generic_payload  *gp_ptr;   ///< Generic Payload pointer

  sc_core::sc_fifo <gp_ptr>  m_request_fifo;   ///< request SC FIFO
  sc_core::sc_fifo <gp_ptr>  m_response_fifo;  ///< response SC FIFO

  const unsigned int         m_ID;             ///< initiator ID

  lt_initiator               m_initiator;      ///< TLM initiator instance
  me_traffic_generator          m_traffic_gen;    ///< traffic generator instance

};

#endif /* __INITIATOR_TOP_H__ */
