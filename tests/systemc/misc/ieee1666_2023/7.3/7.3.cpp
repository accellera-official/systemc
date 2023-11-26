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

  7.3.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc.h>

#define SHOW(X,FMT) \
    {std::cout << #X << ".to_string(" << #FMT << ") = " << X.to_string(FMT) << std::endl;}
#define SHOW_FALSE(X,FMT) { \
    std::cout << #X << ".to_string(" << #FMT << ",false) = " << X.to_string(FMT,false) << std::endl;}
#define SHOW_TRUE(X,FMT) \
    {std::cout << #X << ".to_string(" << #FMT << ",true) = " << X.to_string(FMT,true) << std::endl;}

int sc_main( int argc, char* argv[] ) { 


    std::cout << std::endl; 
    sc_dt::sc_int<4> v1;
    v1 = "0b10100";
    SHOW_TRUE(v1,sc_dt::SC_BIN);

    std::cout << std::endl; 
    sc_dt::sc_int<10> v2;
    v2 = "0d478";
    SHOW_FALSE(v2,sc_dt::SC_CSD);

    std::cout << std::endl; 
    sc_dt::sc_uint<8> v3;
    v3 = "0x7";
    SHOW(v3,sc_dt::SC_HEX);

    std::cout << std::endl; 
    sc_dt::sc_lv<16> lv;
    lv = "0xff";
    SHOW(lv,sc_dt::SC_HEX);

    std::cout << std::endl; 
    sc_dt::sc_bv<8> bv;
    bv = "11110000";
    SHOW(bv,sc_dt::SC_BIN);

    std::cout << "program completed" << std::endl;
    return 0;
}
