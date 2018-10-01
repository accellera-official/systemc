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

// test for sc_big(u)int concatenations

#include "systemc.h"

#define WRITE(a) \
    cout << a << endl

int
sc_main( int, char*[] )
{
    cout << hex;

    sc_bigint<128> i128 = 0;
    sc_biguint<128> u128 = 0;

    (i128,u128) = 0xAABB;

    WRITE( i128 );
    WRITE( u128 );
    WRITE( (i128,u128) );

    i128(31,0) =  0xAAAAAAAA;
    i128(63,32) = 0xBBBBBBBB;
    i128(95,64) = 0xCCCCCCCC;
    i128(127,96)= 0xDDDDDDDD;

    u128 = (i128(63,0),i128(127,64));
    WRITE( i128 );
    WRITE( u128 );

    (i128,u128) = 0;
    (i128(71,64),i128(7,0),u128(71,64),u128(7,0)) = 0xAABBCCDD;

    WRITE( (i128,u128) );

    (i128,u128) = 0;

    for (size_t i = 0; i < 128; i+=4) {
        ( i128[i] , i128[i+1], i128[i+3] , i128[i+2] ) = 0xD; // 0b1101
        ( u128[i] , u128[i+1], u128[i+3] , u128[i+2] ) = 0xD; // 0b1101
    }

    WRITE(i128);
    WRITE(u128);

    (i128,u128) = 0;

    for (size_t i = 0; i < 128; i+=4) {
        ( i128(i+2,i), i128(i+3,i+3) ) = 0xB; // 0b1011
        ( u128(i+2,i), u128(i+3,i+3) ) = 0xB; // 0b1011
    }

    WRITE(i128);
    WRITE(u128);


    (i128,u128) = 0;
    (i128[127],i128[0],u128[127],u128[0]) = 0xF;

    WRITE( (i128,u128) );

    u128 = 0;
    i128 = 0;

    ( i128, u128[3], u128[2], u128(7,7), u128(5,4) ) = 0x61E; // 0b11000011110

    WRITE(u128);
    WRITE(i128);

    sc_int<2> i2;
    sc_uint<2> u2;

    u128 = 0;
    i128 = 0;
    ( i2, u2, u128(1,0), i128[1], i128[0] ) = 0xAB;
    WRITE(( u128(1,0), i128[1], i128[0], i2, u2 ));

    return 0;
}
