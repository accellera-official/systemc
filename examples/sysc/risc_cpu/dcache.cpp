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
 
  dcache.cpp -- Data Cache Unit.
 
  Original Author: Martin Wang, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/


#include "systemc.h"
#include "dcache.h"
#include "directive.h"


void dcache::entry()
{
  unsigned int	address;			// address to DataCache
  unsigned int 	reg_tmp=0;
  unsigned int  dest_tmp = 0;


  while (true) {
    	do { wait(); } while ( !(cs == true) ); 
     	dest_tmp = dest.read();
        reg_tmp = dest.read();
    	address = addr.read();
    	if (we.read() == true) { 		// Write operation
      		wait();
      		out_valid.write(false);
      		dmemory[address] = datain.read();
      		dsmemory[address] = statein.read();
      		dtagmemory[address] = addr.read();
		cout << "\t\t\t\t\t\t\t-------------------------------" << endl;
		printf("\t\t\t\t\t\t\tDCU :St %x->mem[%d]", dmemory[address], address);
		cout << " at CSIM " << sc_time_stamp() << endl;
		cout << "\t\t\t\t\t\t\t-------------------------------" << endl;
		wait();
    	}
    	else { 					// Read operation
		wait(); 	
        	dataout.write(dmemory[address]);
		stateout.write(dsmemory[address]);
		destout.write(dest_tmp);
        	out_valid.write(true);
		cout << "\t\t\t\t\t\t\t-------------------------------" << endl;
		printf("\t\t\t\t\t\t\tDCU :ld %d<-mem[%d]->R%d\n", dmemory[address], address, dest_tmp);
		printf("\t\t\t\t\t\t\tDCU :Tag = 0x%x", dtagmemory[address]);
		if (dsmemory[address] == 3) {
			printf(" (M)");
		} else if (dsmemory[address] == 2) {
			printf(" (E)");
		} else  if (dsmemory[address] == 1) {
			printf(" (S)");
		} else if (dsmemory[address] == 0) {
			printf(" (I)");
		} else 
			printf(" (X)");
		cout << " at CSIM " << sc_time_stamp() << endl;
		cout << "\t\t\t\t\t\t\t-------------------------------" << endl;
        	wait();
        	out_valid.write(false);
        	wait();
    	}    
  }
} // end of entry function

