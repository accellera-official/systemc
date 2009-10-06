//  Boost config.hpp configuration header file  ------------------------------//

//  (C) Copyright John Maddock 2002. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/config for most recent version.

//  Boost config.hpp policy and rationale documentation has been moved to
//  http://www.boost.org/libs/config
//
//  CAUTION: This file is intended to be completely stable -
//           DO NOT MODIFY THIS FILE!
//

#ifndef SC_BOOST_CONFIG_HPP
#define SC_BOOST_CONFIG_HPP

// if we don't have a user config, then use the default location:
#if !defined(SC_BOOST_USER_CONFIG) && !defined(SC_BOOST_NO_USER_CONFIG)
#  define SC_BOOST_USER_CONFIG <sysc/packages/boost/config/user.hpp>
#endif
// include it first:
#ifdef SC_BOOST_USER_CONFIG
#  include SC_BOOST_USER_CONFIG
#endif

// if we don't have a compiler config set, try and find one:
#if !defined(SC_BOOST_COMPILER_CONFIG) && !defined(SC_BOOST_NO_COMPILER_CONFIG) && !defined(SC_BOOST_NO_CONFIG)
#  include <sysc/packages/boost/config/select_compiler_config.hpp>
#endif
// if we have a compiler config, include it now:
#ifdef SC_BOOST_COMPILER_CONFIG
#  include SC_BOOST_COMPILER_CONFIG
#endif

// if we don't have a std library config set, try and find one:
#if !defined(SC_BOOST_STDLIB_CONFIG) && !defined(SC_BOOST_NO_STDLIB_CONFIG) && !defined(SC_BOOST_NO_CONFIG)
#  include <sysc/packages/boost/config/select_stdlib_config.hpp>
#endif
// if we have a std library config, include it now:
#ifdef SC_BOOST_STDLIB_CONFIG
#  include SC_BOOST_STDLIB_CONFIG
#endif

// if we don't have a platform config set, try and find one:
#if !defined(SC_BOOST_PLATFORM_CONFIG) && !defined(SC_BOOST_NO_PLATFORM_CONFIG) && !defined(SC_BOOST_NO_CONFIG)
#  include <sysc/packages/boost/config/select_platform_config.hpp>
#endif
// if we have a platform config, include it now:
#ifdef SC_BOOST_PLATFORM_CONFIG
#  include SC_BOOST_PLATFORM_CONFIG
#endif

// get config suffix code:
#include <sysc/packages/boost/config/suffix.hpp>

#endif  // SC_BOOST_CONFIG_HPP











