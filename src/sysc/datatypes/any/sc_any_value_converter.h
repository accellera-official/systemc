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
#ifdef _MSC_VER // deliberately outside of #include guards
#pragma warning(push)
#pragma warning(disable: 4231) // extern template
#endif // _MSC_VER

#define SC_ANY_VALUE_HAS_CONVERTER_(Type) \
  template<> struct sc_any_value_has_converter<Type> \
    : sc_meta::true_type {}

// --------------------------------------------------------------------------
#ifndef SC_DATATYPES_ANY_VALUE_CONVERTER_H_INCLUDED_
#define SC_DATATYPES_ANY_VALUE_CONVERTER_H_INCLUDED_
/**
 * @file   sc_any_value_converter.h
 * @brief  conversions from and to an @ref sc_dt::sc_any_value
 * @author Philipp A. Hartmann, OFFIS/Intel
 */

#include "sysc/kernel/sc_cmnhdr.h"
#include "sysc/datatypes/any/sc_any_value.h"

#include <cstring> // std::strncpy

namespace sc_core { class sc_time; }

namespace sc_dt {

/**
 * @brief opt-in for @ref sc_any_value_converter conversion
 *
 * To use the sc_any_value_converter based type conversion
 * for custom types to and from sc_any_value, specialize
 * this class via
 * @code
 * class my_type; // can be incomplete
 * namespace sc_dt {
 *   template<> struct sc_any_value_has_converter<my_type>
 *     : sc_meta::true_type {};
 * } // namespace sc_dt
 * @endcode
 *
 * @see sc_any_value_converter, sc_any_value_pack, sc_any_value_unpack
 */
template<typename T>
struct sc_any_value_has_converter : sc_meta::false_type {};

/**
 * @class sc_any_value_converter
 * @brief traits class for sc_any_value conversions
 * @tparam T C++ datatype to convert to/from @ref sc_any_value
 *
 * Value conversion from a specific C++ type to and from @ref sc_any_value
 * can be implemented by this traits class, providing the two conversion
 * functions @ref pack and @ref unpack.
 * Both functions return @c true upon success and @c false otherwise.
 * In case of a failing conversion, it is recommended to leave the given
 * destination object @c dst untouched.
 *
 * @note By default, the primary template is not implemented to
 *       enable instantiations with incomplete types.
 *
 * @note To enable type conversion via the sc_any_value_converter class,
 *       an opt-in via a specialization of @ref sc_any_value_has_converter
 *       is required.
 *
 * @b Example
 * @code
 * struct my_int { int value; };
 *
 * namespace sc_dt {
 * template<> struct sc_any_value_has_converter<my_int> : sc_meta::true_type {};
 * template<> bool
 * sc_any_value_converter<my_int>::pack( sc_any_value::reference dst, type const & src )
 * {
 *    dst.set_int( src.value );
 *    return true;
 * }
 * template<> bool
 * sc_any_value_converter<my_int>::unpack( type & dst, sc_any_value::const_reference src )
 * {
 *    if( ! src.is_int() ) return false;
 *    dst.value  = src.get_int();
 *    return true;
 * }
 * } // namespace sc_dt
 * @endcode
 *
 * To @em disable conversion support for a given type, you can refer
 * to the helper template @ref sc_any_value_converter_disabled.
 *
 * <b> Predefined converters </b>
 * @li C++ fundamental types:
 *     @c bool, @c (unsigned) @c char, @c (unsigned) @c short,
 *     @c (unsigned) @c int, @c (unsigned) @c long,
 *     @c float, @c double
 * @li @c std::string, @c sc_core::sc_string_view
 * @li SystemC data types:
 *     @c sc_dt::int64, @c sc_dt::uint64, @c sc_dt::sc_logic,
 *     @c sc_dt::sc_bv_base, @c sc_dt::sc_lv_base,
 *     @c sc_dt::sc_int_base, @c sc_dt::sc_uint_base,
 *     @c sc_dt::sc_signed, @c sc_dt::sc_unsigned,
 *     @c sc_dt::sc_fxval, @c sc_dt::sc_fxval_fast,
 *     @c sc_dt::sc_fix, @c sc_dt::sc_fix_fast,
 *     @c sc_dt::sc_ufix, @c sc_dt::sc_ufix_fast,
 *     and their templated variants
 * @li SystemC time (@c sc_core::sc_time)
 * @li Fixed-size C++ arrays and @c std::vector<T> of supported types
 */
template<typename T>
struct sc_any_value_converter
{
  typedef T type; ///< common type alias
  /// enable conversion via sc_any_value_converter
  static const bool enabled = sc_any_value_has_converter<T>::value;
  /// convert from \ref type value to a sc_any_value
  static bool pack( sc_any_value::reference dst, type const & src );
  /// convert from sc_any_value to a \ref type value
  static bool unpack( type & dst, sc_any_value::const_reference src );
};

// ---------------------------------------------------------------------------
/**
 * @brief helper to disable sc_any_value conversion for a given type
 * @tparam T type without sc_any_value conversions
 *
 * In order to disable the conversion from/to a sc_any_value for a given type
 * @c T during @em run-time, you can simply inherit from this helper in
 * the specialization of sc_any_value_converter:
 * @code
 * struct my_type { ... };
 * namespace sc_dt {
 * template<>
 * struct sc_any_value_converter<my_type>
 *   : sc_any_value_converter_disabled<my_type> {};
 * } // namespace sc_dt
 * @endcode
 */
template< typename T >
struct sc_any_value_converter_disabled
{
  typedef T type;
  static const bool enabled = true;
  static bool pack( sc_any_value::reference, T const & )      { return false; }
  static bool unpack( type &, sc_any_value::const_reference ) { return false; }
};

// ---------------------------------------------------------------------------

#ifndef SC_DOXYGEN_IS_RUNNING
# define SC_ANY_VALUE_REQUIRES_CONVERTER_(T) \
   typename sc_meta::enable_if< sc_any_value_converter<T>::enabled, bool>::type
#else
# define SC_ANY_VALUE_REQUIRES_CONVERTER_(T) bool
#endif // SC_HIDDEN_FROM_DOXYGEN

/**
 * @brief Try converting from a typed value to @ref sc_any_value::reference
 *
 * This function implements the @ref sc_any_value_converter based
 * conversion from typed values to @ref sc_any_value.
 *
 * @note Only participates in overload resolution, iff
 *       @ref sc_any_value_has_converter inherits from @c true_type.
 *
 * To add support for custom types, models can either specialize
 * @ref sc_any_value_has_converter and implement the required
 * functions in @ref sc_any_value_converter, or add a custom overload
 * for @c sc_any_value_pack directly.
 *
 * @see sc_any_value::set, sc_any_value_converter, sc_any_value_has_converter
 */
template<typename T>
inline
SC_ANY_VALUE_REQUIRES_CONVERTER_(T)
sc_any_value_pack( sc_any_value::reference dst, T const & src )
  { return sc_any_value_converter<T>::pack( dst, src ); }

/**
 * @brief Try converting from an @ref sc_any_value::const_reference to a typed value
 *
 * This function implements the @ref sc_any_value_converter based
 * conversion from @ref sc_any_value and typed values.
 *
 * @note Only participates in overload resolution, iff
 *       @ref sc_any_value_has_converter inherits from @c true_type.
 *
 * To add support for custom types, models can either specialize
 * @ref sc_any_value_has_converter and implement the required
 * functions in @ref sc_any_value_converter, or add a custom overload
 * for @c sc_any_value_unpack directly.
 *
 * @see sc_any_value::get, sc_any_value_converter, sc_any_value_has_converter
 */
template<typename T>
inline
SC_ANY_VALUE_REQUIRES_CONVERTER_(T)
sc_any_value_unpack( T& dst, sc_any_value::const_reference src )
  { return sc_any_value_converter<T>::unpack( dst, src ); }

#undef SC_ANY_VALUE_REQUIRES_CONVERTER_
//@cond SC_HIDDEN_FROM_DOXYGEN

// ---------------------------------------------------------------------------
/// helper to convert compatible types (implementation artefact)
template< typename T, typename U >
struct sc_any_value_delegate_converter
{
  typedef T type;
  typedef sc_any_value_converter<U> traits_type;

