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
// main.cxx<3> -- 
// Copyright Synopsys 1998
// Author          : Ric Hilderink
// Created On      : Mon Jan 11 13:25:23 1999
// Status          : none
// 


#define SC_INCLUDE_FX
#include "systemc.h"
#include "test_all.hh"


extern void test_bit(ostream&, int, int);

static void test_cases(ostream& out, int wl, int iwl)
{
  test_bit(out, wl, iwl);
}


int sc_main( int, char** )
{
  int wl = 0, iwl = 0;
  test_cases(cout, wl, wl);

  return 0;
}
