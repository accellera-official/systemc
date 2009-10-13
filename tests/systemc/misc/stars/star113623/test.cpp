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

  test.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/*
I have closed the star. The testcase I'm attaching shows that Cthreads
used to sample the inputs at the beginnig and therefor it took 2
cycles to see the results back in sc_main. Hope you could take the
action to include it in the regression systemc, since I don't know
what system you guys are using. 

Thanks,
Rocco
*/

#include "systemc.h" 
 
SC_MODULE(adder_reg) { 
   sc_in<sc_int<8> > a; 
   sc_in<sc_int<8> > b; 
   sc_out<sc_int<9> > c; 
   //sc_in<bool> clk; 
   sc_in_clk clk; 
 
void add() { 
  // c.write(a.read() + b.read()); // *** THIS WON'T COMPILE *** 
   c = a.read() + b.read(); // Must use read() method 
  } 
        
SC_CTOR(adder_reg) { 
   SC_CTHREAD(add, clk.pos()); 
   //sensitive_pos(clk);
  } 
}; 
 
int sc_main(int argc, char *argv[]) 
{
  sc_signal< sc_int<8> > a; 
  sc_signal< sc_int<8> > b; 
  sc_signal< sc_int<9> > c; 
  sc_clock clk("CLK", 10, 0.5, 0.0); 
  adder_reg adder_reg("adder");
  adder_reg << a << b << c << clk;
  a = 3; 
  b = 6; 
  cout << c.read().to_int() << endl; 
  sc_start(clk, 1); 
  cout << c.read().to_int() << endl; 
  sc_start(clk, 1); 
 
  // CynAppsTwo clocks to get the answer. 
  // Rocco: No with SC_METHOD it take 1 cycle
  cout << c.read().to_int() << endl; 
  return 0; 
}
