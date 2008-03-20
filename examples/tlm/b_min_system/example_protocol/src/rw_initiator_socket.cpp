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

/*=============================================================================
  @file rw_initiator_socket.cpp
  
  @brief Implementation of rw_initiator_socket
=============================================================================*/
#include "rw_initiator_socket.h"

/*==============================================================================
  Constructor
==============================================================================*/

rw_initiator_socket::rw_initiator_socket(const char * name) 
        : tlm::tlm_initiator_socket<64,
                                    tlm::tlm_fw_b_transport_if<>,
                                    tlm::tlm_bw_b_transport_if>(name)
{
}

/*=============================================================================
  Implementation of function read() of convenience API rw_if

  This function performs the interface between convenience API and
  blocking transport.
=============================================================================*/

bool rw_initiator_socket::read(sc_dt::uint64  bus_address, 
                             unsigned int   byte_count, 
                             unsigned char* read_buffer)
{
    tlm::tlm_generic_payload transaction;

    // Build transaction 
    transaction.set_read();
    transaction.set_address(bus_address);  
    transaction.set_data_ptr(read_buffer);
    transaction.set_data_length(byte_count);

    // Call blocking transport
    (*this)->b_transport(transaction);

    return true; // to be modified
}

/*=============================================================================
  Implementation of function write() of convenience API rw_if

  This function performs the interface between convenience API and
  blocking transport.
=============================================================================*/

bool rw_initiator_socket::write(sc_dt::uint64  bus_address, 
                              unsigned int   byte_count, 
                              unsigned char* read_buffer)
{
    tlm::tlm_generic_payload transaction;

    // Build transaction 
    transaction.set_write();
    transaction.set_address(bus_address);  
    transaction.set_data_ptr(read_buffer);
    transaction.set_data_length(byte_count);

    // Call blocking transport
    (*this)->b_transport(transaction);

    return transaction.is_response_ok();
}
