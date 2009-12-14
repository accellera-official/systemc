
#ifndef SC_BOOST_MPL_BOOL_FWD_HPP_INCLUDED
#define SC_BOOST_MPL_BOOL_FWD_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /Users/acg/CVSROOT/systemc-2.3/src/sysc/packages/boost/mpl/bool_fwd.hpp,v $
// $Date: 2009/10/14 19:11:02 $
// $Revision: 1.2 $

#include <sysc/packages/boost/mpl/aux_/adl_barrier.hpp>

SC_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN

template< bool C_ > struct bool_;

// shorcuts
typedef bool_<true> true_;
typedef bool_<false> false_;

SC_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE

SC_BOOST_MPL_AUX_ADL_BARRIER_DECL(bool_)
SC_BOOST_MPL_AUX_ADL_BARRIER_DECL(true_)
SC_BOOST_MPL_AUX_ADL_BARRIER_DECL(false_)

#endif // SC_BOOST_MPL_BOOL_FWD_HPP_INCLUDED
