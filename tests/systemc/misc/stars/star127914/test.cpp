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
When I do an  sc_biguint + sc_int  addition, then the
code is compiled fine but links fails because the 
operator was never defined.

This happens with SystemC 1.0.2 on Solaris with both
gcc and Sun SC compiler.
*/

#include <systemc.h>

int sc_main(int, char**)
{
  sc_int<8> i8;
  sc_biguint<8> bu8;

  i8=2;
  bu8=3;
  bu8 = bu8 + i8;
  cout << "sum is " << bu8 << "\n";

  return 0;
}

/*
Undefined			first referenced
 symbol  			    in file
operator+(sc_unsigned const &, sc_int_base const &)str.o
*/
