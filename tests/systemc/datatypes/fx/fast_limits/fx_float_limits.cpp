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

  fx_float_limits.cpp -- 

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
// fx_float_limits.cxx -- 
// Copyright Synopsys 1998
// Author          : Ric Hilderink
// Created On      : Fri Jan  8 14:31:37 1999
// Status          : none
// 


#include <limits.h>
#include <math.h>

#define SC_INCLUDE_FX
#include "systemc.h"
#include "fx_precision_double.h"

#define SHOW(a) out << #a << " : " << a.to_string(SC_HEX) << "\n"
#define SHOW_EXP(a) { res = a; out << #a << " : " << res.to_string(SC_HEX) << "\n"; }


#define SHOW_EXPRS(a, b) \
  SHOW_EXP(a b zero_min);   \
  SHOW_EXP(a b zero_plus);  \
  SHOW_EXP(a b zero);       \
  SHOW_EXP(a b nan);        \
  SHOW_EXP(a b inf_plus);   \
  SHOW_EXP(a b inf_min);    \
  SHOW_EXP(a b inf);        \
  SHOW_EXP(a b long_max);   \
  SHOW_EXP(a b long_min);   \
  SHOW_EXP(a b int_max);    \
  SHOW_EXP(a b int_min);    \
  SHOW_EXP(a b uint_max);   \
  SHOW_EXP(a b ulong_max);  \
  SHOW_EXP(a b double_min); \
  SHOW_EXP(a b double_max); \
  SHOW_EXP(a b float_min);  \
  SHOW_EXP(a b float_max);  

#define SHOW_EXPS(a) \
  SHOW_EXPRS(a, /) \
  SHOW_EXPRS(a, *) \
  SHOW_EXPRS(a, +) \
  SHOW_EXPRS(a, -) \
  SHOW_EXPRS(a, * a *)


  extern void test_fx_float_limits_zero(ostream&);
  extern void test_fx_float_limits_inf(ostream&);
  extern void test_fx_float_limits_long(ostream&);
  extern void test_fx_float_limits_double(ostream&);


void test_fx_float_limits(ostream& out)
{
  out << "****************** limits fx_float\n";

  test_fx_float_limits_zero(out);
  test_fx_float_limits_inf(out);
  test_fx_float_limits_long(out);
  test_fx_float_limits_double(out);
  
}

