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

  test01.cpp -- 

  Original Author: David Long, Doulos

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Andy Goodrich, Forte Design Systems
  Description of Modification: Expanded number of tests and added part
                               selection to make sure internal representation
                               works.

 *****************************************************************************/

#include <systemc.h> 

int sc_main(int,char**) { 
        int i;
		int64 j;
        sc_int<64> I; 
        sc_uint<64> UI; 
        sc_bigint<64> BI; 
        sc_biguint<64> BUI; 
        sc_bv<64> BV; 
        sc_bigint<70> BBI; 
        sc_biguint<70> BBUI; 

		cout << "int = 8 " << endl;
		i = 8;
        I = i; 
        cout << "I    =  " << I.to_string(SC_BIN) << " " << I(5,3) << endl; 

        UI = i; 
        cout << "UI   = " << UI.to_string(SC_BIN) << " " << I(5,3) << endl; 

        BI= i; 
        cout << "BI   =  " << BI.to_string(SC_BIN) << " " << I(5,3) << endl; 

        BUI = i; 
        cout << "BUI  = " << BUI.to_string(SC_BIN) << " " << I(5,3) << endl; 

        BV = i; 
        cout << "BV   = " << BV.to_string(SC_BIN) << " " << I(5,3) << endl; 

        BBI= i; 
        cout << "BBI  =  " << BBI.to_string(SC_BIN) << " " << I(5,3) << endl; 

        BBUI = i; 
        cout << "BBUI = " << BBUI.to_string(SC_BIN) << " " << I(5,3) << endl; 

		cout << "int = -1 " << endl;
		i = -1;
        I = i; 
        cout << "I    =  " << I.to_string(SC_BIN) << " " << I(5,3) << endl; 

        UI = i; 
        cout << "UI   = " << UI.to_string(SC_BIN) << " " << I(5,3) << endl; 

        BI= i; 
        cout << "BI   =  " << BI.to_string(SC_BIN) << " " << I(5,3) << endl; 

        BUI = i; 
        cout << "BUI  = " << BUI.to_string(SC_BIN) << " " << I(5,3) << endl; 

        BV = i; 
        cout << "BV   = " << BV.to_string(SC_BIN) << " " << I(5,3) << endl; 

        BBI= i; 
        cout << "BBI  =  " << BBI.to_string(SC_BIN) << " " << I(5,3) << endl; 

        BBUI = i; 
        cout << "BBUI = " << BBUI.to_string(SC_BIN) << " " << I(5,3) << endl; 

        cout << endl;
		cout << "int64 = -2 " << endl;
		j = -2;

        I = j; 
        cout << "I    =  " << I.to_string(SC_BIN) << " " << I(5,3) << endl; 

        UI = j; 
        cout << "UI   = " << UI.to_string(SC_BIN) << " " << I(5,3) << endl; 

        BI= j; 
        cout << "BI   =  " << BI.to_string(SC_BIN) << " " << I(5,3) << endl; 

        BUI = j; 
        cout << "BUI  = " << BUI.to_string(SC_BIN) << " " << I(5,3) << endl; 

        BV = j; 
        cout << "BV   = " << BV.to_string(SC_BIN) << " " << I(5,3) << endl; 

        BBI= j; 
        cout << "BBI  =  " << BBI.to_string(SC_BIN) << " " << I(5,3) << endl; 

        BBUI = j; 
        cout << "BBUI = " << BBUI.to_string(SC_BIN) << " " << I(5,3) << endl; 

        return 0; 
} 

