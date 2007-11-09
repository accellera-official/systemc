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
                       unsigned int start_address, 
                       unsigned int end_address)
    : sc_core::sc_module(name_)
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
    
    m_checker.burst_mode_streaming_not_supported();
    m_checker.byte_enable_not_supported();
}

void tlm_target::nb_transport(tlm::tlm_generic_payload* gp)
{
    // Generic Payload Protocol
    tlm::tlm_response_status m_response_status;
    
    if(m_checker.transactionIsValid(gp))
    {
        unsigned int addr = (unsigned int)gp->get_address() - m_start_address;
        unsigned char* data = gp->get_data_ptr();
        
        if(gp->is_write())
        {
            // Burst write transaction 
            std::cout << " ( target : write : burst_length<";
            std::cout << get_burst_length(gp->get_data_length(),
                                          m_incr_address) << "> )";
            
            for(unsigned int bl=0;
                bl<get_burst_length(gp->get_data_length(), m_incr_address);
                bl++)
            {
                m_mem.write(data,
                            addr,
                            get_nr_bytes_of_burst_element(bl, gp->get_data_length(),
                                                          m_incr_address));
                addr += m_incr_address;
                data += get_nr_bytes_of_burst_element(bl,
                                                      gp->get_data_length(),
                                                      m_incr_address);
            }
            m_response_status = tlm::TLM_OK_RESPONSE;
        }
        else // TLM_READ_COMMAND
        {
            // Burst read transaction 
            std::cout << " ( target : read : burst_length<";
            std::cout << get_burst_length(gp->get_data_length(),
                                          m_incr_address) << "> )";
            
            for(unsigned int bl=0;
                bl<get_burst_length(gp->get_data_length(), m_incr_address);
                bl++)
            {
                m_mem.read(data,
                           addr,
                           get_nr_bytes_of_burst_element(bl,
                                                         gp->get_data_length(),
                                                         m_incr_address));
                addr += m_incr_address;
                data += get_nr_bytes_of_burst_element(bl,
                                                      gp->get_data_length(),
                                                      m_incr_address);
            }
            m_response_status = tlm::TLM_OK_RESPONSE;
        }
    }
    else
    {
        m_response_status = m_checker.get_response_status();
    }
    
    gp->set_response_status(m_response_status);

}


