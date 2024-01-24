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

  test01.cpp -- Test sc_(z)string_view

  Original Author: Philipp A. Hartmann, Intel

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <systemc>
#include <string>
#include <cstring>

using sc_core::sc_string_view;
using sc_core::sc_zstring_view;

static bool cstr_sv_overload( const char* s, bool sv_expected ) {
  std::cout << "cstr_sv_overload( const char* = \"" << s << "\")" << std::endl;
  return !sv_expected;
}
static bool cstr_sv_overload( sc_string_view s, bool sv_expected ) {
  std::cout << "cstr_sv_overload( sc_string_view = \"" << s << "\")" << std::endl;
  return sv_expected;
}

static bool str_sv_overload( const std::string& s, bool sv_expected ) {
  std::cout << "str_sv_overload( const std::string& = \"" << s << "\" )" << std::endl;
  return !sv_expected;
}
static bool str_sv_overload( sc_string_view s, bool sv_expected ) {
  std::cout << "str_sv_overload( sc_string_view = \"" << s << "\")" << std::endl;
  return sv_expected;
}

static bool cstr_zsv_overload( const char* s, bool sv_expected ) {
  std::cout << "cstr_zsv_overload( const char* = \"" << s << "\")" << std::endl;
  return !sv_expected;
}
static bool cstr_zsv_overload( sc_zstring_view s, bool sv_expected ) {
  std::cout << "cstr_zsv_overload( sc_zstring_view = \"" << s << "\")" << std::endl;
  return sv_expected;
}

static bool str_zsv_overload( const std::string& s, bool sv_expected ) {
  std::cout << "str_zsv_overload( const std::string& = \"" << s << "\" )" << std::endl;
  return !sv_expected;
}
static bool str_zsv_overload( sc_zstring_view s, bool sv_expected ) {
  std::cout << "str_zsv_overload( sc_zstring_view = \"" << s << "\")" << std::endl;
  return sv_expected;
}

int sc_main(int , char* [])
{
  // sc_string_view
  {
    const char string_literal[] = "string literal";
    sc_string_view sv = string_literal;

    sc_assert( string_literal == sv );
    sc_assert( sv.data() == string_literal );
    sc_assert( sv.size() == std::strlen(string_literal) );

    sc_assert( sv.starts_with("string") );
    sc_assert( sv.ends_with("literal") );

    std::cout << "sv = " << sv << std::endl;
    sc_assert( cstr_sv_overload(string_literal, false) );
    sc_assert( cstr_sv_overload(sv, true) );

    std::string str( sv );
    sc_assert( str == sv );

    sv = str;
    sc_assert( sv.data() == str.data() );
    sc_assert( sv.size() == str.size() );

    sv.remove_suffix(8);
    str = sv; // requires C++17 or implicit conversion to string
    sc_assert( str == sv );

    std::cout << "sv = " << sv << std::endl;
    // str_sv_overload( string_literal, true ); // ambiguous
    sc_assert( str_sv_overload(str, false) );
    sc_assert( str_sv_overload(sv, true) );
  }
  // sc_zstring_view
  {
    const char string_literal[] = "literal string";
    sc_zstring_view zsv = string_literal;

    sc_assert( string_literal == zsv );
    sc_assert( zsv.data() == string_literal );
    sc_assert( zsv.size() == std::strlen(string_literal) );

    sc_assert( zsv.ends_with("string") );
    sc_assert( zsv.starts_with("literal") );

    std::cout << "zsv = " << zsv << std::endl;
    sc_assert( cstr_zsv_overload(string_literal, false) );
    sc_assert( cstr_zsv_overload(zsv, true) );

    std::string str( zsv );
    sc_assert( str == zsv );

    zsv = str;
    sc_assert( zsv.data() == str.data() );
    sc_assert( zsv.size() == str.size() );

    zsv.remove_prefix(8);
    str = zsv; // requires C++17 or implicit conversion to string
    sc_assert( str == zsv );

    std::cout << "zsv = " << zsv << std::endl;
    // str_sv_overload( string_literal, true ); // ambiguous
    sc_assert( str_zsv_overload(str, false) );
    sc_assert( str_zsv_overload(zsv, true) );
  }
  // std::strng with embedded \0
  {
    const char string_literal[] = "before after";
    std::string str = string_literal;
    str[6] = '\0'; // embed a NULL character in the string
    sc_assert( str.size() == std::strlen(string_literal) );

    sc_string_view sv = str;
    sc_assert( sv == str );
    sc_assert( sv.data() == str.data() );
    sc_assert( sv.size() == str.size() );
    sc_assert( sv.starts_with("before") );
    sc_assert( sv.ends_with("after") );

    sc_zstring_view zsv = str;
    sc_assert( zsv == "before" );
    sc_assert( zsv.data() == str.data() );
    sc_assert( zsv.size() == sc_string_view("before").size() );
    sc_assert( zsv.starts_with("before") );
    sc_assert( zsv.ends_with("before") );
  }

  std::cout << "Program completed" << std::endl;
  return 0;
}
