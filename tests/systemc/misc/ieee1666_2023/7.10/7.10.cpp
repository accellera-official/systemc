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

  7.10.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

// ISSUES WITH THIS EXAMPLE
//
// This test has issues:
//
//  Issue 1:
//
//  sc_dt::sc_fxval b() = -1.5 is not legal, it needs to be sc_dt::sc_fxval b(-1.5)
//
//  Issue 2:
//
//  b.to_string(sc_dt::SC_BIN,false,sc_dt::SC_F) yields 10.1 not 010100 as the comment says
//  b.to_string(sc_dt::SC_BIN,false,sc_dt::SC_E) yields 10.1 not 0101e+2 as the comment says
//
// @@@@ ISSUES @@@@

#define SC_INCLUDE_FX
#include <systemc.h>

#define SHOW(X) \
    {std::cout << #X << " = " << X << std::endl;}
#define SHOW_FMT(X,FMT) { \
    std::cout << #X << ".to_string(" << #FMT << " = " << X.to_string(FMT) << std::endl;}
#define SHOW_REAL(X,FMT,REAL) { \
    std::cout << #X << ".to_string(" << #FMT << ",false," << #REAL << ") = " \
              << X.to_string(FMT,false,REAL) << std::endl;}
#define SHOW_FALSE(X,FMT) { \
    std::cout << #X << ".to_string(" << #FMT << ",false) = " << X.to_string(FMT,false) << std::endl;}
#define SHOW_TRUE(X,FMT) \
    {std::cout << #X << ".to_string(" << #FMT << ",true) = " << X.to_string(FMT,true) << std::endl;}


int sc_main( int argc, char* argv[] ) {

    std::cout << std::endl
              << "================ 7.10.8.1 ==================" << std::endl << std::endl;

    {
	sc_dt::sc_fxtype_params  fxt(32,16);
	sc_dt::sc_fxtype_context fcxt(fxt);
	sc_dt::sc_fix a, b, res;
	a = 10.0;
	b = 0.1;
	res = a * b;
	SHOW(a);
	SHOW(b);
	SHOW(res);
	std::cout << std::endl;

	sc_dt::sc_fxcast_switch fxs(sc_dt::SC_OFF);
	sc_dt::sc_fxcast_context fccxt(fxs);
	sc_dt::sc_fix c, d;
	c = 10.0;
	d = 0.1;
	res = c * d;
	SHOW(c);
	SHOW(d);
	SHOW(res);
	std::cout << std::endl;
    }

    std::cout << std::endl
              << "================ 7.10.9.1 ==================" << std::endl << std::endl;

    sc_dt::sc_fixed<7,4> a = -1.5;
    SHOW_FMT(a,sc_dt::SC_DEC);
    SHOW_FMT(a,sc_dt::SC_BIN);

    sc_dt::sc_fxval b(-1.5);
    SHOW_FMT(b,sc_dt::SC_BIN);

    sc_dt::sc_fixed<4,6> c = 20;
    SHOW_REAL(b,sc_dt::SC_BIN,sc_dt::SC_F);
    SHOW_REAL(b,sc_dt::SC_BIN,sc_dt::SC_E);

    std::cout << std::endl << "program completed" << std::endl;
    return 0;
}
