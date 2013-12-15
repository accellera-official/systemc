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

  signless_conversions.cpp -- Test Casts Of Sign-Less Values

  Original Author: Andy Goodrich, Forte Design Systems, 7 Apr 2005

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

#define DISP(exp) \
{ \
	cout << #exp << " = " << exp << endl; \
}

#define DISPLAY(var) \
{ \
	DISP( sc_int_base(var) ) \
	DISP( sc_uint_base(var) ) \
	DISP( sc_signed(var) ) \
	DISP( sc_unsigned(var) ) \
    cout << endl; \
}

int
sc_main( int argc, char* argv[] )
{
	sc_bigint<6>  bi;
	sc_biguint<6> bui;
    sc_bv<4>      bv;
	sc_int<6>     i;
    sc_lv<4>      lv;
	sc_int<6>     ui;

	bi =  10;
	bui = 10;
	bv =  "1010";
	i =   10;
	lv =  "1010";
	ui =  10;

    DISPLAY(bi(3,0))	
    DISPLAY(bui(3,0))	
    DISPLAY(bv)	
    DISPLAY(i(3,0))	
    DISPLAY(lv)	
    DISPLAY(ui(3,0))	

    DISPLAY((bi(3,2),i(1,0)))	
    DISPLAY((bi(3,2),ui(1,0)))	
    DISPLAY((bi(3,2),bui(1,0)))	
    DISPLAY((bi(3,2),bi(1,0)))	

    DISPLAY((bui(3,2),i(1,0)))	
    DISPLAY((bui(3,2),ui(1,0)))	
    DISPLAY((bui(3,2),bui(1,0)))	
    DISPLAY((bui(3,2),bi(1,0)))	

    DISPLAY((i(3,2),i(1,0)))	
    DISPLAY((i(3,2),ui(1,0)))	
    DISPLAY((i(3,2),bui(1,0)))	
    DISPLAY((i(3,2),bi(1,0)))	

    DISPLAY((ui(3,2),i(1,0)))	
    DISPLAY((ui(3,2),ui(1,0)))	
    DISPLAY((ui(3,2),bui(1,0)))	
    DISPLAY((ui(3,2),bi(1,0)))	

    return 0;
}
