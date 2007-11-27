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
/*******************************************************************************
  @file traffic_generator.h

  @brief traffic generation class
  
 ******************************************************************************/
#include "rw_if.h"
#include "reporting.h"
#include "traffic_generator.h"

/*==============================================================================
  Constructor
==============================================================================*/

traffic_generator::traffic_generator(sc_module_name name)
  : sc_module(name),
    initiator_socket("initiator_socket")
{
	initiator_socket(*this);
	
	SC_THREAD(compute);
}
    
/*==============================================================================
  Destructor
==============================================================================*/

traffic_generator::~traffic_generator()          ///< destructor
{
}

/*==============================================================================
  Memory test routine
==============================================================================*/

void traffic_generator::mem_test(sc_dt::uint64 base_address)
{
	bool status;
	unsigned char buffer[8] = { 0x11, 0x22, 0x33, 0x44 };

	sc_dt::uint64 address = base_address;
		
	for (unsigned int i=0; i<128; i++)
	{
		buffer[4] = (address >> 24) & 0xFF;
		buffer[5] = (address >> 16) & 0xFF;
		buffer[6] = (address >>  8) & 0xFF;
		buffer[7] = (address      ) & 0xFF;
		
		std::cout << REPORT_HEAD << "Writing at address 0x" << HEX_ADDR << address << std::endl;
		status = initiator_socket.write(base_address+address, 8, buffer);
		
		if (!status)
		{
			std::cerr << REPORT_HEAD << "Target responded with error!" << std::endl;
		}
		address += 8;
	}
	
	unsigned char ref_buffer[8] = { 0x11, 0x22, 0x33, 0x44 };
	address = base_address;
	for (unsigned int i=0; i<128; i++)
	{
		ref_buffer[4] = (address >> 24) & 0xFF;
		ref_buffer[5] = (address >> 16) & 0xFF;
		ref_buffer[6] = (address >>  8) & 0xFF;
		ref_buffer[7] = (address      ) & 0xFF;
				
		std::cout << REPORT_HEAD << "Reading at address 0x" << HEX_ADDR << address << std::endl;
		status = initiator_socket.read(base_address+address, 8, buffer);
		
		if (!status)
		{
			std::cerr << REPORT_HEAD << "Target responded with error!" << std::endl;
		}
		
		for (unsigned int j=0; j<7; j++)
		{
			if (buffer[j] != ref_buffer[j])
			{
				std::cerr << REPORT_HEAD << "ERROR: value differ for address 0x" << HEX_ADDR << address << std::endl;
			}
		}
		
		address += 8;
	}
}

/*==============================================================================
  Traffic generation thread
==============================================================================*/

void traffic_generator::compute()
{
	// test memory #1
	mem_test(0x00000000);
	
	// test memory #2
	mem_test(0x10000000);
}

/*==============================================================================
  DMI Backward Interface Implementation
==============================================================================*/

void traffic_generator::invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
                                                  sc_dt::uint64 end_range)
{
	// no support needed for now, since the traffic generator
	// does not use DMI
}
