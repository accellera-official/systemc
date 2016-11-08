/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

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
  unsigned int  dest_tmp = 0;


  while (true) {
    	do { wait(); } while ( !(cs == true) ); 
     	dest_tmp = dest.read();
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

