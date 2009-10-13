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

  rgb.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "rgb.h"

ostream& 
operator<<(ostream& os, struct rgb_t &r)
{
  os << r.red << " " << r.green << " " << r.blue << endl;
  return os;
}

void sc_trace(sc_trace_file* tf,const rgb_t& s, const std::string& NAME) {
  sc_trace(tf, s.red, NAME + ".red");
  sc_trace(tf, s.green, NAME + ".green");
  sc_trace(tf, s.blue, NAME + ".blue");
}


void some_process::entry()
{
  rgb_t clin;
  rgb_t clout;
  
  while(true) {
    clin = color_in.read();
    clout = clin;
    clout.red >>= 1;
    clout.green >>=2;
    clout.blue <<= 1;
    color_out.write(clout);
    wait();

  }
}

int sc_main(int ac, char* av[])
{
  sc_fifo<rgb_t> in;
  sc_fifo<rgb_t> out;

  sc_clock clk("CLK");

  some_process foo("FOO", clk, in, out);
  
  sc_start(1000);

  return 0;
}
