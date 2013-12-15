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

  sc_main.cpp -- 

  Original Author: Ray Ryan, Mentor Graphics, 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

SC_MODULE(sctop) {
  SC_CTOR(sctop);
};

#ifdef MTI_INTEGRATION
  SC_MODULE_EXPORT(sctop);
#endif

sctop::sctop(sc_module_name name) : sc_module(name)
{
	sc_bigint<96> bigword("0x999888fffeeedddcccbbbaaa");

    cout << " bw:" << bigword.to_string(SC_HEX) 
        << "\n f0:"  << bigword.range( 11,  0).to_string(SC_HEX)
        << "\n f1:"  << bigword.range( 23, 12).to_string(SC_HEX)
        << "\n f2:"  << bigword.range( 35, 24).to_string(SC_HEX)
        << "\n f3:"  << bigword.range( 47, 36).to_string(SC_HEX)
        << "\n f4:"  << bigword.range( 59, 48).to_string(SC_HEX)
        << "\n f5:"  << bigword.range( 71, 60).to_string(SC_HEX)
        << "\n f6:"  << bigword.range( 83, 72).to_string(SC_HEX)
        << "\n f7:"  << bigword.range( 95, 84).to_string(SC_HEX)
        << endl;

}                                                                                                                      
int sc_main(int argc, char** argv) {
	sctop top("top"); 
	sc_start(1, SC_NS);
	return 0;
}


