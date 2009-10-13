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

  increment.cpp -- 

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-07-09

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "increment.h"

void increment::entry(){

  #define         ONE 1
  const int       eins = 1;
  int             tmp1;
  sc_bigint<5>    tmp2;

  // reset_loop
  if (reset.read() == true) {
    out_valid.write(false);
    wait();
  } else wait();

  //
  // main loop
  //
  //
  while(1) {
    while(in_valid.read()==false) wait();
    wait();

    //reading the inputs
    tmp1 = in_value1.read();
    tmp2 = in_value2.read();

    //execute simple operations
    tmp1 = tmp1 + 1;
    tmp1 = tmp1 + ONE;
    tmp1 = tmp1 + eins;
    tmp2 = tmp2 + 1;
    tmp2 = tmp2 + ONE;
    tmp2 = tmp2 + eins;
    wait();

    // write outputs
    out_value1.write(tmp1);
    out_value2.write(tmp2);
    out_valid.write(true);
    wait();
    out_valid.write(false);
    wait();
  }
}

// EOF

