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

#include "tlm_initiator.h"

void tlm_initiator::test_without_be()
{
    unsigned int address = 0;

    // Test 256-bit access
    std::cout << std::endl;
    prepare_arrays(256);
    prepare_address_and_data< sc_dt::sc_biguint<256> >(256,
                                                       address,
                                                       reg256);
    write<256>(address,aux_data);
    read<256>(address,rd_data);
    compare_arrays(256);
    
    // Test 128-bit access 
    std::cout << std::endl;
    prepare_arrays(128, true);
    prepare_address_and_data< sc_dt::sc_bigint<128> >(128,
                                                      address,
                                                      reg128);
    write<128>(address,aux_data);
    read<128>(address,rd_data);
    compare_arrays(128);
    
    // Test 64-bit access 
    std::cout << std::endl;
    prepare_arrays(64);
    prepare_address_and_data< sc_dt::sc_uint<64> >(64,
                                                   address,
                                                   reg64);  
    write<64>(address,aux_data);
    read<64>(address,rd_data);
    compare_arrays(64);
    
    // Test 32-bit access 
    std::cout << std::endl;
    prepare_arrays(32, true);
    prepare_address_and_data< unsigned int >(32,
                                             address,
                                             reg32);
    write<32>(address,aux_data);
    read<32>(address,rd_data);
    compare_arrays(32);
    
    // Test 16-bit access 
    std::cout << std::endl;
    prepare_arrays(16);
    prepare_address_and_data< short >(16,
                                      address,
                                      reg16);
    write<16>(address,aux_data);
    read<16>(address,rd_data);
    compare_arrays(16);
    
    // Test 8-bit access 
    std::cout << std::endl;
    prepare_arrays(8, true);
    prepare_address_and_data< unsigned char >(8,
                                              address,
                                              reg8);
    write<8>(address,aux_data);
    read<8>(address,rd_data);
    compare_arrays(8);
}

void tlm_initiator::test_with_be()
{
    unsigned int address = 0;
    
    // Test 256-bit access
    std::cout << std::endl;
    prepare_arrays(256,true);
    prepare_address_and_data< sc_dt::sc_biguint<256> >(256,
                                                       address,
                                                       reg256);
    write<256>(address,aux_data, be, be_length);
    read<256>(address,rd_data, be, be_length);
    compare_arrays(256);

    // Test 128-bit access 
    std::cout << std::endl;
    prepare_arrays(128);
    prepare_address_and_data< sc_dt::sc_bigint<128> >(128,
                                                      address,
                                                      reg128);  
    write<128>(address,aux_data, be, be_length);
    read<128>(address,rd_data, be, be_length);
    compare_arrays(128);
    
    // Test 64-bit access 
    std::cout << std::endl;
    prepare_arrays(64,true);
    prepare_address_and_data< sc_dt::sc_uint<64> >(64,
                                                   address,
                                                   reg64);  
    write<64>(address,aux_data, be, be_length);
    read<64>(address,rd_data, be, be_length);
    compare_arrays(64);
    
    // Test 32-bit access 
    std::cout << std::endl;
    prepare_arrays(32);
    prepare_address_and_data< unsigned int >(32,
                                             address,
                                             reg32);
    write<32>(address,aux_data, be, be_length);
    read<32>(address,rd_data, be, be_length);
    compare_arrays(32);
    
    // Test 16-bit access 
    std::cout << std::endl;
    prepare_arrays(16,true);
    prepare_address_and_data< short >(16,
                                      address,
                                      reg16);
    write<16>(address,aux_data, be, be_length);
    read<16>(address,rd_data, be, be_length);
    compare_arrays(16);
    
    // Test 8-bit access 
    std::cout << std::endl;
    prepare_arrays(8);
    prepare_address_and_data< unsigned char >(8,
                                              address,
                                              reg8);
    write<8>(address,aux_data, be, be_length);
    read<8>(address,rd_data, be, be_length);
    compare_arrays(8);
}

