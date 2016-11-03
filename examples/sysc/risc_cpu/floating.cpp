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
 
  floating.cpp -- Floating Point Execution Unit.
 
  Original Author: Martin Wang, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/
 

#include "systemc.h"
#include "floating.h"
#include "directive.h"

void floating::entry(){

  int			        opcode_tmp = 0;
  signed int		        dout_tmp = 0;
  unsigned int		        dest_tmp = 0;

  unsigned int                  fpua_sign_tmp;
  unsigned int                  fpua_exponent_tmp;
  unsigned int                  fpua_significand_tmp;
  unsigned int                  fpub_sign_tmp;
  unsigned int                  fpub_exponent_tmp;
  unsigned int                  fpub_significand_tmp;
  const char *                  opcode_encode="";
  unsigned int                  fpua_tmp;
  unsigned int                  fpub_tmp;


  int				exponent_diff_tmp = 0;
  unsigned int			significant_result = 0;
  unsigned int			overflow_sign_tmp = 0;
  unsigned int			result_exp_tmp = 0;
  unsigned int			result_sign_tmp = 0;

  //
  // main loop
  //
  // 
  // current implementation only worked if both operand have same sign bits
  // overflow is ignored.

  // initialization of output

  wait(3);
  while(true) {
        do { wait(); } while ( !(in_valid == true) );
	dest_tmp = dest.read();
 	opcode_tmp = opcode.read();

        fpua_tmp = floata.read();
        fpub_tmp = floatb.read();
 
        fpua_sign_tmp = (fpua_tmp & 0x80000000) >> 31 ;
        fpub_sign_tmp = (fpub_tmp & 0x80000000) >> 31 ;

        fpua_exponent_tmp = (fpua_tmp & 0x7f800000) >> 23 ;
        fpub_exponent_tmp = (fpub_tmp & 0x7f800000) >> 23 ;

       	fpua_significand_tmp = (fpua_tmp & 0x007fffff) ;
        fpub_significand_tmp = (fpub_tmp & 0x007fffff) ;

	exponent_diff_tmp = int(fpua_exponent_tmp) - int(fpub_exponent_tmp);


	if (PRINT_FPU) {
	 	printf("\t\t\t\t\t\t\t-------------------------------\n");
	 	printf("\t\t\t\t\t\t\tFPU: A s=0x%x, exp=0x%x, mant=0x%x\n",fpua_sign_tmp, fpua_exponent_tmp, fpua_significand_tmp);
	 	printf("\t\t\t\t\t\t\t   : B s=0x%x, exp=0x%x, mant=0x%x\n",fpub_sign_tmp, fpub_exponent_tmp, fpub_significand_tmp);
		printf("\t\t\t\t\t\t\t   : exponent difference = %d\n",exponent_diff_tmp);
	 	printf("\t\t\t\t\t\t\t-------------------------------\n");
	}	

	if (exponent_diff_tmp > 0) {
		//printf("shift significant B to Right\n");
       		fpub_significand_tmp = fpub_significand_tmp >> exponent_diff_tmp ;
		fpub_exponent_tmp = fpua_exponent_tmp;
	} else {
		//printf("shift significant A to Right\n");
       		fpua_significand_tmp = fpua_significand_tmp >> exponent_diff_tmp ;
		fpua_exponent_tmp = fpub_exponent_tmp;
	}
	wait();

	if (PRINT_FPU) {
	 	printf("\t\t\t\t\t\t\t-------------------------------\n");
	 	printf("\t\t\t\t\t\t\tFPU: After Alignment\n");
	 	printf("\t\t\t\t\t\t\tFPU: A s=0x%x, exp=0x%x, mant=0x%x\n",fpua_sign_tmp, fpua_exponent_tmp, fpua_significand_tmp);
	 	printf("\t\t\t\t\t\t\t   : B s=0x%x, exp=0x%x, mant=0x%x\n",fpub_sign_tmp, fpub_exponent_tmp, fpub_significand_tmp);
	 	printf("\t\t\t\t\t\t\t-------------------------------\n");
	}	

	// output MUX
	switch (opcode_tmp) {
	case 0:         // Stall
		opcode_encode = "STALL";
		// dout_tmp = dout_tmp;  // keeps its value
		wait();
		break;
	case 3:         // add
		opcode_encode = "FADD";
		significant_result = int(fpua_significand_tmp) + int(fpub_significand_tmp); 
		wait();
		break;
	case 4:         // sub
		opcode_encode = "FSUB";
		significant_result = int(fpua_significand_tmp) - int(fpub_significand_tmp); 
		wait();
		break;
	case 5:         // mul
		opcode_encode = "FMUL";
		significant_result = int(fpua_significand_tmp) * int(fpub_significand_tmp); 
		fpub_exponent_tmp *= 2;			// exponent is doubled in value
		wait();
		break;
	case 6:         // div
		opcode_encode = "FDIV";
		significant_result = int(fpua_significand_tmp) / int(fpub_significand_tmp); 
		wait();
		break;
	default:
	 	printf("\t\t\t\t\t\t\t-------------------------------\n");
		printf("\t\t\t\t\t\t\tFPU: Bad Opcode %d.\n",opcode_tmp);
	 	printf("\t\t\t\t\t\t\t-------------------------------\n");
		wait();
		break;
	}	

	overflow_sign_tmp = (significant_result & 0xff800000) >> 23;
	dout_tmp = (significant_result << overflow_sign_tmp) & 0x007fffff ;
	result_exp_tmp = fpub_exponent_tmp  + overflow_sign_tmp; 
	dout_tmp = dout_tmp | ((result_exp_tmp << 23) & 0x7f800000) ;
	result_sign_tmp = fpua_sign_tmp;
	dout_tmp = dout_tmp | ((result_sign_tmp << 31) & 0x80000000) ;
	printf("\t\t\t\t\t\t\t-------------------------------\n");
	printf("\t\t\t\t\t\t\tFPU: Result   Sign=0x%x.\n",result_sign_tmp);
	printf("\t\t\t\t\t\t\t   : Result   Mantissa=0x%x.\n",significant_result);
	printf("\t\t\t\t\t\t\t   : Result   Exponent=0x%x.\n",result_exp_tmp);
	printf("\t\t\t\t\t\t\t   : Overflow Mantissa=0x%x.\n",overflow_sign_tmp);
	cout << "\t\t\t\t\t\t\t   : " << opcode_encode;
	printf(" FPU Output =0x%x.\n",dout_tmp);
	cout <<"\t\t\t\t\t\t\t   : at CSIM " << sc_time_stamp() << endl;
	printf("\t\t\t\t\t\t\t-------------------------------\n");
	fdout.write(dout_tmp);
	fout_valid.write(true);
	fdestout.write(dest_tmp);
	wait();
	fout_valid.write(false);
	wait();
  }

}

