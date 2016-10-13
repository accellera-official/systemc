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
 
  mmxu.cpp -- MMX-Like Execution Unit.
 
  Original Author: Martin Wang, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/


#include "systemc.h"
#include "mmxu.h"
#include "directive.h"

#ifdef __SUNPRO_CC
// Martin, fix the ios stuff!
#define ios std::ios_base
#endif

void mmxu::entry(){

  int			        opcode_tmp = 0;
  unsigned int		        dout_tmp = 0;
  unsigned int		        dest_tmp = 0;
  const char * 			opcode_encode;

  unsigned int			mmxa_tmp = 0;
  unsigned int			mmxb_tmp = 0;
  

  unsigned int			mmxa0_tmp = 0;
  unsigned int			mmxa1_tmp = 0;
  unsigned int			mmxa2_tmp = 0;
  unsigned int			mmxa3_tmp = 0;
  unsigned int			mmxb0_tmp = 0;
  unsigned int			mmxb1_tmp = 0;
  unsigned int			mmxb2_tmp = 0;
  unsigned int			mmxb3_tmp = 0;
  unsigned int			mmxc0_tmp = 0;
  unsigned int			mmxc1_tmp = 0;
  unsigned int			mmxc2_tmp = 0;
  unsigned int			mmxc3_tmp = 0;
  unsigned int			mmxcU_tmp = 0;
  unsigned int			mmxcL_tmp = 0;

  //
  // main loop
  //
  // 

  // initialization of output

  wait(3);
  while(1) {
   	if (mmx_valid.read() == true) {
		 mmxa_tmp = mmxa.read();
		 mmxb_tmp = mmxb.read();

                 mmxa0_tmp = (mmxa_tmp & 0x000000ff) ;        
                 mmxb0_tmp = (mmxb_tmp & 0x000000ff) ;        
                 mmxa1_tmp = (mmxa_tmp & 0x0000ff00) >> 8 ;        
                 mmxb1_tmp = (mmxb_tmp & 0x0000ff00) >> 8 ;        
                 mmxa2_tmp = (mmxa_tmp & 0x00ff0000) >> 16 ;        
                 mmxb2_tmp = (mmxb_tmp & 0x00ff0000) >> 16 ;        
                 mmxa3_tmp = (mmxa_tmp & 0xff000000) >> 24 ;        
                 mmxb3_tmp = (mmxb_tmp & 0xff000000) >> 24 ;        
		 opcode_tmp = opcode.read();
		 dest_tmp = dest.read();

    		// output MUX
    		switch (opcode_tmp) {
       		case 0:         // Stall 
			opcode_encode = "STALL";
                	// dout_tmp = dout_tmp;  // keeps its value
                	wait();
                	break;
        	case 3:         // add 
			opcode_encode = "PADD";
			mmxc3_tmp = mmxa3_tmp + mmxb3_tmp;
			mmxc2_tmp = mmxa2_tmp + mmxb2_tmp;
			mmxc1_tmp = mmxa1_tmp + mmxb1_tmp;
			mmxc0_tmp = mmxa0_tmp + mmxb0_tmp;
			mmxc3_tmp = (mmxc3_tmp << 24) & 0xff000000;
			mmxc2_tmp = (mmxc2_tmp << 16) & 0x00ff0000;
			mmxc1_tmp = (mmxc1_tmp << 8)  & 0x0000ff00;
			dout_tmp  = mmxc0_tmp | mmxc1_tmp | mmxc2_tmp | mmxc3_tmp; 
                	wait();
                	break;
                case 4:         // add with saturation
			opcode_encode = "PADDS";
                        mmxc3_tmp = mmxa3_tmp + mmxb3_tmp;
                        mmxc2_tmp = mmxa2_tmp + mmxb2_tmp;
                        mmxc1_tmp = mmxa1_tmp + mmxb1_tmp;
                        mmxc0_tmp = mmxa0_tmp + mmxb0_tmp;
			if (mmxc3_tmp >= 256) mmxc3_tmp = 0xff;
			if (mmxc2_tmp >= 256) mmxc2_tmp = 0xff;
			if (mmxc1_tmp >= 256) mmxc1_tmp = 0xff;
			if (mmxc0_tmp >= 256) mmxc0_tmp = 0xff;
                        mmxc3_tmp = (mmxc3_tmp << 24) & 0xff000000;
                        mmxc2_tmp = (mmxc2_tmp << 16) & 0x00ff0000;
                        mmxc1_tmp = (mmxc1_tmp << 8)  & 0x0000ff00;
                        dout_tmp  = mmxc0_tmp | mmxc1_tmp | mmxc2_tmp | mmxc3_tmp;
                        wait();
                        break;

                case 5:         // sub
                        opcode_encode = "PSUB";
                        mmxc3_tmp = mmxa3_tmp - mmxb3_tmp;
                        mmxc2_tmp = mmxa2_tmp - mmxb2_tmp;
                        mmxc1_tmp = mmxa1_tmp - mmxb1_tmp;
                        mmxc0_tmp = mmxa0_tmp - mmxb0_tmp;
                        mmxc3_tmp = (mmxc3_tmp << 24) & 0xff000000;
                        mmxc2_tmp = (mmxc2_tmp << 16) & 0x00ff0000;
                        mmxc1_tmp = (mmxc1_tmp << 8)  & 0x0000ff00;
                        dout_tmp  = mmxc0_tmp | mmxc1_tmp | mmxc2_tmp | mmxc3_tmp;
                        wait();
                        break;
                case 6:         // sub with saturation
                        opcode_encode = "PSUBS";
                        mmxc3_tmp = mmxa3_tmp - mmxb3_tmp;
                        mmxc2_tmp = mmxa2_tmp - mmxb2_tmp;
                        mmxc1_tmp = mmxa1_tmp - mmxb1_tmp;
                        mmxc0_tmp = mmxa0_tmp - mmxb0_tmp;
                        if (mmxb3_tmp > mmxa3_tmp) mmxc3_tmp = 0x00;
                        if (mmxb2_tmp > mmxa2_tmp) mmxc2_tmp = 0x00;
                        if (mmxb1_tmp > mmxa1_tmp) mmxc1_tmp = 0x00;
                        if (mmxb0_tmp > mmxa0_tmp) mmxc0_tmp = 0x00;
                        mmxc3_tmp = (mmxc3_tmp << 24) & 0xff000000;
                        mmxc2_tmp = (mmxc2_tmp << 16) & 0x00ff0000;
                        mmxc1_tmp = (mmxc1_tmp << 8)  & 0x0000ff00;
                        dout_tmp  = mmxc0_tmp | mmxc1_tmp | mmxc2_tmp | mmxc3_tmp;
                        wait();
                        break;
                case 7:         // packed multiply add with saturation
				// a3*b3+a2*b2 , a1*b1+a0*b0
                        opcode_encode = "PMADD";
                        mmxc3_tmp = mmxa3_tmp * mmxb3_tmp;
                        mmxc2_tmp = mmxa2_tmp * mmxb2_tmp;
                        mmxc1_tmp = mmxa1_tmp * mmxb1_tmp;
                        mmxc0_tmp = mmxa0_tmp * mmxb0_tmp;
			if (mmxc3_tmp >= 256) mmxc3_tmp = 0xff;
			if (mmxc2_tmp >= 256) mmxc2_tmp = 0xff;
			if (mmxc1_tmp >= 256) mmxc1_tmp = 0xff;
			if (mmxc0_tmp >= 256) mmxc0_tmp = 0xff;
			mmxcU_tmp = mmxc3_tmp + mmxc2_tmp;
			mmxcL_tmp = mmxc1_tmp + mmxc0_tmp;
			if (mmxcU_tmp >= 256) mmxcU_tmp = 0xff;
			if (mmxcL_tmp >= 256) mmxcL_tmp = 0xff;
                        mmxcU_tmp = (mmxcU_tmp << 16) ;
                        dout_tmp  = mmxcU_tmp | mmxcL_tmp; 
                        wait();
                        break;
	
                case 8:         // packed b1 (16bit) b0(16bit) and a1(16 bit) a0(16 bit)
                                // to B1(8bit) B0(8bit) A1(8bit) A0(8bit)
                        opcode_encode = "PACK";
			mmxc3_tmp = mmxb2_tmp << 24; 
			mmxc2_tmp = mmxb0_tmp << 16; 
			mmxc1_tmp = mmxa2_tmp << 8; 
			mmxc0_tmp = mmxa0_tmp ; 
                        dout_tmp  = mmxc3_tmp | mmxc2_tmp | mmxc1_tmp | mmxc0_tmp;
                        wait();
                        break;

                case 9:         // mmx chroma keying
                                // A =green != green green !=green
				// B =green    green green   green
                                //Res=0xff  00   ff    00
                        opcode_encode = "MMXCK";
			if (mmxa3_tmp == mmxb3_tmp) 		
				mmxc3_tmp = 0xff;
			else 	
				mmxc3_tmp = 0x00;
			if (mmxa2_tmp == mmxb2_tmp) 
				mmxc2_tmp = 0xff;
			else 
				mmxc2_tmp = 0x00;
			if (mmxa1_tmp == mmxb1_tmp) 
				mmxc1_tmp = 0xff;
			else 
				mmxc1_tmp = 0x00;
			if (mmxa0_tmp == mmxb0_tmp) 
				mmxc0_tmp = 0xff;
			else 
				mmxc0_tmp = 0x00;
                        mmxc3_tmp = mmxc3_tmp << 24;
                        mmxc2_tmp = mmxc2_tmp << 16;
                        mmxc1_tmp = mmxc1_tmp << 8;
                        // mmxc0_tmp = mmxc0_tmp ;  // keeps its value
                        dout_tmp  = mmxc3_tmp | mmxc2_tmp | mmxc1_tmp | mmxc0_tmp;
                        wait();
                        break;

		default:
			opcode_encode = "INVALID";
			printf("MMX:      Bad Opcode %d.\n",opcode_tmp);
			wait();
			break;
    		}
    

    		mmxdout.write(dout_tmp);
    		mmxout_valid.write(true);
		mmxdestout.write(dest_tmp);

		printf("\t\t\t\t\t\t\t-------------------------------\n");
		cout.setf(ios::hex,ios::basefield);     
    		cout << "\t\t\t\t\t\t\tMMX :" << " op= " << opcode_encode 
			<< " A=0x " << mmxa_tmp << " B=0x " << mmxb_tmp << endl;
    		cout << "\t\t\t\t\t\t\tMMX :" << " C=0x " << dout_tmp << "-> R" << dest_tmp ;
		cout.setf(ios::dec,ios::basefield);     
             	cout	<< " at CSIM " << sc_time_stamp() << endl;
		printf("\t\t\t\t\t\t\t-------------------------------\n");
    		wait();
    		mmxout_valid.write(false);
    		wait();

	} else {
		wait();
	}
	
  }

}

