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
#include "simple_initiator_socket.h"
#include <cassert>

#include "mem_map.h"

class tlm_initiator : public sc_core::sc_module
{
public:	 
	
    SimpleInitiatorSocket<tlm_initiator> socket; // default 32-bit 
    
    SC_HAS_PROCESS(tlm_initiator);
    
    tlm_initiator(sc_core::sc_module_name name_,
                  tlm::tlm_endianness endianness, bool first)
        : sc_core::sc_module(name_)
        , socket("socket")
        , m_endianness(endianness)
        , m_first(first)
    {
        SC_THREAD(main);
    }
    
    void main()
    {
        unsigned int data = 0x01020304;
        unsigned int rd_data = 0;
        unsigned int wr_data = 0;
        unsigned int rd2_data = 0;
        
        if(m_first == false)
        {
            sc_core::wait(100, sc_core::SC_NS);
        }
        
        // The master stores the data in host-endianess (arithmetic mode), 
        // but uses its m_endianess in the GP
        
        //wr_data = data;
        tlm::copy_word_from_array(wr_data, 0, 4,
                                  reinterpret_cast<unsigned char*>(&data),
                                  0, 0);
        
        
        // Test 1: Write and read to a Little-Endian slave
        std::cout << std::endl;
        std::cout << name() << " : TEST 1 : WRITE and READ to SLAVE LE "
                  << std::endl;
        
        std::cout << name() << " : Send write request: A = 0x" 
                  << std::hex << ADDRESS_SLAVE_LE << ", D = 0x"
                  << wr_data << std::dec
                  << " @ " << sc_core::sc_time_stamp() << std::endl;
        writeBurst(ADDRESS_SLAVE_LE,
                   reinterpret_cast<unsigned char*>(&wr_data),
                   4);
        std::cout << std::endl;

        std::cout << name() << " : Send read request: A = 0x" << std::hex
                  << ADDRESS_SLAVE_LE << std::dec
                  << " @ " << sc_core::sc_time_stamp() << std::endl;
        readBurst(ADDRESS_SLAVE_LE,
                  reinterpret_cast<unsigned char*>(&rd_data),
                  4);
        std::cout << name() << " : Receive read request: A = 0x"
                  << std::hex << ADDRESS_SLAVE_LE << ", D = 0x"
                  << rd_data << std::dec
                  << " @ " << sc_core::sc_time_stamp() << std::endl;
        std::cout << std::endl;
        
        std::cout << name() << " : SLAVE LE : WR_DATA equal to RD_DATA : ";
        if(wr_data == rd_data) std::cout << "OK"; else std::cout << "ERROR";
        std::cout << std::endl << std::endl << std::endl;
        
        
        // Test 2: Write and read to a Big-Endian slave
        std::cout << std::endl;
        std::cout << name() << " : TEST 2 : WRITE and READ to SLAVE BE "
                  << std::endl;
        
        std::cout << name() << " : Send write request: A = 0x" 
                  << std::hex << ADDRESS_SLAVE_BE << ", D = 0x"
                  << wr_data << std::dec
                  << " @ " << sc_core::sc_time_stamp() << std::endl;
        writeBurst(ADDRESS_SLAVE_BE,
                   reinterpret_cast<unsigned char*>(&wr_data),
                   4);
        std::cout << std::endl;
        
        std::cout << name() << " : Send read request: A = 0x" << std::hex
                  << ADDRESS_SLAVE_BE << std::dec
                  << " @ " << sc_core::sc_time_stamp() << std::endl;
        readBurst(ADDRESS_SLAVE_BE,
                  reinterpret_cast<unsigned char*>(&rd_data),
                  4);
        std::cout << name() << " : Receive read request: A = 0x"
                  << std::hex << ADDRESS_SLAVE_BE << ", D = 0x"
                  << rd_data << std::dec
                  << " @ " << sc_core::sc_time_stamp() << std::endl;
        std::cout << std::endl;
        
        std::cout << name() << " : SLAVE BE : WR_DATA equal to RD_DATA : ";
        if(wr_data == rd_data) std::cout << "OK"; else std::cout << "ERROR";
        std::cout << std::endl << std::endl << std::endl;
        
        
        // Test 3: LE or BE first master Write to LE and BE slave address 
        //         second master Read from the same LE and BE slave address 
        if(m_first)
        {
            sc_core::wait(200, sc_core::SC_NS);
            
            std::cout << std::endl;
            std::cout << name() << " : TEST 3 : MIX WRITE and READ "
                      << std::endl;
            
            std::cout << name() << " : Send write request: A = 0x" 
                      << std::hex << ADDRESS_COMMON_SLAVE_LE << ", D = 0x"
                      << wr_data << std::dec
                      << " @ " << sc_core::sc_time_stamp() << std::endl;
            writeBurst(ADDRESS_COMMON_SLAVE_LE ,
                       reinterpret_cast<unsigned char*>(&wr_data),
                       4);
            std::cout << std::endl;
            
            std::cout << name() << " : Send write request: A = 0x" 
                      << std::hex << ADDRESS_COMMON_SLAVE_BE << ", D = 0x"
                      << wr_data << std::dec
                      << " @ " << sc_core::sc_time_stamp() << std::endl;
            writeBurst(ADDRESS_COMMON_SLAVE_BE ,
                       reinterpret_cast<unsigned char*>(&wr_data),
                       4);
            std::cout << std::endl;
        }
        else
        {
            sc_core::wait(200, sc_core::SC_NS);
            
            std::cout << name() << " : Send read request: A = 0x" << std::hex
                      << ADDRESS_COMMON_SLAVE_LE  << std::dec
                      << " @ " << sc_core::sc_time_stamp() << std::endl;
            readBurst(ADDRESS_COMMON_SLAVE_LE ,
                      reinterpret_cast<unsigned char*>(&rd_data),
                      4);
            std::cout << name() << " : Receive read request: A = 0x"
                      << std::hex << ADDRESS_COMMON_SLAVE_LE  << ", D = 0x"
                      << std::hex << rd_data << std::dec
                      << " @ " << sc_core::sc_time_stamp() << std::endl;
            std::cout << std::endl;
            
            std::cout << name() << " : SLAVE LE : WR_DATA equal to RD_DATA : ";
            if(wr_data == rd_data)
                std::cout << "OK";
            else
                std::cout << "ERROR";
            std::cout << std::endl << std::endl;
            
            std::cout << name() << " : Send read request: A = 0x" << std::hex
                      << ADDRESS_COMMON_SLAVE_BE  << std::dec
                      << " @ " << sc_core::sc_time_stamp() << std::endl;
            readBurst(ADDRESS_COMMON_SLAVE_BE ,
                      reinterpret_cast<unsigned char*>(&rd2_data),
                      4);
            std::cout << name() << " : Receive read request: A = 0x"
                      << std::hex << ADDRESS_COMMON_SLAVE_BE  << ", D = 0x"
                      << std::hex << rd2_data << std::dec
                      << " @ " << sc_core::sc_time_stamp() << std::endl;
            std::cout << std::endl;
            
            std::cout << name() << " : SLAVE LE : WR_DATA equal to RD_DATA : ";
            if(wr_data == rd2_data)
                std::cout << "OK";
            else
                std::cout << "ERROR";
            std::cout << std::endl << std::endl;
        }
        
        
    }
    
private:
    
