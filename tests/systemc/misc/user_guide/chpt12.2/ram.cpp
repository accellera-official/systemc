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

  ram.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename ram.cc */
/* This is the implementation file for asynchronous process `ram' */

#include "ram.h"

void ram::entry()
{
  int address;

  while (true) {
    do { wait(); } while (cs != true); 
    address = addr.read().to_int();
    if (we.read() == true) { // Write operation
      wait(wait_cycles-1);
      memory[address] = datain.read().to_int();
    }
    else { // Read operation
      if (wait_cycles > 2)
	wait(wait_cycles-2); // Introduce delay needed
      dataout.write(memory[address]);
      wait();
    }    
  }
} // end of entry function

