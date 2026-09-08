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

  7.2.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc.h>
#define VAR_BITS(X) { std::cout << #X << " = " << std::hex << X << std::endl; }
#define VAR_LENGTH(X) { std::cout << #X << ".length() = " << X.length() << std::endl; }

int sc_main( int argc, char* argv[] ) { 

    std::cout << std::endl << "================ 7.2.4 ==================" << std::endl << std::endl;
    sc_dt::sc_length_param                 length10(10);
    sc_dt::sc_length_context               cntx10(length10);
    sc_dt::sc_int_base                     int_array[2]; // array of 10-bit integers
    VAR_LENGTH(int_array[0]);
    sc_core::sc_signal<sc_dt::sc_int_base> S1; // signal of 10-bit integer

    {
	sc_dt::sc_length_param                 length12(12);
	sc_dt::sc_length_context               cntx12(length12,sc_dt::SC_LATER);
	sc_dt::sc_length_param                 length14(14);
	sc_dt::sc_length_context               cntx14(length14,sc_dt::SC_LATER);

        sc_dt::sc_uint_base                    var1;
        VAR_LENGTH(var1);

        cntx12.begin();
        sc_dt::sc_uint_base                    var2;
        VAR_LENGTH(var2);

        cntx14.begin();
        sc_dt::sc_uint_base                    var3;
        VAR_LENGTH(var3);

        cntx14.end();
        sc_dt::sc_bv_base                      var4;
        VAR_LENGTH(var4);
        
    }
    sc_dt::sc_bv_base                      var5;
    VAR_LENGTH(var5);

    std::cout << std::endl << "================ 7.2.4 ==================" << std::endl << std::endl;
    sc_dt::sc_int<4> v1;
    v1[1] = true;
    std::cout << "v1 = ";
    for ( int i = 3; i >=0; --i ) { std::cout << v1[i].to_bool(); }
    std::cout << std::endl;

    std::cout << std::endl << "================ 7.2.6 ==================" << std::endl << std::endl;
    sc_dt::sc_int<8> v2 = 2;
    v2.range(3,2) = v2.range(1,0);
    VAR_BITS(v2);
    sc_dt::sc_int<8> v3 = v2.range(3,0);
    VAR_BITS(v3);
    sc_dt::sc_bv<8> v4 = "0b11110000";
    VAR_BITS(v4);

    std::cout << std::endl << "================ 7.2.6 ==================" << std::endl << std::endl;
    sc_dt::sc_uint<8> u1 = 2;
    sc_dt::sc_uint<2> u2 = 1;
    sc_dt::sc_uint<8> u3 = (true, u1.range(3,0), u2, u2[0]);
    VAR_BITS((true, u1.range(3,0), u2, u2[0]));
    VAR_BITS(u3);

    std::cout << "before (u2[0],u1[0],u1.range(7,1)) = (u1[7],u1)" << std::endl;
    VAR_BITS(u1);
    VAR_BITS(u2);
    (u2[0],u1[0],u1.range(7,1)) = (u1[7],u1);
    std::cout << "after (u2[0],u1[0],u1.range(7,1)) = (u1[7],u1)" << std::endl;
    VAR_BITS(u1);
    VAR_BITS(u2);

    // VAR_BITS(u3);

    u1 = 2;
    u2 = 1;
    std::cout << "before concat(u2[0],concat(u1[0],u1.range(7,1))) = concat(u1[7],u1)" <<std::endl;
    VAR_BITS(u1);
    VAR_BITS(u2);
    concat(u2[0],concat(u1[0],u1.range(7,1))) = concat(u1[7],u1);
    std::cout << "after concat(u2[0],concat(u1[0],u1.range(7,1))) = concat(u1[7],u1)" <<std::endl;
    VAR_BITS(u1);
    VAR_BITS(u2);

    std::cout << std::endl << "program completed" << std::endl;
    return 0;
}
