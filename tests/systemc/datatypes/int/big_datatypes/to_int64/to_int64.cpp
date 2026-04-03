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
#include "systemc.h"
using namespace sc_dt;
using std::cout;
using std::endl;

// Test to_int64() sign extension for sc_bigint<W> and sc_signed.
// Verifies fix for missing sign extension when 33 <= W < 64.
// Also tests to_uint64() unsigned delegation fix.

template<int W>
void test_to_int64(int64 val)
{
    sc_bigint<W> x;
    x = val;
    cout << "sc_bigint<" << W << ">(" << val << ").to_int64() = "
         << x.to_int64() << endl;
}

template<int W>
void test_to_uint64(uint64 val)
{
    sc_bigint<W> x;
    x = (int64)val;
    cout << "sc_bigint<" << W << ">(" << val << ").to_uint64() = "
         << x.to_uint64() << endl;
}

void test_sc_signed_to_int64(int w, int64 val)
{
    sc_signed x(w);
    x = val;
    cout << "sc_signed(" << w << ")(" << val << ").to_int64() = "
         << x.to_int64() << endl;
}

int sc_main(int, char*[])
{
    cout << "--- to_int64 sign extension ---" << endl;

    // -1: all bits set, widths spanning every code path
    test_to_int64<7>(-1);
    test_to_int64<16>(-1);
    test_to_int64<32>(-1);
    test_to_int64<33>(-1);    // first width using 2-digit assembly
    test_to_int64<40>(-1);
    test_to_int64<48>(-1);
    test_to_int64<54>(-1);    // the original bug reproducer
    test_to_int64<63>(-1);
    test_to_int64<64>(-1);
    test_to_int64<100>(-1);

    // -1048577: mixed bit pattern
    test_to_int64<32>(-1048577);
    test_to_int64<33>(-1048577);
    test_to_int64<54>(-1048577);
    test_to_int64<64>(-1048577);
    test_to_int64<100>(-1048577);

    // positive values (should be unchanged)
    test_to_int64<32>(42);
    test_to_int64<54>(42);
    test_to_int64<64>(42);

    // zero
    test_to_int64<32>(0);
    test_to_int64<54>(0);
    test_to_int64<64>(0);

    cout << endl << "--- to_uint64 ---" << endl;

    test_to_uint64<16>(12345);
    test_to_uint64<32>(12345);
    test_to_uint64<54>(12345);
    test_to_uint64<64>(12345);
    test_to_uint64<16>(0);
    test_to_uint64<64>(0);

    cout << endl << "--- sc_signed to_int64 ---" << endl;

    int widths[] = { 7, 16, 32, 33, 40, 48, 54, 63, 64, 100 };
    for ( int i = 0; i < 10; i++ ) {
        test_sc_signed_to_int64(widths[i], -1);
    }
    test_sc_signed_to_int64(54, -1048577);
    test_sc_signed_to_int64(64, -1048577);
    test_sc_signed_to_int64(54, 42);

    return 0;
}
