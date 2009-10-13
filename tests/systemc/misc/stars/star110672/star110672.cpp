/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  star110672.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// Implementation of design
#include "systemc.h"
#include "def.h"

//GILLES copy of the struct for bug report
typedef struct reg {
  sc_uint<32> val;
  //sc_bit flag; // no trace possible ..... so i use char
  char flag;
  } t_flm_reg;


#include "design.h"

enum fsm_t {EMPTY,FULL,READ,WRITE};

void design :: write_in_fifo() {
  // locals
  INDEX_TYPE rp,wp;
  DATA_TYPE data_in;
  fsm_t state;
  //GILLES
  t_flm_reg foofoo;
  sc_uint<32> foo;
  // Main loop
  write_loop: while (1) {
    //foofoo.val.range(1,1) = 1;
    //foo.range(1) = 1;
    foo.range(1,1) = 1;
    wait_until(take_the_data.delayed());
    data_in = input.read();
    rp = read_pointer.read();
    wp = write_pointer.read();
    wp++;
    if (wp>=RING_BUFFER_SIZE) wp = 0 ;
    if (wp == rp ) state = FULL;
    else state = WRITE ;
    
    switch (state) {
    case WRITE :
      ring_buffer[wp] = data_in;
      write_in_is_done.write(1);
      write_pointer.write(wp);
      break;
    case FULL :
      write_in_is_done.write(0);
      break;
    }
    wait();
  }
}
void design :: read_out_fifo() {
  INDEX_TYPE rp,wp;
  bool local_write_in_is_done;
  fsm_t state;
 read_loop:  while (1) {
   //reading inputs
   wait_until(data_request.delayed());
   rp = read_pointer.read();
   wp = write_pointer.read();
   // set the state
   if (rp == wp ) state = EMPTY;
   else state = READ ;
   switch (state) {
     // reading
   case READ :
     rp++;
     if (rp>=RING_BUFFER_SIZE) rp = 0 ;
     output.write(ring_buffer[rp]);
     data_ready.write(1);
     break;
     // blocking
   case EMPTY :
     cout << "fifo is empty cannot read out" << endl;
     data_ready.write(0);
     break;
   }
   read_pointer.write(rp);
   wait();
 }
}
