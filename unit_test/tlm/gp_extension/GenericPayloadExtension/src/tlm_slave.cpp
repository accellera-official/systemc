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

#include "tlm_extensions.h"

#include "assert.h"

// constructor
tlm_slave::tlm_slave(sc_core::sc_module_name _name, 
                     unsigned int start_address, 
                     unsigned int end_address)
    : sc_core::sc_module(_name)
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
	m_checker.byte_enable_not_supported();
}


void tlm_slave::nb_transport(tlm::tlm_generic_payload* gp)
{
    // We also test cloning here:
    tlm::tlm_generic_payload* gp_tmp = gp->clone();

    // handle extensions:
    tlm_extension1* ext1;
    tlm_extension2* ext2;
    tlm_extension3* ext3;
    gp_tmp->get_extension(ext1);
    // or: ext1 = static_cast<tlm_extension1*>(gp_tmp->get_extension(tlm_extension1::ID));
    gp_tmp->get_extension(ext2);
    gp_tmp->get_extension(ext3);
    if (ext1)
    {
        std::cout << std::endl << name() << ": got extension 1 with value = " 
                  << ext1->data1;
    }
    if (ext2)
    {
        std::cout << std::endl << name() << ": got extension 2 with value = " 
                  << ext2->data2;
    }
    if (ext3)
    {
        std::cout << std::endl << name() << ": got extension 3 with value = " 
                  << ext3->data3;
    }
    std::cout << std::endl;

    delete gp_tmp;

    // Generic Payload Protocol
    tlm::tlm_response_status m_response_status;
    
    if(m_checker.transactionIsValid(gp))
    {
        unsigned int addr = (unsigned int)gp->get_address() - m_start_address;
        unsigned char* data = gp->get_data_ptr();
        
		if(gp->is_write())
        {
			m_mem.write(data,addr,gp->get_data_length());
            m_response_status = tlm::TLM_OK_RESPONSE;
        }
        else // TLM_READ_COMMAND
        {
			m_mem.read(data,addr,gp->get_data_length());
            m_response_status = tlm::TLM_OK_RESPONSE;
        }
    }
    else
    {
        m_response_status = m_checker.get_response_status();
    }
    
    gp->set_response_status(m_response_status);
}


