/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  std_ulogic_datatype.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Main file for "std_ulogic" data type */ 

#include "systemc.h"

typedef sc_logic std_ulogic;

int sc_main(int ac, char *av[])
{

// 1. DECLARATION SYNTAX
  std_ulogic	s1;
  std_ulogic    s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;

  
// 2. TYPE CONVERSION

  // std_ulogic <- std_ulogic
  s1  = 'X';
  s2  = '0';
  s3  = '1';
  s4  = 'Z';
  s10 = true;
  s11 = false;

  cout 	<< "\nstd_ulogic \t<=\t\t std_ulogic"
	<< "\n-------------------------------------------"
	<< "\nUNKNOWN \t= "       << s1  << "\t\t 'X'"
       	<< "\nZERO \t\t= "        << s2  << "\t\t '0'"
	<< "\nONE \t\t= "         << s3  << "\t\t '1'"
	<< "\nTRISTATE \t= "      << s4  << "\t\t 'Z'"
	<< "\nTRUE \t\t= "        << s10 << "\t\t true"
	<< "\nFALSE \t\t= "       << s11 << "\t\t false" 
       	<< "\n" << endl;

  // std_ulogic <- literals 
  std_ulogic 	a, b, c, d;

  a = true;
  b = false;
  c = 1;
  d = 0;

  cout  << "std_ulogic   <=\t literals"
        << "\n-------------------------"
        << "\nA = " << a << "\t\t true"
        << "\nB = " << b << "\t\t false"
        << "\nC = " << c << "\t\t 1"
        << "\nD = " << d << "\t\t 0"
        << "\n" << endl;

  // std_ulogic <- bool 
  std_ulogic    su1, su2, su3, su4;	
  bool		b1, b2, b3, b4;

  b1  = true; 
  b2  = false;
  b3  = 1;
  b4  = 0;

  su1 = b1;
  su2 = b2;
  su3 = b3;
  su4 = b4;

  cout  << "std_ulogic  <=\t bool"
        << "\n---------------------"
        << "\nTRUE \t= "  << su1 << "\t " << b1
        << "\nFALSE \t= " << su2 << "\t " << b2
        << "\n1 \t= " 	  << su3 << "\t " << b3
        << "\n0 \t= " 	  << su4 << "\t " << b4
        << "\n" << endl;

  // std_ulogic <- char 
  std_ulogic	sc1, sc2, sc3, sc4, sc5, sc6, sc7, sc8, sc9;
  std_ulogic	sc10, sc11, sc12, sc13, sc14, sc15, sc16, sc17, sc18, sc19;
  char		c1, c2, c3, c4, c5, c6, c7, c8, c9;
  char		c10, c11, c12, c13, c14, c15, c16, c17, c18, c19;

  c3  = 'X';
  c4  = 'x'; 
  c5  = '0';
  c6  = '1';
  c7  = 'Z';
  c8  = 'z';

  sc3  = c3; 
  sc4  = c4; 
  sc5  = c5; 
  sc6  = c6; 
  sc7  = c7; 
  sc8  = c8; 

  cout  << "std_ulogic  <=\t char"
        << "\n---------------------"
        << "\nSC3 \t= "  << sc3  << "\t '" << c3  << "'"
        << "\nSC4 \t= "  << sc4  << "\t '" << c4  << "'"
        << "\nSC5 \t= "  << sc5  << "\t '" << c5  << "'"
        << "\nSC6 \t= "  << sc6  << "\t '" << c6  << "'"
        << "\nSC7 \t= "  << sc7  << "\t '" << c7  << "'"
        << "\nSC8 \t= "  << sc8  << "\t '" << c8  << "'"
        << "\n" << endl;


// 3. OPERATORS
//    Supported operators:	~ & ^ | &= ^= |= == != =

#define VAL1	'1'
#define VAL2	'1'

  std_ulogic 	op1 = sc_logic(VAL1);
  std_ulogic  	op2 = sc_logic(VAL2);
  std_ulogic 	r1, r2, r3, r4, r5, r6, r7, r8, r9; 
  std_ulogic 	r10, r11, r12, r13, r14, r15, r16, r17, r18, r19;  
  std_ulogic 	r20, r21, r22, r23, r24, r25, r26, r27, r28, r29;  
  std_ulogic 	r30, r31, r32, r33, r34, r35;

//  r1 = op1 * op2;			// Multiplication

//  r2 = op1 / op2;			// Division

//  r3 = op1 % op2;			// Modulus

//  r4 = op1 + op2;			// Addition

//  r5 = op1 - op2;			// Subtraction

//  r6 = !op1;				// Logical NOT

//  r7 = op1 && op2;			// Logical AND

//  r8 = op1 || op2;			// Logical OR

//  r9 = op1 < op2;			// Less than

//  r10 = op1 <= op2;			// Less than or equal

//  r11 = op1 > op2;			// Greater than

//  r12 = op1 >= op2;			// Greater than or equal

//  r13 = op1 += op2;			// Compound addition
//    op1 = VAL1; op2 = VAL2;

//  r14 = op1 -= op2;			// Compound subtraction
//    op1 = VAL1; op2 = VAL2;

//  r15 = op1 *= op2;			// Compound multiplication
//    op1 = VAL1; op2 = VAL2;

//  r16 = op1 /= op2;			// Compound division
//    op1 = VAL1; op2 = VAL2;

//  r17 = op1 %= op2;			// Compound modulus
//    op1 = VAL1; op2 = VAL2;

//  r18 = op1 <<= op2;			// Compound shift left 
//    op1 = VAL1; op2 = VAL2;

//  r19 = op1 >>= op2;			// Compound shift right 
//    op1 = VAL1; op2 = VAL2;

  r20 = op1 &= op2;			// Compound bitwise AND 
    op1 = VAL1; op2 = VAL2;

  r21 = op1 ^= op2;			// Compound bitwise XOR 
    op1 = VAL1; op2 = VAL2;

  r22 = op1 |= op2;			// Compound bitwise OR 
    op1 = VAL1; op2 = VAL2;

//  r23 = op2++;				// Postfix increment 
//    op1 = VAL1; op2 = VAL2;

//  r24 = ++op2;				// Prefix increment 
//    op1 = VAL1; op2 = VAL2;

//  r25 = op2--;				// Postfix decrement 
//    op1 = VAL1; op2 = VAL2;

//  r26 = --op2;				// Prefix decrement 
//    op1 = VAL1; op2 = VAL2;

//  r27 = (op1 > op2) ? true : false;	// Arithmetic if
//  r28 = (op1 < op2) ? true : false;	// Arithmetic if

//  r29 = op1, r29 = op2;		 	// Comma  

  r30 = ~op1;				// Bitwise NOT

//  r31 = op1 << op2;			// Left shift 
//    op1 = VAL1; op2 = VAL2;

//  r32 = op1 >> op2;			// Right shift 
//    op1 = VAL1; op2 = VAL2;

  r33 = op1 & op2;			// Bitwise AND 

  r34 = op1 ^ op2;			// Bitwise XOR 

  r35 = op1 | op2;			// Bitwise OR 

  cout 	<< "op1 \t operator \t op2 \t result  [All operands are std_ulogic]"
	<< "\n----------------------------------------------------------------"
// 	<< "\n" << op1 << "\t    * \t\t " << op2 << "\t = " << r1
// 	<< "\n" << op1 << "\t    / \t\t " << op2 << "\t = " << r2
// 	<< "\n" << op1 << "\t    % \t\t " << op2 << "\t = " << r3
// 	<< "\n" << op1 << "\t    + \t\t " << op2 << "\t = " << r4
// 	<< "\n" << op1 << "\t    - \t\t " << op2 << "\t = " << r5
//  	<< "\n!(" << op1 << ") \t\t\t\t = " << r6 
//  	<< "\n" << op1 << "\t    && \t\t " << op2 << "\t = " << r7
//  	<< "\n" << op1 << "\t    || \t\t " << op2 << "\t = " << r8
//  	<< "\n" << op1 << "\t    < \t\t "  << op2 << "\t = " << r9
//  	<< "\n" << op1 << "\t    <= \t\t " << op2 << "\t = " << r10
//  	<< "\n" << op1 << "\t    > \t\t "  << op2 << "\t = " << r11
//  	<< "\n" << op1 << "\t    >= \t\t " << op2 << "\t = " << r12
//  	<< "\n" << op1 << "\t    += \t\t " << op2 << "\t = " << r13 
//  	<< "\n" << op1 << "\t    -= \t\t " << op2 << "\t = " << r14 
//  	<< "\n" << op1 << "\t    *= \t\t " << op2 << "\t = " << r15 
//  	<< "\n" << op1 << "\t    /= \t\t " << op2 << "\t = " << r16 
//  	<< "\n" << op1 << "\t    %= \t\t " << op2 << "\t = " << r17 
//  	<< "\n" << op1 << "\t    <<=\t\t " << op2 << "\t = " << r18 
//  	<< "\n" << op1 << "\t    >>=\t\t " << op2 << "\t = " << r19 
  	<< "\n" << op1 << "\t    &= \t\t " << op2 << "\t = " << r20 
  	<< "\n" << op1 << "\t    ^= \t\t " << op2 << "\t = " << r21 
  	<< "\n" << op1 << "\t    |= \t\t " << op2 << "\t = " << r22 
//  	<< "\n" << "\t    ()++ \t " << op2 << "\t = " << r23 
//  	<< "\n" << "\t    ++() \t " << op2 << "\t = " << r24 
//  	<< "\n" << "\t    ()-- \t " << op2 << "\t = " << r25 
//  	<< "\n" << "\t    --() \t " << op2 << "\t = " << r26 
//  	<< "\n" << op1 << "\t    > ?: \t " << op2 << "\t = " << r27 
//  	<< "\n" << op1 << "\t    < ?: \t " << op2 << "\t = " << r28 
//  	<< "\n" << op1 << "\t    , \t\t " << op2 << "\t = " << r29 
  	<< "\n~(" << op1 << ") \t\t\t\t = " << r30 
//  	<< "\n" << op1 << "\t    << \t\t " << op2 << "\t = " << r31 
//  	<< "\n" << op1 << "\t    >> \t\t " << op2 << "\t = " << r32 
  	<< "\n" << op1 << "\t    & \t\t " << op2 << "\t = " << r33 
  	<< "\n" << op1 << "\t    ^ \t\t " << op2 << "\t = " << r34 
  	<< "\n" << op1 << "\t    | \t\t " << op2 << "\t = " << r35 
 	<< endl;

  if (op1 == op2)			// Equality
   cout << op1 << "\t    == \t\t " << op2 << "\t -> true" << endl;
  else
   cout << op1 << "\t    == \t\t " << op2 << "\t -> false" << endl;

  if (op1 != op2)			// Inequality
   cout << op1 << "\t    != \t\t " << op2 << "\t -> true" << endl;
  else
   cout << op1 << "\t    != \t\t " << op2 << "\t -> false" << endl;

  op1 = op2 = 0;			// Assignment operator concatenation
   cout << op1 << "\t    = \t\t " << op2 << endl;

// 4. OPERATOR DEFINITIONS
//	& | ^ ~
  std_ulogic 	v1, v2, v3, v4, v5, v6, v7, v8, v9;
  std_ulogic	uu, ux, u0, u1, uz, uw, ul, uh, ud;
  std_ulogic	xu, xx, x0, x1, xz, xw, xl, xh, xd;
  std_ulogic	_0u, _0x, _00, _01, _0z, _0w, _0l, _0h, _0d;
  std_ulogic	_1u, _1x, _10, _11, _1z, _1w, _1l, _1h, _1d;
  std_ulogic	zu, zx, z0, z1, zz, zw, zl, zh, zd;
  std_ulogic	wu, wx, w0, w1, wz, ww, wl, wh, wd;
  std_ulogic	lu, lx, l0, l1, lz, lw, ll, lh, ld;
  std_ulogic	hu, hx, h0, h1, hz, hw, hl, hh, hd;
  std_ulogic	du, dx, d0, d1, dz, dw, dl, dh, dd;

  v2  = 'X';
  v3  = '0';
  v4  = '1';
  v5  = 'Z';

  xx = v2 & v2;    x0 = v2 & v3;
  x1 = v2 & v4;	   xz = v2 & v5;    
   
  _0x = v3 & v2;    _00 = v3 & v3;
  _01 = v3 & v4;   _0z = v3 & v5;    
   
  _1x = v4 & v2;    _10 = v4 & v3;
  _11 = v4 & v4;   _1z = v4 & v5;    
   
  zx = v5 & v2;    z0 = v5 & v3;
  z1 = v5 & v4;	   zz = v5 & v5;    
   
  cout	<< "\n+-------------------------+"
       	<< "\n| AND (&) | X | 0 | 1 | Z |" 
	<< "\n+-------------------------+"
  	<< "\n|    X    | " << xx << " | " << x0 << " | "
	   	            << x1 << " | " << xz << " | "
	<< "\n+-------------------------+"
  	<< "\n|    0    | " << _0x << " | " << _00 << " | "
	   	            << _01 << " | " << _0z << " | "
	<< "\n+-------------------------+"
  	<< "\n|    1    | " << _1x << " | " << _10 << " | "
	   	            << _11 << " | " << _1z << " | "
	<< "\n+-------------------------+"
  	<< "\n|    Z    | " << zx << " | " << z0 << " | "
	   	            << z1 << " | " << zz << " | "
	<< "\n+-------------------------+"
	<< endl;

  xx = v2 | v2;    x0 = v2 | v3;
  x1 = v2 | v4;	   xz = v2 | v5;    
   
  _0x = v3 | v2;   _00 = v3 | v3;
  _01 = v3 | v4;   _0z = v3 | v5;    
   
  _1x = v4 | v2;   _10 = v4 | v3;
  _11 = v4 | v4;   _1z = v4 | v5;    
   
  zx = v5 | v2;    z0 = v5 | v3;
  z1 = v5 | v4;	   zz = v5 | v5;    
   
  cout	<< "\n+-------------------------+"
       	<< "\n| OR  (|) | X | 0 | 1 | Z |" 
	<< "\n+-------------------------+"
  	<< "\n|    X    | " << xx << " | " << x0 << " | "
	   	            << x1 << " | " << xz << " | "
	<< "\n+-------------------------+"
  	<< "\n|    0    | " << _0x << " | " << _00 << " | "
	   	            << _01 << " | " << _0z << " | "
	<< "\n+-------------------------+"
  	<< "\n|    1    | " << _1x << " | " << _10 << " | "
	   	            << _11 << " | " << _1z << " | "
	<< "\n+-------------------------+"
  	<< "\n|    Z    | " << zx << " | " << z0 << " | "
	   	            << z1 << " | " << zz << " | "
	<< "\n+-------------------------+"
	<< endl;

  xx = v2 ^ v2;    x0 = v2 ^ v3;
  x1 = v2 ^ v4;	   xz = v2 ^ v5;    
   
  _0x = v3 ^ v2;   _00 = v3 ^ v3;
  _01 = v3 ^ v4;   _0z = v3 ^ v5;    
   
  _1x = v4 ^ v2;   _10 = v4 ^ v3;
  _11 = v4 ^ v4;   _1z = v4 ^ v5;    
   
  zx = v5 ^ v2;    z0 = v5 ^ v3;
  z1 = v5 ^ v4;	   zz = v5 ^ v5;    
   
  cout	<< "\n+-------------------------+"
       	<< "\n| XOR (^) | X | 0 | 1 | Z |" 
	<< "\n+-------------------------+"
  	<< "\n|    X    | " << xx << " | " << x0 << " | "
	   	            << x1 << " | " << xz << " | "
	<< "\n+-------------------------+"
  	<< "\n|    0    | " << _0x << " | " << _00 << " | "
	   	            << _01 << " | " << _0z << " | "
	<< "\n+-------------------------+"
  	<< "\n|    1    | " << _1x << " | " << _10 << " | "
	   	            << _11 << " | " << _1z << " | "
	<< "\n+-------------------------+"
  	<< "\n|    Z    | " << zx << " | " << z0 << " | "
	   	            << z1 << " | " << zz << " | "
	<< "\n+-------------------------+"
	<< endl;
  
  std_ulogic 	nu, nx, n0, n1, nz, nw, nl, nh, nd;

  nu = ~v1;
  nx = ~v2;
  n0 = ~v3;
  n1 = ~v4;
  nz = ~v5;
  nw = ~v6;
  nl = ~v7;
  nh = ~v8;
  nd = ~v9;
 
  cout	<< "\n+-------------------------+"
       	<< "\n| NOT (~) | X | 0 | 1 | Z |" 
	<< "\n+-------------------------+"
  	<< "\n|         | " << nx << " | " << n0 << " | "
	   	            << n1 << " | " << nz << " | "
	<< "\n+-------------------------+"
	<< endl;
    return 0;
}
