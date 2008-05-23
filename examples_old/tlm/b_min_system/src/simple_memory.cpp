/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
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
/*==============================================================================

  @file simple_memory.cpp
  
  @brief Implementation of simple_memory class
  
  @Details
    Implements the simple_memory class. This class does not support memory refresh or 
    DMI.

  @Note
    1. transport_dbg() not implemented
    2. get_direct_mem_ptr() not implemented
===============================================================================*/

#include "simple_memory.h"
#include "reporting.h"

/*==============================================================================
  Constructor
==============================================================================*/

simple_memory::simple_memory(sc_module_name  module_name, 
		                     sc_dt::uint64   memory_size) 
      : sc_module(module_name),
        target_socket("target_socket"),
        m_memory_size(memory_size)
{
   // Allocate an array for the target's memory
   m_memory = new unsigned char[size_t(memory_size)];
      
   // clear the memory
   memset(m_memory, 0, size_t(memory_size));
   
   // Bind the target port to the interface
   target_socket(*this);
}
     
/*==============================================================================
  Destructor
==============================================================================*/

simple_memory::~simple_memory()
{
  // Free up the target's memory array
  delete [] m_memory;
}

/*=============================================================================
  Implementation of the read function of convenience API (rw_if)
  
  This function implements the behavior of the memory when receiving
  a read transaction.
=============================================================================*/

bool simple_memory::read(sc_dt::uint64  bus_address, 
		                 unsigned int   byte_count, 
		                 unsigned char* read_buffer)
{
	std::cout << REPORT_HEAD << "Received read at address 0x" << HEX_ADDR << bus_address;

	// clear read buffer
	memset(read_buffer, 0, byte_count);
	        
	if  (bus_address >= m_memory_size)
	{
		// out-of-bounds read
	}
	else
	{
		std::cout << " Data: 0x";
		for (unsigned int i = 0; i<byte_count; i++)
		{
			if (bus_address >= m_memory_size)
			{
				// ignore out-of-bounds reads
				return true;
			}
	          
			read_buffer[i] = m_memory[bus_address++];
			std::cout << HEX_DATA << (int)read_buffer[i];
		}
		
	}
	std::cout << std::endl << std::endl;  
	
	return true;
}

/*=============================================================================
  Implementation of the write function of convenience API (rw_if)
  
  This function implements the behavior of the memory when receiving
  a write transaction.
=============================================================================*/

bool simple_memory::write(sc_dt::uint64   bus_address, 
		                  unsigned int    byte_count, 
		                  unsigned char*  write_buffer)
{
	std::cout << REPORT_HEAD << "Received write at address 0x" << HEX_ADDR << bus_address;
		
    if  (bus_address >= m_memory_size)
    {
    	// ignore out-of-bounds writes
    }
    else
    {
    	std::cout << " Data: 0x";
    	for (unsigned int i = 0; i < byte_count; i++)
    	{
    		if ( bus_address >= m_memory_size )
    		{
    			return true;
    		}
      
    		m_memory[bus_address++] = write_buffer[i];
    		std::cout << HEX_DATA << (int)write_buffer[i];
    	}
    }
    std::cout << std::endl << std::endl;
	
	return true; // to be modified
}

/*==============================================================================
  Transport Debug Implementation (not supported here)
==============================================================================*/

unsigned int simple_memory::transport_dbg(tlm::tlm_generic_payload & payload)
{
	// It's perfectly allowed to not support Transport Debug, no need to
	// throw an error
	
	return 0;
}

/*==============================================================================
  DMI interface implementation (not supported here)
==============================================================================*/

bool simple_memory::get_direct_mem_ptr(tlm::tlm_generic_payload &payload,
                                       tlm::tlm_dmi         & dmi_data)
{
    // It's perfectly allowed to not support DMI, no need to
    // throw an error
	
    return false;
}
