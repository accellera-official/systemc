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
    cout << "--- sc_bigint operator=(double) ---" << endl;

    sc_bigint<6> x1;
    x1 = 12.0;
    cout << "sc_bigint<6>(12.0).to_int() = " << x1.to_int() << endl;

    sc_bigint<6> x2;
    x2 = -12.7;
    cout << "sc_bigint<6>(-12.7).to_int() = " << x2.to_int() << endl;

    sc_bigint<6> x3;
    x3 = 0.0;
    cout << "sc_bigint<6>(0.0).to_int() = " << x3.to_int() << endl;

    sc_bigint<40> x4;
    x4 = 1099511627776.0; // 2^40
    cout << "sc_bigint<40>(2^40).to_int64() = " << x4.to_int64() << endl;

    cout << endl << "--- sc_biguint operator=(double) ---" << endl;

    sc_biguint<6> u1;
    u1 = 44.9;
    cout << "sc_biguint<6>(44.9).to_uint() = " << u1.to_uint() << endl;

    sc_biguint<6> u2;
    u2 = 0.0;
    cout << "sc_biguint<6>(0.0).to_uint() = " << u2.to_uint() << endl;

    cout << endl << "--- sc_signed operator=(double) ---" << endl;

    sc_signed s1(6);
    s1 = 12.0;
    cout << "sc_signed(6)(12.0).to_int() = " << s1.to_int() << endl;

    sc_signed s2(6);
    s2 = -12.7;
    cout << "sc_signed(6)(-12.7).to_int() = " << s2.to_int() << endl;

    cout << endl << "--- sc_unsigned operator=(double) ---" << endl;

    sc_unsigned us1(6);
    us1 = 44.9;
    cout << "sc_unsigned(6)(44.9).to_uint() = " << us1.to_uint() << endl;

    return 0;
}
