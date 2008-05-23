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
 *  lt_initiator.h
 *  $Id:$
 * @brief
 * Changed from mini-spec TBD by Bill
 * @Details
 * Changed from mini-spec TBD by Bill
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

#ifndef __LT_INITIATOR_H__
#define __LT_INITIATOR_H__

#include "tlm.h"                                      ///< TLM headers
#include "examples_rw_if.h"                           ///< examples read/write interface
 
class lt_initiator                                 ///< lt_initiator 
  : public sc_core::sc_module                      ///< module base class
  , virtual public tlm::tlm_bw_transport_if<>      ///< initiator socket
  , virtual public examples_rw_if                  ///< export for socket msgs
{
  public:
  //==============================================================================
  // Ports, exports and sockets
  //==============================================================================
  sc_core::sc_export<examples_rw_if>  rw_export;          ///< read/write export
  tlm::tlm_initiator_socket<>         initiator_socket;   ///< initiator socket

  //==============================================================================
  // Constructors & Destructor 
  //==============================================================================
    /// Constructor
    lt_initiator(                                 ///< constructor
      sc_core::sc_module_name name);              ///< name

    /// Destructor
    ~lt_initiator(void);                          ///< destructor

  //==============================================================================
  // Member methods
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

  /// read() - used by other modules connected to the export
  /// to read from the socket
  bool                                    ///< success/failure
    read(                                 ///< read
      sc_dt::uint64 bus_address,          ///< bus address
      unsigned int  data_transfer_size,   ///< data transfer size
      unsigned char* read_buffer          ///< pointer to read buffer
      );             ///< blocking read

  /// write() - used by other modules connected to the export
  /// to write to the socket
  bool                                    ///< success/failure
    write(                                ///< write
      sc_dt::uint64 bus_address,          ///< bus address
      unsigned int  data_transfer_size,   ///< data transfer size
      unsigned char* write_buffer);       ///< pointer to write buffer
};
#endif /* __LT_INITIATOR_H__ */
