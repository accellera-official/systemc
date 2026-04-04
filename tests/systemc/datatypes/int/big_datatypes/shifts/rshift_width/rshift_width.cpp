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

// Test that operator>> preserves the result width (IEEE 1666-2023 §7.2.8).

#include "systemc.h"

using std::cout;
using std::endl;

int sc_main(int, char*[])
{
    // --- sc_bigint (signed) ---

    sc_bigint<54> si54 = -1048577;
    sc_signed     r_si54 = si54 >> 32;
    cout << "sc_bigint<54>(-1048577) >> 32: value=" << r_si54.to_int64()
         << " width=" << r_si54.length() << endl;

    sc_bigint<128> si128 = -1;
    sc_signed      r_si128 = si128 >> 64;
    cout << "sc_bigint<128>(-1) >> 64: value=" << r_si128.to_int64()
         << " width=" << r_si128.length() << endl;

    sc_bigint<54> si54z = 0x3FFFFF00000LL;
    sc_signed     r_si54z = si54z >> 48;
    cout << "sc_bigint<54>(pos) >> 48: value=" << r_si54z.to_int64()
         << " width=" << r_si54z.length() << endl;

    // shift >= width
    sc_signed r_si54_full = si54 >> 100;
    cout << "sc_bigint<54>(-1048577) >> 100: value=" << r_si54_full.to_int64()
         << " width=" << r_si54_full.length() << endl;

    // --- sc_signed ---

    sc_signed ss(54);
    ss = -1048577;
    sc_signed r_ss = ss >> 32;
    cout << "sc_signed(54)(-1048577) >> 32: value=" << r_ss.to_int64()
         << " width=" << r_ss.length() << endl;

    // --- sc_biguint (unsigned) ---

    sc_biguint<54> ui54 = 0x3FFFFFFFFFLL;
    sc_unsigned    r_ui54 = ui54 >> 32;
    cout << "sc_biguint<54>(0x3FFFFFFFFF) >> 32: value=" << r_ui54.to_int64()
         << " width=" << r_ui54.length() << endl;

    // shift >= width
    sc_unsigned r_ui54_full = ui54 >> 100;
    cout << "sc_biguint<54>(0x3FFFFFFFFF) >> 100: value=" << r_ui54_full.to_int64()
         << " width=" << r_ui54_full.length() << endl;

    // --- sc_unsigned ---

    sc_unsigned su(54);
    su = 0x3FFFFFFFFFLL;
    sc_unsigned r_su = su >> 32;
    cout << "sc_unsigned(54)(0x3FFFFFFFFF) >> 32: value=" << r_su.to_int64()
         << " width=" << r_su.length() << endl;

    cout << "PASSED" << endl;
    return 0;
}
