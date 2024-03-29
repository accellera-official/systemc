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

  5.2.19.cpp -- Test code snipped from IEEE Std 1666-2023 LRM 

  Original Author: Andy Goodrich, Accellera Systems Initiative

 *****************************************************************************/

#include <systemc>

SC_MODULE(M1) {
    sc_core::sc_inout<int> SC_NAMED(P), SC_NAMED(Q), SC_NAMED(R);

    SC_CTOR(M1) { }
};

SC_MODULE(Top1)
{
    sc_core::sc_inout<int>  SC_NAMED(A), SC_NAMED(B);
    sc_core::sc_signal<int> SC_NAMED(C);

    M1 m1;

    SC_CTOR(Top1) : m1("m1") {
        m1(A,B,C);
    }
};

SC_MODULE(M2) {
    sc_core::sc_inout<int> SC_NAMED(S);
    sc_core::sc_inout<int>* T;
    sc_core::sc_inout<int> SC_NAMED(U);

    SC_CTOR(M2) { T = new sc_core::sc_inout<int>("T"); }
};

SC_MODULE(Top2) {
    sc_core::sc_inout<int> SC_NAMED(D), SC_NAMED(E);
    sc_core::sc_signal<int> SC_NAMED(F);
    M2 m2;

    SC_CTOR(Top2) : m2("m2") {
        m2(D,E,F);
    }
};
        
int sc_main( int argc, char* argv[] ) { 

    sc_core::sc_signal<int> signalA;
    sc_core::sc_signal<int> signalB;
    sc_core::sc_signal<int> signalD;
    sc_core::sc_signal<int> signalE;
    Top1                    top1("top1");
    Top2                    top2("top2");

    top1.A(signalA);
    top1.B(signalB);
    top2.D(signalD);
    top2.E(signalE);

    sc_core::sc_start(sc_core::SC_ZERO_TIME);
    std::cout << "program completed" << std::endl;
    return 0;
}
