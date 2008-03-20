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

#define WORD 4
#define HALF 2
#define BYTE 1

class tlm_initiator : public sc_core::sc_module
{
public:
    
    SimpleInitiatorSocket< 32 > socket;  // bus data width in bits

    SC_HAS_PROCESS(tlm_initiator);

    // Constructor
    tlm_initiator(sc_core::sc_module_name name_,
                  tlm::tlm_endianness endianness = tlm::TLM_LITTLE_ENDIAN)
        : sc_core::sc_module(name_)
        , socket("socket")
        , m_endianness(endianness)
    {
        SC_THREAD(main);

		m_socket_width = socket.get_bus_width()/8;
    }

    // Destructor
    ~tlm_initiator()
    {
    }

    void main()
    {
		unsigned int address = 0x0;
		unsigned int word_1 = 0x04030201;
		unsigned int word_2 = 0x08070605;
		unsigned short half_1 = 0x0403;
		unsigned short half_2 = 0x0605;
		unsigned char byte_1 = 0x02;

		std::cout << std::endl;
		if(tlm::hostHasLittleEndianness())
			std::cout << "< LITTLE-ENDIAN HOST "; 
		else
			std::cout << "< BIG-ENDIAN HOST "; 
		std::cout << "-";
		if(m_endianness == tlm::TLM_LITTLE_ENDIAN)
			std::cout << " LITTLE-ENDIAN MODEL >"; 
		else
			std::cout << " BIG-ENDIAN MODEL >"; 
		std::cout << std::endl; 

		std::cout << std::endl;
                std::cout << " ###### TEST ALIGNED WORDS BURST ###### " << std::endl; 
		for(int i=0;i<8;i++) wr_data[i] = 0; 
		for(int i=0;i<8;i++) rd_data[i] = 0; 
		tlm::copy_word_to_array< unsigned int >(word_1, 
                                         tlm::get_subword_offset(0, m_socket_width, WORD, m_endianness), 
										 WORD,
										 wr_data, 
										 0, 0);
		tlm::copy_word_to_array< unsigned int >(word_2, 
			                             tlm::get_subword_offset(0, m_socket_width, WORD, m_endianness),
										 WORD, 
										 wr_data+m_socket_width, 
										 0, 0);
		write(address,wr_data,8);
		read(address,rd_data,8);
		compare_arrays(wr_data, rd_data, 8); 

		
		std::cout << std::endl;
                std::cout << " ###### TEST ALIGNED PART-WORDS BURST ###### " << std::endl; 
		for(int i=0;i<8;i++) wr_data[i] = 0; 
		for(int i=0;i<8;i++) rd_data[i] = 0; 
		for(int i=0;i<8;i++) be[i] = false;
		tlm::copy_word_to_array< unsigned int >(word_1,
                                         tlm::get_subword_offset(0, m_socket_width, WORD, m_endianness),
										 WORD,
										 wr_data,
										 0, 0);

		unsigned int offsethalf;
		if(m_endianness == tlm::TLM_LITTLE_ENDIAN)
		{
			offsethalf = 0;

			tlm::copy_word_to_array< unsigned short >(half_2,
			                             tlm::get_subword_offset(offsethalf, m_socket_width, HALF, m_endianness),
										 HALF,
										 wr_data+m_socket_width,
										 0, 0);
			write(address,wr_data,6);
			read(address,rd_data,6);
			compare_arrays(wr_data, rd_data, 8);
		}
		else
		{
			offsethalf = 2;

			tlm::copy_word_to_array< unsigned short >(half_2,
			                             tlm::get_subword_offset(offsethalf, m_socket_width, HALF, m_endianness),
										 HALF,
										 wr_data+m_socket_width,
										 0, 0);
			if(tlm::hostHasLittleEndianness())
			{
              be[0] = be[1] = be[2] = be[3] = be[4] = be[5] = true;
              be[6] = be[7] = false;
			}
			else
			{
              be[0] = be[1] = be[2] = be[3] = true;
              be[4] = be[5] = false;
              be[6] = be[7] = true;
			}

			write(address,wr_data,8,be, 8);
			read(address,rd_data,8,be, 8);
			compare_arrays(wr_data, rd_data, 8);
		}


		std::cout << std::endl;
                std::cout << " ###### TEST NON-ALIGNED BYTES USING BYTE_ENABLE ###### " << std::endl;
		for(int i=0;i<8;i++) wr_data[i] = 0; 
		for(int i=0;i<8;i++) rd_data[i] = 0;
		for(int i=0;i<8;i++) be[i] = false;

		unsigned int OffsetSignificantByte;
		if(m_endianness == tlm::TLM_LITTLE_ENDIAN)
			OffsetSignificantByte = 1;
		else
			OffsetSignificantByte = 2;

		tlm::copy_word_to_array< unsigned char >(byte_1, 
                                         tlm::get_subword_offset(OffsetSignificantByte, m_socket_width, BYTE, m_endianness), 
										 BYTE,
										 wr_data, 
										 0, 0);
		be[tlm::get_subword_offset(OffsetSignificantByte, m_socket_width, BYTE, m_endianness)] = true; 
		write(address,wr_data,4);
		read(address,rd_data,4);
		compare_arrays(wr_data, rd_data, 8); 


		std::cout << std::endl;
                std::cout << " ###### TEST NON-ALIGNED PART-WORDS USING BYTE_ENABLE ###### " << std::endl;
		for(int i=0;i<8;i++) wr_data[i] = 0;
		for(int i=0;i<8;i++) rd_data[i] = 0;
		for(int i=0;i<8;i++) be[i] = false;

		unsigned int OffsetSignificantHalf_1;
		unsigned int OffsetSignificantHalf_2;
		if(m_endianness == tlm::TLM_LITTLE_ENDIAN){
			OffsetSignificantHalf_1 = 2;
			OffsetSignificantHalf_2 = 0;
		}else{
			OffsetSignificantHalf_1 = 0;
			OffsetSignificantHalf_2 = 2;
		}
		tlm::copy_word_to_array< unsigned short >(half_1,
			                           tlm::get_subword_offset(OffsetSignificantHalf_1, m_socket_width, HALF, m_endianness),
									   HALF,
									   wr_data,
									   0, 0);
		tlm::copy_word_to_array< unsigned short >(half_2,
			                           tlm::get_subword_offset(OffsetSignificantHalf_2, m_socket_width, HALF, m_endianness),
									   HALF, 
									   wr_data+m_socket_width,
									   0, 0);

		if(tlm::hostHasLittleEndianness())
		{
			be[0] = be[1] = false;
			be[2] = be[3] = be[4] = be[5] = true;
			be[6] = be[7] = false;
		}
		else
		{
			be[0] = be[1] = true;
			be[2] = be[3] = be[4] = be[5] = false;
			be[6] = be[7] = true;
		}
		write(address,wr_data,8, be, 8);
		read(address,rd_data,8, be, 8);
		compare_arrays(wr_data, rd_data, 8);

		std::cout << std::endl;
                std::cout << " ###### TEST NON-ALIGNED PART-WORDS WITH UNLIGNED ADDRESS ###### " << std::endl;
		for(int i=0;i<8;i++) wr_data[i] = 0;
		for(int i=0;i<8;i++) rd_data[i] = 0;

		OffsetSignificantHalf_1 = 2;
		OffsetSignificantHalf_2 = 0;

		tlm::copy_word_to_array< unsigned short >(half_1,
			                           tlm::get_subword_offset(OffsetSignificantHalf_1, m_socket_width, HALF, m_endianness),
									   HALF,
									   wr_data,
									   0, 0);
		tlm::copy_word_to_array< unsigned short >(half_2,
			                           tlm::get_subword_offset(OffsetSignificantHalf_2, m_socket_width, HALF, m_endianness),
									   HALF, 
									   wr_data+m_socket_width,
									   0, 0);
		write(address+2,wr_data,4);
		read(address+2,rd_data,4);
		compare_arrays(wr_data, rd_data, 8);

		std::cout << std::endl;

        sc_core::sc_stop();
        sc_core::wait(sc_core::SC_ZERO_TIME);
    }

private:

    tlm::tlm_generic_payload  m_gp;

    unsigned char wr_data[8];
    unsigned char rd_data[8];
    unsigned char be[8];
    unsigned int  be_length;

    tlm::tlm_endianness m_endianness;
    unsigned int        m_socket_width;

	void write(sc_dt::uint64 address,
                   unsigned char *data,
				   unsigned int nr_bytes,
                   unsigned char* byte_enable = 0,
                   unsigned int byte_enable_length = 0);
    
	void read(sc_dt::uint64 address,
                  unsigned char *data,
                  unsigned int nr_bytes,
                  unsigned char* byte_enable = 0,
                  unsigned int byte_enable_length = 0);

	void compare_arrays(unsigned char* wrdata, unsigned char* rddata, int nbytes) ;
        
};


#endif