    tlm::tlm_generic_payload  m_gp;
    tlm::tlm_endianness       m_endianness;
    bool                      m_first;
    
    void writeBurst(unsigned int address, 
                    unsigned char *data, 
                    unsigned int nr_bytes,
                    unsigned char* be = 0, 
                    unsigned int be_length = 0);
    
    void readBurst(unsigned int address, 
                   unsigned char *data, 
                   unsigned int nr_bytes,
                   unsigned char* be = 0, 
                   unsigned int be_length = 0);

};


void tlm_initiator::writeBurst(unsigned int address, 
                               unsigned char *data, 
                               unsigned int nr_bytes,
                               unsigned char* be, 
                               unsigned int be_length)
{
    sc_core::sc_time t;
    
    m_gp.set_write();
    m_gp.set_address(address);
    m_gp.set_data_ptr(data);
    m_gp.set_data_length(nr_bytes);
    m_gp.set_byte_enable_ptr(be);
    m_gp.set_byte_enable_length(be_length);
    
    t = sc_core::SC_ZERO_TIME;
    
    socket->b_transport(m_gp, t);
    wait(t);
    
    if(m_gp.is_response_ok())
    {
        std::cout << name() <<  " : Received ok response";
        std::cout << " @ " << sc_core::sc_time_stamp() << std::endl;
    }
    else
    {
        std::cout << name() << " : Received error response <" << m_gp.get_response_string() << "> : ";
        std::cout << " @ " << sc_core::sc_time_stamp() << std::endl;
    }
}

void tlm_initiator::readBurst(unsigned int address, 
                              unsigned char *data, 
                              unsigned int nr_bytes,
                              unsigned char* be, 
                              unsigned int be_length)
{
    sc_core::sc_time t;
    
    m_gp.set_read();
    m_gp.set_address(address);
    m_gp.set_data_ptr(data);
    m_gp.set_data_length(nr_bytes);
    m_gp.set_byte_enable_ptr(be);
    m_gp.set_byte_enable_length(be_length);
	
    t = sc_core::SC_ZERO_TIME;
    
    socket->b_transport(m_gp, t);
    wait(t);
    
    if(m_gp.is_response_ok())
    {
        std::cout << name() <<  " : Received ok response";
        std::cout << " @ " << sc_core::sc_time_stamp() << std::endl;
    }
    else
    {
        std::cout << name() << " : Received error response <"
                  << m_gp.get_response_string() << "> : ";
        std::cout << " @ " << sc_core::sc_time_stamp() << std::endl;
    }
}


#endif
