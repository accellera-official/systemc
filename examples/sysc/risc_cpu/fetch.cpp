/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
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

/*****************************************************************************
 
  fetch.cpp -- Instruction Fetch Unit.
 
  Original Author: Martin Wang, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/
 

#include "systemc.h"
#include "fetch.h"
#include "directive.h"


void fetch::entry()
{
   unsigned addr_tmp=0;
   unsigned datao_tmp=0;
   unsigned datai_tmp=0;
   unsigned lock_tmp = 0;

   addr_tmp = 1;
   datao_tmp = 0xdeadbeef;

   // Now booting from default values
   reset.write(true);
   ram_cs.write(true);
   ram_we.write(false);
   address.write(addr_tmp);
   wait(memory_latency); 		// For data to appear

   do { wait(); } while ( !((bios_valid == true) || (icache_valid == true)) );
   if (stall_fetch.read() == true) {
   	datai_tmp = 0;
   } else {
   	datai_tmp = ramdata.read();
   }
   cout.setf(ios::hex,ios::basefield);
   cout << "-----------------------" << endl;   
   cout << "IFU :" << " mem=0x" << datai_tmp << endl;
   cout << "IFU : pc= " << addr_tmp ;   
   cout.setf(ios::dec,ios::basefield);
   cout << " at CSIM " << sc_time_stamp() << endl;
   cout << "-----------------------" << endl;   

   instruction_valid.write(true);
   instruction.write(datai_tmp);
   program_counter.write(addr_tmp);
   ram_cs.write(false);
   wait();
   instruction_valid.write(false);
   addr_tmp++;
   wait();


  while (true) {
	if (addr_tmp == 5) {
		reset.write(false);
	}
	if (interrupt.read() == true) {

    		ram_cs.write(true);
		addr_tmp = int_vectno.read();
    		ram_we.write(false);
    		wait(memory_latency); 
    		datai_tmp = ramdata.read();

		printf("IF ALERT: **INTERRUPT**\n");
   		cout.setf(ios::hex,ios::basefield);
   		cout << "------------------------" << endl;   
   		cout << "IFU :" << " mem=0x" << datai_tmp << endl;
   		cout << "IFU : pc= " << addr_tmp ;   
   		cout.setf(ios::dec,ios::basefield);
   		cout << " at CSIM " << sc_time_stamp() << endl;
   		cout << "------------------------" << endl;   

		instruction_valid.write(true);
		instruction.write(datai_tmp);
    		ram_cs.write(false);
		interrupt_ack.write(true);
		if (next_pc.read() == true) { addr_tmp++; }
		wait();
                instruction_valid.write(false);
		interrupt_ack.write(false);
		wait();

	}

	if (branch_valid.read() == true) {
		printf("IFU ALERT: **BRANCH**\n");
		lock_tmp ++;
    		ram_cs.write(true);
		addr_tmp = branch_address.read();
    		ram_we.write(false);
    		wait(memory_latency); 
		do { wait(); } while ( !((bios_valid == true) || (icache_valid == true)) );
    		datai_tmp = ramdata.read();

   		cout.setf(ios::hex,ios::basefield);
   		cout << "------------------------" << endl;   
   		cout << "IFU :" << " mem=0x" << datai_tmp << endl;
   		cout << "IFU : pc= " << addr_tmp ;   
   		cout.setf(ios::dec,ios::basefield);
   		cout << " at CSIM " << sc_time_stamp() << endl;
   		cout << "------------------------" << endl;   

		instruction_valid.write(true);
		instruction.write(datai_tmp);
    		ram_cs.write(false);
		if (next_pc.read() == true) { addr_tmp++; }
		wait();
                instruction_valid.write(false);
		wait();
	} else {
		lock_tmp = 0;
    		ram_cs.write(true);
    		address.write(addr_tmp);
    		ram_we.write(false);
    		wait(memory_latency); // For data to appear
		do { wait(); } while ( !((bios_valid == true) || (icache_valid == true)) );
    		datai_tmp = ramdata.read();

   		cout.setf(ios::hex,ios::basefield);
   		cout << "------------------------" << endl;   
   		cout << "IFU :" << " mem=0x" << datai_tmp << endl;
   		cout << "IFU : pc= " << addr_tmp ;   
   		cout.setf(ios::dec,ios::basefield);
   		cout << " at CSIM " << sc_time_stamp() << endl;
   		cout << "------------------------" << endl;   

                instruction_valid.write(true);
                instruction.write(datai_tmp);
   		program_counter.write(addr_tmp);
		branch_clear.write(false);
    		ram_cs.write(false);
		if (next_pc.read() == true) { addr_tmp++; }
                wait();
                instruction_valid.write(false);
    		wait();
        }
	if (lock_tmp == 1) {
		branch_clear.write(true);
		wait();
	}

/* Unless you wanted to write to your instruction cache.  Usually Instruction cache is read only.
    	// Write memory location first
    	chip_select.write(true);
    	write_enable.write(true);
    	address.write(addr);
    	instruction_write.write(datao);
    	printf("fetch: Data Written = %x at address %x\n", datao, addr);
    	wait(memory_latency); // To make all the outputs appear at the interface

    	// some process functionality not shown here during which chip
    	// chip select is deasserted and bus is tristated
    	chip_select.write(false);
    	instruction_write.write(0);
    	wait();

*/

  }
} // end of entry function

