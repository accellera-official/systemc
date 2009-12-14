
#ifndef SC_BOOST_MPL_BOOL_HPP_INCLUDED
#define SC_BOOST_MPL_BOOL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /Users/acg/CVSROOT/systemc-2.3/src/sysc/packages/boost/mpl/bool.hpp,v $
// $Date: 2009/10/14 19:11:02 $
// $Revision: 1.2 $

#include <sysc/packages/boost/mpl/bool_fwd.hpp>
#include <sysc/packages/boost/mpl/integral_c_tag.hpp>
#include <sysc/packages/boost/mpl/aux_/config/static_constant.hpp>

SC_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN

template< bool C_ > struct bool_
{
    SC_BOOST_STATIC_CONSTANT(bool, value = C_);
    typedef integral_c_tag tag;
    typedef bool_ type;
    typedef bool value_type;
    operator bool() const { return this->value; }
};

#if !defined(SC_BOOST_NO_INCLASS_MEMBER_INITIALIZATION)
template< bool C_ >
bool const bool_<C_>::value;
#endif

SC_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE

#endif // SC_BOOST_MPL_BOOL_HPP_INCLUDED
