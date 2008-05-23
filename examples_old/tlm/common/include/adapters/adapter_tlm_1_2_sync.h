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
/*
 * @file
 *  adapter_tlm_1_2_sync.h
 *  $Id:$
 * @brief TLM 1 to TLM 2 loosely time with synchronization 

 * @Details

 *  This module implements two interfaces a TLM 1 slave export and a TLM 2 initiator 
 *  socket. The TLM 1 port implements the base TLM 1 protocol from the TLM 1 kit 
 *  and the TLM 2 socket uses the generic payload. 
 *
 *  The TLM 2 implementation uses LT with synchronization allowing the request and 
 *  response portions of a transaction to be split. 
 *
 *  The TLM 1 base protocol and payload are translated to and from the TLM2 
 *  generic payload. 

 */

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
*****************************************************************************/

#ifndef __ADAPTER_TLM_1_2_H_SYNC__
#define __ADAPTER_TLM_1_2_H_SYNC__

#include "basic_protocol.h"                   // TLM 1 base protocol from TLM 1 kit
#include "basic_slave_base.h"                 // TLM 1 slave base from TLM 1 kit

using basic_protocol::basic_status;
using basic_protocol::basic_slave_base;
using tlm::tlm_transport_if;
 
typedef unsigned int ADDRESS_TYPE;            // define address type of TLM1
typedef unsigned int DATA_TYPE;               // define data type of TLM1

class adapter_tlm_1_2_sync                         ///< adapter_tlm_1_2_sync 
  : public sc_core::sc_module                      ///< module base class
  , virtual public basic_slave_base< ADDRESS_TYPE , DATA_TYPE >  
  , virtual public tlm::tlm_bw_transport_if<>      ///< initiator socket
{
  public:
//==============================================================================
// Ports, exports and sockets
//==============================================================================
  sc_core::sc_export<if_type>  tlm_1_export;       ///< TLM 1 read/write export

  tlm::tlm_initiator_socket<>  initiator_socket;   ///< initiator socket

//==============================================================================
// Constructors & Destructor 
//==============================================================================
    adapter_tlm_1_2_sync(                           ///< constructor
      sc_core::sc_module_name name);                ///< name

    ~adapter_tlm_1_2_sync(void);                    ///< destructor

//==============================================================================
// member methods implement TLM 1 slave interface 
//==============================================================================
//-----------------------------------------------------------------------------
/// read() - used by TLM 1 base slave interface 

  basic_status                          ///< TLM 1 status ERROR or SUCCESS 
    read(                               ///< TLM 1 read 
      const ADDRESS_TYPE &              ///< TLM 1 address from typedef 
    ,       DATA_TYPE &                 ///< TLM 1 data from typedef
    );

//-----------------------------------------------------------------------------
/// write() - used by TLM 1 base slave interface 

    basic_status                        ///< TLM 1 status ERROR or SUCCESS 
    write(                              ///< TLM 1 write 
      const ADDRESS_TYPE &              ///< TLM 1 address from typedef 
    , const DATA_TYPE &                 ///< TLM 1 data from typedef
    );

//==============================================================================
// TLM 2 member methods implement the backwards nb_transport interface 
//==============================================================================
  tlm::tlm_sync_enum nb_transport(                ///< nb_transport
      tlm::tlm_generic_payload  &gp,              ///< transaction
      tlm::tlm_phase            &phase,           ///< transaction phase
      sc_core::sc_time          &delay_time);     ///< elapsed time

//==============================================================================
// TLM 2 member methods not implemented 
//==============================================================================
  /// Not implemented
  void invalidate_direct_mem_ptr(                 ///< invalidate_direct_mem_ptr
      sc_dt::uint64 start_range,                  ///< start range
      sc_dt::uint64 end_range);                   ///< end range

//==============================================================================
// Member data 
//==============================================================================
private:
  sc_core::sc_event m_Response_event;


};
#endif /* __ADAPTER_TLM_1_2_H_SYNC__ */