  static const bool enabled = traits_type::enabled;

  static bool pack( sc_any_value::reference dst, type const & src )
  {
    return traits_type::pack( dst, static_cast<U>(src) );
  }

  static bool unpack( type & dst, sc_any_value::const_reference src )
  {
      U u_dst;
      bool ret = traits_type::unpack( u_dst, src );
      if( ret )
        dst = static_cast<T>(u_dst);
      return ret;
  }
};

// C++ builtin types

SC_ANY_VALUE_HAS_CONVERTER_(bool);
SC_ANY_VALUE_HAS_CONVERTER_(int);
SC_ANY_VALUE_HAS_CONVERTER_(int64);
SC_ANY_VALUE_HAS_CONVERTER_(unsigned);
SC_ANY_VALUE_HAS_CONVERTER_(uint64);
SC_ANY_VALUE_HAS_CONVERTER_(double);
SC_ANY_VALUE_HAS_CONVERTER_(std::string);

#ifndef SC_BUILD_ANY_VALUE // defined in sc_any_value_converter.cpp
SC_TPLEXTERN_ template struct sc_any_value_converter<bool>;
SC_TPLEXTERN_ template struct sc_any_value_converter<int>;
SC_TPLEXTERN_ template struct sc_any_value_converter<int64>;
SC_TPLEXTERN_ template struct sc_any_value_converter<unsigned>;
SC_TPLEXTERN_ template struct sc_any_value_converter<uint64>;
SC_TPLEXTERN_ template struct sc_any_value_converter<double>;
SC_TPLEXTERN_ template struct sc_any_value_converter<std::string>;
#endif // SC_BUILD_ANY_VALUE

// related numerical types
// (without range checks for now)
#define SC_ANY_VALUE_CONVERTER_DERIVED_( UnderlyingType, SpecializedType ) \
  template<> \
  struct sc_any_value_converter<SpecializedType> \
    : sc_any_value_delegate_converter<SpecializedType, UnderlyingType > {}


SC_ANY_VALUE_CONVERTER_DERIVED_( int, char );
SC_ANY_VALUE_CONVERTER_DERIVED_( int, signed char );
SC_ANY_VALUE_CONVERTER_DERIVED_( int, short );
SC_ANY_VALUE_CONVERTER_DERIVED_( unsigned, unsigned char );
SC_ANY_VALUE_CONVERTER_DERIVED_( unsigned, unsigned short );
SC_ANY_VALUE_CONVERTER_DERIVED_( int64, long );
SC_ANY_VALUE_CONVERTER_DERIVED_( uint64, unsigned long );
SC_ANY_VALUE_CONVERTER_DERIVED_( double, float );

// ----------------------------------------------------------------------------
// C++ string literals

template<int N>
struct sc_any_value_converter<char[N]>
{
  typedef char type[N]; ///< common type alias
  static const bool enabled = true;

