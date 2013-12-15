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

  mean.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename mean.cc */
/* This is the implementation file for synchronous process `mean' */

#include "mean.h"

void calculate_geometric_mean(const sc_signed& a, const sc_signed& b, 
			      const sc_signed& c, const sc_signed& d,
			      sc_signed& mean){}

void calculate_harmonic_mean(const sc_signed& a, const sc_signed& b, 
			      const sc_signed& c, const sc_signed& d,
			      sc_signed& mean){}

void mean::entry()
{
  sc_signed i(24);  // Will hold input a
  sc_signed j(24);  // Will hold input b
  sc_signed k(24);  // Will hold input c
  sc_signed l(24);  // Will hold input d

  sc_signed arith(24);    // Arithmetic mean
  sc_signed geom(24);     // Geometric mean
  sc_signed harmonic(24); // Harmonic mean

  while (true) {
    // read all inputs
    i = a.read().to_int();
    j = b.read().to_int();
    k = c.read().to_int();
    l = d.read().to_int();
    
    // Calculate arithmetic mean
    arith = (i + j + k + l) / 4;
    // Calculate geometric mean
    calculate_geometric_mean(i, j, k, l, geom);
    // Calculate harmonic mean
    calculate_harmonic_mean(i, j, k, l, harmonic);

    // write all outputs
    am.write(arith);
    gm.write(geom);
    hm.write(harmonic);

    wait();
  }
} // end of entry function

// Just to make the compiler shut up

int sc_main(int ac, char *av[])
{
  return 0;
}
