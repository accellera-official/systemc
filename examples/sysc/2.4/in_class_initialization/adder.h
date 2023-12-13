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

  adder.h : N-port adder. Header-only SystemC module, for PImpl idiom
  demonstration


  Original Author: Roman I. Popov, Intel

 *****************************************************************************/

#ifndef ADDER_H_
#define ADDER_H_

#include <systemc.h>

template <typename T, int N_INPUTS>
struct adder : sc_module {

    // SC_NAMED - convenient macro to initialize sc_object name with variable name
    sc_out<T>           SC_NAMED(res);
    // SC_NAMED supports passing arbitrary number of parameters
    sc_vector<sc_in<T>> SC_NAMED(din, N_INPUTS);

    SC_CTOR(adder){}
private:
    // In-class initialization of SC_METHOD
    // As a second parameter SC_METHOD_IMP takes arbitrary code that can be used to initialize process
    // curly braces are optional
    SC_METHOD_IMP(add_method, { for(auto &d : din) sensitive << d; } ); // Body can be in or outside of class
};

template <typename T, int N_INPUTS>
void adder<T,N_INPUTS>::add_method()
{
    T result = 0;
    for(auto &d : din)
        result += d.read();
    res = result;
}

#endif // ADDER_H_