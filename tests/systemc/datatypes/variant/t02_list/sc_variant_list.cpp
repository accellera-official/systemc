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
 * @file   sc_variant_list.cpp
 * @author Philipp A. Hartmann, Intel
 * @brief  Test of the sc_variant list interface
 */

#include <systemc>
#include <iostream>
#include <algorithm>

using sc_core::sc_string_view;
using sc_dt::sc_variant;
using sc_dt::sc_variant_list;

struct value_int_compare
{
  bool operator()(sc_variant::const_reference a, sc_variant::const_reference b) const
    { return a.get_int64() < b.get_int64(); }
};

int sc_main( int, char*[] )
{
  sc_variant_list orig;
  {
    sc_variant_list list; // empty
    sc_variant::list_reference lref = list;
    sc_variant::const_reference v = list;

    lref.reserve(10);
    sc_assert( lref.capacity() == 10u );
    sc_assert( list.capacity() == 10u );

    // add some elements
    lref.push_back(true);
    lref.push_back(42);
    lref.push_back("s");

    sc_assert( lref.size() == 3u );
    sc_assert( lref.capacity() >= lref.size() );
    sc_assert( v.get_list().size() == 3u );

    sc_assert( lref[0].get_bool() == true );
    sc_assert( lref.front() == lref[0] );
    sc_assert( lref[1].get_int() == 42 );
    sc_assert( lref.back() == lref[lref.size()-1] );
    sc_assert( lref.back().is_string() );

    // checked access
    sc_assert( v.get_list().at(0) == lref.at(0) );
    sc_assert( list.at(1) == lref[1] );
    sc_assert( list[2] == lref.at(2) );

    bool has_thrown = false;
    try {
      // at() checks out-of-bounds access
      lref.at( list.size() );
    } catch( const sc_core::sc_report& rpt ) {
      sc_assert( sc_string_view(rpt.get_msg_type())
                 == sc_core::SC_ID_VARIANT_ACCESS_FAILED_ );
      has_thrown = true;
    }
    sc_assert( has_thrown );

    sc_assert( lref.end() - lref.cbegin() == lref.ssize() );
    sc_assert( lref.end() > lref.cbegin() );
    sc_assert( lref.cend()-1 > lref.begin() );
    sc_assert( ++lref.begin() <= --lref.end() );
    sc_assert( lref.begin() + lref.size() >= --lref.end() );

    // compare across references
    sc_assert( v.get_list().begin() == lref.begin() );
    sc_assert( v.get_list().end() == lref.cend() );
    sc_assert( v.get_list().begin() != v.get_list().cend() );

    // reverse iterators
    sc_assert( lref.rbegin().base() == lref.cend() );
    sc_assert( lref.cbegin() == lref.rend().base() );
    sc_assert( lref.end() == lref.crbegin().base() );
    sc_assert( lref.rend() - lref.crbegin() == lref.ssize() );

    sc_assert( *lref.crbegin() == lref.back() );

    std::copy( lref.cbegin(), lref.cend()
             , std::ostream_iterator<sc_variant::const_reference>(std::cout," ") );
    std::cout << std::endl;

    std::copy( lref.crbegin(), lref.crend()
             , std::ostream_iterator<sc_variant::const_reference>(std::cout," ") );
    std::cout << std::endl;

    lref.swap(orig);
    sc_assert( list.size() == 0u );
    sc_assert( orig.size() == 3u );
    sc_assert( orig[0].is_true() );
  }
  {
    sc_variant_list list = orig; // copy

    sc_variant_list::const_iterator it = std::find(list.begin(), list.end(), sc_variant(42) );

    sc_assert( it != list.end() );
    sc_assert( it->get_int() == 42 );

    it = list.insert( it, 3, sc_variant() );

    sc_assert( list.size() == 6u );
    sc_assert( (it + 2)->is_null() );
    sc_assert( it - list.begin() == 1 );

    std::copy( list.cbegin(), list.cend()
             , std::ostream_iterator<sc_variant::const_reference>(std::cout," ") );
    std::cout << std::endl;

    // erase-remove idiom
    it = list.erase( std::remove(list.begin(), list.end(), sc_variant() ), list.end() );

    std::copy( list.cbegin(), list.cend()
             , std::ostream_iterator<sc_variant::const_reference>(std::cout," ") );
    std::cout << std::endl;

    sc_assert( list.size() == 3u );
    sc_assert( list == orig );

    list.pop_back();
    sc_assert( list.size() == 2u );
    sc_assert( list.back().get_uint() == 42 );

    list.erase( list.begin() + 1 );
    sc_assert( list.size() == 1u );
    sc_assert( list.back().is_true() );
  }
  {
    sc_variant_list list = orig; // copy

    list.clear();
    sc_assert( list.size() == 0u );

    const int values[] = { 42, 17, 101, 2 };
    list.insert( list.begin(), values, values + sizeof(values)/sizeof(*values) );
    sc_assert( list.size() == sizeof(values)/sizeof(*values) );

    std::copy( list.cbegin(), list.cend()
             , std::ostream_iterator<sc_variant::const_reference>(std::cout," ") );
    std::cout << std::endl;

    std::sort( list.begin(), list.end(), value_int_compare() );

    std::copy( list.cbegin(), list.cend()
             , std::ostream_iterator<sc_variant::const_reference>(std::cout," ") );
    std::cout << std::endl;
  }
  {
    const int v1[] = { 1, 5 };
    const int v2[] = { 0, 2, 3, 4, 6 };

    sc_variant value( v1 ); // deserialize from array
    sc_assert( value.is_list() );

    sc_variant::list_reference list = value.get_list();
    sc_variant::list_reference::const_iterator it = list.end();
    sc_assert( list.size() == 2u );

    it = list.insert( list.begin(), v2, v2 + 1 ); // 0
    sc_assert( list.size() == 3u );
    sc_assert( it == list.begin() );
    sc_assert( *it == sc_variant(0) );

    it = list.insert( list.begin() + 2, v2 + 1, v2 + 4 ); // 2,3,4
    sc_assert( list.size() == 6u );
    sc_assert( it == list.begin() + 2 );

    it = list.insert( list.end(), v2 + 4, v2 + 5 ); // 6
    sc_assert( list.size() == 7u );
    sc_assert( *it == *list.rbegin() );
    sc_assert( *it == sc_variant(6) );

    std::copy( list.cbegin(), list.cend()
             , std::ostream_iterator<sc_variant::const_reference>(std::cout," ") );
    std::cout << std::endl;
  }

  return EXIT_SUCCESS;
}
