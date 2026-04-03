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

int sc_main(int, char*[]) {
    cout << "--- sc_bigint set/clear via bitref ---" << endl;

    // clear sign bit
    sc_bigint<6> x1;
    x1 = 0x2c; // -20
    x1[5] = 0;
    cout << "sc_bigint<6>(0x2c) clear bit 5: to_int() = " << x1.to_int() << endl;

    // set bit on positive value
    sc_bigint<6> x2;
    x2 = 12; // 001100
    x2[5] = 1;
    cout << "sc_bigint<6>(12) set bit 5: to_int() = " << x2.to_int() << endl;

    // clear bit 0
    sc_bigint<8> x3;
    x3 = 0x55; // 01010101 = 85
    x3[0] = 0;
    cout << "sc_bigint<8>(0x55) clear bit 0: to_int() = " << x3.to_int() << endl;

    // set bit 0
    sc_bigint<8> x4;
    x4 = 0x54; // 01010100 = 84
    x4[0] = 1;
    cout << "sc_bigint<8>(0x54) set bit 0: to_int() = " << x4.to_int() << endl;

    cout << endl << "--- sc_biguint set/clear via bitref ---" << endl;

    // set high bit
    sc_biguint<6> u1;
    u1 = 12; // 001100
    u1[5] = 1;
    cout << "sc_biguint<6>(12) set bit 5: to_uint() = " << u1.to_uint() << endl;

    // clear high bit
    sc_biguint<6> u2;
    u2 = 44; // 101100
    u2[5] = 0;
    cout << "sc_biguint<6>(44) clear bit 5: to_uint() = " << u2.to_uint() << endl;

    // clear bit 0
    sc_biguint<8> u3;
    u3 = 0x55; // 01010101 = 85
    u3[0] = 0;
    cout << "sc_biguint<8>(0x55) clear bit 0: to_uint() = " << u3.to_uint() << endl;

    cout << endl << "--- sc_signed set/clear via bitref ---" << endl;

    sc_signed s1(6);
    s1 = 0x2c; // -20
    s1[5] = 0;
    cout << "sc_signed(6)(0x2c) clear bit 5: to_int() = " << s1.to_int() << endl;

    sc_signed s2(6);
    s2 = 12;
    s2[5] = 1;
    cout << "sc_signed(6)(12) set bit 5: to_int() = " << s2.to_int() << endl;

    cout << endl << "--- sc_unsigned set/clear via bitref ---" << endl;

    sc_unsigned us1(6);
    us1 = 12;
    us1[5] = 1;
    cout << "sc_unsigned(6)(12) set bit 5: to_uint() = " << us1.to_uint() << endl;

    sc_unsigned us2(6);
    us2 = 44;
    us2[5] = 0;
    cout << "sc_unsigned(6)(44) clear bit 5: to_uint() = " << us2.to_uint() << endl;

    return 0;
}
