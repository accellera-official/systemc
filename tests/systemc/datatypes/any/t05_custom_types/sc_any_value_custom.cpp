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
 * @file   sc_any_value_custom.cpp
 * @author Philipp A. Hartmann, Intel
 * @brief  Test for custom type support in sc_any_value
 */

#include <systemc>

using sc_dt::sc_any_value;

// -----------------------------------------------------------------------

namespace my_ns {

struct unsupported_type { int x; };

struct supported_type
{
  std::string name;
  int id;
};

// ADL-based conversion
bool sc_any_value_pack(sc_any_value::reference dst, const supported_type& src)
{
  dst.set_list()
     .push_back(src.name)
     .push_back(src.id);
  return true;
}

bool sc_any_value_unpack(supported_type& dst, sc_any_value::const_reference src)
{
  if (!src.is_list())
    return false;
  sc_any_value::const_list_reference list = src.get_list();
  return list.size() == 2 &&
         list[0].try_get(dst.name) &&
         list[1].try_get(dst.id);
}

} // namespace my_ns

using my_ns::unsupported_type;
using my_ns::supported_type;

// -----------------------------------------------------------------------

struct packable_type { bool x; };

bool sc_any_value_pack(sc_any_value::reference dst, const packable_type& src)
{
  dst.set_bool(src.x);
  return true;
}

// -----------------------------------------------------------------------

struct unpackable_type { double x; };

bool sc_any_value_unpack(unpackable_type& dst, sc_any_value::const_reference src)
{
  if (!src.is_number())
    return false;
  dst.x = src.get_number();
  return true;
}

// -----------------------------------------------------------------------

struct disabled_type { int x; };

namespace sc_dt {
template<> struct sc_any_value_converter<disabled_type>
  : sc_any_value_converter_disabled<disabled_type> {};
} // namespace sc_dt

