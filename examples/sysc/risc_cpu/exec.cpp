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
 
  exec.cpp -- Integer Execution Unit.
 
  Original Author: Martin Wang, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/


#include "systemc.h"
#include "exec.h"
#include "directive.h"


void exec::entry(){

  int			        opcode_tmp = 0;
  int				add1_tmp = 0;
  signed int		        dina_tmp = 0;
  signed int		        dinb_tmp = 0;
  sc_dt::int64		        dout_tmp = 0;
  unsigned int		        dest_tmp = 0;

  //
  // main loop
  //
  // 

  // initialization of output

  wait(3);
  while(1) {
   	if (in_valid.read() == true) {
		 dina_tmp = dina.read();
		 dinb_tmp = dinb.read();
		 opcode_tmp = opcode.read();
		 dest_tmp = dest.read();

    		// output MUX
    		switch (opcode_tmp) {
        		case 0:         // Stall 
                	// dout_tmp = dout_tmp;  // keeps its value
                	wait();
                	break;
		case 1: 	// add with carry
    			dout_tmp = dina_tmp + dinb_tmp + add1_tmp;
			wait();
			break;
		case 2: 	// sub with carry
    			dout_tmp = dina_tmp - dinb_tmp - add1_tmp;
			wait();
			break;
        	case 3:         // add without carry
               	 	dout_tmp = dina_tmp + dinb_tmp;
                	wait();
                	break;
        	case 4:         // sub without carry
               	 	dout_tmp = dina_tmp - dinb_tmp;
                	wait();
                	break;
        	case 5:         // multiply assume 2 clock cycle multiplication
                	dout_tmp = dina_tmp * dinb_tmp;
                	wait();	// so that BC has something to do
                	wait();
                	break;
        	case 6:         // divide assume 2 clock cycle division
			if (dinb_tmp == 0) {
				printf("Division Exception - Divide by zero \n");
			} else {
                		dout_tmp = dina_tmp / dinb_tmp;
			}
                	wait();	 // so that BC has something to do
                	wait();
                	break;
        	case 7:         // bitwise NAND
                	dout_tmp = ~(dina_tmp & dinb_tmp);
                	wait();
                	break;
        	case 8:         // bitwise AND 
                	dout_tmp = dina_tmp & dinb_tmp;
                	wait();
			break;
        	case 9:         // bitwise OR
                	dout_tmp = dina_tmp | dinb_tmp;
                	wait();
			break;
        	case 10:         // bitwise XOR
               	 	dout_tmp = dina_tmp ^ dinb_tmp;
                	wait();
			break;
        	case 11:         // bitwise complement
                	dout_tmp = ~ dina_tmp;
                	wait();
			break;
        	case 12:         // left shift
                	dout_tmp = dina_tmp << dinb_tmp;
                	wait();
			break;
        	case 13:         // right shift
                	dout_tmp = dina_tmp >> dinb_tmp;
                	wait();
			break;
		case 14: 	// modulo
    			dout_tmp = dina_tmp % dinb_tmp;
			wait();
			break;
		default:
			printf("ALU:      Bad Opcode %d.\n",opcode_tmp);
			break;
    		}
    

    		dout.write(static_cast<signed int>(dout_tmp));
    		out_valid.write(true);
		destout.write(dest_tmp);

		if (dout_tmp == 0) {
			Z.write(true);
		} else {
			Z.write(false);
		}
                sc_dt::int64 abs_dout = dout_tmp >= 0 ? dout_tmp : -dout_tmp;
                const sc_dt::int64 carry_mask = sc_dt::int64(1) << 32;
                if (abs_dout & carry_mask) {
                       C.write(true);
                } else {
                       C.write(false);
                }
                if (abs_dout > carry_mask) {
			V.write(true);
		} else {
			V.write(false);
		}
		printf("\t\t\t\t\t\t\t-------------------------------\n");
    		cout << "\t\t\t\t\t\t\tALU :" << " op= " << opcode_tmp 
			<< " A= " << dina_tmp << " B= " << dinb_tmp << endl;
    		cout << "\t\t\t\t\t\t\tALU :" << " R= " << dout_tmp << "-> R" << dest_tmp;
             	cout << " at CSIM " << sc_time_stamp() << endl;
		printf("\t\t\t\t\t\t\t-------------------------------\n");
    		wait();
    		out_valid.write(false);
    		wait();

	} else {
		wait();
	}
	
  }

}

