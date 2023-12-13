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

  adder_int_5_pimpl.cpp : adder specialization using PImpl idiom


  Original Author: Roman I. Popov, Intel

 *****************************************************************************/

#include "adder_int_5_pimpl.h"
#include "adder.h"

struct adder_int_5_pimpl::adder_impl : adder<int,5> {
    adder_impl(const sc_module_name& name) : adder(name) {}
};

adder_int_5_pimpl::adder_int_5_pimpl(const char* name)
        : p_impl(new adder_impl(name))
        , res(p_impl->res)
        , din(p_impl->din)
{ }

adder_int_5_pimpl::~adder_int_5_pimpl()
{
    delete p_impl;
}
