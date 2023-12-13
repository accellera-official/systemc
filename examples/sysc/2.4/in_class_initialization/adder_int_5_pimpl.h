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

  adder_int_5_pimpl.h : adder specialization using PImpl idiom
  Header file exposes only interface of a module. Implementatation is hidden
  inside adder_int_5_pimpl.cpp


  Original Author: Roman I. Popov, Intel

 *****************************************************************************/

#ifndef ADDER_PIMPL_H_
#define ADDER_PIMPL_H_

#include <systemc.h>

// Demonstrates how to remove compile dependency on adder.h header using PImpl
class adder_int_5_pimpl {
    struct adder_impl;
    adder_impl* p_impl;
public:
    adder_int_5_pimpl(const char*name);
    ~adder_int_5_pimpl();

    sc_out<int>           &res;
    sc_vector<sc_in<int>> &din;
};

#endif //ADDER_PIMPL_H_