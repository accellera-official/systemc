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

  star106015.cpp -- 

  Original Author: Stan Liao, Synopsys, Inc., 2000-07-11

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <systemc.h>
#include "mem.h"

void memory::entry(){

  unsigned             tmp1;
  sc_uint<8> mem[128];
  sc_uint<8> mem2[128];

  out_valid1.write(false);
  wait();
  
  while(1) {
    while(in_valid1.read()==false) {
      mem[1] = 1;
      wait();
    }
    wait();
    out_valid1.write(true);
    wait();
  }
  while(1) {
    out_valid1.write(false);
  }
  while(1) {
    out_valid1.write(false);
  }
}
