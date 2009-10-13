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

  arith05.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <stdlib.h>
#include "systemc.h"
#include "isaac.h"

QTIsaac<8> rng;		// Platform independent random number generator.

void
check_string( const sc_signed& z, int v )
{
    std::string buf( z.to_string( SC_BIN ) );
    if (z < 0) {
        assert(buf[2] == '1');
    } else {
        assert(buf[2] == '0');
    }
}

int
sc_main( int argc, char* argv[] )
{
    unsigned vali[5] = { 0, 1, (unsigned)-1, 7, (unsigned)-8 };
    unsigned valj[5] = { 0, 1, (unsigned)-1, 7, (unsigned)-8 };

    for (unsigned i = 3; i < 32; ++i) {
        for (unsigned j = 3; j < 32; ++j) {
            cout << "i = " << i << ", j = " << j << endl;

            sc_unsigned x(i);
            sc_unsigned y(j);
            sc_unsigned z(64);

            vali[3] = (1 << (i - 1)) - 1;
            vali[4] = - (1 << (i - 1));

            valj[3] = (1 << (j - 1)) - 1;
            valj[4] = - (1 << (j - 1));

            for (unsigned ii = 0; ii < 100; ++ii) {
                for (unsigned jj = 0; jj < 100; ++jj) {
                    unsigned qi = (ii < 5) ? vali[ii] : (rng.rand() & ((1 << i) - 1));
                    unsigned qj = (jj < 5) ? valj[jj] : (rng.rand() & ((1 << j) - 1));
                    unsigned tqi;

                    if (qi & (1 << (i - 1))) {
                        qi = (qi << (32 - i)) >> (32 - i);
                    }
                    if (qj & (1 << (j - 1))) {
                        qj = (qj << (32 - j)) >> (32 - j);
                    }

                    x = qi;
                    tqi = qi;
                    assert( x == qi );
                    y = qj;
                    assert( y == qj );
                    assert((x == qj) == (qi == qj));
                    assert((x == qj) == (qj == x));
                    assert((x != qj) == (qi != qj));
                    assert((x != qj) == (qj != x));
                    assert((x < qj) == (qi < qj));
                    assert((x < qj) == (qj > x));
                    assert((x <= qj) == (qi <= qj));
                    assert((x <= qj) == (qj >= x));
                    assert((x > qj) == (qi > qj));
                    assert((x > qj) == (qj < x));
                    assert((x >= qj) == (qi >= qj));
                    assert((x >= qj) == (qj <= x));

                    x += y;
                    tqi += qj;
                    tqi = (tqi << (32 - i)) >> (32 - i);
                    assert( x == tqi );

                    x = qi;
                    tqi = qi;
                    x -= y;
                    tqi -= qj;
                    tqi = (tqi << (32 - i)) >> (32 - i);
                    assert( x == tqi );

                    x = qi;
                    tqi = qi;
                    x *= y;
                    tqi *= qj;
                    tqi = (tqi << (32 - i)) >> (32 - i);
                    assert( x == tqi );

                    if (y != 0) {
                        x = qi;
                        tqi = qi;
                        x /= y;
                        tqi /= qj;
                        tqi = (tqi << (32 - i)) >> (32 - i);
                        assert( x == tqi );

                        x = qi;
                        tqi = qi;
                        x %= y;
                        tqi %= qj;
                        tqi = (tqi << (32 - i)) >> (32 - i);
                        assert( x == tqi );
                    }

                    x = qi;
                    tqi = qi;
                    x &= y;
                    tqi &= qj;
                    tqi = (tqi << (32 - i)) >> (32 - i);
                    assert( x == tqi );

                    x = qi;
                    tqi = qi;
                    x |= y;
                    tqi |= qj;
                    tqi = (tqi << (32 - i)) >> (32 - i);
                    assert( x == tqi );

                    x = qi;
                    tqi = qi;
                    x ^= y;
                    tqi ^= qj;
                    tqi = (tqi << (32 - i)) >> (32 - i);
                    assert( x == tqi );

                    if (jj < i - 1) {
                        x = qi;
                        tqi = qi;
                        x <<= jj;
                        tqi <<= jj;
                        tqi = (tqi << (32 - i)) >> (32 - i);
                        assert( x == tqi );

                        x = qi;
                        tqi = qi;
                        x >>= jj;
                        tqi >>= jj;
                        tqi = (tqi << (32 - i)) >> (32 - i);
                        assert( x == tqi );
                    }
                }
            }
        }
    }
    return 0;
}
