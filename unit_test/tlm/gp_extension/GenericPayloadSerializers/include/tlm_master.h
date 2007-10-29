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
    
	tlm::tlm_gp_port< 256 > bus_port; // bus data width in bits

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
		// Test 256-bit access
		prepare_arrays< sc_biguint<256> >(256, reg256);
		write<256>(0,aux_data);
		read<256>(0,rd_data);
		compare_arrays(256);

		// Test 128-bit access 
		prepare_arrays< sc_bigint<128> >(128, reg128);
		write<128>(0,aux_data);
		read<128>(0,rd_data);
		compare_arrays(128);

		// Test 64-bit access 
		prepare_arrays< sc_uint<64> >(64, reg64);
		write<64>(0,aux_data);
		read<64>(0,rd_data);
		compare_arrays(64);

 		// Test 48-bit access 
		prepare_arrays< sc_int<48> >(48, reg48);
		write<48>(0,aux_data);
		read<48>(0,rd_data);
		compare_arrays(48);

 		// Test 32-bit access 
		prepare_arrays< unsigned int >(32, reg32);
		write<32>(0,aux_data);
		read<32>(0,rd_data);
		compare_arrays(32);

 		// Test 16-bit access 
		prepare_arrays< short >(16, reg16);
		write<16>(0,aux_data);
		read<16>(0,rd_data);
		compare_arrays(16);

 		// Test 8-bit access 
		prepare_arrays< unsigned char >(8, reg8);
		write<8>(0,aux_data);
		read<8>(0,rd_data);
		compare_arrays(8);

		sc_stop();
        sc_core::wait(SC_ZERO_TIME);
	}




private:

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

	template< class T >
	void prepare_arrays(int nbits, T& reg);

	void compare_arrays(int nbits);

    //
    // Read data to address (data in host endianness)
    //
	template< int W >
	void write(unsigned int address, unsigned char *data);
 	//
    // Read data from address (returns data in host endianness)
    //
	template< int W >
	void read(unsigned int address, unsigned char *data);


};

template< class T >
void tlm_master::prepare_arrays(int nbits, T& reg) 
{
	wr_data = new unsigned char [nbits/8]; 
	aux_data = new unsigned char [nbits/8]; 
	rd_data = new unsigned char [nbits/8]; 

	// Initialiaze arrays
	for(int i=0;i<nbits/8;i++)
	{
		wr_data[i] = i;	aux_data[i] = 0; rd_data[i] = 0;
	}

	// test serializers in the initiator 
	tlm::copy_from_array< T >(reg,0,wr_data);
	tlm::copy_to_array< T >(reg,0,aux_data);
}

void tlm_master::compare_arrays(int nbits) 
{
	// check if everything went ok
	bool error_flag = false;

	for(int i=0;i<nbits/8;i++)
	{
		if(aux_data[i]!= rd_data[i])
			error_flag =true;
	}

	if(error_flag)
		std::cout << name() << " : read and write " << nbits << "-bit operation : ERROR " << std::endl;
	else
		std::cout << name() << " : read and write " << nbits << "-bit operation : OK  " << std::endl;

	delete [] wr_data;	wr_data = 0;
	delete [] aux_data;	aux_data = 0; 
	delete [] rd_data;	rd_data = 0;
}
    
//
// Read data to address (data in host endianness)
//
template< int W >
void tlm_master::write(unsigned int address, unsigned char *data)
{
	// Single WRITE transaction 

	std::cout << name() << " : Single WRITE transaction : "; 

	m_gp.set_command(tlm::TLM_WRITE_COMMAND);
	m_gp.set_address(address);
	m_gp.set_data_ptr(data);
	m_gp.set_length(W/8);
	
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
void tlm_master::read(unsigned int address, unsigned char *data)
{
	// Single READ transaction 
	std::cout << name() << " : Single READ transaction : "; 
	
	m_gp.set_command(tlm::TLM_READ_COMMAND);
	m_gp.set_address(address);
	m_gp.set_data_ptr(data);
	m_gp.set_length(W/8);
	
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
