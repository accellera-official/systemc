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
#ifndef SC_DATATYPES_VAR_RAPIDJSON_H_INCLUDED_
#define SC_DATATYPES_VAR_RAPIDJSON_H_INCLUDED_

/**
 * @file   sc_rapidjson.h
 * @brief  internal include file for the RapidJSON library
 * @author Philipp A. Hartmann, OFFIS/Intel
 *
 * This file provides a local indirection to include RapidJSON from within
 * the SystemC library.  RapidJSON is used for serialization from/to
 * @ref sc_dt::sc_variant.
 *
 * @note This file is @b not part of the public API!
 */

#include <stdexcept>
#include <iosfwd>

#include "sysc/kernel/sc_cmnhdr.h"
#include "sysc/datatypes/var/sc_variant_ids.h" // SC_REPORT_ERROR

// --------------------------------------------------------------------------
// configure RapidJSON

#define RAPIDJSON_NAMESPACE \
  sc_dt::sc_rapidjson
#define RAPIDJSON_NAMESPACE_BEGIN \
  namespace sc_dt { namespace sc_rapidjson {
#define RAPIDJSON_NAMESPACE_END \
  } }

// enable support for std::string
#define RAPIDJSON_HAS_STD_STRING 1

// parse floating point numbers with full precision
#define RAPIDJSON_PARSE_DEFAULT_FLAGS \
  ::RAPIDJSON_NAMESPACE::kParseFullPrecisionFlag

// don't use explicit member iterator class
#define RAPIDJSON_NOMEMBERITERATORCLASS 1

// report error on failing assertions
#define RAPIDJSON_ASSERT( expr ) \
  ((void)(( SC_LIKELY_(expr) ) ? 0 : \
    ( SC_REPORT_ERROR( ::sc_core::SC_ID_VARIANT_ASSERTION_FAILED_, #expr ), 0 )))

#include "sysc/packages/rapidjson/include/rapidjson/rapidjson.h"

// throw exception by default
#define RAPIDJSON_PARSE_ERROR_EARLY_RETURN( what ) \
  ((void)0)
#define RAPIDJSON_PARSE_ERROR_NORETURN(parseErrorCode,offset) \
  throw ::RAPIDJSON_NAMESPACE::ParseException( \
               ::RAPIDJSON_NAMESPACE::parseErrorCode, \
               #parseErrorCode, offset \
  )

#include "sysc/packages/rapidjson/include/rapidjson/error/error.h"
#include "sysc/packages/rapidjson/include/rapidjson/error/en.h"

RAPIDJSON_NAMESPACE_BEGIN

struct ParseException
  : std::runtime_error, ParseResult
{
  ParseException( ParseErrorCode code, const char* msg, size_t offset )
     : std::runtime_error(GetParseError_En(code)), ParseResult(code,offset) {}
};

struct StringOutputStream
{
  typedef char Ch;

  explicit StringOutputStream(std::string& s) : s_(s) {}

  void   Put(Ch c)    { s_.push_back(c); }
  void   Clear()      { s_.clear(); }
  void   Flush()      { return; }
  size_t Size() const { return s_.length(); }

  friend void PutReserve(StringOutputStream &stream, size_t count)
    { stream.s_.reserve(stream.Size() + count); }

private:
  std::string& s_;
};

RAPIDJSON_NAMESPACE_END

#include "sysc/packages/rapidjson/include/rapidjson/document.h"
#include "sysc/packages/rapidjson/include/rapidjson/istreamwrapper.h"
#include "sysc/packages/rapidjson/include/rapidjson/ostreamwrapper.h"
#include "sysc/packages/rapidjson/include/rapidjson/writer.h"

#endif // SC_DATATYPES_VAR_RAPIDJSON_H_INCLUDED_
