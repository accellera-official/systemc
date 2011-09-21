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
I found this while trying to improve coverage of the SystemC regression tests.

Compile the following program, bug.cpp, with

g++ -g -I /u/scp/src/systemc-2.0/include bug.cpp /u/scp/src/systemc-2.0/lib-gccsparcOS5/libsystemc.a
(in mountain view, elsewhere use SYSTEMC_HOME pointing to the 2.0 release)

-------------------------------------------------------------------------------
*/

#define SC_INCLUDE_FX 1
#include "systemc.h"

double bug()
{
    sc_fxval_fast fast(2);
    sc_fxval slow(1);
    fast  = slow;
    fast += slow;
    fast -= slow;
    fast *= slow;
    fast /= slow;
    return fast.to_double();
}

int sc_main(int, char*[])
{
    cout << bug() << endl;
    return 0;
}

/*
-------------------------------------------------------------------------------
It fails to link, giving the message

Undefined                       first referenced
 symbol                             in file
sc_fxval_fast::operator+=(sc_fxval const &)      /var/tmp/ccNbIiHN.o
sc_fxval_fast::operator-=(sc_fxval const &)       /var/tmp/cclXNLsO.o
sc_fxval_fast::operator/=(sc_fxval const &)      /var/tmp/cclXNLsO.o
sc_fxval_fast::operator*=(sc_fxval const &)       /var/tmp/cclXNLsO.o
*/

