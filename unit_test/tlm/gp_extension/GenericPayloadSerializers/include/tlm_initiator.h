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

#ifndef _TLM_INITIATOR_H
#define _TLM_INITIATOR_H

#include "systemc.h"

#include "tlm_gp_port.h"

#include "tlm.h"


class tlm_initiator : public sc_module
{
public:
    
    tlm::tlm_gp_port< 256 > bus_port; // bus data width in bits

    SC_HAS_PROCESS(tlm_initiator);

    // Constructor
    tlm_initiator(sc_module_name _name)
        : sc_module(_name)
        , bus_port("bus_port")
    {
        SC_THREAD(main);
    }

    // Destructor
    ~tlm_initiator()
    {
    }

    void main()
    {
        std::cout << std::endl;
        std::cout << " ###### TEST SERIALIZERS ###### " << std::endl; 
        test_without_be();
        std::cout << std::endl;
        
        std::cout << std::endl;
        std::cout << " ###### TEST SERIALIZERS WITH BYTE-ENABLES ###### " << std::endl; 
        test_with_be();
        std::cout << std::endl;
        
        std::cout << std::endl;
        std::cout << " ###### TEST SERIALIZERS WITH CUSTOMIZED BYTE-ENABLES ###### " << std::endl; 
        test_customized_be();
        std::cout << std::endl;
        
        sc_stop();
        sc_core::wait(SC_ZERO_TIME);
    }
    
private:
    
    void test_without_be();
    void test_with_be();
    void test_customized_be();
    
    tlm::tlm_generic_payload  m_gp;
    
    sc_biguint<256> reg256;
    sc_bigint<128>	reg128;
    sc_uint<64>		reg64;
    sc_int<48>		reg48;
    unsigned int	reg32;
    short			reg16;
    unsigned char	reg8;
    
    unsigned char *wr_data; 
    unsigned char *aux_data; 
    unsigned char *rd_data; 
    bool          *be;
    unsigned int   be_length;
    
    template< class T >
	void prepare_arrays(int nbits, T& reg, bool shift = false);
    
    template< int W >
	void write(unsigned int address, unsigned char *data, bool* be = 0, unsigned int be_length = 0);
    template< int W >
	void read(unsigned int address, unsigned char *data, bool* be = 0, unsigned int be_length = 0);
    
    void compare_arrays(int nbits);
    
};

template< class T >
void tlm_initiator::prepare_arrays(int nbits, T& reg, bool shift) 
{
    wr_data = new unsigned char [nbits/8]; 
    aux_data = new unsigned char [nbits/8]; 
    rd_data = new unsigned char [nbits/8];
    be = new bool [nbits/8];
    be_length = nbits/8;
    
    // Initialiaze arrays
    for(int i=0;i<nbits/8;i++)
    {
        if(shift) {
            wr_data[i] = i << 4;	aux_data[i] = 0; rd_data[i] = 0; be[i] = true;
        } else {
            wr_data[i] = i;	aux_data[i] = 0; rd_data[i] = 0; be[i] = true;
        }
    }
    
    // test serializers in the initiator 
    tlm::copy_from_array< T >(reg, 0, wr_data, 0, 0);
    tlm::copy_to_array< T >(reg, 0, aux_data, 0, 0);
}


//
// Read data to address (data in host endianness)
//
template< int W >
void tlm_initiator::write(unsigned int address, unsigned char *data, bool* be, unsigned int be_length)
{
    // Single WRITE transaction 
    
    std::cout << name() << " : Single WRITE transaction : "; 
    
    m_gp.set_command(tlm::TLM_WRITE_COMMAND);
    m_gp.set_address(address);
    m_gp.set_data_ptr(data);
    m_gp.set_data_length(W/8);
    m_gp.set_byte_enable_ptr(be);
    m_gp.set_byte_enable_length(be_length);
    
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

//
// Read data from address (returns data in host endianness)
//
template< int W >
void tlm_initiator::read(unsigned int address, unsigned char *data, bool* be, unsigned int be_length)
{
    // Single READ transaction 
    std::cout << name() << " : Single READ transaction : "; 
    
    m_gp.set_command(tlm::TLM_READ_COMMAND);
    m_gp.set_address(address);
    m_gp.set_data_ptr(data);
    m_gp.set_data_length(W/8);
    m_gp.set_byte_enable_ptr(be);
    m_gp.set_byte_enable_length(be_length);
    
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



#endif
