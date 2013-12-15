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

  proc1.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename proc1.cc */
/* This is the implementation file for synchronous process `proc1' */

#include "proc1.h"

void proc1::entry()
{
  int i = 10;

  data_ready.write(false);
  wait();

  while(true) {
    cout << endl << "Proc1: Initiating Transfer" << endl;
    data_ready.write(true);
    data.write(i++);
    wait();
    cout << "Proc1: Data Ready has value = " << data_ready.read() 
	 << " at time " << sc_time_stamp() << endl;
    cout << "Proc1: Data Ack has value = " << data_ack.read() 
	 << " at same time" << endl;
    do { wait(); } while (data_ack != true);
    cout << "Proc1: Data Ack Received at time " << sc_time_stamp() << 
      endl;
    data_ready.write(false);
    wait();
    cout << "Proc1: Data Ready has value = " << data_ready.read() 
	 << " at time " << sc_time_stamp() << endl;
    cout << "Proc1: Transfer complete" << endl;
  }
} // end of entry function

