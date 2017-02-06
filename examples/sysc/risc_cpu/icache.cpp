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
 
  icache.cpp -- Instruction Cache Unit.
 
  Original Author: Martin Wang, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date: Gene Bushuyev, Synopsys, Inc.
  Description of Modification: - bug - read/write outside of allocated memory
 
      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc.h"
#include "icache.h"
#include "directive.h"


void icache::entry()
{
    unsigned int 	address;

 while (true) {
    	do { wait(); } while ( !(cs == true) ); 
    	address = addr.read();
	if (address == BOOT_LENGTH) {
		 printf("ICU ALERT: *********************************************************************\n");
		 printf("         : *****************************AFTER RESET*****************************\n");
		 printf("ICU ALERT: *********************************************************************\n");
	}
	if (address >= BOOT_LENGTH) {
		if (ld_valid.read() == true) {
			pid = ld_data.read();
			printf("------------------------\n");
			printf("ICU: PID = [%d]", pid); 
                       	cout << " at CSIM " << sc_time_stamp() << endl;
			printf("------------------------\n");
      			wait();
      			wait();
		}
    		if (we.read() == true) { // Write operation
      			wait();
			if (address < MAX_CODE_LENGTH)
                          icmemory[address] = datain.read();
                        else
                          printf("ICU ALERT: **MEMORY OUT OF RANGE**\n");
      			wait();
    		}
    		else { // Read operation
			wait(); // Introduce delay needed
			if (address >= MAX_CODE_LENGTH) {
				dataout.write(0xffffffff);
				printf("ICU ALERT: **MEMORY OUT OF RANGE**\n");
			}
                        else
        		  dataout.write(icmemory[address]);

			icache_valid.write(true);

			if (PRINT_ICU) {
				printf("------------------------\n");
				printf("ICU: fetching mem[%d]\n", address);
                                if (address < MAX_CODE_LENGTH) 
				  printf("ICU: (%0x)", icmemory[address]); 
       		                cout.setf(ios::dec,ios::basefield);
                        	cout << " at CSIM " << sc_time_stamp() << endl;
				printf("------------------------\n");
			}

      			wait();
			icache_valid.write(false);
      			wait();
    		}    
	}
 }
} // end of entry function

