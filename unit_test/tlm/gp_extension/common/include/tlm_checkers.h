/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2004 by all Contributors.
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

#ifndef _TLM_CHECKERS_H
#define _TLM_CHECKERS_H

#include "tlm.h"

namespace tlm {

class tlm_checker
{
public:

    // constructor (a tlm_checker applies on a certain address space)
    tlm_checker(sc_dt::uint64 start_address_range, // first address 
                sc_dt::uint64 end_address_range,   // last address
                unsigned int bus_data_size_in_bytes)
        : m_bus_data_size(bus_data_size_in_bytes)
        , m_start_address_range(start_address_range)
        , m_end_address_range(end_address_range)
        , m_write_command_supported(true)
        , m_read_command_supported(true)
        , m_burst_supported(true)
        , m_burst_mode_streaming_supported(true)
        , m_byte_enable_supported(true)
        , m_response_status(TLM_OK_RESPONSE)
    {
    }
    
    // destructor
    ~tlm_checker(){}
    
    // convenient methods to set the tlm_checker options
    inline void write_command_not_supported()        {m_write_command_supported = false;}
    inline void read_command_not_supported()         {m_read_command_supported = false;}
    inline void burst_not_supported()                {m_burst_supported = false;}
    inline void burst_mode_streaming_not_supported() {m_burst_mode_streaming_supported = false;}
    inline void byte_enable_not_supported()          {m_byte_enable_supported = false;}
    
    // main function to check if the transaction is valid 
    bool transactionIsValid(tlm::tlm_generic_payload* gp)
    {
        m_response_status = do_check(gp);
        
        return (m_response_status == TLM_OK_RESPONSE);
    }
    
    tlm_response_status get_response_status() {return m_response_status;}
    
private:
    
    tlm_response_status do_check(tlm::tlm_generic_payload* gp)
    {
        // Check 1: Write transaction supported
        if((gp->get_command() == TLM_WRITE_COMMAND) &&
           (m_write_command_supported == false))
            return TLM_COMMAND_ERROR_RESPONSE;
        
        // Check 2: Read transacion supported
        if((gp->get_command() == TLM_READ_COMMAND) &&
           (m_read_command_supported == false))
            return TLM_COMMAND_ERROR_RESPONSE;
        
        // Check 3-4: check supported burst and data length
        if(gp->get_burst_length(m_bus_data_size) > 1)
        {
            if(m_burst_supported == false)
            {
                return TLM_BURST_ERROR_RESPONSE;
            }
            else
            {
                if((gp->get_streaming_mode() == true) &&
                   (m_burst_mode_streaming_supported == false))
                    return TLM_BURST_ERROR_RESPONSE;
            }
        }
        
        // Check 5: check address within range (in case burst_mode is
        //          incremental)
        if(gp->get_streaming_mode() == false)
        {
            sc_dt::uint64 begin_address = gp->get_address();
            sc_dt::uint64 end_address = begin_address + gp->get_data_length();
            
            if((begin_address < m_start_address_range) &&
               (end_address > m_end_address_range))
                return TLM_ADDRESS_ERROR_RESPONSE;
        }
        
        // Check 6: check byte enables
        if((gp->get_byte_enable_ptr() != NULL) &&
           (m_byte_enable_supported == false))
        {
            return TLM_BYTE_ENABLE_ERROR_RESPONSE;
        }
        
        return TLM_OK_RESPONSE;
    }
    
private:
    
    unsigned int m_bus_data_size;
    sc_dt::uint64 m_start_address_range;
    sc_dt::uint64 m_end_address_range;
    bool m_write_command_supported;
    bool m_read_command_supported;
    bool m_burst_supported;
    bool m_burst_mode_streaming_supported;
    bool m_byte_enable_supported;

    tlm_response_status m_response_status;

};

}

#endif
