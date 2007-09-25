
#include "tlm_master.h"

void tlm_master::test_single_write(bool use_ext1,
                                   bool use_ext2,
                                   bool use_ext3)
{
    unsigned int m_address = 0x10; 
    unsigned int m_data = 100;
    
    // Single WRITE transaction 
    
    cout << name() << " : Single WRITE transaction : ";
    
    m_gp.set_address(m_address);
    m_gp.set_command(tlm::TLM_WRITE_COMMAND);
    m_gp.set_burst_data_size(bus_port.getBusDataWidth()/8); // in bytes
    m_gp.set_burst_length(1);
    m_gp.set_burst_mode(tlm::TLM_INCREMENT_BURST);
    m_gp.set_data_ptr((unsigned char*)&m_data);
    
    if (use_ext1)
    {
        m_gp.add_extension(&m_ext1);
    }
    else
    {
        m_gp.clear_extension(&m_ext1);
    }
    if (use_ext2)
    {
        m_gp.add_extension(&m_ext2);
    }
    else
    {
        m_gp.clear_extension(&m_ext2);
    }
    if (use_ext3)
    {
        m_gp.add_extension(&m_ext3);
    }
    else
    {
        m_gp.clear_extension(&m_ext3);
    }

    bus_port->nb_transport(&m_gp);
    
    if(m_gp.get_response_status() == tlm::TLM_OK_RESP)
    {
        cout << " OK " << endl;
    }
    else
    {
        cout << " ERROR " << endl;
    }
}

void tlm_master::test_single_read()
{
    unsigned int m_address = 0x10; 
    unsigned int m_data;
    
    // Single READ transaction
    
    cout << name() << " : Single READ transaction : ";
    
    m_gp.set_address(m_address);
    m_gp.set_command(tlm::TLM_READ_COMMAND);
    m_gp.set_burst_data_size(bus_port.getBusDataWidth()/8); // in bytes
    m_gp.set_burst_length(1);
    m_gp.set_burst_mode(tlm::TLM_INCREMENT_BURST);
    m_gp.set_data_ptr((unsigned char*)&m_data);
    

    // Clear by direct access:
    m_gp.clear_extension(&m_ext1);
    m_gp.clear_extension(&m_ext2);
    m_gp.clear_extension(&m_ext3);

    bus_port->nb_transport(&m_gp);
    
    if(m_gp.get_response_status() == tlm::TLM_OK_RESP)
    {
        cout << " OK, data = "<< m_data << endl;
    }
    else
    {
        cout << " ERROR " << endl;
    }
}


