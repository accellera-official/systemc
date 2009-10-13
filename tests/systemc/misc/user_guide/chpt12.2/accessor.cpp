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

  accessor.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename accessor.cc */
/* This is the implementation file for synchronous process 'accessor' */

#include "accessor.h"

void accessor::entry()
{
  int addr;
  int datao;
  int datai;

  addr = 10;
  datao = 0xdeadbeef;

  while (true) {
    // Write memory location first
    chip_select.write(true);
    write_enable.write(true);
    address.write(addr);
    dataout.write(datao);
    cout << "Accessor: Data Written = " << hex << datao << " at address "
         << hex << addr << endl;
    wait(memory_latency); // To make all the outputs appear at the interface

    // some process functionality not shown here during which chip
    // chip select is deasserted and bus is tristated
    chip_select.write(false);
    dataout.write(0);
    wait();

    // Now read memory location
    chip_select.write(true);
    write_enable.write(false);
    address.write(addr);
    wait(memory_latency); // For data to appear

    datai = datain.read().to_int();
    cout << "Accessor: Data Read = " << hex << datai << " from address "
         << hex << addr << endl;
    chip_select.write(false);
    wait();

    addr++;
    datao++;
  }
} // end of entry function

