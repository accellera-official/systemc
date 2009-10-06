//  (C) Copyright John Maddock 2001 - 2003.
//  (C) Copyright David Abrahams 2002 - 2003.
//  (C) Copyright Aleksey Gurtovoy 2002.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  Borland C++ compiler setup:

// Version 5.0 and below:
#   if __BORLANDC__ <= 0x0550
// Borland C++Builder 4 and 5:
#     define SC_BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#     if __BORLANDC__ == 0x0550
// Borland C++Builder 5, command-line compiler 5.5:
#       define SC_BOOST_NO_OPERATORS_IN_NAMESPACE
#     endif
#   endif

// Version 5.51 and below:
#if (__BORLANDC__ <= 0x551)
#  define SC_BOOST_NO_CV_SPECIALIZATIONS
#  define SC_BOOST_NO_CV_VOID_SPECIALIZATIONS
#  define SC_BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS
#  define SC_BOOST_NO_DEDUCED_TYPENAME
// workaround for missing WCHAR_MAX/WCHAR_MIN:
#include <climits>
#include <cwchar>
#ifndef WCHAR_MAX
#  define WCHAR_MAX 0xffff
#endif
#ifndef WCHAR_MIN
#  define WCHAR_MIN 0
#endif
#endif

// Version 7.0 (Kylix) and below:
#if (__BORLANDC__ <= 0x570)
#  define SC_BOOST_NO_SFINAE
#  define SC_BOOST_NO_INTEGRAL_INT64_T
#  define SC_BOOST_NO_DEPENDENT_NESTED_DERIVATIONS
#  define SC_BOOST_NO_PRIVATE_IN_AGGREGATE
#  define SC_BOOST_NO_USING_TEMPLATE
#  define SC_BOOST_BCB_PARTIAL_SPECIALIZATION_BUG
#  define SC_BOOST_NO_TEMPLATE_TEMPLATES
#  define SC_BOOST_NO_USING_DECLARATION_OVERLOADS_FROM_TYPENAME_BASE
#  define SC_BOOST_NO_MEMBER_TEMPLATE_FRIENDS
   // we shouldn't really need this - but too many things choke
   // without it, this needs more investigation:
#  define SC_BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS
#  define SC_BOOST_FUNCTION_SCOPE_USING_DECLARATION_BREAKS_ADL
#  define SC_BOOST_NO_IS_ABSTRACT
#  ifdef NDEBUG
      // fix broken <cstring> so that Boost.test works:
#     include <cstring>
#     undef strcmp
#  endif

//
// new bug in 5.61:
#if (__BORLANDC__ >= 0x561) && (__BORLANDC__ <= 0x570)
   // this seems to be needed by the command line compiler, but not the IDE:
#  define SC_BOOST_NO_MEMBER_FUNCTION_SPECIALIZATIONS
#endif

#  ifdef _WIN32
#     define SC_BOOST_NO_SWPRINTF
#  elif defined(linux) || defined(__linux__) || defined(__linux)
      // we should really be able to do without this
      // but the wcs* functions aren't imported into std::
#     define SC_BOOST_NO_STDC_NAMESPACE
      // _CPPUNWIND doesn't get automatically set for some reason:
#     pragma defineonoption SC_BOOST_CPPUNWIND -x
#  endif
#endif

//
// Post 0x561 we have long long and stdint.h:
#if __BORLANDC__ >= 0x561
#  ifndef __NO_LONG_LONG
#     define SC_BOOST_HAS_LONG_LONG
#  endif
   // On non-Win32 platforms let the platform config figure this out:
#  ifdef _WIN32
#      define SC_BOOST_HAS_STDINT_H
#  endif
#endif

// Borland C++Builder 6 defaults to using STLPort.  If _USE_OLD_RW_STL is
// defined, then we have 0x560 or greater with the Rogue Wave implementation
// which presumably has the std::DBL_MAX bug.
#if ((__BORLANDC__ >= 0x550) && (__BORLANDC__ < 0x560)) || defined(_USE_OLD_RW_STL)
// <climits> is partly broken, some macros define symbols that are really in
// namespace std, so you end up having to use illegal constructs like
// std::DBL_MAX, as a fix we'll just include float.h and have done with:
#include <float.h>
#endif
//
// __int64:
//
#if (__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__)
#  define SC_BOOST_HAS_MS_INT64
#endif
//
// check for exception handling support:
//
#if !defined(_CPPUNWIND) && !defined(SC_BOOST_CPPUNWIND) && !defined(__EXCEPTIONS)
#  define SC_BOOST_NO_EXCEPTIONS
#endif
//
// all versions have a <dirent.h>:
//
#ifndef __STRICT_ANSI__
#  define SC_BOOST_HAS_DIRENT_H
#endif
//
// all versions support __declspec:
//
#ifndef __STRICT_ANSI__
#  define SC_BOOST_HAS_DECLSPEC
#endif
//
// ABI fixing headers:
//
#if __BORLANDC__ < 0x600 // not implemented for version 6 compiler yet
#ifndef SC_BOOST_ABI_PREFIX
#  define SC_BOOST_ABI_PREFIX "sysc/packages/boost/config/abi/borland_prefix.hpp"
#endif
#ifndef SC_BOOST_ABI_SUFFIX
#  define SC_BOOST_ABI_SUFFIX "sysc/packages/boost/config/abi/borland_suffix.hpp"
#endif
#endif
//
// Disable Win32 support in ANSI mode:
//
#if __BORLANDC__ < 0x600
#  pragma defineonoption SC_BOOST_DISABLE_WIN32 -A
#elif defined(__STRICT_ANSI__)
#  define SC_BOOST_DISABLE_WIN32
#endif
//
// MSVC compatibility mode does some nasty things:
//
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
#  define SC_BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
#  define SC_BOOST_NO_VOID_RETURNS
#endif

#define SC_BOOST_COMPILER "Borland C++ version " SC_BOOST_STRINGIZE(__BORLANDC__)

//
// versions check:
// we don't support Borland prior to version 5.4:
#if __BORLANDC__ < 0x540
#  error "Compiler not supported or configured - please reconfigure"
#endif
//
// last known and checked version is 1536 (Builder X preview):
#if (__BORLANDC__ > 1536)
#  if defined(SC_BOOST_ASSERT_CONFIG)
#     error "Unknown compiler version - please run the configure tests and report the results"
#  else
#     pragma message( "Unknown compiler version - please run the configure tests and report the results")
#  endif
#endif








