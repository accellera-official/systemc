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

  for_exit.cpp -- 

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-07-29

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "for_exit.h"

#define max 10

void for_exit::entry()
{

  int  i, inp_tmp;

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

    // execution of for loop with continues
    out_valid.write(true);
    wait();
    for (i=1; i<=max; i++) {
      inp_tmp = in_value.read();
      if (i==8) {
	wait();
	continue;
      } else if (inp_tmp<5 && i!=1) {
	wait();
	continue;
      } else {
	result.write(inp_tmp);
	wait();
      };
    };
    out_valid.write(false);
    wait(5);

    // for loop with break
    out_valid.write(true);
    wait();
    for (i=1; i<=max; i++) {
      inp_tmp = in_value.read();
      if (inp_tmp==7) {
	wait();
	break;
      }	else {
	result.write(inp_tmp);
	wait();
      }
    };
    out_valid.write(false);
    wait();

  }
}

// EOF

