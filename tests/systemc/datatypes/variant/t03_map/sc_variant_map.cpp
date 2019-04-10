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
 * @file   sc_variant_map.cpp
 * @author Philipp A. Hartmann, Intel
 * @brief  Test of the sc_variant map interface
 */

#include <systemc>
#include <iostream>
#include <algorithm>

using sc_core::sc_string_view;
using sc_dt::sc_variant;
using sc_dt::sc_variant_map;

namespace sc_dt {
std::ostream& operator<<( std::ostream& os, sc_variant_map::const_element_reference e )
{
  return os << e.key << "=" << e.value;
}
} // namespace sc_dt

int sc_main( int, char*[] )
{
  sc_variant_map orig;
  {
    sc_variant_map map; // empty
    sc_variant::map_reference mref = map;
    sc_variant::const_map_reference cref = map;
    sc_variant::const_reference v = map;

    sc_assert( map.size() == 0u );

    // add some elements
    mref.push_entry( "b", true);
    mref.push_entry( "i", 42);

    bool has_thrown = false;
    try {
      // at() cannot be used for insertion
      map.at("s") = sc_variant();
    } catch( const sc_core::sc_report& rpt ) {
      sc_assert( sc_string_view(rpt.get_msg_type())
                 == sc_core::SC_ID_VARIANT_ACCESS_FAILED_ );
      has_thrown = true;
    }
    sc_assert( has_thrown );

    // inserting element access
    sc_assert( mref["s"].is_null() );
    mref["s"] = sc_variant("SystemC");

    sc_assert( mref.size() == 3u );
    sc_assert( v.get_map().size() == 3u );

    sc_assert( cref.at("b").get_bool() == true );
    sc_assert( cref.at("i").get_int() == 42 );
    sc_assert( cref.at("s").get_string() == "SystemC" );
    sc_assert( (--cref.end())->value == mref["s"] );

    sc_assert( mref.end() - mref.cbegin() == mref.ssize() );
    sc_assert( mref.end() > mref.cbegin() );
    sc_assert( mref.cend()-1 > mref.begin() );
    sc_assert( ++mref.begin() <= --mref.end() );
    sc_assert( mref.begin() + mref.size() >= --mref.end() );

    // compare across references
    sc_assert( v.get_map().begin() == mref.begin() );
    sc_assert( v.get_map().end() == cref.cend() );
    sc_assert( v.get_map().begin() != cref.cend() );

    // reverse iterators
    sc_assert( mref.rbegin().base() == mref.cend() );
    sc_assert( mref.cbegin() == mref.rend().base() );
    sc_assert( mref.end() == mref.crbegin().base() );
    sc_assert( mref.rend() - mref.crbegin() == mref.ssize() );

    sc_assert( mref.crbegin()->key == (--mref.end())->key );
    sc_assert( mref.crbegin()->value == (--mref.end())->value );

    std::copy( mref.cbegin(), mref.cend()
             , std::ostream_iterator<sc_variant_map::const_element_reference>
                (std::cout," ") );
    std::cout << std::endl;

    std::copy( mref.crbegin(), mref.crend()
             , std::ostream_iterator<sc_variant_map::const_element_reference>
                (std::cout," ") );
    std::cout << std::endl;

    mref.push_entry( "i", 43); // JSON does not enforce unique keys
    mref.push_entry( "i", 44);
    mref.push_entry( "i", 45);

    mref.push_entry( "u", 1u);
    mref.push_entry( "v", 2u);
    mref.push_entry( "w", sc_variant());

    std::copy( mref.cbegin(), mref.cend()
             , std::ostream_iterator<sc_variant_map::const_element_reference>
                (std::cout," ") );
    std::cout << std::endl;

    sc_assert( cref.has_entry("s") );
    sc_assert( cref.find("s") != cref.end() );
    sc_assert( ! cref.has_entry("t") );
    sc_assert( cref.find("t") == cref.end() );
    sc_assert( mref.find("u") != map.end() );
    sc_assert( mref.find("u") == cref.find(std::string("u")) );
    sc_assert( mref.find("u")->key == "u" );
    sc_assert( mref.find("u")->value == mref["u"] );

    size_t erased = mref.erase("i");
    sc_assert( erased == 4u ); // all duplicate entries removed

    std::copy( mref.cbegin(), mref.cend()
             , std::ostream_iterator<sc_variant_map::const_element_reference>
                (std::cout," ") );
    std::cout << std::endl;

    mref.swap(orig);
    sc_assert( map.size() == 0u );
    sc_assert( orig.size() == 5u );
    sc_assert( orig.at("b").is_true() );
    sc_assert( orig.at("w").is_null() );
  }
  {
    sc_variant_map map = orig; // copy

    sc_variant_map::const_iterator it0 = map.find("u");
    sc_variant_map::const_iterator it1 = map.end() - 1;

    sc_assert( it0 != map.end() );
    sc_assert( it0->value.get_int() == 1 );
    sc_assert( it1 != map.end() );
    sc_assert( it1->value.is_null() );
    sc_assert( it1 - it0 == 2 );

    sc_variant_map::iterator it2 = map.erase(it0, it1);
    sc_assert( it2 != map.end() );

    std::copy( map.cbegin(), map.cend()
             , std::ostream_iterator<sc_variant_map::const_element_reference>
                (std::cout," ") );
    std::cout << std::endl;
  }

  return EXIT_SUCCESS;
}
