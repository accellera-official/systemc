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

  star107460.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// If one assigns a sc_lv vector containing X or Z to
// sc_bv the error message tells you that this is not
// possible, but doesn't tell you where(which variable
// at least) this happened. In a large program such a 
// message is impossible to trace

#include "systemc.h"

int sc_main(int, char**)
{
  sc_lv<8> lv("00X0100");
  sc_bv<8> bv;

  cout << "\nbv = "<<bv <<"\nlv = "<<lv;
  bv = lv;
  cout << "\nbv = "<<bv <<"\nlv = "<<lv<<"\n";
  
  return 0;
}
