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

  add_big.cpp -- 

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
// add_big.cxx -- 
// Copyright Synopsys 1998
// Author          : Ric Hilderink
// Created On      : Fri Jan 15 12:35:43 1999
// Status          : none
// 


#define SC_INCLUDE_FX
#include "systemc.h"


#define SHOW(r, m) \
out << r.to_string(SC_BIN) << " " << flush; \
out << r.to_string(SC_HEX) << " " << flush; \
out << m.to_string(SC_BIN) << " " << flush; \
out << m.to_string(SC_HEX) << "\n" << flush


#define BIG_ARROW(T_op)							      \
{									      \
  out << "______________________ " #T_op " ______________________\n";         \
  out << "______________________ UP        ______________________\n";         \
  sc_fxtype_params fooc(T_WL, T_IWL, SC_RND, SC_SAT);                        \
  T_op m(1); \
  T_op r(1); \
  int i;								      \
  for (i = 0; i < T_WL + 10; ++i)						      \
    {									      \
      r += r + (r >> 1);								      \
      SHOW(r, m);							      \
      m *= 16;								      \
    }									      \
  out << "______________________ DOWN      ______________________\n";         \
  m = 1;								      \
  for (i = 0; i < T_WL + 10; ++i)			                      \
    {									      \
      r -= m;								      \
      SHOW(r, m);							      \
      m *= 16;								      \
    }									      \
}

#define T_FX_FLOAT  sc_fxval
#define T_FX_UFIX   sc_ufix
#define T_FX_FIX    sc_fix
#define T_FX_FIXED  sc_fixed<T_WL, T_IWL>
#define T_FX_UFIXED sc_ufixed<T_WL, T_IWL>

#define THE_BIG_ARROW							      \
BIG_ARROW(T_FX_FLOAT)							      \
BIG_ARROW(T_FX_UFIX)							      \
BIG_ARROW(T_FX_FIX)							      \
BIG_ARROW(T_FX_FIXED)							      \
BIG_ARROW(T_FX_UFIXED)


void add_big(ostream& out)
{
  out.precision(15);

#define T_WL 16
#define T_IWL T_WL
  out << "************* add_big " << T_WL << " ***************\n";
  THE_BIG_ARROW;
#undef T_WL
#undef T_IWL
#define T_WL 67
#define T_IWL T_WL
  out << "************* add_big " << T_WL << " ***************\n";
  THE_BIG_ARROW;
#undef T_WL
#undef T_IWL
#define T_WL 150
#define T_IWL T_WL
  out << "************* add_big " << T_WL << " ***************\n";
  THE_BIG_ARROW;
}
