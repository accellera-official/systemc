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

  main.cpp --

  Original Author: Frederic Doucet, Qualcomm, Inc., 2021-01-26

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test conversion of fixed point objects to uint64

#define SC_INCLUDE_FX
#include <systemc>

using namespace std;
using namespace sc_dt;

template<typename T>
void f ( const T& v) {
    cout
        << v.to_string()
        << "\n\t  " << v.to_string(SC_HEX)
        << "\n\t  " << v.to_string(SC_BIN)
        << "\n\t  " << v.to_int64() << endl;
}

int
sc_main( int, char*[] )
{
    {
        cout << "----------------------------------------------------" << endl;
        sc_fix v1(INT64_MIN, 64, 64);
        f(v1);
        sc_assert(v1.to_int64()==INT64_MIN);
    }
    {
        cout << "----------------------------------------------------" << endl;
        sc_fix v1(INT64_MAX, 64, 64);
        f(v1);
        sc_assert(v1.to_int64()==INT64_MAX);
    }
    {
        cout << "----------------------------------------------------" << endl;
        sc_ufix v1(INT64_MIN, 64, 64);
        f(v1);
        sc_assert(v1.to_int64()==INT64_MIN);
    }
    {
        cout << "----------------------------------------------------" << endl;
        sc_ufix v1(INT64_MAX, 64, 64);
        f(v1);
        sc_assert(v1.to_int64()==INT64_MAX);
    }
    {   // 32 bits
        cout << "----------------------------------------------------" << endl;
        f (sc_fix (INT64_MIN));
        sc_assert(sc_fix (INT64_MIN).to_int64()==0);
    }
    {   // 32 bits
        cout << "----------------------------------------------------" << endl;
        f (sc_fix (INT64_MAX));
        sc_assert(sc_fix (INT64_MAX).to_int64()==-1);
    }
    {
        sc_fixed<64,64> v(0);
        f(v);
        cout << "----------------------------------------------------" << endl;
        v[33] = 1;
        f(v);
        cout << "----------------------------------------------------" << endl;
        v = 0;
        v[48] = 1;
        f(v);
        cout << "----------------------------------------------------" << endl;
        v[0] = 1;
        f(v);
    }
    {
        cout << "----------------------------------------------------" << endl;
        sc_fixed<64,64> v( 8589934592 );
        f(v);
    }
    {
        cout << "----------------------------------------------------" << endl;
        cout << "----------------------------------------------------" << endl;
        sc_fixed<64,64> v( 8589934593 );
        f(v);
        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
        v[0] = 0;
        f(v);
    }
    {
        cout << "----------------------------------------------------" << endl;
        cout << "----------------------------------------------------" << endl;
        sc_fixed<96,64> v(0);
        f(v);
        cout << "----------------------------------------------------" << endl;
        v = 0;
        v[64] = 1;
        f(v);
        cout << "----------------------------------------------------" << endl;
        v[31] = 1;
        f(v);
    }
    {
        cout << "----------------------------------------------------" << endl;
        cout << "----------------------------------------------------" << endl;
        sc_fixed<128,64> v(0);
        f(v);
        cout << "----------------------------------------------------" << endl;
        v = 0;
        v[96] = 1;
        f(v);
        cout << "----------------------------------------------------" << endl;
        v[63] = 1;
        sc_assert(v.to_uint64()==4294967296);
        f(v);
    }
    cout << "test passed!" << endl;
    return 0;
}
