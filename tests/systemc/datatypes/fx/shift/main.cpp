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

  main.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// This may look like C code, but it is really -*- C++ -*-
// 
// 5.cxx -- 
// Copyright Synopsys 1998
// Author          : Ric Hilderink
// Created On      : Wed Dec 30 12:41:10 1998
// Status          : none
// 

#include <limits.h>
#define SC_INCLUDE_FX
#include "systemc.h"

extern void operator_shift_right(ostream& out);
extern void operator_shift_left(ostream& out);
extern void operator_shift_both(ostream& out);

int sc_main( int, char** )
{
  ostream& out = cout;

  out.precision(15);

 out << "**************** operator << **********\n";
 operator_shift_left(out);
 out << "**************** operator >> **********\n";
 operator_shift_right(out);

 out << "**************** operator << >> *******\n";
 operator_shift_both(out);

  return 0;
}
