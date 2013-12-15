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
Dec/1/00 Ulli Holtmann

The following program works fine with gcc on  Solaris but does not compile with SC5.0:
*/

#include <systemc.h>

int sc_main(int argc, char* arg[]) 
{
    sc_bv<64> bv = "0111";
    sc_bv<64> res;
    
    res = bv << 2; 

    // gcc: OK
    // SC5.0: Error: Cannot assign const 
    //  sc_bv_ns::sc_shift_left<sc_bv_ns::sc_bv_base> to sc_bv_ns::sc_bv<64> 
    //  without "sc_bv_ns::sc_bv<64>::operator=(const sc_bv_ns::sc_bv<64>&)";.

    cout << res.to_string() << "\n";

    return 0;
}
