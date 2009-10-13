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

  array.cpp -- 

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
// array.cxx -- 
// Copyright Synopsys 1998
// Author          : Ric Hilderink
// Created On      : Wed Dec 30 11:55:50 1998
// Status          : none
// 

#include <limits.h>
#define SC_INCLUDE_FX
#include "systemc.h"
 
typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned long  ulong;

#define SHOW_ARRAY(a, b, c) cerr << #a << "[" << b << "] : " << double(c) << " : " << c.to_string(SC_HEX) << "\n"
#define IDENT_ARRAY(a) cerr << "--array-Inf-Inf-Inf-Inf-Inf- " << a << "\n"

//-----------------------------------------------------------------
static void test_fx_float_int()
{
  IDENT_ARRAY("test_fx_float_int");

  sc_fxval *a = new sc_fxval[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (int)i*i;
  
  sc_fxval *b = new sc_fxval[4];
  b[0] = (int)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (int)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_float_uint()
{
  IDENT_ARRAY("test_fx_float_uint");

  sc_fxval *a = new sc_fxval[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (uint)i*i;
  
  sc_fxval *b = new sc_fxval[4];
  b[0] = (uint)-1;
  for (i = 1; i < 4; ++i)
    // b[i] = (uint)(b[i-1] * i * -1);
    b[i] = sc_ufixed<32,32>(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_float_short()
{
  IDENT_ARRAY("test_fx_float_short");

  sc_fxval *a = new sc_fxval[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (short)i*i;
  
  sc_fxval *b = new sc_fxval[4];
  b[0] = (short)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (short)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_float_ushort()
{
  IDENT_ARRAY("test_fx_float_ushort");

  sc_fxval *a = new sc_fxval[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (ushort)i*i;
  
  sc_fxval *b = new sc_fxval[4];
  b[0] = (ushort)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (ushort)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_float_long()
{
  IDENT_ARRAY("test_fx_float_long");

  sc_fxval *a = new sc_fxval[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (long)i*i;
  
  sc_fxval *b = new sc_fxval[4];
  b[0] = (long)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (long)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_float_ulong()
{
  IDENT_ARRAY("test_fx_float_ulong");

  sc_fxval *a = new sc_fxval[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (ulong)i*i;
  
  sc_fxval *b = new sc_fxval[4];
  b[0] = (ulong)-1;
  for (i = 1; i < 4; ++i)
    // b[i] = (ulong)(b[i-1] * i * -1);
    b[i] = sc_ufixed<32,32>(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_float_float()
{
  IDENT_ARRAY("test_fx_float_float");
  
  sc_fxval *a = new sc_fxval[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (float)i*i*1.123456789;
  
  sc_fxval *b = new sc_fxval[4];
  b[0] = (float)-1.987654321;
  for (i = 1; i < 4; ++i)
    b[i] = (float)(b[i-1] * i * -1.789654123);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_float_double()
{
  IDENT_ARRAY("test_fx_float_double");
  
  
  sc_fxval *a = new sc_fxval[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (double)i*i*1.123456789;
  
  sc_fxval *b = new sc_fxval[4];
  b[0] = (double)-1.987654321;
  for (i = 1; i < 4; ++i)
    b[i] = (double)(b[i-1] * i * -1.789654123);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

//-----------------------------------------------------------------
static void test_fx_ufix_int()
{
  IDENT_ARRAY("test_fx_ufix_int");

  sc_ufix *a = new sc_ufix[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (int)i*i;
  
  sc_ufix *b = new sc_ufix[4];
  b[0] = (int)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (int)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
  
}

static void test_fx_ufix_uint()
{
  IDENT_ARRAY("test_fx_ufix_uint");

  sc_ufix *a = new sc_ufix[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (uint)i*i;
  
  sc_ufix *b = new sc_ufix[4];
  b[0] = (uint)-1;
  for (i = 1; i < 4; ++i)
    // b[i] = (uint)(b[i-1] * i * -1);
    b[i] = sc_ufixed<32,32>(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_ufix_short()
{
  IDENT_ARRAY("test_fx_ufix_short");

  sc_ufix *a = new sc_ufix[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (short)i*i;
  
  sc_ufix *b = new sc_ufix[4];
  b[0] = (short)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (short)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
  
}

static void test_fx_ufix_ushort()
{
  IDENT_ARRAY("test_fx_ufix_ushort");

  sc_ufix *a = new sc_ufix[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (ushort)i*i;
  
  sc_ufix *b = new sc_ufix[4];
  b[0] = (ushort)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (ushort)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_ufix_long()
{
  IDENT_ARRAY("test_fx_ufix_long");

  sc_ufix *a = new sc_ufix[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (long)i*i;
  
  sc_ufix *b = new sc_ufix[4];
  b[0] = (long)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (long)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_ufix_ulong()
{
  IDENT_ARRAY("test_fx_ufix_ulong");

  sc_ufix *a = new sc_ufix[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (ulong)i*i;
  
  sc_ufix *b = new sc_ufix[4];
  b[0] = (ulong)-1;
  for (i = 1; i < 4; ++i)
    // b[i] = (ulong)(b[i-1] * i * -1);
    b[i] = sc_ufixed<32,32>(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_ufix_float()
{
  IDENT_ARRAY("test_fx_ufix_float");
  
  sc_ufix *a = new sc_ufix[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (float)i*i*1.123456789;
  
  sc_ufix *b = new sc_ufix[4];
  b[0] = (float)-1.987654321;
  for (i = 1; i < 4; ++i)
    b[i] = (float)(b[i-1] * i * -1.789654123);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_ufix_double()
{
  IDENT_ARRAY("test_fx_ufix_double");
  
  
  sc_ufix *a = new sc_ufix[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (double)i*i*1.123456789;
  
  sc_ufix *b = new sc_ufix[4];
  b[0] = (double)-1.987654321;
  for (i = 1; i < 4; ++i)
    b[i] = (double)(b[i-1] * i * -1.789654123);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

//-----------------------------------------------------------------
static void test_fx_fix_int()
{
  IDENT_ARRAY("test_fx_fix_int");

  sc_fix *a = new sc_fix[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (int)i*i;
  
  sc_fix *b = new sc_fix[4];
  b[0] = (int)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (int)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_fix_uint()
{
  IDENT_ARRAY("test_fx_fix_uint");

  sc_fix *a = new sc_fix[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (uint)i*i;
  
  sc_fix *b = new sc_fix[4];
  b[0] = (uint)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (uint)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_fix_short()
{
  IDENT_ARRAY("test_fx_fix_short");

  sc_fix *a = new sc_fix[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (short)i*i;
  
  sc_fix *b = new sc_fix[4];
  b[0] = (short)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (short)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_fix_ushort()
{
  IDENT_ARRAY("test_fx_fix_ushort");

  sc_fix *a = new sc_fix[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (ushort)i*i;
  
  sc_fix *b = new sc_fix[4];
  b[0] = (ushort)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (ushort)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_fix_long()
{
  IDENT_ARRAY("test_fx_fix_long");

  sc_fix *a = new sc_fix[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (long)i*i;
  
  sc_fix *b = new sc_fix[4];
  b[0] = (long)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (long)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_fix_ulong()
{
  IDENT_ARRAY("test_fx_fix_ulong");

  sc_fix *a = new sc_fix[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (ulong)i*i;
  
  sc_fix *b = new sc_fix[4];
  b[0] = (ulong)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (ulong)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_fix_float()
{
  IDENT_ARRAY("test_fx_fix_float");
  
  sc_fix *a = new sc_fix[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (float)i*i*1.123456789;
  
  sc_fix *b = new sc_fix[4];
  b[0] = (float)-1.987654321;
  for (i = 1; i < 4; ++i)
    b[i] = (float)(b[i-1] * i * -1.789654123);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_fix_double()
{
  IDENT_ARRAY("test_fx_fix_double");
  
  
  sc_fix *a = new sc_fix[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (double)i*i*1.123456789;
  
  sc_fix *b = new sc_fix[4];
  b[0] = (double)-1.987654321;
  for (i = 1; i < 4; ++i)
    b[i] = (double)(b[i-1] * i * -1.789654123);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

//-----------------------------------------------------------------
static void test_fx_fixed_int()
{
  IDENT_ARRAY("test_fx_fixed_int");

  sc_fixed<8, 5> *a = new sc_fixed<8, 5>[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (int)i*i;
  
  sc_fixed<8, 5> *b = new sc_fixed<8, 5>[4];
  b[0] = (int)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (int)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_fixed_uint()
{
  IDENT_ARRAY("test_fx_fixed_uint");

  sc_fixed<8, 5> *a = new sc_fixed<8, 5>[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (uint)i*i;
  
  sc_fixed<8, 5> *b = new sc_fixed<8, 5>[4];
  b[0] = (uint)abs(-1);
  for (i = 1; i < 4; ++i)
    b[i] = (uint)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_fixed_short()
{
  IDENT_ARRAY("test_fx_fixed_short");

  sc_fixed<8, 5> *a = new sc_fixed<8, 5>[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (short)i*i;
  
  sc_fixed<8, 5> *b = new sc_fixed<8, 5>[4];
  b[0] = (short)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (short)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_fixed_ushort()
{
  IDENT_ARRAY("test_fx_fixed_ushort");

  sc_fixed<8, 5> *a = new sc_fixed<8, 5>[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (ushort)i*i;
  
  sc_fixed<8, 5> *b = new sc_fixed<8, 5>[4];
  b[0] = (ushort)abs(-1);
  for (i = 1; i < 4; ++i)
    b[i] = (ushort)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_fixed_long()
{
  IDENT_ARRAY("test_fx_fixed_long");

  sc_fixed<8, 5> *a = new sc_fixed<8, 5>[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (long)i*i;
  
  sc_fixed<8, 5> *b = new sc_fixed<8, 5>[4];
  b[0] = (long)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (long)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_fixed_ulong()
{
  IDENT_ARRAY("test_fx_fixed_ulong");

  sc_fixed<8, 5> *a = new sc_fixed<8, 5>[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (ulong)i*i;
  
  sc_fixed<8, 5> *b = new sc_fixed<8, 5>[4];
  b[0] = (ulong)abs(-1);
  for (i = 1; i < 4; ++i)
    b[i] = (ulong)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_fixed_float()
{
  IDENT_ARRAY("test_fx_fixed_float");
  
  sc_fixed<8, 5> *a = new sc_fixed<8, 5>[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (float)i*i*1.123456789;
  
  sc_fixed<8, 5> *b = new sc_fixed<8, 5>[4];
  b[0] = (float)-1.987654321;
  for (i = 1; i < 4; ++i)
    b[i] = (float)(b[i-1] * i * -1.789654123);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_fixed_double()
{
  IDENT_ARRAY("test_fx_fixed_double");
  
  
  sc_fixed<8, 5> *a = new sc_fixed<8, 5>[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (double)i*i*1.123456789;
  
  sc_fixed<8, 5> *b = new sc_fixed<8, 5>[4];
  b[0] = (double)-1.987654321;
  for (i = 1; i < 4; ++i)
    b[i] = (double)(b[i-1] * i * -1.789654123);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

//-----------------------------------------------------------------
static void test_fx_ufixed_int()
{
  IDENT_ARRAY("test_fx_ufixed_int");

  sc_ufixed<8, 5> *a = new sc_ufixed<8, 5>[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (int)i*i;
  
  sc_ufixed<8, 5> *b = new sc_ufixed<8, 5>[4];
  b[0] = (int)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (int)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_ufixed_uint()
{
  IDENT_ARRAY("test_fx_ufixed_uint");

  sc_ufixed<8, 5> *a = new sc_ufixed<8, 5>[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (uint)i*i;
  
  sc_ufixed<8, 5> *b = new sc_ufixed<8, 5>[4];
  b[0] = (uint)abs(-1);
  for (i = 1; i < 4; ++i)
    b[i] = (uint)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_ufixed_short()
{
  IDENT_ARRAY("test_fx_ufixed_short");

  sc_ufixed<8, 5> *a = new sc_ufixed<8, 5>[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (short)i*i;
  
  sc_ufixed<8, 5> *b = new sc_ufixed<8, 5>[4];
  b[0] = (short)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (short)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_ufixed_ushort()
{
  IDENT_ARRAY("test_fx_ufixed_ushort");

  sc_ufixed<8, 5> *a = new sc_ufixed<8, 5>[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (ushort)i*i;
  
  sc_ufixed<8, 5> *b = new sc_ufixed<8, 5>[4];
  b[0] = (ushort)abs(-1);
  for (i = 1; i < 4; ++i)
    b[i] = (ushort)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_ufixed_long()
{
  IDENT_ARRAY("test_fx_ufixed_long");

  sc_ufixed<8, 5> *a = new sc_ufixed<8, 5>[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (long)i*i;
  
  sc_ufixed<8, 5> *b = new sc_ufixed<8, 5>[4];
  b[0] = (long)-1;
  for (i = 1; i < 4; ++i)
    b[i] = (long)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_ufixed_ulong()
{
  IDENT_ARRAY("test_fx_ufixed_ulong");

  sc_ufixed<8, 5> *a = new sc_ufixed<8, 5>[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (ulong)i*i;
  
  sc_ufixed<8, 5> *b = new sc_ufixed<8, 5>[4];
  b[0] = (ulong)abs(-1);
  for (i = 1; i < 4; ++i)
    b[i] = (ulong)(b[i-1] * i * -1);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_ufixed_float()
{
  IDENT_ARRAY("test_fx_ufixed_float");
  
  sc_ufixed<8, 5> *a = new sc_ufixed<8, 5>[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (float)i*i*1.123456789;
  
  sc_ufixed<8, 5> *b = new sc_ufixed<8, 5>[4];
  b[0] = (float)-1.987654321;
  for (i = 1; i < 4; ++i)
    b[i] = (float)(b[i-1] * i * -1.789654123);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

static void test_fx_ufixed_double()
{
  IDENT_ARRAY("test_fx_ufixed_double");
  
  
  sc_ufixed<8, 5> *a = new sc_ufixed<8, 5>[4];
  int i;
  for (i = 0; i < 4; ++i)
    a[i] = (double)i*i*1.123456789;
  
  sc_ufixed<8, 5> *b = new sc_ufixed<8, 5>[4];
  b[0] = (double)-1.987654321;
  for (i = 1; i < 4; ++i)
    b[i] = (double)(b[i-1] * i * -1.789654123);

  for (i = 0; i < 4; ++i)
    {
      SHOW_ARRAY(a, i, a[i]);
      SHOW_ARRAY(b, i, b[i]);
    }
  delete [] a;
  delete [] b;
}

void array()
{
  cerr << "************** array test_fx_float_\n";
  test_fx_float_int();
  test_fx_float_uint();
  test_fx_float_short();
  test_fx_float_ushort();
  test_fx_float_long();
  test_fx_float_ulong();
  test_fx_float_float();
  test_fx_float_double();
  cerr << "************** array test_fx_ufix_\n";
  test_fx_ufix_int();
  test_fx_ufix_uint();
  test_fx_ufix_short();
  test_fx_ufix_ushort();
  test_fx_ufix_long();
  test_fx_ufix_ulong();
  test_fx_ufix_float();
  test_fx_ufix_double();
  cerr << "************** array test_fx_fix_\n";
  test_fx_fix_int();
  test_fx_fix_uint();
  test_fx_fix_short();
  test_fx_fix_ushort();
  test_fx_fix_long();
  test_fx_fix_ulong();
  test_fx_fix_float();
  test_fx_fix_double();
  cerr << "************** array test_fx_fixed_\n";
  test_fx_fixed_int();
  test_fx_fixed_uint();
  test_fx_fixed_short();
  test_fx_fixed_ushort();
  test_fx_fixed_long();
  test_fx_fixed_ulong();
  test_fx_fixed_float();
  test_fx_fixed_double();
  cerr << "************** array test_fx_ufixed_\n";
  test_fx_ufixed_int();
  test_fx_ufixed_uint();
  test_fx_ufixed_short();
  test_fx_ufixed_ushort();
  test_fx_ufixed_long();
  test_fx_ufixed_ulong();
  test_fx_ufixed_float();
  test_fx_ufixed_double();
}
