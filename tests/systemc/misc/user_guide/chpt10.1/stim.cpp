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

  stim.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename stim.cc */
/* This is the interface file for synchronous process `stim' */

#include "stim.h"

void stim::entry()
{
  // Variable definition
  sc_unsigned	a_tmp(data_width);
  sc_unsigned	b_tmp(data_width);
  bool		c_tmp;
  int 		i, j;

  // Reset handler
  reset.write(0);
  wait(2);
   
  reset.write(1);
  wait();
  cout  << sc_simulation_time() << "\t : "
        << "RESET off \t...by stim" << endl;

  // Stimulus Generation
  c_tmp = 0;
  for (i=0; i<16; i++) {
    for (j=0; j<16; j++) {     
      a_tmp = j;
      b_tmp = i;
      c_tmp = !c_tmp;

      a.write(a_tmp);	
      b.write(b_tmp);	
      cin.write(c_tmp);

      ready.write(1);
      // wait();
      wait_until(done.delayed() == 1); 

      ready.write(0);
      // wait_until(done.delayed() == 1); 
      wait();
    }
  }
 
  sc_stop(); 
}
