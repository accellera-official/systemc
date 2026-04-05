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
    cout << "--- sc_biguint subref operator=(double) ---" << endl;

    sc_biguint<8> u1;
    u1 = 255; // 11111111
    u1(3, 0) = 5.0;
    cout << "sc_biguint<8>(255)(3,0)=5.0: to_uint() = " << u1.to_uint() << endl;

    sc_biguint<8> u2;
    u2 = 0;
    u2(7, 4) = 10.0;
    cout << "sc_biguint<8>(0)(7,4)=10.0: to_uint() = " << u2.to_uint() << endl;

    sc_biguint<6> u3;
    u3 = 0;
    u3(5, 0) = 44.9;
    cout << "sc_biguint<6>(0)(5,0)=44.9: to_uint() = " << u3.to_uint() << endl;

    return 0;
}
