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

  5.12.7.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>

SC_MODULE(M) 
{
    sc_core::sc_inout<int> SC_NAMED(P), SC_NAMED(Q), SC_NAMED(R), SC_NAMED(S);
    sc_core::sc_inout<int>*T;

    SC_CTOR(M) {
        T = new sc_core::sc_inout<int>("T");
    }
};

SC_MODULE(Top)
{
    sc_core::sc_inout<int> SC_NAMED(A), SC_NAMED(B);
    sc_core::sc_signal<int> SC_NAMED(C), SC_NAMED(D), SC_NAMED(E);

    M m;

    SC_CTOR(Top) : m("m") {
        m.P(A);
	m.Q.bind(B);
	m.R(C);
	m.S.bind(D);
	m.T->bind(E);
    }
};

    

int sc_main( int argc, char* argv[] ) { 

    sc_core::sc_signal<int> SC_NAMED(X), SC_NAMED(Y);

    Top top("top");
    top.A(X);
    top.B(Y);

    sc_core::sc_start(sc_core::SC_ZERO_TIME);

    std::cout << "program completed" << std::endl;
    return 0;
}
