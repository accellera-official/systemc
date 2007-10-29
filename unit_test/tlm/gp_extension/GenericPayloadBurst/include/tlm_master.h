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

#ifndef _TLM_MASTER_H
#define _TLM_MASTER_H

#include "systemc.h"

#include "tlm_gp_port.h"

#include "tlm.h"


class tlm_master : public sc_module
{
public:
    
	tlm::tlm_gp_port< 32 > bus_port; // bus data width in bits

    SC_HAS_PROCESS(tlm_master);

	// Constructor
    tlm_master(sc_module_name _name)
		: sc_module(_name)
		, bus_port("bus_port")
	{
		SC_THREAD(main);
	}

	// Destructor
    ~tlm_master()
	{
	}

    void main()
	{
		unsigned char *wr_data;
		unsigned char *rd_data;

		wr_data = new unsigned char [66]; 
		rd_data = new unsigned char [66]; 

		// Initialize arrays
		for(int i=0;i<66;i++)
		{
			wr_data[i] = i;
			rd_data[i] = 0;
		}

		std::cout << name() << " : TEST 1 : write and read burst operation : length 64 bytes \n";
		// Write Burst transaction
		writeBurst(0x0,wr_data,64);
		// Read Burst transaction
		readBurst(0x0,rd_data,64);
		// Compare wr_data and rd_data
		compare_arrays(wr_data,rd_data,64);
		std::cout << std::endl;

		std::cout << name() << " : TEST 2 : write and read burst operation : length 66 bytes \n";
		for(int i=0;i<66;i++) rd_data[i]=0;
		// Write Burst transaction
		writeBurst(0x100,wr_data,66);
		// Read Burst transaction
		readBurst(0x100,rd_data,66);
		// Compare wr_data and rd_data
		compare_arrays(wr_data,rd_data,66);
		std::cout << std::endl;



		delete [] wr_data;	wr_data = 0;
		delete [] rd_data;	rd_data = 0;

		sc_core::sc_stop();
		sc_core::wait(SC_ZERO_TIME);
	}

private:

    tlm::tlm_generic_payload  m_gp;

	void writeBurst(unsigned int base_address, unsigned char *data, unsigned int nr_bytes);
	void readBurst(unsigned int base_address, unsigned char *data, unsigned int nr_bytes);
	void compare_arrays(unsigned char* wr_data, unsigned char* rd_data, unsigned int nr_bytes);
};

//
// Incremental burst WRITE transaction 
//
void tlm_master::writeBurst(unsigned int base_address, unsigned char *data, unsigned int nr_bytes)
{
	cout << name() << " : Incremental burst WRITE transaction : "; 

	m_gp.set_command(tlm::TLM_WRITE_COMMAND);
	m_gp.set_address(base_address);
	m_gp.set_data_ptr(data);
	m_gp.set_data_length(nr_bytes);
	
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
// Incremental burst READ transaction 
//
void tlm_master::readBurst(unsigned int base_address, unsigned char *data, unsigned int nr_bytes)
{
	std::cout << name() << " : Incremental burst READ transaction : "; 
	
	m_gp.set_command(tlm::TLM_READ_COMMAND);
	m_gp.set_address(base_address);
	m_gp.set_data_ptr(data);
	m_gp.set_data_length(nr_bytes);
	
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


// Compare arrays
void tlm_master::compare_arrays(unsigned char* wr_data, unsigned char* rd_data, unsigned int nr_bytes)
{
	bool error_flag = false;
	for(int i=0;i<nr_bytes;i++)
	{
		if(wr_data[i] != rd_data[i])
			error_flag = true;
	}
		
	if(error_flag)
		std::cout << name() << " : WR and RD arrays are different : ERROR " << std::endl;
	else
		std::cout << name() << " : WR and RD arrays are equal : OK  " << std::endl;
}

#endif
