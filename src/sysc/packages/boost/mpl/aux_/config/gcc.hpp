
#ifndef SC_BOOST_MPL_AUX_CONFIG_GCC_HPP_INCLUDED
#define SC_BOOST_MPL_AUX_CONFIG_GCC_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /Users/acg/CVSROOT/systemc-2.3/src/sysc/packages/boost/mpl/aux_/config/gcc.hpp,v $
// $Date: 2009/10/14 19:11:07 $
// $Revision: 1.2 $

#if defined(__GNUC__) && !defined(__EDG_VERSION__)
#   define SC_BOOST_MPL_CFG_GCC ((__GNUC__ << 8) | __GNUC_MINOR__)
#else
#   define SC_BOOST_MPL_CFG_GCC 0
#endif

#endif // SC_BOOST_MPL_AUX_CONFIG_GCC_HPP_INCLUDED
