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

#ifndef __TLM_INITIATOR_H__
#define __TLM_INITIATOR_H__

#include "tlm.h"
#include "tlm_gp_port.h"


class tlm_initiator : public sc_core::sc_module
{
public:
    
    tlm::tlm_gp_port< 256 > bus_port; // bus data width in bits

    SC_HAS_PROCESS(tlm_initiator);

    // Constructor
    tlm_initiator(sc_core::sc_module_name name_,
                  tlm::tlm_endianness endianness = tlm::TLM_LITTLE_ENDIAN)
        : sc_core::sc_module(name_)
        , bus_port("bus_port")
        , m_endianness(endianness)
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
        std::cout << " ###### TEST SERIALIZERS WITH BYTE-ENABLES ###### "
                  << std::endl; 
        test_with_be();
        std::cout << std::endl;
        
        std::cout << std::endl;
        std::cout << " ###### TEST SERIALIZERS WITH CUSTOMIZED BYTE-ENABLES ###### "
                  << std::endl; 
        test_customized_be();
        std::cout << std::endl;
        
        sc_core::sc_stop();
        sc_core::wait(sc_core::SC_ZERO_TIME);
    }
    
private:
    
    void test_without_be();
    void test_with_be();
    void test_customized_be();
    
    tlm::tlm_generic_payload  m_gp;
    
    sc_dt::sc_biguint<256> reg256;
    sc_dt::sc_bigint<128>  reg128;
    sc_dt::sc_uint<64>	   reg64;
    sc_dt::sc_int<48>	   reg48;
    unsigned int           reg32;
    short	           reg16;
    unsigned char          reg8;
    
    unsigned char *wr_data; 
    unsigned char *aux_data; 
    unsigned char *rd_data; 
    bool          *be;
    unsigned int   be_length;
    
    
    template< int W >
	void write(sc_dt::uint64 address,
                   unsigned char *data, 
                   bool* byte_enable = 0,
                   unsigned int byte_enable_length = 0);
    template< int W >
	void read(sc_dt::uint64 address,
                  unsigned char *data,
                  bool* byte_enable = 0,
                  unsigned int byte_enable_length = 0);
    
    template< class T >
	void prepare_address_and_data(int nbits,
                                      sc_dt::uint64& address,
                                      T& data);
    
    void prepare_arrays(int nbits, bool shift = false);
    void compare_arrays(int nbits);
    
    tlm::tlm_endianness m_endianness;
  
};





template< class T >
void tlm_initiator::prepare_address_and_data(int nbits,
                                             sc_dt::uint64& address,
                                             T& data)
{
    address = 0;
    if(hasHostEndianness(m_endianness) == false)
    {
        address += (bus_port.getBusDataWidth() - nbits)/8;
    }
    
    unsigned int offset = tlm::get_subword_offset(address, bus_port.getBusDataWidth()/8, nbits/8, m_endianness);
    
    // test serializers in the initiator 
    tlm::copy_word_from_array< T >(data, offset, nbits/8, wr_data, 0, 0);
    tlm::copy_word_to_array< T >(data, offset, nbits/8, aux_data, 0, 0);
}


//
// Read data to address (data in host endianness)
//
template< int W >
void tlm_initiator::write(sc_dt::uint64 address,
                          unsigned char *data,
                          bool* byte_enable,
                          unsigned int byte_enable_length)
{
    // Single WRITE transaction 
    
    std::cout << name() << " : Single WRITE transaction : ADDRESS = 0x"
              << std::hex << address << std::dec << " : "; 
    
    m_gp.set_command(tlm::TLM_WRITE_COMMAND);
    m_gp.set_address(address);
    m_gp.set_data_ptr(data);
    m_gp.set_data_length(W/8);
    m_gp.set_byte_enable_ptr(byte_enable);
    m_gp.set_byte_enable_length(byte_enable_length);
    
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
void tlm_initiator::read(sc_dt::uint64 address,
                         unsigned char *data,
                         bool* byte_enable,
                         unsigned int byte_enable_length)
{
    // Single READ transaction 
    std::cout << name() << " : Single READ transaction : ADDRESS = 0x"
              << std::hex << address << std::dec << " : ";  
    
    m_gp.set_command(tlm::TLM_READ_COMMAND);
    m_gp.set_address(address);
    m_gp.set_data_ptr(data);
    m_gp.set_data_length(W/8);
    m_gp.set_byte_enable_ptr(byte_enable);
    m_gp.set_byte_enable_length(byte_enable_length);
    
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
