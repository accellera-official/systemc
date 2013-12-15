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

  sign_extension.cpp -- 

  Original Author: Philipp A. Hartmann, OFFIS, 2012-07-17

 -----------------------------------------------------------------------------

 This test demonatrates a bug in the sc_biguint constructor(?), where
 construction from bit-wise expressions with "unsigned int" and sc_bigint
 may lead to wrongly sign-extended values (at least on 32-bit machines) when
 the destination size is bigger than the source size.

 *****************************************************************************/

#include <systemc>
using sc_dt::sc_int;
using sc_dt::sc_uint;
using sc_dt::sc_bigint;
using sc_dt::sc_biguint;
using sc_dt::SC_BIN;

#define DUMP_VAR( Var ) \
  std::cout << #Var "=" << (Var).to_string(SC_BIN) << "\n"

int sc_main(int, char*[])
{
  
    sc_int<31>     v_sc_int_31_min     = (1<<30); // 010...0 (31 bit)
    sc_bigint<31>  v_sc_bigint_31_min  = (1<<30); // 010...0 
    unsigned int   v_uint_max          = ~0;      // 11....1 (32-bit)
    sc_uint<32>    v_sc_uint32_max     = ~0;      // 11....1 (32-bit)
    sc_biguint<32> v_sc_biguint_32_max = -1;      // 11....1 (32-bit)

    DUMP_VAR( v_sc_int_31_min );
    DUMP_VAR( v_sc_bigint_31_min );
    DUMP_VAR( v_sc_uint32_max );
    DUMP_VAR( v_sc_biguint_32_max );

    sc_bigint<64> int31_uint         = (v_sc_int_31_min    & v_uint_max);
    // the following expression yields 0b1....10...0 on 32-bit machines
    sc_bigint<64> bigint31_uint      = (v_sc_bigint_31_min & v_uint_max);
    sc_bigint<64> bigint31_uint32    = (v_sc_bigint_31_min & v_sc_uint32_max);
    sc_bigint<64> bigint31_biguint32 = (v_sc_bigint_31_min & v_sc_biguint_32_max);

    DUMP_VAR( v_sc_bigint_31_min & v_uint_max );
    DUMP_VAR( v_sc_bigint_31_min & v_sc_biguint_32_max );

    DUMP_VAR(int31_uint);
    DUMP_VAR(bigint31_uint); // <--
    DUMP_VAR(bigint31_uint32);
    DUMP_VAR(bigint31_biguint32);

    return 0;
}

