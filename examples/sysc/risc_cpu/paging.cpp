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
 
  paging.cpp -- Instruction Paging Unit.
 
  Original Author: Martin Wang, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/
 

#include "systemc.h"
#include "paging.h"
#include "directive.h"

void paging::entry()
{
 int		address=0;
 int	 	dataout_tmp =0;

 while (true) {
    	do { wait(); } while ( !(paging_csin == true) ); 
	address = logical_address.read();

	if (address >= 5) {
    		if (paging_wein.read() == true) { 	// Write operation
			paging_dout.write(paging_din.read());
			paging_csout.write(true);
			paging_weout.write(true);
			physical_address.write(logical_address.read());
      			wait();
			paging_csout.write(false);
			paging_weout.write(false);
			
    		}
    		else { 					// Read operation
			paging_csout.write(true);
			paging_weout.write(false);
			physical_address.write(logical_address.read());
			wait();
			do { wait(); } while ( !(icache_validin == true) );
			dataout_tmp = icache_din.read();
			
			if (PRINT_PU){
   				cout << "-----------------------" << endl;
				printf( "PAGE : mem=%x\n",dataout_tmp);
   				cout << "PAGE : " ;
   				cout << " at CSIM " << sc_time_stamp() << endl;
   				cout << "-----------------------" << endl;
			}

			dataout.write(icache_din.read());
			data_valid.write(true);
			paging_csout.write(false);
			wait(); 
			data_valid.write(false);
			wait();
    		}    
	}
 }
} // end of entry function

