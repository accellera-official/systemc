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

  bios.cpp -- System Bios.

  Original Author: Martin Wang, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc.h"
#include "bios.h"
#include "directive.h"


void bios::entry()
{
  unsigned address;

  while (true) {
	do { wait(); } while ( !(cs == true) );
	address = addr.read();
	if (address < BOOT_LENGTH) {		// in BOOTING STAGE
    		if (we.read() == true) { // Write operation
      			wait(wait_cycles-1);
      			imemory[address] = datain.read();
    		}
    		else { // Read operation
			if (wait_cycles > 2) {
				wait(wait_cycles-2); // Introduce delay needed
			}
			dataout.write(imemory[address]);

			if (PRINT_BIOS) {
				printf("------------------------\n");
				printf("BIOS: fetching mem[%d]\n", address);
				printf("BIOS: (%0x)", imemory[address]);
 				cout.setf(ios::dec,ios::basefield);
   				cout << " at CSIM " << sc_time_stamp() << endl;
				printf("------------------------\n");
			}

			bios_valid.write(true);
      			wait();
			bios_valid.write(false);
			wait();
    		}
	} else {
		bios_valid.write(false);
		wait();
	}
  }
} // end of entry function
