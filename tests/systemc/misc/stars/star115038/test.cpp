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
Jan/4/00 ulrich

In SystemC 1.0.1:
Conversion (via assignment or constructor) from an integer to sc_lv<32> is broken
on Sun SC5.0. I think it's an endless recursion.

It works fine with gcc. 

Example:
*/

#include "systemc.h"

int sc_main(int argc, char* arg[]) 
{
    int a;
    sc_lv<32> b;
    a=10;

    b=a;     // core dump
    cout << b << endl;
    b=sc_lv<32>(a);  // core dump
    cout << b << endl;

    return 0;
}



