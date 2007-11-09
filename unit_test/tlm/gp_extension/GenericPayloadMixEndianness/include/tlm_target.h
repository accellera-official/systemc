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

#ifndef __TLM_TARGET_H__
#define __TLM_TARGET_H__

#include "tlm.h"
#include "simple_target_socket.h"

#include "tlm_checkers.h"


class tlm_target : public sc_core::sc_module
{
public:

    SimpleTargetSocket<> socket; // default 32-bit

    SC_HAS_PROCESS(tlm_target);
    
    // constructor & destructor
    tlm_target(sc_core::sc_module_name name_, 
               unsigned int start_address, 
               unsigned int end_address,
               tlm::tlm_endianness endianness)
        : sc_core::sc_module(name_)
        , socket("socket")
        , m_start_address(start_address)
        , m_end_address(end_address)
        , m_endianness(endianness)
        , m_checker(start_address, end_address, socket.get_bus_width()/8)
    {
        // register nb_transport method
        REGISTER_NBTRANSPORT(socket, myNBTransport);
        
        mem = new unsigned int [(end_address-start_address+1)/4];
        
        socket_bus_width = socket.get_bus_width()/8;
        
        m_checker.burst_mode_streaming_not_supported();
    }

    ~tlm_target() 
    { 
        delete [] mem;
    }
    

    tlm::tlm_sync_enum myNBTransport(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& t)
    {
        tlm::tlm_response_status m_response_status;
        
        if(m_checker.transactionIsValid(&trans))
        {
            unsigned int   m_addr      = (unsigned int)trans.get_address() - m_start_address;
            unsigned char* m_data      = trans.get_data_ptr();
            unsigned int   m_length    = trans.get_data_length();
            bool*          m_be        = trans.get_byte_enable_ptr();
            unsigned int   m_be_length = trans.get_byte_enable_length();
            
            for(unsigned int b=0; b<get_burst_length(m_length, socket_bus_width); b++)
            {
                unsigned int nr_bytes = get_nr_bytes_of_burst_element(b, m_length, socket_bus_width);
                unsigned int offset = tlm::get_subword_offset(m_addr, socket_bus_width, nr_bytes, m_endianness);
                
                
                // The slave assumes that GP comes in its m_endianess,
                // but stores the data in host-endianess (arithmetic mode) 
                if(trans.is_write()) 
                {						
                    tlm::copy_word_from_array( mem[b], 
                                               offset, 
                                               nr_bytes, 
                                               m_data, 
                                               m_be, 
                                               m_be_length);
                    
                    std::cout << name() << " : burst write : A = 0x"
                              << std::hex << m_addr
                              << ", D = 0x" << mem[b] << std::dec
                              << " @ " << sc_core::sc_time_stamp()
                              << std::endl;
                    
                } else {
                    
                    tlm::copy_word_to_array(   mem[b], 
                                               offset, 
                                               nr_bytes, 
                                               m_data, 
                                               m_be, 
                                               m_be_length);
                    
                    std::cout << name() << " : burst read : A = 0x"
                              << std::hex << m_addr << std::dec
                              << " @ " << sc_core::sc_time_stamp()
                              << std::endl;
                }
                
                m_addr += socket.get_bus_width()/8;
                m_data += nr_bytes;
            }
            
            m_response_status = tlm::TLM_OK_RESPONSE;
        }
        else
        {
            m_response_status = m_checker.get_response_status();
        }
        
        trans.set_response_status(m_response_status);
        
        // Time annotation
        t += sc_core::sc_time(10*get_burst_length(trans.get_data_length(),
                                                  socket_bus_width),
                              sc_core::SC_NS);
        
        // LT slave
        // - always return true
        // - not necessary to update phase (if true is returned)
        return tlm::TLM_COMPLETED;
    }

private:
    
    unsigned int* mem;

    unsigned int m_start_address;
    unsigned int m_end_address;
    tlm::tlm_endianness m_endianness;
    tlm::tlm_checker m_checker;

    unsigned int   socket_bus_width;

    unsigned int get_burst_length(unsigned int length,
                                  unsigned int bus_data_size)
    {
        return (length+bus_data_size-1)/bus_data_size;
    }

    unsigned int get_nr_bytes_of_burst_element(unsigned int count,
                                               unsigned int length,
                                               unsigned int bus_data_size)
    {
        unsigned int remainder = length-(bus_data_size*count);
        if(remainder < bus_data_size)
            return remainder;
        else
            return bus_data_size;
    }

}; 

#endif
