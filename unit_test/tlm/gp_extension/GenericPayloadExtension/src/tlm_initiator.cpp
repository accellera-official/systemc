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

#include "tlm_initiator.h"

using namespace std;

// Single WRITE transaction
void tlm_initiator::test_single_write(bool use_ext1,
                                      bool use_ext2,
                                      bool use_ext3)
{
    unsigned int m_address = 0x10; 
    unsigned int m_data = 100;
    
	std::cout << name() << " : Single WRITE transaction : ";
    
    m_gp.set_command(tlm::TLM_WRITE_COMMAND);
    m_gp.set_address(m_address);
    m_gp.set_data_ptr((unsigned char*)&m_data);
    m_gp.set_data_length(sizeof(m_data));
    
    // Add/clear extensions:
    if (use_ext1)
    {
        // set the extension normally:
        m_gp.set_extension(&m_ext1);
 
        // check for mutatability, getting the extension "normally":
        tlm_extension1* p_ext1;
        m_gp.get_extension(p_ext1);
        p_ext1->data1 *= 2;

        // get the extension using the explicit ID
        p_ext1 = static_cast<tlm_extension1 *> (m_gp.get_extension(tlm_extension1::ID));
        p_ext1->data1 /= 2;
    }
    else
    {
        m_gp.clear_extension(&m_ext1);
        // or: m_gp.clear_extension(tlm_extension1::ID);
    }
    if (use_ext2)
    {
        // test setting the extension using the explicit ID 
        m_gp.set_extension(tlm_extension2::ID, &m_ext2);
    }
    else
    {
        m_gp.clear_extension(&m_ext2);
    }
    if (use_ext3)
    {
        m_gp.set_extension(&m_ext3);
    }
    else
    {
        m_gp.clear_extension(&m_ext3);
    }

    bus_port->nb_transport(&m_gp);
    
    if(m_gp.is_response_ok())
    {
        std::cout << " OK " << std::endl;
    }
    else
    {
        std::cout << m_gp.get_response_string() << std::endl;
    }
}

    
// Single READ transaction
void tlm_initiator::test_single_read()
{
    unsigned int m_address = 0x10; 
    unsigned int m_data;
    
    std::cout << name() << " : Single READ transaction : ";
    
    m_gp.set_command(tlm::TLM_READ_COMMAND);
    m_gp.set_address(m_address);
    m_gp.set_data_ptr((unsigned char*)&m_data);
    m_gp.set_data_length(sizeof(m_data));
    

    // Clear extension array using direct indexes:
    m_gp.clear_extension(tlm_extension1::ID);
    m_gp.clear_extension(tlm_extension2::ID);
    m_gp.clear_extension(tlm_extension3::ID);

    bus_port->nb_transport(&m_gp);
    
    if(m_gp.is_response_ok())
    {
        std::cout << " OK, data = "<< m_data << std::endl;
    }
    else
    {
        std::cout << m_gp.get_response_string() << std::endl;
    }
}


