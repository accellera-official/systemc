//  (C) Copyright John Maddock 2001 - 2003. 
//  (C) Copyright Darin Adler 2001 - 2002. 
//  (C) Copyright Peter Dimov 2001. 
//  (C) Copyright Aleksey Gurtovoy 2002. 
//  (C) Copyright David Abrahams 2002 - 2003. 
//  (C) Copyright Beman Dawes 2002 - 2003. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  Microsoft Visual C++ compiler setup:

#define SC_BOOST_MSVC _MSC_VER

// turn off the warnings before we #include anything
#pragma warning( disable : 4503 ) // warning: decorated name length exceeded

#if _MSC_VER < 1300  // 1200 == VC++ 6.0, 1200-1202 == eVC++4
#  pragma warning( disable : 4786 ) // ident trunc to '255' chars in debug info
#  define SC_BOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS
#  define SC_BOOST_NO_VOID_RETURNS
#  define SC_BOOST_NO_EXCEPTION_STD_NAMESPACE
   // disable min/max macro defines on vc6:
   //
#endif

#if (_MSC_VER <= 1300)  // 1300 == VC++ 7.0

#  if !defined(_MSC_EXTENSIONS) && !defined(SC_BOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS)      // VC7 bug with /Za
#    define SC_BOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS
#  endif

#  define SC_BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS
#  define SC_BOOST_NO_INCLASS_MEMBER_INITIALIZATION
#  define SC_BOOST_NO_PRIVATE_IN_AGGREGATE
#  define SC_BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
#  define SC_BOOST_NO_INTEGRAL_INT64_T
#  define SC_BOOST_NO_DEDUCED_TYPENAME
#  define SC_BOOST_NO_USING_DECLARATION_OVERLOADS_FROM_TYPENAME_BASE

//    VC++ 6/7 has member templates but they have numerous problems including
//    cases of silent failure, so for safety we define:
#  define SC_BOOST_NO_MEMBER_TEMPLATES
//    For VC++ experts wishing to attempt workarounds, we define:
#  define SC_BOOST_MSVC6_MEMBER_TEMPLATES

#  define SC_BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#  define SC_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#  define SC_BOOST_NO_CV_VOID_SPECIALIZATIONS
#  define SC_BOOST_NO_FUNCTION_TEMPLATE_ORDERING
#  define SC_BOOST_NO_USING_TEMPLATE
#  define SC_BOOST_NO_SWPRINTF
#  define SC_BOOST_NO_TEMPLATE_TEMPLATES
#  define SC_BOOST_NO_SFINAE
#  define SC_BOOST_NO_POINTER_TO_MEMBER_TEMPLATE_PARAMETERS
#  define SC_BOOST_NO_IS_ABSTRACT
// TODO: what version is meant here? Have there really been any fixes in cl 12.01 (as e.g. shipped with eVC4)?
#  if (_MSC_VER > 1200)
#     define SC_BOOST_NO_MEMBER_FUNCTION_SPECIALIZATIONS
#  endif

#endif

#if _MSC_VER < 1310 // 1310 == VC++ 7.1
#  define SC_BOOST_NO_SWPRINTF
#endif

#if _MSC_VER <= 1400  // 1400 == VC++ 8.0
#  define SC_BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#endif

#ifndef _NATIVE_WCHAR_T_DEFINED
#  define SC_BOOST_NO_INTRINSIC_WCHAR_T
#endif

#ifdef _WIN32_WCE
#  define SC_BOOST_NO_THREADEX
#  define SC_BOOST_NO_GETSYSTEMTIMEASFILETIME
#endif

//   
// check for exception handling support:   
#ifndef _CPPUNWIND   
#  define SC_BOOST_NO_EXCEPTIONS   
#endif 

