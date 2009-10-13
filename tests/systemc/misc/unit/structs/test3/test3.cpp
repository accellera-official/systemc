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

  test3.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <systemc.h>
#include "arraytypes.h"

void arraytypes::entry(){

  char    tmp1_uns_lv[4][4];
  arr_struct1  a;
  arr_struct2  tmp2_uns_lv;

  // reset_loop
  out_valid.write(sc_bit(0));

  wait();
  while (true) { 
  while(in_valid.read() == "0") wait();
    wait();
    a = in_value1.read();
     for (int i = 0; i < 4; i++) {
       for (int j = 0; j < 4; j++) {
	 tmp1_uns_lv[i][j] = a.a[i][j];
       }
     }

     for (int i = 0; i < 4; i++) 
       for (int j = 0; j < 4; j++) 
	 tmp2_uns_lv.b[i][j] = tmp1_uns_lv[j][i];
     out_value1.write (tmp2_uns_lv);

    wait();

    out_valid.write( sc_bit(1));
    wait(); 


    out_valid.write( sc_bit(0));
    wait();

}
}
// EOF

