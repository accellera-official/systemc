// Copyright (C) 2003, Fernando Luis Cacciola Carballal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/lib/optional for documentation.
//
// You are welcome to contact the author at:
//  fernando_cacciola@hotmail.com
//
#ifndef SC_BOOST_UTILITY_TYPED_INPLACE_FACTORY_25AGO2003_HPP
#define SC_BOOST_UTILITY_TYPED_INPLACE_FACTORY_25AGO2003_HPP

#include <sysc/packages/boost/utility/detail/in_place_factory_prefix.hpp>

namespace sc_boost {

class typed_in_place_factory_base {} ;

#define SC_BOOST_DEFINE_TYPED_INPLACE_FACTORY_CLASS(z,n,_) \
template< class T, SC_BOOST_PP_ENUM_PARAMS(SC_BOOST_PP_INC(n),class A) > \
class SC_BOOST_PP_CAT(typed_in_place_factory, SC_BOOST_PP_INC(n) ) : public typed_in_place_factory_base \
{ \
public: \
\
  typedef T value_type ; \
\
  SC_BOOST_PP_CAT(typed_in_place_factory, SC_BOOST_PP_INC(n) ) ( SC_BOOST_PP_ENUM_BINARY_PARAMS(SC_BOOST_PP_INC(n),A,const& a) ) \
    : \
    SC_BOOST_PP_ENUM( SC_BOOST_PP_INC(n), SC_BOOST_DEFINE_INPLACE_FACTORY_CLASS_MEMBER_INIT, _ ) \
  {} \
\
  void apply ( void* address ) const \
  { \
    new ( address ) T ( SC_BOOST_PP_ENUM_PARAMS( SC_BOOST_PP_INC(n), m_a ) ) ; \
  } \
\
  SC_BOOST_PP_REPEAT( SC_BOOST_PP_INC(n), SC_BOOST_DEFINE_INPLACE_FACTORY_CLASS_MEMBER_DECL, _) \
} ; \
\
template< class T, SC_BOOST_PP_ENUM_PARAMS(SC_BOOST_PP_INC(n),class A) > \
SC_BOOST_PP_CAT(typed_in_place_factory, SC_BOOST_PP_INC(n) ) < T , SC_BOOST_PP_ENUM_PARAMS( SC_BOOST_PP_INC(n), A ) > \
in_place ( SC_BOOST_PP_ENUM_BINARY_PARAMS(SC_BOOST_PP_INC(n),A, const& a) ) \
{ \
  return SC_BOOST_PP_CAT(typed_in_place_factory, SC_BOOST_PP_INC(n) ) < T, SC_BOOST_PP_ENUM_PARAMS( SC_BOOST_PP_INC(n), A ) > \
           ( SC_BOOST_PP_ENUM_PARAMS( SC_BOOST_PP_INC(n), a ) ) ; \
} ; \

SC_BOOST_PP_REPEAT( SC_BOOST_MAX_INPLACE_FACTORY_ARITY, SC_BOOST_DEFINE_TYPED_INPLACE_FACTORY_CLASS, SC_BOOST_PP_EMPTY() )

} // namespace sc_boost

#include <sysc/packages/boost/utility/detail/in_place_factory_suffix.hpp>

#endif