void tlm_initiator::test_customized_be()
{
    unsigned int address = 0;
    bool* aux_be = new bool [4];
    
    // Test 256-bit access - Customize byte-enables
    
    std::cout << std::endl;
    std::cout << name() << " : case when WRITE uses BE and READ uses same BE "; 
    aux_be[0] = true; 
    aux_be[1] = true;
    aux_be[2] = true; 
    aux_be[3] = true;		
    std::cout << std::endl;
    prepare_arrays(256);
    prepare_address_and_data< sc_dt::sc_biguint<256> >(256,
                                                       address,
                                                       reg256);  
    write<256>(address,aux_data, aux_be, 4);
    read<256>(address,rd_data, aux_be, 4);
    compare_arrays(256);
    std::cout << name() << " : BE    ARRAY = "; 
    for(int i=0;i<4;i++){std::cout << (int)aux_be[i];}
    std::cout << " : BE  LENGTH = " << 4;
    std::cout << std::endl;
    
    
    std::cout << std::endl;
    std::cout << name() << " : case when WRITE uses BE and READ uses same BE "; 
    aux_be[0] = true;
    aux_be[1] = true;
    aux_be[2] = false;
    aux_be[3] = false;		
    std::cout << std::endl;
    prepare_arrays(256,true);
    prepare_address_and_data< sc_dt::sc_biguint<256> >(256,
                                                       address,
                                                       reg256);
    write<256>(address,aux_data, aux_be, 4);
    read<256>(address,rd_data, aux_be, 4);
    compare_arrays(256);
    std::cout << name() << " : BE    ARRAY = "; 
    for(int i=0;i<4;i++){std::cout << (int)aux_be[i];}
    std::cout << " : BE  LENGTH = " << 4;
    std::cout << std::endl;
    
    std::cout << std::endl;
    std::cout << name() << " : case when WRITE uses BE and READ does not "; 
    aux_be[0] = true;
    aux_be[1] = true;
    aux_be[2] = false;
    aux_be[3] = false;
    std::cout << std::endl;
    prepare_arrays(256,true);
    prepare_address_and_data< sc_dt::sc_biguint<256> >(256,
                                                       address,
                                                       reg256);
    write<256>(address,aux_data, aux_be, 4);
    read<256>(address,rd_data);
    compare_arrays(256);
    std::cout << name() << " : BE    ARRAY = "; 
    for(int i=0;i<4;i++){std::cout << (int)aux_be[i];}
    std::cout << " : BE  LENGTH = " << 4;
    std::cout << std::endl;
    
    
    // Test 32-bit access - Customize byte-enables
    
    std::cout << std::endl;
    std::cout << name() << " : case when WRITE uses BE and READ uses same BE "; 
    aux_be[0] = true; 
    aux_be[1] = true;
    std::cout << std::endl;
    prepare_arrays(32);
    prepare_address_and_data< unsigned int >(32,
                                             address,
                                             reg32);
    write<32>(address,aux_data, aux_be, 2);
    read<32>(address,rd_data, aux_be, 2);
    compare_arrays(32);
    std::cout << name() << " : BE    ARRAY = "; 
    for(int i=0;i<2;i++){std::cout << (int)aux_be[i];}
    std::cout << " : BE  LENGTH = " << 2;
    std::cout << std::endl;
    
    
    std::cout << std::endl;
    std::cout << name() << " : case when WRITE uses BE and READ uses same BE "; 
    aux_be[0] = true;
    aux_be[1] = false;
    std::cout << std::endl;
    prepare_arrays(32,true);
    prepare_address_and_data< unsigned int >(32,
                                             address,
                                             reg32);
    write<32>(address,aux_data, aux_be, 2);
    read<32>(address,rd_data, aux_be, 2);
    compare_arrays(32);
    std::cout << name() << " : BE    ARRAY = "; 
    for(int i=0;i<2;i++){std::cout << (int)aux_be[i];}
    std::cout << " : BE  LENGTH = " << 2;
    std::cout << std::endl;
    
    std::cout << std::endl;
    std::cout << name() << " : case when WRITE uses BE and READ does not "; 
    aux_be[0] = true;
    aux_be[1] = false;
    std::cout << std::endl;
    prepare_arrays(32,true);
    prepare_address_and_data< unsigned int >(32,
                                             address,
                                             reg32);
    write<32>(address,aux_data, aux_be, 2);
    read<32>(address,rd_data);
    compare_arrays(32);
    std::cout << name() << " : BE    ARRAY = "; 
    for(int i=0;i<2;i++){std::cout << (int)aux_be[i];}
    std::cout << " : BE  LENGTH = " << 2;
    std::cout << std::endl;
    
    
    delete [] aux_be;
}


void tlm_initiator::prepare_arrays(int nbits, bool shift) 
{
    wr_data = new unsigned char [nbits/8]; 
    aux_data = new unsigned char [nbits/8]; 
    rd_data = new unsigned char [nbits/8];
    be = new bool [nbits/8];
    be_length = nbits/8;
    
    // Initialiaze arrays
    for(int i=0;i<nbits/8;i++)
    {
        if(shift) {
            wr_data[i] = i << 4;
            aux_data[i] = 0; rd_data[i] = 0; be[i] = true;
        } else {
            wr_data[i] = i;
            aux_data[i] = 0; rd_data[i] = 0; be[i] = true;
        }
    }
}

void tlm_initiator::compare_arrays(int nbits) 
{
    // check if everything went ok
    bool error_flag = false;
    
    for(int i=0;i<nbits/8;i++)
    {
        if(aux_data[i]!= rd_data[i])
            error_flag =true;
    }
    
    if(error_flag)
        std::cout << name() << " : read and write " << nbits
                  << "-bit operation : DON'T MATCH \n";
    else
        std::cout << name() << " : read and write " << nbits
                  << "-bit operation : MATCH  \n";
    
    std::cout << name() << " : WRITE ARRAY = 0x" << std::hex;
    for(int i=0; i<nbits/8; i++)
    {
        std::cout.width(2);
        std::cout.fill('0');
        std::cout << (int)aux_data[i];
    }
    std::cout << std::endl;
    std::cout << name() << " : READ  ARRAY = 0x" << std::hex; 
    for(int i=0;i<nbits/8;i++)
    {
        std::cout.width(2);
        std::cout.fill('0');
        std::cout << (int)rd_data[i];
    }
    std::cout << std::endl;
    
    std::cout << std::dec;
    
    delete [] wr_data;	wr_data = 0;
    delete [] aux_data;	aux_data = 0; 
    delete [] rd_data;	rd_data = 0;
    delete [] be; be = 0;
}
