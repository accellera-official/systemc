//  (C) Copyright John Maddock 2001. 
//  (C) Copyright Darin Adler 2001. 
//  (C) Copyright Peter Dimov 2001. 
//  (C) Copyright David Abrahams 2001 - 2002. 
//  (C) Copyright Beman Dawes 2001 - 2003. 
//  (C) Copyright Stefan Slapeta 2004. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  Metrowerks C++ compiler setup:

// locale support is disabled when linking with the dynamic runtime
#   ifdef _MSL_NO_LOCALE
#     define SC_BOOST_NO_STD_LOCALE
#   endif 

#   if __MWERKS__ <= 0x2301  // 5.3
#     define SC_BOOST_NO_FUNCTION_TEMPLATE_ORDERING
#     define SC_BOOST_NO_POINTER_TO_MEMBER_CONST
#     define SC_BOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS
#     define SC_BOOST_NO_MEMBER_TEMPLATE_KEYWORD
#   endif

#   if __MWERKS__ <= 0x2401  // 6.2
//#     define SC_BOOST_NO_FUNCTION_TEMPLATE_ORDERING
#   endif

#   if(__MWERKS__ <= 0x2407)  // 7.x
#     define SC_BOOST_NO_MEMBER_FUNCTION_SPECIALIZATIONS
#     define SC_BOOST_NO_UNREACHABLE_RETURN_DETECTION
#   endif

#   if(__MWERKS__ <= 0x3003)  // 8.x
#     define SC_BOOST_NO_SFINAE
#    endif

// the "|| !defined(SC_BOOST_STRICT_CONFIG)" part should apply to the last
// tested version *only*:
#   if(__MWERKS__ <= 0x3206) || !defined(SC_BOOST_STRICT_CONFIG) // 9.5
#     define SC_BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#     define SC_BOOST_NO_IS_ABSTRACT
#    endif

#if !__option(wchar_type)
#   define SC_BOOST_NO_INTRINSIC_WCHAR_T
#endif

#if !__option(exceptions)
#   define SC_BOOST_NO_EXCEPTIONS
#endif

#if (__INTEL__ && _WIN32) || (__POWERPC__ && macintosh)
#   if __MWERKS__ == 0x3000
#     define SC_BOOST_COMPILER_VERSION 8.0
#   elif __MWERKS__ == 0x3001
#     define SC_BOOST_COMPILER_VERSION 8.1
#   elif __MWERKS__ == 0x3002
#     define SC_BOOST_COMPILER_VERSION 8.2
#   elif __MWERKS__ == 0x3003
#     define SC_BOOST_COMPILER_VERSION 8.3
#   elif __MWERKS__ == 0x3200
#     define SC_BOOST_COMPILER_VERSION 9.0
#   elif __MWERKS__ == 0x3201
#     define SC_BOOST_COMPILER_VERSION 9.1
#   elif __MWERKS__ == 0x3202
#     define SC_BOOST_COMPILER_VERSION 9.2
#   elif __MWERKS__ == 0x3204
#     define SC_BOOST_COMPILER_VERSION 9.3
#   elif __MWERKS__ == 0x3205
#     define SC_BOOST_COMPILER_VERSION 9.4
#   elif __MWERKS__ == 0x3206
#     define SC_BOOST_COMPILER_VERSION 9.5
#   else
#     define SC_BOOST_COMPILER_VERSION __MWERKS__
#   endif
#else
#  define SC_BOOST_COMPILER_VERSION __MWERKS__
#endif

#define SC_BOOST_COMPILER "Metrowerks CodeWarrior C++ version " SC_BOOST_STRINGIZE(SC_BOOST_COMPILER_VERSION)

//
// versions check:
// we don't support Metrowerks prior to version 5.3:
#if __MWERKS__ < 0x2301
#  error "Compiler not supported or configured - please reconfigure"
#endif
//
// last known and checked version:
#if (__MWERKS__ > 0x3205)
#  if defined(SC_BOOST_ASSERT_CONFIG)
#     error "Unknown compiler version - please run the configure tests and report the results"
#  endif
#endif







