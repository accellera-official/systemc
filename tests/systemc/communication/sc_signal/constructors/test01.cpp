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

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// sc_signal test;
// constructors

#include "systemc.h"

template <class T>
void
test_constructors()
{
    sc_signal<T> sig1;
    sc_signal<T> sig2( "sig2" );

    cout << sig1.name() << endl;
    cout << sig2.name() << endl;
}

int
sc_main( int, char*[] )
{
    test_constructors<int>();

    // bool specialization
    test_constructors<bool>();

    // sc_logic specialization
    test_constructors<sc_logic>();

    return 0;
}
