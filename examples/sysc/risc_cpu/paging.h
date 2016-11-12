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
 
  paging.h -- Instruction Paging Unit.
 
  Original Author: Martin Wang, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/


struct paging : sc_module {
        sc_in<unsigned >   paging_din; 	// input data       
        sc_in<bool>           paging_csin;    	// chip select        
        sc_in<bool>           paging_wein;    	// write enable       
        sc_in<unsigned >   logical_address; 	// logical address  
        sc_in<unsigned >   icache_din;      	// data from BIOS/icache
        sc_in<bool>           icache_validin;   // data valid bit 
        sc_in<bool>           icache_stall;     // stall IFU if busy

        sc_out<unsigned >  paging_dout;            // output data
        sc_out<bool>  	      paging_csout;           // output cs to cache/BIOS
        sc_out<bool>  	      paging_weout;           // write enable to cache/BIOS
        sc_out<unsigned >  physical_address;       // physical address 
        sc_out<unsigned >  dataout;        	      // dataout from memory
        sc_out<bool>  	      data_valid;             // data valid
        sc_out<bool>  	      stall_ifu;              // stall IFU if busy
	sc_in_clk 	      CLK;

        signed int pid_reg;             //CPU process ID register      

  SC_CTOR(paging) {
      SC_CTHREAD(entry, CLK.pos());
      pid_reg = 0;
  }
  
  void entry();
};

// EOF
