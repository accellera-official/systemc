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

  arith01.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "systemc.h"

/*

 int9 x;

 x.q = -256;

 x.q is equal to 256 in Sun's CC, and -256 in gcc. The standard leaves
 this issue as implementation dependent unless the definition is
 qualified explicitly by signed or unsigned.  

 In order to force consistent behavior, I've qualified every int
 definition accordingly in all the arith files.  

*/

typedef struct int9 {
    signed int q : 9;
} int9;

typedef struct int31 {
    signed int q : 31;
} int31;

void
crunch(sc_signed& z, int31 v31, int u, int v)
{
    for (int i = 0; i < 100000; ++i) {
        z *= u;
        z += v;
        v31.q *= u;
        v31.q += v;
        assert(z == v31.q);
    }
}

int
sc_main( int argc, char* argv[] )
{
    sc_signed x(31);
    sc_signed y(9);
    sc_signed z(31);
    int9 v;

    y = -256;
    v.q = -256;
    for (int i = 0; i < 1000; ++i) {
        cout << y << '\t' << v.q << endl;
        y++;
        v.q++;
        assert(y == v.q);
    }
    for (int i = 0; i < 1000; ++i) {
        cout << y << '\t' << v.q << endl;
        y--;
        v.q--;
        assert(y == v.q);
    }
    for (int i = 0; i < 1000; ++i) {
        cout << ++y << '\t' << ++v.q << endl;
        assert(y == v.q);
    }
    for (int i = 0; i < 1000; ++i) {
        cout << --y << '\t' << --v.q << endl;
        assert(y == v.q);
    }

    z = 129023;
    int31 v31;
    v31.q = 129023;
    crunch(z, v31, 491, 12089);

    x = -129023;
    v31.q = -129023;
    crunch(x, v31, 109, -426);

    x = -1;
    v31.q = -1;
    crunch(x, v31, 30941, -1188);
    
    return 0;
}
