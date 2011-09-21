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

  test03.cpp -- 

  Original Author: Andy Goodrich, Forte Design Systems, 2003-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test and l-value bit selection concatenation on sc_lv and sc_bv
// also test set_word() and set_cword on bit selections

#include "systemc.h"

int sc_main(int argc, char* argv[])
{
    sc_bv<64> bv1;
    sc_bv<64> bv2;
    sc_lv<64> lv1;
    sc_lv<64> lv2;

	cout << endl;
	cout << "6666555555555544444444443333333333222222222211111111110000000000"
	     << endl;
	cout << "3210987654321098765432109876543210987654321098765432109876543210"
		 << endl;
	cout << "----------------------------------------------------------------"
		 << endl;

	// LV SUPPORT:

	cout << lv1 << " lv1" << endl;
	cout << lv2 << " lv2 " << endl;

	cout << " (lv1[33], lv2[34]) = 0 " << endl;
	(lv1[33], lv2[34]) = 0;
	cout << lv1 << " lv1" << endl;
	cout << lv2 << " lv2 " << endl;
	
	cout << "lv1[43].set_cword(0, 0) " << endl;
	lv1[43].set_cword(0, 0);
	cout << lv1 << " lv1" << endl;

	cout << "lv2[44].set_word(0, 0)" << endl;
	lv2[44].set_word(0, 0);
	cout << lv2 << " lv2" << endl;

	cout << "(lv1,lv2[9]) = 0" << endl;
	(lv1,lv2[9]) = 0; 
	cout << lv1 << " lv1 " << endl;
	cout << lv2 << " lv2 " << endl;

	cout << "(lv2[9], lv1) = -1ll" << endl;
	(lv2[9], lv1) = -1ll;
	cout << lv1 << " lv1 " << endl;
	cout << lv2 << " lv2 " << endl;


	// BV SUPPORT:

	cout << endl;
	cout << bv1 << " bv1" << endl;
	cout << bv2 << " bv2" << endl;

	cout << "(bv1[33], bv2[34]) = 3" << endl;
	(bv1[33], bv2[34]) = 3;
	cout << bv1 << " bv1" << endl;
	cout << bv2 << " bv2" << endl;

	cout << "bv1[43].set_word(0, 1)" << endl;
	bv1[43].set_word(0, 1);
	cout << bv1 << " bv1" << endl;


	cout << "(bv1,bv2[9]) = 0" << endl;
	(bv1,bv2[9]) = 0;
	cout << bv1 << " bv1" << endl;
	cout << bv2 << " bv2 " << endl;

	cout << "(bv2[9], bv1) = -1ll" << endl;
	(bv2[9], bv1) = -1ll;
	cout << bv1 << " bv1" << endl;
	cout << bv2 << " bv2 " << endl;

	cout << "Program completed" << endl;
	return 0;
}
