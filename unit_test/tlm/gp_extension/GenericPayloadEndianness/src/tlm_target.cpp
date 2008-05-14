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
#include "tlm_target.h"

// constructor
tlm_target::tlm_target(sc_core::sc_module_name name_, 
                       tlm::tlm_endianness endianness)
    : sc_core::sc_module(name_)
    , socket("socket")
    , m_endianness(endianness)
{
  // register nb_transport method
  socket.register_nb_transport_fw(this, &tlm_target::myNBTransport);

  m_socket_width = socket.get_bus_width()/8; // bus data width in bytes
}

tlm::tlm_sync_enum tlm_target::myNBTransport(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& t)
{    
    unsigned int      addr          = (unsigned int)trans.get_address() & 0x0000007;
    unsigned char*    data          = trans.get_data_ptr();
    unsigned int      data_length   = trans.get_data_length();
    unsigned int      data_offset   = 0;
    unsigned char*    m_be          = trans.get_byte_enable_ptr();
    unsigned int      m_be_length   = trans.get_byte_enable_length();
    unsigned int      m_be_offset   = 0;
    
	unsigned int      nr_bytes;
	unsigned int      unalignment;

	// Three paths:
	// 1) unaligned address
	// 2) aligned addresses of size socket width
	// 3) remaining bytes less than socket width

	while(data_length > 0)
	{
		unalignment = addr%m_socket_width; 

		// Calculate the nr_bytes to copy (one of the 3 options above)
		if(unalignment != 0)
			nr_bytes = ((addr/m_socket_width+1)*m_socket_width) - addr;
		else if(data_length >= m_socket_width)
			nr_bytes = m_socket_width;
		else
			nr_bytes = data_length;

		// Security check (specially for unalignment case)
		if(nr_bytes > trans.get_data_length()) nr_bytes = trans.get_data_length();

		// Calculate the offset in the data array to use in the tlm::copy_word_from/to_array functions
		if(tlm::hasHostEndianness(m_endianness))
			data_offset = unalignment;
		else
			data_offset = m_socket_width - unalignment - nr_bytes;

		// Write or Read
		if(trans.is_write())
			tlm::copy_word_from_array< unsigned int >(mem[addr/m_socket_width], data_offset , nr_bytes, data, m_be, m_be_length);
		else
			tlm::copy_word_to_array< unsigned int >(mem[addr/m_socket_width], data_offset , nr_bytes, data, m_be, m_be_length);

		// Update local variables
		data_length -= nr_bytes;
		addr += nr_bytes;
		data += m_socket_width;
		if(m_be)
		{
			m_be_offset =  (m_be_offset + m_socket_width) % m_be_length;
			m_be += m_be_offset; // TO DO: m_be_offset could be input to the tlm::copy_word_from/to_array functions
		}
	}
	
    trans.set_response_status(tlm::TLM_OK_RESPONSE);

    // LT slave
    // - always return true
    // - not necessary to update phase (if true is returned)
    return tlm::TLM_COMPLETED; 
}

