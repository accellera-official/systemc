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

    template <typename DT>
    void print(DT value, tlm::tlm_endianness endianness)
    {
      DT tmp = tlm::swapIfNeeded(value, endianness);
      std::cout << "  Data (simulated endianness) = [ ";
      std::cout << std::hex;
      for (int i = 0; i < (int)sizeof(DT); ++i, tmp >>= 8) {
        if (i != 0) {
          std::cout << ", ";
        }
        std::cout << "0x" << (tmp & 0xff);
      }
      std::cout << " ]" << std::endl;
      std::cout << std::dec;
    }

    void doLEReadAccesses(unsigned int address)
    {
      // read word (4 bytes)
      print(read<unsigned int>(address), tlm::TLM_LITTLE_ENDIAN);

      // read subwords (2 bytes), lsb to msb
      print(read<unsigned short>(address), tlm::TLM_LITTLE_ENDIAN);
      print(read<unsigned short>(address + 2), tlm::TLM_LITTLE_ENDIAN);

      // read subwords (1 byte), lsb to msb
      print(read<unsigned char>(address), tlm::TLM_LITTLE_ENDIAN);
      print(read<unsigned char>(address + 1), tlm::TLM_LITTLE_ENDIAN);
      print(read<unsigned char>(address + 2), tlm::TLM_LITTLE_ENDIAN);
      print(read<unsigned char>(address + 3), tlm::TLM_LITTLE_ENDIAN);
    }

    void doBEReadAccesses(unsigned int address)
    {
      // read word (4 bytes)
      print(read<unsigned int>(address), tlm::TLM_BIG_ENDIAN);

      // read subwords (2 bytes), lsb to msb
      print(read<unsigned short>(address + 2), tlm::TLM_BIG_ENDIAN);
      print(read<unsigned short>(address), tlm::TLM_BIG_ENDIAN);

      // read subwords (1 byte), lsb to msb
      print(read<unsigned char>(address + 3), tlm::TLM_BIG_ENDIAN);
      print(read<unsigned char>(address + 2), tlm::TLM_BIG_ENDIAN);
      print(read<unsigned char>(address + 1), tlm::TLM_BIG_ENDIAN);
      print(read<unsigned char>(address), tlm::TLM_BIG_ENDIAN);
    }

    void main()
	{
          //
          // Write one word (4 bytes)
          // 
          write<unsigned int>(0x10, 0x04030201);

          //
          // LE initiator:
          // - lsb lowest (byte) address
          // - msb highest (byte) address
          //
          std::cout << "\n\nLE Initiator:\n" << std::endl;
          doLEReadAccesses(0x10);

          //
          // BE initiator:
          // - msb lowest (byte) address
          // - lsb highest (byte) address
          //
          std::cout << "\n\nBE Initiator:\n" << std::endl;
          doBEReadAccesses(0x10);

          sc_stop();
          sc_core::wait(SC_ZERO_TIME);
	}

    //
    // Read data to address (data in host endianness)
    //
    template <typename DT>
    void write(unsigned int address, DT data)
    {
    
    	// Single WRITE transaction 
    	
    	cout << name() << " : Single WRITE transaction : "; 
    
    	m_gp.set_address(address);
    	m_gp.set_command(tlm::TLM_WRITE_COMMAND);
    	m_gp.set_burst_data_size(sizeof(DT)); // in bytes
    	m_gp.set_burst_length(1);
    	m_gp.set_burst_mode(tlm::TLM_INCREMENT_BURST);
    	m_gp.set_data_ptr((unsigned char*)&data);
    
        bus_port->nb_transport(&m_gp);
    
    	if(m_gp.get_response_status() == tlm::TLM_OK_RESP)
    	{
    		cout << " OK " << endl;
                    std::cout << "  writing " << sizeof(DT) << " bytes:\n"
                              << "   - address = " << (void*)address << "\n"
                              << "   - data = 0x" << std::hex << (unsigned int)data << std::dec
                              << std::endl;
    
    	}
    	else
    	{
    		cout << " ERROR " << endl;
    	}
    }
    
    //
    // Read data from address (returns data in host endianness)
    //
    template <typename DT>
    DT read(unsigned int address)
    {
    	unsigned int data = 0x0;
    
    	// Single READ transaction 
    	
    	cout << name() << " : Single READ transaction : "; 
    
    	m_gp.set_address(address);
    	m_gp.set_command(tlm::TLM_READ_COMMAND);
    	m_gp.set_burst_data_size(sizeof(DT)); // in bytes
    	m_gp.set_burst_length(1);
    	m_gp.set_burst_mode(tlm::TLM_INCREMENT_BURST);
    	m_gp.set_data_ptr((unsigned char*)&data);
    
        bus_port->nb_transport(&m_gp);
    
    	if(m_gp.get_response_status() == tlm::TLM_OK_RESP)
    	{
    		cout << " OK " << endl;
                    std::cout << "  Reading " << sizeof(DT) << " bytes:\n"
                              << "   - address = " << (void*)address << "\n"
                              << "   - data = 0x" << std::hex << (unsigned int)data << std::dec
                              << std::endl;
    	}
    	else
    	{
    		cout << " ERROR " << endl;
    	}
        return data;
    }


private:

    tlm::tlm_generic_payload  m_gp;

	// GP tests
	void test_single_write();
	void test_single_read();

};

#endif
