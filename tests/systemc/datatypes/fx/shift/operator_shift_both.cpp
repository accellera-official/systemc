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

  operator_shift_both.cpp -- 

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
// operator_shift_both.cxx -- 
// Copyright Synopsys 1998
// Author          : Ric Hilderink
// Created On      : Fri Jan 15 14:08:43 1999
// Status          : none
// 


#include <limits.h>
#define SC_INCLUDE_FX
#include "systemc.h"



#define T_FX_FLOAT  sc_fxval
#define T_FX_UFIX   sc_ufix
#define T_FX_FIX    sc_fix
#define T_FX_FIXED  sc_fixed<T_WL, T_IWL>
#define T_FX_UFIXED sc_ufixed<T_WL, T_IWL>

#define SHOW(i, a) \
  out << i << " : " << a.to_string(SC_BIN) << "\n";


#define SHIFT_BOTH_OP(T_op)						      \
{									      \
  sc_fxtype_params fooc(T_WL, T_IWL, SC_TRN, SC_WRAP);		      \
  									      \
  T_op r(2); \
  int i;								      \
  for (i = 0; i < T_WL+10; ++i)						      \
    {									      \
      r -= 1;								      \
      r >>= 1;								      \
      SHOW(i, r);							      \
    }									      \
  r *= 1.010203;							      \
  for (i = 0; i < T_WL+10; ++i)						      \
    {									      \
      r -= 1;								      \
      r <<= 1;								      \
      SHOW(i, r);							      \
    }									      \
}

#define SHIFT_BOTH							      \
SHIFT_BOTH_OP(T_FX_FLOAT)						      \
SHIFT_BOTH_OP(T_FX_UFIX)						      \
SHIFT_BOTH_OP(T_FX_FIX)							      \
SHIFT_BOTH_OP(T_FX_FIXED)						      \
SHIFT_BOTH_OP(T_FX_UFIXED)



void operator_shift_both(ostream& out)
{
#define T_WL 26
#define T_IWL 13
  SHIFT_BOTH;
#undef T_WL
#undef T_IWL
#define T_WL 130
#define T_IWL 65
  SHIFT_BOTH;
#undef T_WL
#undef T_IWL
#define T_WL 300
#define T_IWL 150
  SHIFT_BOTH;
}