  static bool pack( sc_any_value::reference dst, type const & src )
  {
    dst.set_string( src );
    return true;
  }
  static bool unpack( type & dst, sc_any_value::const_reference src )
  {
    if( src.is_null() )
    {
      dst[0] = '\0'; // convert "null" to empty string
      return true;
    }
    if( !src.is_string() )
      return false;

    sc_zstring_view str = src.get_string();
    std::strncpy( dst, str.c_str(), N-1 );
    dst[N-1] = '\0';
    return ( str.size() <= N-1 );
  }
};

// ----------------------------------------------------------------------------
// C++ arrays

template<typename T, int N>
struct sc_any_value_converter<T[N]>
{
  typedef T type[N]; ///< common type alias
  static const bool enabled = true;

  static bool pack( sc_any_value::reference dst, type const & src )
  {
    sc_any_value_list ret;
    ret.reserve( N );

    for( size_t i = 0; i < N; ++i )
      ret.push_back( src[i] );
    ret.swap( dst.set_list() );
    return true;
  }
  static bool unpack( type & dst, sc_any_value::const_reference src )
  {
    if( !src.is_list() )
      return false;

    sc_any_value::const_list_reference lst = src.get_list();
    size_t i = 0;
    for( ; i < N && i < lst.size() && lst[i].try_get<T>( dst[i] ); ++i ) {}

    return ( i == lst.size() );
  }
};

// ----------------------------------------------------------------------------
// std::vector<T, Alloc>

template< typename T, typename Alloc >
struct sc_any_value_converter< std::vector<T,Alloc> >
{
  typedef std::vector<T,Alloc> type; ///< common type alias
  static const bool enabled = true;

