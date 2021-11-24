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

  test02.cpp -- Test sc_module::set_stack_size

  Original Author: Andy Goodrich, Forte Design Systemc, Inc. 2003-10-13

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <systemc.h>

#define OP(OPERATION) cout << #OPERATION << " = " << ( OPERATION ) << endl;
int sc_main(int argc, char* argv[])
{
    sc_lv<5>  lv5;
    sc_lv<10> lv10;
    sc_bv<4>  bv4;
    sc_bv<8>  bv8;

    bv4 = 3;
    bv8 = 0x40;

    lv5 = "1z0x1";

    lv10 = "101zzzzz";

    OP(bv4)
    OP(bv8)
    OP(bv4 | bv8 )
    OP(bv8 | bv4 )
    cout << endl;
    OP(lv5)
    OP(lv10)
    OP(lv5 | lv10 )
    OP(lv10 | lv5 )

    return 0;
}
