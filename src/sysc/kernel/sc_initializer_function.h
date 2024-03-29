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
  sc_initializer_function.h -- Helper class and macros to enable in-class
  initialization of complex objects

  Original Authors: Roman I. Popov, Intel
                    Philipp A. Hartmann, Intel
 *****************************************************************************/


#ifndef SC_CORE_SC_INITIALIZER_FUNCTION_H_INCLUDED_
#define SC_CORE_SC_INITIALIZER_FUNCTION_H_INCLUDED_

#include "sysc/kernel/sc_cmnhdr.h"

#include "sysc/kernel/sc_macros.h"
#include "sysc/kernel/sc_module.h"
#include <functional>
#include <utility>

#define SC_INIT(object_name) \
  sc_core::sc_initializer_function \
  SC_CONCAT_HELPER_(object_name, _initialization_fn_lambda) { [this]() { \
  SC_CONCAT_HELPER_(object_name, _initialization_fn)(); \
  }};\
  void SC_CONCAT_HELPER_(object_name,_initialization_fn)()

#define SC_NAMED_WITH_INIT(object_name,...) \
  object_name { sc_core::sc_initializer_function_name_fwd(SC_STRINGIFY_HELPER_(object_name), \
   [this]{ SC_CONCAT_HELPER_(object_name,_initialization_fn)(); }), __VA_ARGS__ }; \
  void SC_CONCAT_HELPER_(object_name,_initialization_fn)()

#define SC_THREAD_IMP(thread_name, ...) \
  SC_INIT(thread_name) { \
    SC_THREAD(thread_name); \
    { __VA_ARGS__ } \
    } \
  void thread_name()

#define SC_CTHREAD_IMP(thread_name, edge, ...) \
  SC_INIT(thread_name) { \
    SC_CTHREAD(thread_name, edge); \
    { __VA_ARGS__ } \
    } \
  void thread_name()

#define SC_METHOD_IMP(method_name, ...) \
  SC_INIT(method_name) { \
    SC_METHOD(method_name); \
    { __VA_ARGS__ } \
    } \
  void method_name()


namespace sc_core {

class sc_initializer_function {
public:
    template<class F>
    explicit sc_initializer_function(F&& fn) {

        auto simctx = sc_get_curr_simcontext();

        if (simctx->elaboration_done())
            SC_REPORT_ERROR(SC_ID_INSERT_INITIALIZER_FN_, "after elaboration done");
        else {
            auto curr_module = static_cast<sc_module*>( simctx->hierarchy_curr() );
            if (curr_module == nullptr)
                SC_REPORT_ERROR(SC_ID_INSERT_INITIALIZER_FN_, "outside of module hierarchy");
            else {
                sc_assert(curr_module->m_module_name_p != nullptr);
                curr_module->m_module_name_p->m_initializer_fn_vec.emplace_back(std::move(fn));
            }

        }
    }
};

template <class F>
inline const char * sc_initializer_function_name_fwd (const char *name, F&& fn)
{
    sc_initializer_function(std::move(fn));
    return name;
}


}

#endif // SC_CORE_SC_INITIALIZER_FUNCTION_H_INCLUDED_