  static bool pack( sc_any_value::reference dst, type const & src )
  {
    sc_any_value_list ret;
    ret.reserve( src.size() );

    for( size_t i = 0; i < src.size(); ++i )
      ret.push_back( src[i] );
    ret.swap( dst.set_list() );
    return true;
  }
  static bool unpack( type & dst, sc_any_value::const_reference src )
  {
    if( !src.is_list() )
      return false;

    sc_any_value::const_list_reference lst = src.get_list();
    type ret;
    T    cur;
    size_t i = 0;
    ret.reserve( lst.size() );
    for( ; i < lst.size() && lst[i].try_get(cur); ++i )
      ret.push_back( cur );

    return ( i == lst.size() ) ? ( dst.swap(ret), true) : false;
  }
};

// ----------------------------------------------------------------------------
// SystemC builtin types

class sc_bit;
class sc_logic;
class sc_int_base;
class sc_uint_base;
class sc_signed;
class sc_unsigned;
class sc_bv_base;
class sc_lv_base;
template<int N> class sc_int;
template<int N> class sc_uint;
template<int N> class sc_bigint;
template<int N> class sc_biguint;
template<int N> class sc_bv;
template<int N> class sc_lv;

SC_ANY_VALUE_HAS_CONVERTER_(sc_core::sc_string_view);
SC_ANY_VALUE_HAS_CONVERTER_(sc_core::sc_time);
SC_ANY_VALUE_HAS_CONVERTER_(sc_bit);
SC_ANY_VALUE_HAS_CONVERTER_(sc_logic);
SC_ANY_VALUE_HAS_CONVERTER_(sc_int_base);
SC_ANY_VALUE_HAS_CONVERTER_(sc_uint_base);
SC_ANY_VALUE_HAS_CONVERTER_(sc_signed);
SC_ANY_VALUE_HAS_CONVERTER_(sc_unsigned);
SC_ANY_VALUE_HAS_CONVERTER_(sc_bv_base);
SC_ANY_VALUE_HAS_CONVERTER_(sc_lv_base);

SC_ANY_VALUE_CONVERTER_DERIVED_( sc_core::sc_string_view, sc_core::sc_zstring_view );

#ifndef SC_BUILD_ANY_VALUE // defined in sc_any_value_converter.cpp
SC_TPLEXTERN_ template struct sc_any_value_converter<sc_core::sc_string_view>;
SC_TPLEXTERN_ template struct sc_any_value_converter<sc_core::sc_time>;
SC_TPLEXTERN_ template struct sc_any_value_converter<sc_bit>;
SC_TPLEXTERN_ template struct sc_any_value_converter<sc_logic>;
SC_TPLEXTERN_ template struct sc_any_value_converter<sc_int_base>;
SC_TPLEXTERN_ template struct sc_any_value_converter<sc_uint_base>;
SC_TPLEXTERN_ template struct sc_any_value_converter<sc_signed>;
SC_TPLEXTERN_ template struct sc_any_value_converter<sc_unsigned>;
SC_TPLEXTERN_ template struct sc_any_value_converter<sc_bv_base>;
SC_TPLEXTERN_ template struct sc_any_value_converter<sc_lv_base>;
#endif // SC_BUILD_ANY_VALUE

/// @see sc_any_value_converter primary template
template<int N>
struct sc_any_value_converter< sc_int<N> >
  : sc_any_value_converter< sc_int_base >
{
  typedef sc_int<N> type;
};

/// @see sc_any_value_converter primary template
template<int N>
struct sc_any_value_converter< sc_uint<N> >
  : sc_any_value_converter< sc_uint_base >
{
  typedef sc_uint<N> type;
};

/// @see sc_any_value_converter primary template
template<int N>
struct sc_any_value_converter< sc_bigint<N> >
  : sc_any_value_converter< sc_signed >
{
  typedef sc_bigint<N> type;
};

/// @see sc_any_value_converter primary template
template<int N>
struct sc_any_value_converter< sc_biguint<N> >
  : sc_any_value_converter< sc_unsigned >
{
  typedef sc_biguint<N> type;
};

/// @see sc_any_value_converter primary template
template<int N>
struct sc_any_value_converter< sc_bv<N> >
  : sc_any_value_converter< sc_bv_base >
{
  typedef sc_bv<N> type;
};

/// @see sc_any_value_converter primary template
template<int N>
struct sc_any_value_converter< sc_lv<N> >
  : sc_any_value_converter< sc_lv_base >
{
  typedef sc_lv<N> type;
};

//@endcond
} // namespace sc_dt