//
// __int64 support:
//
#if (_MSC_VER >= 1200)
#   define SC_BOOST_HAS_MS_INT64
#endif
#if (_MSC_VER >= 1310) && defined(_MSC_EXTENSIONS)
#   define SC_BOOST_HAS_LONG_LONG
#endif
//
// disable Win32 API's if compiler extentions are
// turned off:
//
#ifndef _MSC_EXTENSIONS
#  define SC_BOOST_DISABLE_WIN32
#endif

//
// all versions support __declspec:
//
#define SC_BOOST_HAS_DECLSPEC
//
// prefix and suffix headers:
//
#ifndef SC_BOOST_ABI_PREFIX
#  define SC_BOOST_ABI_PREFIX "sysc/packages/boost/config/abi/msvc_prefix.hpp"
#endif
#ifndef SC_BOOST_ABI_SUFFIX
#  define SC_BOOST_ABI_SUFFIX "sysc/packages/boost/config/abi/msvc_suffix.hpp"
#endif

// TODO:
// these things are mostly bogus. 1200 means version 12.0 of the compiler. The 
// artificial versions assigned to them only refer to the versions of some IDE
// these compilers have been shipped with, and even that is not all of it. Some
// were shipped with freely downloadable SDKs, others as crosscompilers in eVC.
// IOW, you can't use these 'versions' in any sensible way. Sorry.
# if defined(UNDER_CE)
#   if _MSC_VER < 1200
      // Note: these are so far off, they are not really supported
#   elif _MSC_VER < 1300 // eVC++ 4 comes with 1200-1202
#     define SC_BOOST_COMPILER_VERSION evc4.0
#     error unknown CE compiler
#   else
#     error unknown CE compiler
#   endif
# else
#   if _MSC_VER < 1200
      // Note: these are so far off, they are not really supported
#     define SC_BOOST_COMPILER_VERSION 5.0
#   elif _MSC_VER < 1300
#     define SC_BOOST_COMPILER_VERSION 6.0
#   elif _MSC_VER < 1310
#     define SC_BOOST_COMPILER_VERSION 7.0
#   elif _MSC_VER < 1400
#     define SC_BOOST_COMPILER_VERSION 7.1
#   elif _MSC_VER < 1500
#     define SC_BOOST_COMPILER_VERSION 8.0
#   elif _MSC_VER < 1600
#     define SC_BOOST_COMPILER_VERSION 9.0
#   elif _MSC_VER < 1700
#     define SC_BOOST_COMPILER_VERSION 10.0
#   elif _MSC_VER < 1800
#     define SC_BOOST_COMPILER_VERSION 11.0
#   elif _MSC_VER < 1900
#     define SC_BOOST_COMPILER_VERSION 12.0
#   elif _MSC_VER < 1910
#     define SC_BOOST_COMPILER_VERSION 14.0
#   elif _MSC_VER < 1920
#     define SC_BOOST_COMPILER_VERSION 14.1
#   elif _MSC_VER < 1930
#     define SC_BOOST_COMPILER_VERSION 14.2
#   else
#     define SC_BOOST_COMPILER_VERSION _MSC_VER
#   endif
# endif

#define SC_BOOST_COMPILER "Microsoft Visual C++ version " SC_BOOST_STRINGIZE(SC_BOOST_COMPILER_VERSION)

//
// versions check:
// we don't support Visual C++ prior to version 6:
#if _MSC_VER < 1200
#error "Compiler not supported or configured - please reconfigure"
#endif

//
// last known and checked version is 19.20.27508 (VC++ 2019 RC3):
#if (_MSC_VER > 1920)
#  if defined(BOOST_ASSERT_CONFIG)
#     error "Boost.Config is older than your current compiler version."
#  elif !defined(BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)
      //
      // Disabled as of March 2018 - the pace of VS releases is hard to keep up with
      // and in any case, we have relatively few defect macros defined now.
      // BOOST_PRAGMA_MESSAGE("Info: Boost.Config is older than your compiler version - probably nothing bad will happen - but you may wish to look for an updated Boost version. Define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE to suppress this message.")
#  endif
#endif
