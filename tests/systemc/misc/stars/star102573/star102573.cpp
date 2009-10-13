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

  star102573.cpp -- 

  Original Author: Preeti Panda, Synopsys, Inc., 2000-08-09

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <systemc.h>
#include "for_nest.h"

#define max  5
#define max1 4
#define max2 3
#define max3 2

void for_nest::entry()
{

  sc_signed tmp2(2);
  sc_signed tmp4(4);
  sc_signed tmp8(8);
  sc_signed tmp6(6);

  int i, j,  inp_tmp;

  result.write(0);
  out_valid.write(false);
  wait();

  main_loop:while(1) {

    while (in_valid.read()==false) wait();
    out_valid.write(true);
    wait();
    /* unrolled loop inside rolled loop */
    loop1:for (i=1; i<=max; i++)  {
      tmp2    = in_value.read();
      inp_tmp = tmp2.to_int() +  i ;
      loop2:for (j=1; j<=max1; j++)
	     inp_tmp = inp_tmp - tmp2.to_int() ;
      result.write(inp_tmp);
      wait();
    };
    out_valid.write(false);
    wait();
    out_valid.write(true);
    wait();

    inp_tmp = in_value.read();
    wait();
    /* unrolled loop inside unrolled loop */
    loop3:for (i=1; i<=max2; i++)  {
      inp_tmp += i ;
      loop4:for (j=1; j<=max3; j++) {
	inp_tmp -= j ;
      }
    };
    result.write(inp_tmp);
    out_valid.write(false);
    wait();
    out_valid.write(true);
    wait();

  }
}

