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
 * @file   sc_any_value_converter.cpp
 * @brief  conversions from and to a @ref sc_any_value (common types)
 * @author Philipp A. Hartmann, OFFIS/Intel
 */

#define SC_BUILD_ANY_VALUE
#include "sysc/datatypes/any/sc_any_value_converter.h"

#include "sysc/kernel/sc_time.h"
#include "sysc/datatypes/bit/sc_logic.h"
#include "sysc/datatypes/bit/sc_bv_base.h"
#include "sysc/datatypes/bit/sc_lv_base.h"
#include "sysc/datatypes/int/sc_int_base.h"
#include "sysc/datatypes/int/sc_uint_base.h"
#include "sysc/datatypes/int/sc_signed.h"
#include "sysc/datatypes/int/sc_unsigned.h"

#ifdef SC_INCLUDE_FX
#include "sysc/datatypes/fx/sc_fxval.h"
#include "sysc/datatypes/fx/sc_fix.h"
#include "sysc/datatypes/fx/sc_ufix.h"
#endif // SC_INCLUDE_FX

//@cond SC_HIDDEN_FROM_DOXYGEN
namespace sc_dt {

#define DEFINE_PACK_( Type )                                                  \
  template<> bool                                                             \
  sc_any_value_converter<Type>::pack( sc_any_value::reference dst, type const & src )

#define DEFINE_UNPACK_(Type)                                                  \
  template<> bool                                                             \
  sc_any_value_converter<Type>::unpack( type & dst, sc_any_value::const_reference src )


// ----------------------------------------------------------------------------
// C++ builtin types

#define DEFINE_BUILTIN_( Type, Alias )                                        \
  DEFINE_PACK_(Type)                                                          \
  {                                                                           \
    dst.set_ ## Alias( src );                                                 \
    return true;                                                              \
  }                                                                           \
  DEFINE_UNPACK_(Type)                                                        \
  {                                                                           \
    if( !src.is_ ## Alias() ) return false;                                   \
    dst = src.get_ ## Alias();                                                \
    return true;                                                              \
  }

DEFINE_BUILTIN_(bool,     bool)
DEFINE_BUILTIN_(int,      int)
DEFINE_BUILTIN_(int64,    int64)
DEFINE_BUILTIN_(unsigned, uint)
DEFINE_BUILTIN_(uint64,   uint64)
DEFINE_BUILTIN_(double,   number)

#if SC_CPLUSPLUS >= 201703L
DEFINE_BUILTIN_(std::string,string)
#else
DEFINE_PACK_(std::string)
{
  dst.set_string( src );
  return true;
}
DEFINE_UNPACK_(std::string)
{
  if( !src.is_string() )
    return false;
  sc_zstring_view s = src.get_string();
  dst.assign( s.data(), s.size() );
  return true;
}
#endif // SC_CPLUSPLUS

// ----------------------------------------------------------------------------
// SystemC builtin types

DEFINE_BUILTIN_( sc_core::sc_string_view, string );

DEFINE_PACK_( sc_core::sc_time )
{
  sc_core::sc_time_tuple tp = src;
  dst.set_list()
    .push_back( tp.value() )
    .push_back<sc_string_view>( tp.unit_symbol() );
  return true;
}

DEFINE_UNPACK_( sc_core::sc_time )
{
  // encoded as string: "NN <unit>"
  if( src.is_string() )
  {
    dst = sc_core::sc_time::from_string( src.get_string().c_str() );
    return true;
  }

  sc_any_value value, unit;
  // encoded as ordered (value,unit) tuple
  if( src.is_list() && src.get_list().size() == 2 )
  {
    sc_any_value::const_list_reference l = src.get_list();
      value = l[0];
      unit  = l[1];
  }
  // encoded as map: "value"=, "unit"=
  else if( src.is_map() && src.get_map().size() == 2 )
  {
    sc_any_value::const_map_reference m = src.get_map();
    if( m.has_entry("value") && m.has_entry("unit") )
    {
      value = m.at("value");
      unit  = m.at("unit");
    }
  }

  if( !value.is_number() )
    return false;

  if( unit.is_string() )
  {
    dst = sc_core::sc_time( value.get_number(), unit.get_string().c_str() );
    return true;
  }
  else if( unit.is_uint() && unit.get_uint() <= sc_core::SC_SEC )
  {
    dst = sc_core::sc_time( value.get_number()
                          , (sc_core::sc_time_unit)unit.get_uint() );
    return true;
  }
  return false;
}

DEFINE_BUILTIN_( sc_bit, bool )

DEFINE_PACK_( sc_logic )
{
  switch( src.value() )
  {
  case Log_0:
  case Log_1:
    dst.set_bool( src.to_bool() );
    break;
  default:
    dst.set_string( std::string( 1, src.to_char() ) );
    break;
  }
  return true;
}

DEFINE_UNPACK_( sc_logic )
{
  if( src.is_bool() )
  {
    dst = src.get_bool();
    return true;
  }
  else if( src.is_int() )
  {
    dst = sc_logic( src.get_int() );
    return true;
  }
  else if( src.is_string() && src.get_string().size() == 1 )
  {
    switch( src.get_string()[0] )
    {
      case '0': dst = SC_LOGIC_0; return true;
      case '1': dst = SC_LOGIC_1; return true;
      case 'z': /* fallthrough */
      case 'Z': dst = SC_LOGIC_Z; return true;
      case 'x': /* fallthrough */
      case 'X': dst = SC_LOGIC_X; return true;
      default:  /* nothing */ ;
    }
  }
  return false;
}

DEFINE_PACK_( sc_int_base )
{
  dst.set_int64( src );
  return true;
}

DEFINE_UNPACK_( sc_int_base )
{
  if( src.is_int64() )
    dst = src.get_int64();
  else if( src.is_string() )
    dst = src.get_string().c_str();
  else
    return false;

  return true;
}

DEFINE_PACK_( sc_uint_base )
{
  dst.set_uint64( src );
  return true;
}

DEFINE_UNPACK_( sc_uint_base )
{
  if( src.is_uint64() )
    dst = src.get_uint64();
  else if( src.is_string() )
    dst = src.get_string().c_str();
  else
    return false;

  return true;
}

DEFINE_PACK_( sc_signed )
{
  if( src.length() <= 64 )
    dst.set_int64( src.to_int64() );
  else
    dst.set_string( src.to_string() );

  return true;
}

DEFINE_UNPACK_( sc_signed )
{
  if( src.is_int64() )
    dst = src.get_int64();
  else if( src.is_string() )
    dst = src.get_string().c_str();
  else
    return false;

  return true;
}

DEFINE_PACK_( sc_unsigned )
{
  if( src.length() <= 64 )
    dst.set_uint64( src.to_uint64() );
  else
    dst.set_string( src.to_string() );

  return true;
}

DEFINE_UNPACK_( sc_unsigned )
{
  if( src.is_uint64() )
    dst = src.get_uint64();
  else if( src.is_string() )
    dst = src.get_string().c_str();
  else
    return false;

  return true;
}

DEFINE_PACK_( sc_bv_base )
{
  dst.set_string( src.to_string() );
  return true;
}

DEFINE_UNPACK_( sc_bv_base )
{
  if( !src.is_string() )
    return false;

  dst = src.get_string().c_str();
  return true;
}

DEFINE_PACK_( sc_lv_base )
{
  dst.set_string( src.to_string() );
  return true;
}

DEFINE_UNPACK_( sc_lv_base )
{
  if( !src.is_string() )
    return false;

  dst = src.get_string().c_str();
  return true;
}

// ----------------------------------------------------------------------------
#ifdef SC_INCLUDE_FX

template<typename FxType>
static bool
sc_any_value_unpack_fx( FxType& dst, sc_any_value::const_reference src )
{
  if( src.is_int64() ) {
    dst = src.get_int64();
    return true;
  }
  if( src.is_uint64() ) {
    dst = src.get_uint64();
    return true;
  }
  if( src.is_double() ) {
    dst = src.get_double();
    return true;
  }
  if( src.is_string() ) {
    dst = src.get_string().c_str();
    return true;
  }
  return false;
}

DEFINE_PACK_( sc_fxval )
{
  dst.set_string( src.to_string() );
  return true;
}

DEFINE_UNPACK_( sc_fxval )
{
  return sc_any_value_unpack_fx(dst,src);
}

DEFINE_PACK_( sc_fxval_fast )
{
  dst.set_double( src.to_double() );
  return true;
}

DEFINE_UNPACK_( sc_fxval_fast )
{
  return sc_any_value_unpack_fx(dst,src);
}

DEFINE_PACK_( sc_fix )
{
  dst.set_string( src.to_string() );
  return true;
}

DEFINE_UNPACK_( sc_fix )
{
  return sc_any_value_unpack_fx(dst,src);
}

DEFINE_PACK_( sc_fix_fast )
{
  dst.set_double( src.to_double() );
  return true;
}

DEFINE_UNPACK_( sc_fix_fast )
{
  return sc_any_value_unpack_fx(dst,src);
}

DEFINE_PACK_( sc_ufix )
{
  dst.set_string( src.to_string() );
  return true;
}

DEFINE_UNPACK_( sc_ufix )
{
  return sc_any_value_unpack_fx(dst,src);
}

DEFINE_PACK_( sc_ufix_fast )
{
  dst.set_double( src.to_double() );
  return true;
}

DEFINE_UNPACK_( sc_ufix_fast )
{
  return sc_any_value_unpack_fx(dst,src);
}

#endif // SC_INCLUDE_FX
// ----------------------------------------------------------------------------
// explicit template instantiations

template struct sc_any_value_converter<bool>;
template struct sc_any_value_converter<int>;
template struct sc_any_value_converter<int64>;
template struct sc_any_value_converter<unsigned>;
template struct sc_any_value_converter<uint64>;
template struct sc_any_value_converter<double>;
template struct sc_any_value_converter<std::string>;

template struct sc_any_value_converter<sc_core::sc_string_view>;
template struct sc_any_value_converter<sc_core::sc_time>;
template struct sc_any_value_converter<sc_logic>;
template struct sc_any_value_converter<sc_int_base>;
template struct sc_any_value_converter<sc_uint_base>;
template struct sc_any_value_converter<sc_signed>;
template struct sc_any_value_converter<sc_unsigned>;
template struct sc_any_value_converter<sc_bv_base>;
template struct sc_any_value_converter<sc_lv_base>;

#ifdef SC_INCLUDE_FX
template struct sc_any_value_converter<sc_fxval>;
template struct sc_any_value_converter<sc_fxval_fast>;
template struct sc_any_value_converter<sc_fix>;
template struct sc_any_value_converter<sc_fix_fast>;
template struct sc_any_value_converter<sc_ufix>;
template struct sc_any_value_converter<sc_ufix_fast>;
#endif // SC_INCLUDE_FX

} // namespace sc_dt
//@endcond
// Taf!
