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
Dec/19/00 ulrich

Assignment to a bit-slice of an sc_bv, even with an sc_bv with proper size, does not
compile on Solaris SC5.0. It works for gcc, though.

It works for sc_int and sc_bigint on both compilers.


I am using SystemC 1.0.1 (someone **PLEASE** add 1.0.1 to the Stellar list of release!)

Example:
*/

#include <systemc.h>

int sc_main(int argc, char* arg[]) 
{
  sc_bv<8> bv8 = 3;
  sc_bv<4> bv4 = 3;
  sc_int<8> i8 = 3;
  sc_int<4> i4 = 3;
  sc_bigint<8> bi8=3;
  sc_bigint<4> bi4=3;

  // OK
  bi8.range(5,2) = bi4.range(3,0);
  bi8.range(5,2) = bi4;
  bi8.range(5,2) = 3;
  bi8.range(5,2) = (sc_bigint<4>(3)).range(3,0);

  // OK
  i8.range(5,2) = i4.range(3,0);
  i8.range(5,2) = i4;
  i8.range(5,2) = 3;
  i8.range(5,2) = (sc_int<4>(3)).range(3,0);


  // OK
  bv8.range(5,2) = bv4.range(3,0);

  // OK gcc, error SC5.0
  bv8.range(5,2) = bv4;
  bv8.range(5,2) = (sc_bv<4>(3)).range(3,0);

  return 0;
}
