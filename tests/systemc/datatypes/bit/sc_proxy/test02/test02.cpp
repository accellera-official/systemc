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

  test02.cpp -- 

  Original Author: Andy Goodrich, Synopsys, Inc., 2021-12-12

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
#include "systemc.h"

#define TO(WHAT,FUNC) { cout << #WHAT << "." << #FUNC << " = " << dec << (WHAT.FUNC) << endl; }
int sc_main(int argc, char* argv[])
{
    sc_lv<64> w;
    sc_bv<3>  x = "100";
    sc_lv<32> y = 0x80000004;
    sc_bv<68> z;

    w = "0x29000000080000004";
    cout << endl << "sc_lv<64>w = " << hex << w << endl;
    TO(w,to_int())
    TO(w,to_long())
    TO(w,to_int64())
    TO(w,to_uint())
    TO(w,to_ulong())
    TO(w,to_uint64())

    w = "0x24000003080000004";
    cout << endl << "sc_lv<64>w = " << hex << w  << endl;
    TO(w,to_int())
    TO(w,to_long())
    TO(w,to_int64())
    TO(w,to_uint())
    TO(w,to_ulong())
    TO(w,to_uint64())

    x = "100";
    cout << endl << "sc_bv<3> x = 100" << endl;
    TO(x,to_int())
    TO(x,to_long())
    TO(x,to_int64())
    TO(x,to_uint())
    TO(x,to_ulong())
    TO(x,to_uint64())

    y = 0x80000004;
    cout << endl << "sc_bv<32>y = 0x80000004" << endl;
    TO(y,to_int())
    TO(y,to_long())
    TO(y,to_int64())
    TO(y,to_uint())
    TO(y,to_ulong())
    TO(y,to_uint64())

    y = 0x40000004;
    cout << endl << "sc_bv<32>y = 0x40000004" << endl;
    TO(y,to_int())
    TO(y,to_long())
    TO(y,to_int64())
    TO(y,to_uint())
    TO(y,to_ulong())
    TO(y,to_uint64())

    z = "0x29000000080000004";
    cout << endl << "sc_bv<68>z = " << hex << z << endl;
    TO(z,to_int())
    TO(z,to_long())
    TO(z,to_int64())
    TO(z,to_uint())
    TO(z,to_ulong())
    TO(z,to_uint64())

    z = "0x24000000080000004";
    cout << endl << "sc_bv<68>z = " << hex << z  << endl;
    TO(z,to_int())
    TO(z,to_long())
    TO(z,to_int64())
    TO(z,to_uint())
    TO(z,to_ulong())
    TO(z,to_uint64())

    cout << endl << "Program completed" << endl;
    return 0;
}
