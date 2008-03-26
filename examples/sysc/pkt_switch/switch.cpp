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

  switch.cpp - This is the implementation file for the asynchronous process
               "switch".

  Original Author: Rashmi Goswami, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"
#include "pkt.h"
#include "fifo.h"
#include "switch_reg.h"
#include "switch.h"
#define SIM_NUM 500

void mcast_pkt_switch :: entry()
{
  wait();

  // declarations
  switch_reg R0;
  switch_reg R1;
  switch_reg R2;
  switch_reg R3;
  switch_reg temp;

  int sim_count;
  int pkt_count;
  int drop_count;

  fifo q0_in;
  fifo q1_in;
  fifo q2_in;
  fifo q3_in;

  fifo q0_out;
  fifo q1_out;
  fifo q2_out;
  fifo q3_out;

  // FILE *result;

  // initialization
  pkt_count = 0;
  drop_count = 0;
  sim_count  = 0;

  q0_in.pntr = 0;
  q1_in.pntr = 0;
  q2_in.pntr = 0;
  q3_in.pntr = 0;

  q0_out.pntr = 0;
  q1_out.pntr = 0;
  q2_out.pntr = 0;
  q3_out.pntr = 0;

  q0_in.full  = false;
  q1_in.full  = false;
  q2_in.full  = false;
  q3_in.full  = false;

  q0_in.empty = true;
  q1_in.empty = true;
  q2_in.empty = true;
  q3_in.empty = true;

  q0_out.full = false;
  q1_out.full = false;
  q2_out.full = false;
  q3_out.full = false;

  q0_out.empty = true;
  q1_out.empty = true;
  q2_out.empty = true;
  q3_out.empty = true;
  
  R0.free = true;
  R1.free = true;
  R2.free = true;
  R3.free = true;

  // result = fopen("result","w");

  cout << endl;
  cout << "-------------------------------------------------------------------------------" << endl;
  cout << endl << "             4x4 Multicast Helix Packet Switch Simulation" << endl;
  cout << "-------------------------------------------------------------------------------" << endl;
  cout << "  This is the simulation of a 4x4 non-blocking multicast helix packet switch.  " << endl; 
  cout << "  The switch uses a self-routing ring of shift registers to transfer cells     " << endl;
  cout << "  from one port to another in a pipelined fashion, resolving output contention " << endl;
  cout << "  and handling multicast switch efficiently." << endl << endl;
 
  cout << "  Press \"Return\" key to start the simulation..." << endl << endl;

  getchar();

  wait();
  // functionality
  while( sim_count++ < SIM_NUM )
    { 
       wait();

       /////read input packets//////////////////////////////////     
      if (in0.event()) 
       {
	 pkt_count++;
	 if (q0_in.full == true) drop_count++;
         else q0_in.pkt_in(in0.read());
       };  

       if (in1.event()) 
       {
	 pkt_count++;
	 if (q1_in.full == true) drop_count++;
         else q1_in.pkt_in(in1.read());
       };

       if (in2.event()) 
       {
	 pkt_count++;
	 if (q2_in.full == true) drop_count++;
         else q2_in.pkt_in(in2.read());
       };

       if (in3.event()) 
       {
	 pkt_count++;
	 if (q3_in.full == true) drop_count++;
         else q3_in.pkt_in(in3.read());
       };

      /////move the packets from fifo to shift register ring/////
   
      if((!q0_in.empty) && R0.free) 
	{
          R0.val  = q0_in.pkt_out();
	  R0.free = false;
	}

      if((!q1_in.empty) && R1.free) 
	{
          R1.val  = q1_in.pkt_out();
	  R1.free = false;
	}
      if((!q2_in.empty) && R2.free) 
	{
          R2.val  = q2_in.pkt_out();
	  R2.free = false;
	}
      if((!q3_in.empty) && R3.free) 
	{
          R3.val  = q3_in.pkt_out();
	  R3.free = false;
	}

      if((bool)switch_cntrl && switch_cntrl.event())
	{
            /////shift the channel registers /////////////////////////
            temp = R0;
            R0 = R1;
	    R1 = R2;
	    R2 = R3;
	    R3 = temp;

	    /////write the register values to output fifos////////////
	    if ((!R0.free) && (R0.val.dest0) && (!q0_out.full))
	      {
		q0_out.pkt_in(R0.val); 
		R0.val.dest0 = false;
		if (!(R0.val.dest0|R0.val.dest1|R0.val.dest2|R0.val.dest3)) R0.free = true;
	      }

	    if ((!R1.free) && (R1.val.dest1) && (!q1_out.full))
	      {
		q1_out.pkt_in(R1.val);
		R1.val.dest1 = false;
		if (!(R1.val.dest1|R1.val.dest1|R1.val.dest2|R1.val.dest3)) R1.free = true;
	      }
	    if ((!R2.free) && (R2.val.dest2) && (!q2_out.full))
	      {
		q2_out.pkt_in(R2.val);
		R2.val.dest2 = false;
		if (!(R2.val.dest2|R2.val.dest1|R2.val.dest2|R2.val.dest3)) R2.free = true;
	      }
	    if ((!R3.free) && (R3.val.dest3) && (!q3_out.full))
	      {
		q3_out.pkt_in(R3.val);
		R3.val.dest3 = false;
		if (!(R3.val.dest3|R3.val.dest1|R3.val.dest2|R3.val.dest3)) R3.free = true;
	      }

	    /////write the packets out//////////////////////////////////    
	    if (!q0_out.empty) out0.write(q0_out.pkt_out()); 
	    if (!q1_out.empty) out1.write(q1_out.pkt_out());
	    if (!q2_out.empty) out2.write(q2_out.pkt_out());
	    if (!q3_out.empty) out3.write(q3_out.pkt_out());
	}
    } 

  sc_stop();

  cout << endl << endl << "-------------------------------------------------------------------------------" << endl;
  cout << "End of switch operation..." << endl;
  cout << "Total number of packets received: " <<  pkt_count << endl;
  cout << "Total number of packets dropped: " <<  drop_count << endl;
  cout << "Percentage packets dropped:  " <<  drop_count*100/pkt_count << endl;
  cout << "-------------------------------------------------------------------------------" << endl;
  
}
