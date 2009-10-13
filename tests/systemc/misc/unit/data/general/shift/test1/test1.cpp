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

  test1.cpp -- 

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
sc_main(int argc, char* argv[])
{
    sc_signed x(7);

    x = -34;
    cout << x << endl;
    x.dump(cout);
    for (int i = 0; i < 5; ++i) {
        sc_signed y(7);
        y = x >> i;
        cout << y << endl;
        y.dump(cout);
    }
    for (int i = 0; i < 5; ++i) {
        x = x >> 1;
        cout << x << endl;
        x.dump(cout);
    }
    sc_initialize();
    return 0;
}
