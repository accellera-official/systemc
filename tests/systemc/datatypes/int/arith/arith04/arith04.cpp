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

  arith04.cpp --

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <cstdlib>
#include <cstdint>
#include "systemc.h"
#include "isaac.h"

QTIsaac<8> rng;		// Platform independent random number generator.

// sign_bit = number of sign bit from 1 to 32

int64_t sign_extend(int64_t target, int64_t sign_bit)
{
    int64_t result;
    int64_t bit_mask = (((int64_t) 1) << (sign_bit - 1));
    if ( target & bit_mask ) {
        result = target | (UINT64_MAX << (sign_bit - 1));
    }
    else {
        result = target & ~(UINT64_MAX << (sign_bit - 1));
    }
    return result;
}


int
sc_main( int argc, char* argv[] )
{
    int64_t vali[5] = { 0, 1, -1, 7, -8 };
    int64_t valj[5] = { 0, 1, -1, 7, -8 };

    for (int i = 3; i < 32; ++i) {
        for (int j = 3; j < 32; ++j) {
            cout << "i = " << i << ", j = " << j << endl;

            sc_signed x(i);
            sc_signed y(j);
            sc_signed z(64);

            vali[3] = (1 << (i - 1)) - 1;
            vali[4] = - (1 << (i - 1));

            valj[3] = (1 << (j - 1)) - 1;
            valj[4] = - (1 << (j - 1));

            for (int ii = 0; ii < 100; ++ii) {
                for (int jj = 0; jj < 100; ++jj) {
                    int64_t qi = (ii < 5) ? vali[ii] : (rng.rand() & ((1u << i) - 1));
                    int64_t qj = (jj < 5) ? valj[jj] : (rng.rand() & ((1u << j) - 1));
                    int64_t tqi;

          		    qi = sign_extend(qi,i);
          		    qj = sign_extend(qj,j);

                    x = qi;
                    tqi = qi;
                    sc_assert( x == qi );
                    y = qj;
                    sc_assert( y == qj );
                    sc_assert((x == qj) == (qi == qj));
                    sc_assert((x == qj) == (qj == x));
                    sc_assert((x != qj) == (qi != qj));
                    sc_assert((x != qj) == (qj != x));
                    sc_assert((x < qj) == (qi < qj));
                    sc_assert((x < qj) == (qj > x));
                    sc_assert((x <= qj) == (qi <= qj));
                    sc_assert((x <= qj) == (qj >= x));
                    sc_assert((x > qj) == (qi > qj));
                    sc_assert((x > qj) == (qj < x));
                    sc_assert((x >= qj) == (qi >= qj));
                    sc_assert((x >= qj) == (qj <= x));

                    x += y;
                    tqi += qj;
                    tqi = sign_extend(tqi,i);
                    sc_assert( x == tqi );

                    x = qi;
                    tqi = qi;

                    x -= y;
                    tqi -= qj;
                    tqi = sign_extend(tqi,i);
                    sc_assert( x == tqi );

                    x = qi;
                    tqi = qi;
                    x *= y;

                    // Cast to unsigned so that overflow wraps instead of being UB.
                    tqi = static_cast<uint64_t>(tqi) * static_cast<uint64_t>(qj);
                    tqi = sign_extend(tqi, i);
                    sc_assert(x == tqi);

                    if (y != 0) {
                        x = qi;
                        tqi = qi;
                        x /= y;
                        tqi /= qj;
                        tqi = sign_extend(tqi,i);
                        sc_assert( x == tqi );

                        x = qi;
                        tqi = qi;
                        x %= y;
                        tqi %= qj;
                        tqi = sign_extend(tqi,i);
                        sc_assert( x == tqi );
                    }

                    x = qi;
                    tqi = qi;
                    x &= y;
                    tqi &= qj;
                    tqi = sign_extend(tqi,i);
                    sc_assert( x == tqi );

                    x = qi;
                    tqi = qi;
                    x |= y;
                    tqi |= qj;
                    tqi = sign_extend(tqi,i);
                    sc_assert( x == tqi );

                    x = qi;
                    tqi = qi;
                    x ^= y;
                    tqi ^= qj;
                    tqi = sign_extend(tqi,i);
                    sc_assert( x == tqi );

                    if (jj < i - 1) {
                        x = qi;
                        tqi = qi;
                        x <<= jj;
                        tqi = static_cast<uint64_t>(tqi) << jj;
                        tqi = sign_extend(tqi,i);
                        sc_assert( x == tqi );

                        x = qi;
                        tqi = qi;
                        x >>= jj;
                        tqi >>= jj;
                        tqi = sign_extend(tqi,i);
                        sc_assert( x == tqi );
                    }
                }
            }
        }
    }
    return 0;
}
