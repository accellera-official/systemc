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

  tx.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename driver.cc */
/* This is a SIO transmitter */

#include "tx.h"

void sio_tx::byte (char c)
{
  bool parity = false;
  int n;

  // Start bit
  tx.write(false);
  wait();
  
  // 8 data bits
  for (n=0; n<8; n++) {
    if (c & 0x1) parity=!parity;
    tx.write (c & 0x1);
    c = c >>1;
    wait();
  }
  
  // parity bit
  tx.write(parity);
  wait();
  
  // stop bits;
  if (two_stop_bits) {
    tx.write(true);
    wait();
    tx.write(true);
    wait();
  } else {
    tx.write(true);
    wait();
  }
} // end of entry function


void sio_tx::mark (int cycles)
{
  int n;

  for (n=0; n<cycles; n++) {
    tx.write(true);
    wait();
  }
}


void sio_tx::entry()
{
  while (1) {
    mark(5);
    byte((char)0x5a);
    byte((char)0xff);
    mark(1);
    byte((char)0xab);
  }
} // end of entry function

