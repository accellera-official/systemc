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

  while_datatypes.cpp -- 

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-07-29

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "while_datatypes.h"

#define max 10

void while_datatypes::entry()
{

  int  i, inp_tmp;
  sc_signed   signed_counter(8);
  sc_unsigned unsigned_counter(8);

  // reset_loop
  if (reset.read()==true) {
    result.write(0);
    out_valid.write(false);
    wait();
  } else wait(); 

  //----------
  // main loop
  //----------
  while(1) {

    // read inputs
    while (in_valid.read()==false) wait();

    // execution of for loop 
    out_valid.write(true);
    i=1;
    wait();
    while (i<=max) {
      inp_tmp = in_value.read();
      result.write(inp_tmp); 
      i++;
      wait();
    };
    out_valid.write(false);
    wait(5);

    // execution of for loop with continues
    out_valid.write(true);
    signed_counter=0;
    wait();
    do {
      signed_counter++;
      inp_tmp = in_value.read();
      if (signed_counter==8) {
	wait();
	continue;
      } else if (in_value.read()<5) {
	wait();
	continue;
      } else {
	result.write(inp_tmp);
	wait();
      }
    } while (signed_counter.to_int()<=max);
    out_valid.write(false);
    wait(5);

    // for loop with break
    out_valid.write(true);
    wait();
    unsigned_counter=0;
    do {
      unsigned_counter++;
      inp_tmp = in_value.read();
      if (inp_tmp==7) {
	wait();
	break;
      } else {
	result.write(inp_tmp);
	wait();
      };
    } while (unsigned_counter.to_uint()<=max);
    out_valid.write(false);
    wait();
  }
}

// EOF

