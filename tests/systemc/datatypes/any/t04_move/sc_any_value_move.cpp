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

 ****************************************************************************/

/**
 * @file   sc_any_value_move.cpp
 * @author Philipp A. Hartmann, Intel
 * @brief  Test of the sc_any_value move semantics
 */

#include <systemc>
#include <iostream>

using sc_dt::sc_any_value;

#define DUMP(v) \
  std::cout << SC_STRINGIFY_HELPER_(v) << " = " << (v).to_json() << std::endl;

int sc_main( int, char*[] )
{
  // manual move checks (still copies root node in C++03)
  {
    sc_any_value v("some string");
    DUMP(v);

    sc_assert( v.is_string() );
    sc_assert( v.get_string() == "some string" );
    std::string str( v.get_string() );

    // manual move
    sc_any_value w( v.move() );

    DUMP(w);
    DUMP(v);
    sc_assert( v.is_null() );
    sc_assert( w.is_string() );
    sc_assert( w.get_string() == str );

    v.set_list().reserve(2)
      .push_back(1).push_back(true);
    DUMP(v);
    sc_assert( v.is_list() );
    sc_assert( v.get_list()[1].is_bool() );

    w.set_list()
      .push_back( v.move() );

    DUMP(w);
    DUMP(v);
    sc_assert( v.is_null() );
    sc_assert( w.is_list() );
    sc_assert( w.get_list()[0].get_list()[1].is_true() );

    w = w.get_list()[0].move();
    v.set_map();
    v.get_map().push_entry("copy", w);
    v.get_map().push_entry("move", w.move());
    DUMP(v);
    DUMP(w);
  }

  // TODO: add explicit C++11 move tests
  //       (only implicitly tested via move() above)

  return EXIT_SUCCESS;
}
