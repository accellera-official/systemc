
#ifndef SC_BOOST_MPL_INTEGRAL_C_TAG_HPP_INCLUDED
#define SC_BOOST_MPL_INTEGRAL_C_TAG_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /Users/acg/CVSROOT/systemc-2.3/src/sysc/packages/boost/mpl/integral_c_tag.hpp,v $
// $Date: 2009/10/14 19:11:02 $
// $Revision: 1.2 $


#include <sysc/packages/boost/mpl/aux_/adl_barrier.hpp>
#include <sysc/packages/boost/mpl/aux_/config/static_constant.hpp>

SC_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN
struct integral_c_tag { SC_BOOST_STATIC_CONSTANT(int, value = 0); };
SC_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE
SC_BOOST_MPL_AUX_ADL_BARRIER_DECL(integral_c_tag)

#endif // SC_BOOST_MPL_INTEGRAL_C_TAG_HPP_INCLUDED
