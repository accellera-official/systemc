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

#if defined(_MSC_VER) && _MSC_VER <= 1600
# pragma warning(disable:4661)
# define CCI_TPLEXTERN_
#endif // excluded from MSVC'2010

#include "cci_core/cci_value_converter.h"

/**
 * @file   cci_value_converter.cpp
 * @brief  conversions from and to a @ref cci_value (common types)
 * @author Philipp A. Hartmann, OFFIS/Intel
 */

CCI_OPEN_NAMESPACE_

#define DEFINE_PACK_( Type )                                                  \
  template<> bool                                                             \
  cci_value_converter<Type>::pack( cci_value::reference dst, type const & src )

#define DEFINE_UNPACK_(Type)                                                  \
  template<> bool                                                             \
  cci_value_converter<Type>::unpack( type & dst, cci_value::const_reference src )

// ----------------------------------------------------------------------------
// explicit template instantiations

template struct cci_value_converter<bool>;
template struct cci_value_converter<int>;
template struct cci_value_converter<int64>;
template struct cci_value_converter<unsigned>;
template struct cci_value_converter<uint64>;
template struct cci_value_converter<double>;
template struct cci_value_converter<std::string>;

template struct cci_value_converter<sc_core::sc_time>;
template struct cci_value_converter<sc_dt::sc_logic>;
template struct cci_value_converter<sc_dt::sc_int_base>;
template struct cci_value_converter<sc_dt::sc_uint_base>;
template struct cci_value_converter<sc_dt::sc_signed>;
template struct cci_value_converter<sc_dt::sc_unsigned>;
template struct cci_value_converter<sc_dt::sc_bv_base>;
template struct cci_value_converter<sc_dt::sc_lv_base>;

#ifdef SC_INCLUDE_FX
template struct cci_value_converter<sc_dt::sc_fxval>;
template struct cci_value_converter<sc_dt::sc_fxval_fast>;
template struct cci_value_converter<sc_dt::sc_fix>;
template struct cci_value_converter<sc_dt::sc_fix_fast>;
template struct cci_value_converter<sc_dt::sc_ufix>;
template struct cci_value_converter<sc_dt::sc_ufix_fast>;
#endif // SC_INCLUDE_FX

#ifndef CCI_DOXYGEN_IS_RUNNING
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

DEFINE_BUILTIN_(std::string, string)

// ----------------------------------------------------------------------------
// SystemC builtin types

DEFINE_PACK_( sc_core::sc_time )
{
#if CCI_SYSTEMC_VERSION_CODE_ >= CCI_VERSION_HELPER_(2,3,2)
  sc_core::sc_time_tuple tp = src;
  dst.set_list()
    .push_back( tp.value() )
    .push_back<std::string>( tp.unit_symbol() );
#else // print as seconds
  dst.set_list()
    .push_back( src.to_seconds() )
    .push_back( "s" );
#endif
  return true;
}

static inline sc_core::sc_time sc_time_from_unit( unsigned unit )
{
  ///@todo add error handling
  sc_assert( unit <= sc_core::SC_SEC );
  sc_core::sc_time_unit tu = static_cast<sc_core::sc_time_unit>( unit );
  return sc_core::sc_time( 1, tu );
}

#if CCI_SYSTEMC_VERSION_CODE_ < CCI_VERSION_HELPER_(2,3,2)
static inline sc_core::sc_time sc_time_from_symbol( cci_value_string_cref sym )
{
  static struct symbol
  {
    const char *          sym[2];
    sc_core::sc_time_unit unit;
  }
  symbol_table[] =
  {
    { { "fs", "SC_FS"  }, sc_core::SC_FS  },
    { { "ps", "SC_PS"  }, sc_core::SC_PS  },
    { { "ns", "SC_NS"  }, sc_core::SC_NS  },
    { { "us", "SC_US"  }, sc_core::SC_US  },
    { { "ms", "SC_MS"  }, sc_core::SC_MS  },
    { { "s",  "SC_SEC" }, sc_core::SC_SEC },
    { { "sec" },          sc_core::SC_SEC },
    { { NULL, NULL },     sc_core::sc_time_unit() }
  };
  symbol* cursor = symbol_table;
  while( *cursor->sym && !(sym == cursor->sym[0] || sym == cursor->sym[1] ) )
    cursor++;

  sc_assert( unsigned(cursor - symbol_table) < (sizeof(symbol_table)/sizeof(symbol) ) );
  return sc_time_from_unit( unsigned(cursor - symbol_table) );
}
#endif // SystemC 2.3.2

