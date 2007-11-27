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
    , bus_port("bus_port")
    , m_endianness(endianness)
{
    bus_port.bind(*this);
    
    m_socket_width = bus_port.getBusDataWidth()/8; // bus data width in bytes
}

void tlm_target::nb_transport(tlm::tlm_generic_payload* gp)
{    
    unsigned int      addr              = (unsigned int)gp->get_address() & 0x0000007;
    unsigned char* data              = gp->get_data_ptr();
    unsigned int      data_length   = gp->get_data_length();
    unsigned int      data_offset    = 0;
    bool*                 m_be             = gp->get_byte_enable_ptr();
    unsigned int      m_be_length = gp->get_byte_enable_length();
    unsigned int      m_be_offset  = 0;
    unsigned int     nr_bytes;

	// Three conditions:
	// 1) unaligned address
	// 2) aligned addresses of size socket width
	// 3) remaining bytes less than socket width

	// 1) unaligned address
	if(addr%m_socket_width)
	{
		nr_bytes = ((addr/m_socket_width+1)*m_socket_width) - addr;

		if(nr_bytes>gp->get_data_length()) nr_bytes = gp->get_data_length();

		 if(tlm::hasHostEndianness(m_endianness))
			data_offset = (addr%m_socket_width);
		else
			data_offset = m_socket_width - (addr%m_socket_width) - nr_bytes;


		if(gp->is_write())
			tlm::copy_word_from_array< unsigned int >(mem[addr/m_socket_width], data_offset , nr_bytes, data, m_be, m_be_length);
		else
			tlm::copy_word_to_array< unsigned int >(mem[addr/m_socket_width], data_offset , nr_bytes, data, m_be, m_be_length);

		data_length -= nr_bytes;
		addr += nr_bytes;
		data += m_socket_width;
		if(m_be)
		{
			m_be_offset =  (m_be_offset + m_socket_width) % m_be_length;
			m_be += m_be_offset;
		}
	}

	// 2) aligned addresses of size socket width
	while(data_length >= m_socket_width)
	{
		if(gp->is_write())
			tlm::copy_word_from_array< unsigned int >(mem[addr/m_socket_width], 0, m_socket_width, data, m_be, m_be_length);
		else
			tlm::copy_word_to_array< unsigned int >(mem[addr/m_socket_width], 0, m_socket_width, data, m_be, m_be_length);

		data_length -= m_socket_width;
		addr += m_socket_width;
		data += m_socket_width;
		if(m_be)
		{
			m_be_offset =  (m_be_offset + m_socket_width) % m_be_length;
			m_be += m_be_offset;
		}
	}
	
	// 3) remaining bytes less than socket width
	if(data_length > 0)
	{
		if(tlm::hasHostEndianness(m_endianness))
		  data_offset = 0;
		else
		  data_offset = m_socket_width - data_length;;

		if(gp->is_write())
			tlm::copy_word_from_array< unsigned int >(mem[addr/m_socket_width], data_offset , data_length, data, m_be, m_be_length);
		else
			tlm::copy_word_to_array< unsigned int >(mem[addr/m_socket_width], data_offset, data_length, data, m_be, m_be_length);
	}

    gp->set_response_status(tlm::TLM_OK_RESPONSE);
}

