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

    sc_bigint<6> x2;
    x2 = 0;
    sc_digit buf2[1] = { 0x2C }; // 101100 = -20 in 6-bit signed
    x2.get_packed_rep(buf2);
    cout << "sc_bigint<6>(0) get_packed_rep(0x2C): to_int() = " << x2.to_int() << endl;

    cout << endl << "--- sc_signed get_packed_rep ---" << endl;

    sc_signed s1(6);
    s1 = 0;
    sc_digit buf3[1] = { 0x2C };
    s1.get_packed_rep(buf3);
    cout << "sc_signed(6)(0) get_packed_rep(0x2C): to_int() = " << s1.to_int() << endl;

    return 0;
}