DEFINE_UNPACK_( sc_core::sc_time )
{
  cci_value value, unit;
  // encoded as tuple
  if( src.is_list() && src.get_list().size() == 2 )
  {
    cci_value::const_list_reference l = src.get_list();
      value = l[0];
      unit  = l[1];
  }
  // encoded as map: "value"=, "unit"=
  else if( src.is_map() && src.get_map().size() == 2 )
  {
    cci_value::const_map_reference m = src.get_map();
    if( m.has_entry("value") && m.has_entry("unit") )
    {
      value = m.at("value");
      unit  = m.at("unit");
    }
  }
#if CCI_SYSTEMC_VERSION_CODE_ >= CCI_VERSION_HELPER_(2,3,2)
  // encoded as string: "NN <unit>"
  else if( src.is_string() )
  {
    dst = sc_core::sc_time::from_string( src.get_string().c_str() );
    return true;
  }
#endif // SystemC 2.3.2

  if( !value.is_number() )
    return false;

  if( unit.is_string() )
  {
#if CCI_SYSTEMC_VERSION_CODE_ >= CCI_VERSION_HELPER_(2,3,2)
    dst = sc_core::sc_time( value.get_double(), unit.get_string().c_str() );
#else
    dst = value.get_double() * sc_time_from_symbol( unit.get_string() );
#endif // SystemC 2.3.2
    return true;
  }
  else if( unit.is_uint() && unit.get_uint() <= sc_core::SC_SEC )
  {
    dst = value.get_double() * sc_time_from_unit( unit.get_uint() ) ;
    return true;
  }
  return false;
}

DEFINE_BUILTIN_( sc_dt::sc_bit, bool )

DEFINE_PACK_( sc_dt::sc_logic )
{
  switch( src.value() )
  {
  case sc_dt::Log_0:
  case sc_dt::Log_1:
    dst.set_bool( src.to_bool() );
    break;
  default:
    dst.set_string( std::string( 1, src.to_char() ) );
    break;
  }
  return true;
}

DEFINE_UNPACK_( sc_dt::sc_logic )
{
  if( src.is_bool() )
  {
    dst = src.get_bool();
    return true;
  }
  else if( src.is_int() )
  {
    dst = sc_dt::sc_logic( src.get_int() );
    return true;
  }
  else if( src.is_string() && src.get_string().size() == 1 )
  {
    switch( src.get_string()[0] )
    {
      case '0': dst = sc_dt::SC_LOGIC_0; return true;
      case '1': dst = sc_dt::SC_LOGIC_1; return true;
      case 'z': /* fallthrough */
      case 'Z': dst = sc_dt::SC_LOGIC_Z; return true;
      case 'x': /* fallthrough */
      case 'X': dst = sc_dt::SC_LOGIC_X; return true;
      default:  /* nothing */ ;
    }
  }
  return false;
}

DEFINE_PACK_( sc_dt::sc_int_base )
{
  dst.set_int64( src );
  return true;
}

DEFINE_UNPACK_( sc_dt::sc_int_base )
{
  if( src.is_int64() )
    dst = src.get_int64();
  else if( src.is_string() )
    dst = src.get_string().c_str();
  else
    return false;

  return true;
}

DEFINE_PACK_( sc_dt::sc_uint_base )
{
  dst.set_uint64( src );
  return true;
}

DEFINE_UNPACK_( sc_dt::sc_uint_base )
{
  if( src.is_uint64() )
    dst = src.get_uint64();
  else if( src.is_string() )
    dst = src.get_string().c_str();
  else
    return false;

  return true;
}

DEFINE_PACK_( sc_dt::sc_signed )
{
  if( src.length() <= 64 )
    dst.set_int64( src.to_int64() );
  else
    dst.set_string( src.to_string() );

  return true;
}

DEFINE_UNPACK_( sc_dt::sc_signed )
{
  ///@todo add bounds checks
  if( src.is_int64() )
    dst = src.get_int64();
  else if( src.is_string() )
    dst = src.get_string().c_str();
  else
    return false;

  return true;
}

DEFINE_PACK_( sc_dt::sc_unsigned )
{
  if( src.length() <= 64 )
    dst.set_uint64( src.to_uint64() );
  else
    dst.set_string( src.to_string() );

  return true;
}

DEFINE_UNPACK_( sc_dt::sc_unsigned )
{
  ///@todo add bounds checks
  if( src.is_uint64() )
    dst = src.get_uint64();
  else if( src.is_string() )
    dst = src.get_string().c_str();
  else
    return false;

  return true;
}

DEFINE_PACK_( sc_dt::sc_bv_base )
{
  dst.set_string( src.to_string() );
  return true;
}

DEFINE_UNPACK_( sc_dt::sc_bv_base )
{
  ///@todo add bounds checks
  if( !src.is_string() )
    return false;

  dst = src.get_string().c_str();
  return true;
}

DEFINE_PACK_( sc_dt::sc_lv_base )
{
  dst.set_string( src.to_string() );
  return true;
}

DEFINE_UNPACK_( sc_dt::sc_lv_base )
{
  ///@todo add bounds checks
  if( !src.is_string() )
    return false;

  dst = src.get_string().c_str();
  return true;
}

#endif // CCI_DOXYGEN_IS_RUNNING
CCI_CLOSE_NAMESPACE_
