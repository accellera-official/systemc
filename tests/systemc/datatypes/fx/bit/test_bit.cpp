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

  test_bit.cpp -- 

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
// test_bit.cxx -- 
// Copyright Synopsys 1998
// Author          : Ric Hilderink
// Created On      : Thu Jan 21 10:27:26 1999
// Status          : none
// 


#define SC_INCLUDE_FX
#include "systemc.h"
#include "test_all.hh"


void test_bit(ostream& out, int wl, int iwl)
{
#undef T_WL
#undef T_IWL
#define T_WL 13
#define T_IWL 13
  TEST_BIT;
#undef T_WL
#undef T_IWL
#define T_WL 13
#define T_IWL 0
  TEST_BIT;
#undef T_WL
#undef T_IWL
#define T_WL 5
#define T_IWL 13
  TEST_BIT;

#undef T_WL
#undef T_IWL
#define T_WL 65
#define T_IWL 65
  TEST_BIT;
#undef T_WL
#undef T_IWL
#define T_WL 65
#define T_IWL 33
  TEST_BIT;
#undef T_WL
#undef T_IWL
#define T_WL 65
#define T_IWL 111
  TEST_BIT;
}
