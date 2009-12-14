
#ifndef SC_BOOST_MPL_AUX_CONFIG_STATIC_CONSTANT_HPP_INCLUDED
#define SC_BOOST_MPL_AUX_CONFIG_STATIC_CONSTANT_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /Users/acg/CVSROOT/systemc-2.3/src/sysc/packages/boost/mpl/aux_/config/static_constant.hpp,v $
// $Date: 2009/10/14 19:11:07 $
// $Revision: 1.2 $

#if !defined(SC_BOOST_MPL_PREPROCESSING_MODE)
// SC_BOOST_STATIC_CONSTANT is defined here:
#   include <sysc/packages/boost/config.hpp>
#else
// undef the macro for the preprocessing mode
#   undef SC_BOOST_STATIC_CONSTANT
#endif

#endif // SC_BOOST_MPL_AUX_CONFIG_STATIC_CONSTANT_HPP_INCLUDED
