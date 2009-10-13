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

  star110069.cpp -- 

  Original Author: Stan Liao, Synopsys, Inc., 2000-09-19

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <systemc.h>
#include "mem0.h"
 
void mem0::entry(){


  unsigned int tmp1;
  unsigned int tmp2;
int test[16];
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
    tmp1 = in_value1.read().to_uint();
    tmp2 = in_value2.read().to_uint();

    wait();
    wait();
    tmp2 = memory[tmp1];
    cout << "memory content " << tmp2 << endl;
    // write outputs
    out_value1.write( sc_bv<8>( tmp1 ) );
    out_value2.write( sc_bv<8>( tmp2 ) );
    out_valid.write(true);
    wait();
    out_valid.write(false);
    wait();
  }
}

// EOF

