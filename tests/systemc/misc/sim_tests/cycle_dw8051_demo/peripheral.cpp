/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  peripheral.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "peripheral.h"

void peripheral::entry() {
  unsigned int buffer_in = 0;
  unsigned int buffer_out = 0;
  wait();

  while(true) {
    unsigned int addr = mem_addr.read().to_uint();
    unsigned int data = mem_data_out.read().to_uint();

    if(!mem_wr_n.read() && (addr==0x10)) {
      // write
      cout << "peripheral: receive " << data << endl;
      buffer_in = data;
      wait(100);
      buffer_out = buffer_in;
    }

    if(!mem_rd_n.read() && (addr==0x11)) {
      // read
      mem_data_in.write( sc_bv<8>( buffer_out ) );
      cout << "peripheral: send " << buffer_out << endl;
      wait();
    }
    wait();
  }
}
