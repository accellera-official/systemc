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

  bsps1.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

int
sc_main(int ac, char* av[])
{
    sc_int<32> x;

    x = 0;
    x[1] = 1;
    x[2] = 1;
    x[3] = 0;
    x[4] = 0;
    x[5] = 1;

    assert( x == 38 );
    cout << x << endl;

    x[5] = 0;
    x[31] = 1;

#if !defined( _MSC_VER )
    assert((uint64)x == 0xffffffff80000006ULL);
#else
    assert((uint64)x == 0xffffffff80000006ui64);
#endif
    cout << x << endl;

    x.range(31,5) = 10;
    assert(x == 326);
    cout << x << endl;

    x = 0;
    x.range(21,7) = 9;
    assert(x == 1152);
    cout << x << endl;

    return 0;
}
