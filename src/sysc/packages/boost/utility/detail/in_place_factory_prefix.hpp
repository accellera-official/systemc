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
#ifndef SC_BOOST_UTILITY_DETAIL_INPLACE_FACTORY_PREFIX_25AGO2003_HPP
#define SC_BOOST_UTILITY_DETAIL_INPLACE_FACTORY_PREFIX_25AGO2003_HPP

#include <sysc/packages/boost/config.hpp>
#include <sysc/packages/boost/preprocessor/repetition/enum.hpp>
#include <sysc/packages/boost/preprocessor/repetition/enum_params.hpp>
#include <sysc/packages/boost/preprocessor/repetition/enum_binary_params.hpp>
#include <sysc/packages/boost/preprocessor/cat.hpp>
#include <sysc/packages/boost/preprocessor/arithmetic/inc.hpp>
#include <sysc/packages/boost/preprocessor/punctuation/paren.hpp>
#include <sysc/packages/boost/preprocessor/facilities/empty.hpp>

#define SC_BOOST_DEFINE_INPLACE_FACTORY_CLASS_MEMBER_INIT(z,n,_) SC_BOOST_PP_CAT(m_a,n) SC_BOOST_PP_LPAREN() SC_BOOST_PP_CAT(a,n) SC_BOOST_PP_RPAREN()
#define SC_BOOST_DEFINE_INPLACE_FACTORY_CLASS_MEMBER_DECL(z,n,_) SC_BOOST_PP_CAT(A,n) const& SC_BOOST_PP_CAT(m_a,n);
#define SC_BOOST_DEFINE_INPLACE_FACTORY_CLASS_MEMBER_ARG(z,n,_)  SC_BOOST_PP_CAT(m_a,n)

#define SC_BOOST_MAX_INPLACE_FACTORY_ARITY 10

#undef SC_BOOST_UTILITY_DETAIL_INPLACE_FACTORY_SUFFIX_25AGO2003_HPP

#endif

