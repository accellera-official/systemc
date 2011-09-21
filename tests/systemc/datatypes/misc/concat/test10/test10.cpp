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

  test10.cpp -- Test that low order word of negative sc_signed value is
                properly masked when used in sc_signed::concat_get_data()

  Original Author: Andy Goodrich, Forte Design Systems, 29 April 2008

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

int sc_main(int argc, char* argv[])
{

    sc_bigint<65> hi;
    hi[64] = 1; hi(63,32) = 0xffffe47b;
    hi[64] = 1; hi(63,32) = 0xffffe47b;
    sc_bigint<31> lo(1);
    sc_bigint<96> x;
    cout << "x = " << x.to_string(SC_BIN) << endl << endl;
    x = (hi,lo);
    cout  << hex << "hi = " << hi << endl
          << "lo = " << lo << endl
          << "x = " << x << endl;
    cout  << dec << "hi = " << hi.to_string(SC_BIN) << endl
          << "lo = " << lo.to_string(SC_BIN) << endl
          << "x = " << x.to_string(SC_BIN) << endl;
    cout << "Program completed" << endl;
    return 0;
}
