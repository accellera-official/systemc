/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
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

  proc2.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename proc2.cc */
/* This is the implementation file for synchronous process `proc2' */

#include "systemc.h"
#include "proc2.h"

void proc2::entry()
{
  data_ack.write(false);
  wait();
  cout << "Ack \t = False" << endl;
  
  while (true) {
    do { wait(); } while (data_ready != true);
    data_ack.write(true);
    cout << "Ready \t = True" << endl;

    do { wait(); } while (data_ready != false);
    data_ack.write(false);
    cout << "Ready \t = False" << endl;
  }
} // end of entry function