// -----------------------------------------------------------------------
int sc_main( int, char*[] )
{
  {
    // unsupported conversion
#if SC_CPLUSPLUS >= 201103L
    static_assert( sc_dt::sc_any_value_has_pack<unsupported_type>::value == false
                 , "unsupported_type has sc_any_value_pack function" );
    static_assert( sc_dt::sc_any_value_has_unpack<unsupported_type>::value == false
                 , "unsupported_type has sc_any_value_unpack function" );
    static_assert( sc_dt::sc_any_value_has_converter<unsupported_type>::value == false
                 , "unsupported_type has sc_any_value_converter support" );
#endif // C++11

    std::cout
      << "unsupported_type: " << std::boolalpha
      << "has_pack=" << sc_dt::sc_any_value_has_pack<unsupported_type>::value
      << ", has_unpack=" << sc_dt::sc_any_value_has_unpack<unsupported_type>::value
      << ", has_converter=" << sc_dt::sc_any_value_has_converter<unsupported_type>::value
      << std::endl;

    // doesn't compile:
    // sc_assert( v.try_set(u) == false );
    // sc_assert( v.try_get(u) == false );
  }
  {
   // supported conversion
#if SC_CPLUSPLUS >= 201103L
    static_assert( sc_dt::sc_any_value_has_pack<supported_type>::value == true
                 , "supported_type has no sc_any_value_pack function" );
    static_assert( sc_dt::sc_any_value_has_unpack<supported_type>::value == true
                 , "supported_type has no sc_any_value_unpack function" );
    static_assert( sc_dt::sc_any_value_has_converter<supported_type>::value == false
                 , "supported_type has sc_any_value_converter support" );
#endif // C++11

    std::cout
      << "supported_type: " << std::boolalpha
      << "has_pack=" << sc_dt::sc_any_value_has_pack<supported_type>::value
      << ", has_unpack=" << sc_dt::sc_any_value_has_unpack<supported_type>::value
      << ", has_converter=" << sc_dt::sc_any_value_has_converter<supported_type>::value
      << std::endl;

    std::string json = "[\"check\",1]";
    sc_any_value var = sc_any_value::from_json( json );

    sc_assert( var.is_list() );
    sc_assert( var.to_json() == json );

    supported_type s = {};
    sc_assert( s.name == "" );
    sc_assert( s.id == 0 );

    sc_assert( var.try_get(s) == true );
    sc_assert( s.name == "check" );
    sc_assert( s.id == 1 );
    std::cout << "supported_type: json=" << var.to_json() << std::endl;

    s.id++;
    var.set_null();
    sc_assert( var.is_null() );
    sc_assert( var.try_set(s) == true );
    sc_assert( var.is_list() );
    sc_assert( var.get_list()[0].get_string() == "check" );
    sc_assert( var.get_list()[1].get_int() == 2 );
    std::cout << "supported_type: json=" << var.to_json() << std::endl;
  }
  {
    sc_any_value var;

    // only "packable" conversion
#if SC_CPLUSPLUS >= 201103L
    static_assert( sc_dt::sc_any_value_has_pack<packable_type>::value == true
                 , "packable_type has no sc_any_value_pack function" );
    static_assert( sc_dt::sc_any_value_has_unpack<packable_type>::value == false
                 , "packable_type has sc_any_value_unpack function" );
    static_assert( sc_dt::sc_any_value_has_converter<packable_type>::value == false
                 , "packable_type has sc_any_value_converter support" );
#endif // C++11

    std::cout
      << "packable_type: " << std::boolalpha
      << "has_pack=" << sc_dt::sc_any_value_has_pack<packable_type>::value
      << ", has_unpack=" << sc_dt::sc_any_value_has_unpack<packable_type>::value
      << ", has_converter=" << sc_dt::sc_any_value_has_converter<packable_type>::value
      << std::endl;

    packable_type p = { true };
    sc_assert( var.try_set(p) == true );
    sc_assert( var.is_bool() == true );
    sc_assert( var.get_bool() == true );
    std::cout << "packable_type: json=" << var.to_json() << std::endl;

    // sc_assert( var.try_get(p) == false ); // doesn't compile
  }
  {
    sc_any_value var;

    // only "unpackable" conversion
#if SC_CPLUSPLUS >= 201103L
    static_assert( sc_dt::sc_any_value_has_pack<unpackable_type>::value == false
                 , "unpackable_type has sc_any_value_pack function" );
    static_assert( sc_dt::sc_any_value_has_unpack<unpackable_type>::value == true
                 , "unpackable_type has no sc_any_value_unpack function" );
    static_assert( sc_dt::sc_any_value_has_converter<unpackable_type>::value == false
                 , "unpackable_type has sc_any_value_converter support" );
#endif // C++11

    std::cout
      << "unpackable_type: " << std::boolalpha
      << "has_pack=" << sc_dt::sc_any_value_has_pack<unpackable_type>::value
      << ", has_unpack=" << sc_dt::sc_any_value_has_unpack<unpackable_type>::value
      << ", has_converter=" << sc_dt::sc_any_value_has_converter<unpackable_type>::value
      << std::endl;

    unpackable_type u = {};
    var.set_double(42);
    std::cout << "unpackable_type: json=" << var.to_json() << std::endl;
    sc_assert( var.is_double() == true );
    sc_assert( var.try_get(u) == true );
    sc_assert( u.x == var.get_double() );

    // sc_assert( var.try_set(u) == false ); // doesn't compile
  }
  {
    sc_any_value var;

    // disabled conversion
    disabled_type d = { 42 };

#if SC_CPLUSPLUS >= 201103L
    static_assert( sc_dt::sc_any_value_has_pack<disabled_type>::value == true
                 , "disabled_type has no sc_any_value_pack function" );
    static_assert( sc_dt::sc_any_value_has_unpack<disabled_type>::value == true
                 , "disabled_type has no sc_any_value_unpack function" );
    static_assert( sc_dt::sc_any_value_has_converter<disabled_type>::value == false
                 , "disabled_type has sc_any_value_converter specialization" );
#endif // C++11

    std::cout
      << "disabled_type: " << std::boolalpha
      << "has_pack=" << sc_dt::sc_any_value_has_pack<disabled_type>::value
      << ", has_unpack=" << sc_dt::sc_any_value_has_unpack<disabled_type>::value
      << ", has_converter=" << sc_dt::sc_any_value_has_converter<disabled_type>::value
      << std::endl;

    sc_assert( var.is_null() );
    sc_assert( var.try_set(d) == false );
    std::cout << "disabled_type: json=" << var.to_json() << std::endl;
    sc_assert( var.is_null() );
    var.set_int(42);
    sc_assert( var.try_get(d) == false );
    sc_assert( d.x == 42 );
  }
  return EXIT_SUCCESS;
}
