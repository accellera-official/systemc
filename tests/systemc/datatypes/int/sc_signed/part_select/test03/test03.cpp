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

  test03 -- test for unsigned data values

  Original Author: 

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

int sc_main(int argc, char** argv) { 
    sc_int<16> a, tmp; 
    sc_bigint<16> biga, bigtmp; 
	sc_biguint<6> bigu6;
	sc_biguint<16> bigu16;
 
     a = 97; // 0b1100001 
     biga = 97; 
	 bigu6 = 97;
  
  tmp = a.range( 5,0 ); // results in 33 - 0b100001, not sign extended 
   bigtmp = biga.range( 5,0 ); // results in 65505, sign extended 
   cout << hex << tmp << " " << bigtmp << endl;
    
  tmp = biga.range( 5,0 ); // results in 33 - 0b100001, not sign extended 
   bigtmp = a.range( 5,0 ); // results in 65505, sign extended 
   cout << hex << tmp << " " << bigtmp << endl;
    
	cout << hex << a.range(5,0) << " " << biga.range(5,0) << endl;

	bigtmp = bigu6;
	cout << hex << bigtmp << endl;

	bigu16 = biga.range(5,0);
	cout << bigu16 << endl;

	bigu16 = a.range(5,0);
	cout << hex << bigu16 << endl;

    sc_start(1, SC_NS); 
    return 0; 
}
