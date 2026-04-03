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
    cout << "--- sc_bigint get_packed_rep (writes digit[]) ---" << endl;

    sc_bigint<6> x1;
    x1 = -1;
    sc_digit buf1[1] = { 0x0C }; // 001100 = 12
    x1.get_packed_rep(buf1);
    cout << "sc_bigint<6>(-1) get_packed_rep(0x0C): to_int() = " << x1.to_int() << endl;

    cout << endl << "--- sc_biguint set_packed_rep ---" << endl;

    sc_biguint<6> u1;
    u1 = 63;
    sc_digit buf2[1] = { 0x05 }; // 000101 = 5
    u1.set_packed_rep(buf2);
    cout << "sc_biguint<6>(63) set_packed_rep(0x05): to_uint() = " << u1.to_uint() << endl;

    sc_biguint<6> u2;
    u2 = 0;
    sc_digit buf3[1] = { 0x3F }; // 111111 = 63
    u2.set_packed_rep(buf3);
    cout << "sc_biguint<6>(0) set_packed_rep(0x3F): to_uint() = " << u2.to_uint() << endl;

    return 0;
}
