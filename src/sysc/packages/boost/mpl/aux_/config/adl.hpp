
#ifndef SC_BOOST_MPL_AUX_CONFIG_ADL_HPP_INCLUDED
#define SC_BOOST_MPL_AUX_CONFIG_ADL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /Users/acg/CVSROOT/systemc-2.3/src/sysc/packages/boost/mpl/aux_/config/adl.hpp,v $
// $Date: 2009/10/14 19:11:07 $
// $Revision: 1.2 $

#include <sysc/packages/boost/mpl/aux_/config/msvc.hpp>
#include <sysc/packages/boost/mpl/aux_/config/intel.hpp>
#include <sysc/packages/boost/mpl/aux_/config/gcc.hpp>
#include <sysc/packages/boost/mpl/aux_/config/workaround.hpp>

// agurt, 25/apr/04: technically, the ADL workaround is only needed for GCC,
// but putting everything expect public, user-specializable metafunctions into
// a separate global namespace has a nice side effect of reducing the length 
// of template instantiation symbols, so we apply the workaround on all 
// platforms that can handle it

#if !defined(SC_BOOST_MPL_CFG_NO_ADL_BARRIER_NAMESPACE) \
    && (   SC_BOOST_WORKAROUND(SC_BOOST_MSVC, SC_BOOST_TESTED_AT(1400)) \
        || SC_BOOST_WORKAROUND(__BORLANDC__, < 0x600) \
        || SC_BOOST_WORKAROUND(__DMC__, SC_BOOST_TESTED_AT(0x840)) \
        || SC_BOOST_WORKAROUND(__MWERKS__, SC_BOOST_TESTED_AT(0x3202)) \
        || SC_BOOST_WORKAROUND(SC_BOOST_INTEL_CXX_VERSION, SC_BOOST_TESTED_AT(810)) \
        )

#   define SC_BOOST_MPL_CFG_NO_ADL_BARRIER_NAMESPACE

#endif

#endif // SC_BOOST_MPL_AUX_CONFIG_ADL_HPP_INCLUDED
