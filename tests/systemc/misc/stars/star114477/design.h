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

  design.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

struct fun:sc_module
{
  sc_in<bool>        clk;
  sc_in<int>         count;
  sc_bv<9>           x,y,z;
  sc_out<sc_bv<9> >  out_a0, out_a1;
  
  SC_CTOR(fun) {    
    SC_METHOD(entry1);
    sensitive << clk;
    SC_METHOD(entry2);
    sensitive << clk;
}
  
  void entry1();
  void entry2();
};
    
void fun::entry1()
{ 
  z = 0;      
  z[0] = x[1]&y[2];
  z[0] = x[1]|y[2];
  z[0] = x[1]^y[2];
  out_a0 = z;
  out_a1 = x;
}


void fun::entry2()
{ 
  if (count%3==0) {
    x = 120;
    y = 10;
  }
  else {
    x = 44;
    y = 5;
  }
}