#endif // SC_DATATYPES_ANY_VALUE_CONVERTER_H_INCLUDED_

#if defined(SC_INCLUDE_FX) && !defined(SC_DATATYPES_ANY_VALUE_CONVERTER_H_INCLUDED_FX_)
#define SC_DATATYPES_ANY_VALUE_CONVERTER_H_INCLUDED_FX_

#include "sysc/datatypes/fx/sc_fxdefs.h"

//@cond SC_HIDDEN_FROM_DOXYGEN
namespace sc_dt {

class sc_fxval;
class sc_fxval_fast;
class sc_fix;
class sc_fix_fast;
class sc_ufix;
class sc_ufix_fast;
template<int W, int I, sc_q_mode Q, sc_o_mode O, int N > class sc_fixed;
template<int W, int I, sc_q_mode Q, sc_o_mode O, int N > class sc_fixed_fast;
template<int W, int I, sc_q_mode Q, sc_o_mode O, int N > class sc_ufixed;
template<int W, int I, sc_q_mode Q, sc_o_mode O, int N > class sc_ufixed_fast;

SC_ANY_VALUE_HAS_CONVERTER_(sc_fxval);
SC_ANY_VALUE_HAS_CONVERTER_(sc_fxval_fast);
SC_ANY_VALUE_HAS_CONVERTER_(sc_fix);
SC_ANY_VALUE_HAS_CONVERTER_(sc_fix_fast);
SC_ANY_VALUE_HAS_CONVERTER_(sc_ufix);
SC_ANY_VALUE_HAS_CONVERTER_(sc_ufix_fast);

#ifndef SC_BUILD_ANY_VALUE // defined in sc_any_value_converter.cpp
SC_TPLEXTERN_ template struct sc_any_value_converter<sc_fxval>;
SC_TPLEXTERN_ template struct sc_any_value_converter<sc_fxval_fast>;
SC_TPLEXTERN_ template struct sc_any_value_converter<sc_fix>;
SC_TPLEXTERN_ template struct sc_any_value_converter<sc_fix_fast>;
SC_TPLEXTERN_ template struct sc_any_value_converter<sc_ufix>;
SC_TPLEXTERN_ template struct sc_any_value_converter<sc_ufix_fast>;
#endif // SC_BUILD_ANY_VALUE

template<int W, int I, sc_q_mode Q, sc_o_mode O, int N >
struct sc_any_value_converter< sc_fixed<W,I,Q,O,N> >
  : sc_any_value_converter< sc_fix >
{
  typedef sc_fixed<W,I,Q,O,N> type;
};

template<int W, int I, sc_q_mode Q, sc_o_mode O, int N >
struct sc_any_value_converter< sc_fixed_fast<W,I,Q,O,N> >
  : sc_any_value_converter< sc_fix_fast >
{
  typedef sc_fixed_fast<W,I,Q,O,N> type;
};

template<int W, int I, sc_q_mode Q, sc_o_mode O, int N >
struct sc_any_value_converter< sc_ufixed<W,I,Q,O,N> >
  : sc_any_value_converter< sc_ufix >
{
  typedef sc_ufixed<W,I,Q,O,N> type;
};

template<int W, int I, sc_q_mode Q, sc_o_mode O, int N >
struct sc_any_value_converter< sc_ufixed_fast<W,I,Q,O,N> >
  : sc_any_value_converter< sc_ufix_fast >
{
  typedef sc_ufixed_fast<W,I,Q,O,N> type;
};

} // namespace sc_dt
//@endcond
#endif // SC_INCLUDE_FX && ! SC_DATATYPES_ANY_VALUE_CONVERTER_H_INCLUDED_FX_

#ifdef SC_ANY_VALUE_CONVERTER_DERIVED_
# undef SC_ANY_VALUE_CONVERTER_DERIVED_
#endif // SC_ANY_VALUE_CONVERTER_DERIVED_
#ifdef SC_ANY_VALUE_HAS_CONVERTER_
# undef SC_ANY_VALUE_HAS_CONVERTER_
#endif // SC_ANY_VALUE_HAS_CONVERTER_

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER
