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

  test02.cpp -- Test bit ops.

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

#define BIT_OPS(LEFT,RIGHT) { \
    cout << endl << "BIT operations:" << endl; \
    cout << "    operands: " << endl; \
    cout << "        " << #LEFT << " = " << LEFT << endl; \
    cout << "        " << #RIGHT << " = " << RIGHT << endl; \
    cout << "        "; OP(LEFT & RIGHT) \
    cout << "        "; OP(RIGHT & LEFT) \
    cout << "        "; OP(LEFT | RIGHT) \
    cout << "        "; OP(RIGHT | LEFT) \
    cout << "        "; OP(LEFT ^ RIGHT) \
    cout << "        "; OP(RIGHT ^ LEFT) \
}


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

    BIT_OPS(bv4,bv8);
    BIT_OPS(lv5,lv10);
    BIT_OPS(bv4,lv10);
    BIT_OPS(lv5,bv8);


    bv4 = 3;
    bv8 = 0x42;
    lv5 = "1x1x1";
    lv10 = "0000000000";

    BIT_OPS(bv4,bv8);

    BIT_OPS(lv5,lv10);
    BIT_OPS(bv4,lv10);
    BIT_OPS(lv5,bv8);

    lv10 = "xxxxxxxxxx";

    BIT_OPS(lv5,lv10);
    BIT_OPS(bv4,lv10);
    BIT_OPS(lv5,bv8);
    BIT_OPS(lv10,lv10);

    lv10 = "zzzzzzzzzz";

    BIT_OPS(lv5,lv10);
    BIT_OPS(bv4,lv10);
    BIT_OPS(lv5,bv8);
    BIT_OPS(lv10,lv10);

    return 0;
}
