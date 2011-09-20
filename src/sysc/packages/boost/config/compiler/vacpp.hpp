//  (C) Copyright John Maddock 2001 - 2003. 
//  (C) Copyright Toon Knapen 2001 - 2003. 
//  (C) Copyright Lie-Quan Lee 2001. 
//  (C) Copyright Markus Schöpflin 2002 - 2003. 
//  (C) Copyright Beman Dawes 2002 - 2003. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  Visual Age (IBM) C++ compiler setup:

#if __IBMCPP__ <= 501
#  define SC_BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#  define SC_BOOST_NO_MEMBER_FUNCTION_SPECIALIZATIONS
#endif

#if (__IBMCPP__ <= 502) 
// Actually the compiler supports inclass member initialization but it
// requires a definition for the class member and it doesn't recognize
// it as an integral constant expression when used as a template argument.
#  define SC_BOOST_NO_INCLASS_MEMBER_INITIALIZATION
#  define SC_BOOST_NO_INTEGRAL_INT64_T
#  define SC_BOOST_NO_MEMBER_TEMPLATE_KEYWORD
#endif

#if (__IBMCPP__ <= 600) || !defined(SC_BOOST_STRICT_CONFIG)
#  define SC_BOOST_NO_POINTER_TO_MEMBER_TEMPLATE_PARAMETERS
#  define SC_BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES 1
#endif

//
// On AIX thread support seems to be indicated by _THREAD_SAFE:
//
#ifdef _THREAD_SAFE
#  define SC_BOOST_HAS_THREADS
#endif

#define SC_BOOST_COMPILER "IBM Visual Age version " SC_BOOST_STRINGIZE(__IBMCPP__)

//
// versions check:
// we don't support Visual age prior to version 5:
#if __IBMCPP__ < 500
#error "Compiler not supported or configured - please reconfigure"
#endif
//
// last known and checked version is 600:
#if (__IBMCPP__ > 600)
#  if defined(SC_BOOST_ASSERT_CONFIG)
#     error "Unknown compiler version - please run the configure tests and report the results"
#  endif
#endif




