/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

#include "tlm_slave.h"

//#define TLM_MEMORY_ENABLE

// constructor
tlm_slave::tlm_slave(sc_module_name _name, 
                   unsigned int start_address, 
                   unsigned int end_address)
	: sc_module(_name)
	, bus_port("bus_port")
	, m_start_address(start_address)
	, m_end_address(end_address)
	, m_mem(end_address-start_address+1, tlm::TLM_LITTLE_ENDIAN)
	, m_mem_size(end_address-start_address+1)
	, m_checker(start_address, end_address, bus_port.getBusDataWidth()/8)
{
	assert((m_end_address-m_start_address+1)%4 == 0);
	
	bus_port.bind(*this);
	
	m_incr_address = bus_port.getBusDataWidth()/8; // bus data width in bytes

	m_checker.burst_not_supported();
}

void tlm_slave::nb_transport(tlm::tlm_generic_payload* gp)
{
	// Generic Payload Protocol
	tlm::tlm_response_status m_response_status;

	if(m_checker.transactionIsValid(gp))
	{
#ifdef TLM_MEMORY_ENABLE
            unsigned int addr = (unsigned int)gp->get_address() - m_start_address;
#endif
            unsigned char* data = gp->get_data_ptr();

		if(gp->get_command() == tlm::TLM_WRITE_COMMAND)
		{
#ifdef TLM_MEMORY_ENABLE
			// copy data in memory
			m_mem.write(data,addr,gp->get_length());
#else				
			// alternative copy data to a specific data type
			switch(gp->get_length())
			{
			case 1: // 8 bits
				tlm::copy_from_array< unsigned char >(reg8,0,data);
				break;
			case 2: // 16-bits
				tlm::copy_from_array< unsigned short >(reg16,0,data);
				break;
			case 4: // 32-bits 
				tlm::copy_from_array< unsigned int >(reg32,0,data);
				break;
			case 6: // 48-bits
				tlm::copy_from_array< sc_uint<48> >(reg48,0,data);
				break;
			case 8: // 64-bits
				tlm::copy_from_array< unsigned long long >(reg64,0,data);
				break;
			case 16: // 128-bits
				tlm::copy_from_array< sc_biguint<128> >(reg128,0,data);
				break;
			case 32: // 256-bits
			default:
				tlm::copy_from_array< sc_biguint<256> >(reg256,0,data);
				break;
			}			
#endif					
			m_response_status = tlm::TLM_OK_RESP;
		}
		else // TLM_READ_COMMAND
		{
#ifdef TLM_MEMORY_ENABLE
			// copy data to GP
			m_mem.read(data,addr,gp->get_length());
#else				
			// alternative copy data from a specific data type
			switch(gp->get_length())
			{
			case 1: // 8 bits
				tlm::copy_to_array< unsigned char >(reg8,0,data);
				break;
			case 2: // 16-bits
				tlm::copy_to_array< unsigned short >(reg16,0,data);
				break;
			case 4: // 32-bits 
				tlm::copy_to_array< unsigned int >(reg32,0,data);
				break;
			case 6: // 48-bits
				tlm::copy_to_array< sc_uint<48> >(reg48,0,data);
				break;
			case 8: // 64-bits
				tlm::copy_to_array< unsigned long long >(reg64,0,data);
				break;
			case 16: // 128-bits
				tlm::copy_to_array< sc_biguint<128> >(reg128,0,data);
				break;
			case 32: // 256-bits
			default:
				tlm::copy_to_array< sc_biguint<256> >(reg256,0,data);
				break;
			}
#endif					
			m_response_status = tlm::TLM_OK_RESP;
		}
	}
	else
	{
		m_response_status = m_checker.get_response_status();
	}

	gp->set_response_status(m_response_status);
}


