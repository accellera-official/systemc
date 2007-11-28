/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
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
 *  adapter_tlm_1_2.h
 *  $Id:$
 * @brief

 * @Details

 * This module implements two interfaces. The first implements the functionality
 * required to access the tlm_master_socket. The second implements the functionality
 * required to allow other modules to read and write the socket by way of an export.
 *  
 */

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
*****************************************************************************/

#ifndef __ADAPTER_TLM_1_2_H__
#define __ADAPTER_TLM_1_2_H__

#include "basic_protocol.h"                       // TLM 1 base protocol 
#include "basic_slave_base.h"                     // TLM 1 slave base 

using basic_protocol::basic_status;
using basic_protocol::basic_slave_base;
using tlm::tlm_transport_if;
 
typedef unsigned int ADDRESS_TYPE;      // define address type of TLM1
typedef unsigned int DATA_TYPE;         // define data type of TLM1


class adapter_tlm_1_2                              ///< adapter_tlm_1_2 
  : public sc_core::sc_module                      ///< module base class
  , virtual public tlm::tlm_bw_nb_transport_if<>   ///< initiator socket
  , virtual public basic_slave_base< ADDRESS_TYPE , DATA_TYPE >  ///< 
{
  public:
  //==============================================================================
  // Ports, exports and sockets
  //==============================================================================
  sc_core::sc_export<if_type>  tlm_1_export;          ///< TLM 1 read/write export
  tlm::tlm_initiator_socket<>  initiator_socket;   ///< initiator socket

  //==============================================================================
  // Constructors & Destructor 
  //==============================================================================
    /// Constructor
    adapter_tlm_1_2(                                 ///< constructor
      sc_core::sc_module_name name);                 ///< name

    /// Destructor
    ~adapter_tlm_1_2(void);                          ///< destructor

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

    basic_status                          ///< TLM 1 status ERROR or SUCCESS 
    write(                              ///< TLM 1 write 
      const ADDRESS_TYPE &              ///< TLM 1 address from typedef 
    , const DATA_TYPE &                 ///< TLM 1 data from typedef
    );

//==============================================================================
// TLM 2 member methods not implemented 
//==============================================================================
  /// Not implemented
  tlm::tlm_sync_enum nb_transport(                ///< nb_transport
      tlm::tlm_generic_payload& transaction,      ///< transaction
      tlm::tlm_phase&           phase,            ///< transaction phase
      sc_core::sc_time&         time);            ///< elapsed time

  /// Not implemented
  void invalidate_direct_mem_ptr(                 ///< invalidate_direct_mem_ptr
      sc_dt::uint64 start_range,                  ///< start range
      sc_dt::uint64 end_range);                   ///< end range

};
#endif /* __ADAPTER_TLM_1_2_H__ */
