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

  arith08.cpp --

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"
#include "isaac.h"

QTIsaac<8> rng;		// Platform independent random number generator.

#define TEST(A,B) \
{ \
    if ( (A) != (B) ) \
        cout << #A << " (" << (A) << ") != " << #B << "(" << (B) << ")" << endl; \
}

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
sc_main(int, char**)
{
    uint64_t vali[5] = { 0, 1, UINT64_MAX, 7, UINT64_MAX - 7 };
    int64_t valj[5] = { 0, 1, -1, 7, -8 };

    for (int i = 3; i < 30; ++i) {
        for (int j = 3; j < 30; ++j) {
            cout << "i = " << i << ", j = " << j << endl;
            sc_unsigned x(i);
            sc_signed y(j);
            sc_signed z(64);

            vali[3] = (1 << (i - 1)) - 1;
            vali[4] = - (1 << (i - 1));

            valj[3] = (1 << (j - 1)) - 1;
            valj[4] = - (1 << (j - 1));

            for (int ii = 0; ii < 100; ++ii) {
                for (int jj = 0; jj < 100; ++jj) {
                    uint64_t qi = (ii < 5) ? vali[ii] : (rng.rand() & ((1 << i) - 1));
                    int64_t qj = (jj < 5) ? valj[jj] : (rng.rand() & ((1 << j) - 1));

          		    qi = (qi << (64 - i)) >> (64 - i);
          		    qj = sign_extend(qj, j);

                    x = qi;
                    y = qj;
                    sc_assert(x == (int64_t) qi);
                    sc_assert(y == qj);

                    sc_signed ty(x);
                    TEST(x,ty );
                    sc_assert( ty >= 0 );
                    sc_assert((! x[i-1]) || (ty.length() == i+1) );

                    z = x + y;
                    // The result on the RHS of the comparison will be promoted
                    // to an unsigned type, so we cast it for comparison
                    // purposes.
                    TEST(z, static_cast<int64_t>(qi + qj));
                    z = x - y;
                    TEST(z, static_cast<int64_t>(qi - qj));
                    z = x * y;

                    // Cast the operands to be sure to avoid to signed overflow.
                    TEST(z, static_cast<int64_t>(qi * static_cast<uint64_t>(qj)));
                    sc_unsigned xx(i);
                    xx = x;
                    xx *= y;
                    sc_unsigned xx2(i);
                    xx2 = z.range(i - 1, 0);
                    TEST(xx,xx2);

                    if (y != 0) {
                        z = x / y;
                        // A cast here makes sures that our division signed.
                        TEST(z, static_cast<int64_t>(qi) / qj);
                        z = x % y;
                        TEST(z, static_cast<int64_t>(qi) % qj);
                    }
                    z = x & y;
                    TEST(z, static_cast<int64_t>(qi & qj));
                    z = x | y;
                    TEST(z, static_cast<int64_t>(qi | qj));
                    z = x ^ y;
                    TEST(z, static_cast<int64_t>(qi ^ qj));
                }
            }
        }
    }
    return 0;
}
