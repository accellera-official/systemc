//  (C) Copyright John Maddock 2001 - 2003.
//  (C) Copyright Jens Maurer 2001.
//  (C) Copyright Peter Dimov 2001.
//  (C) Copyright David Abrahams 2002.
//  (C) Copyright Guillaume Melquiond 2003.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  Dinkumware standard library config:

#if !defined(_YVALS) && !defined(_CPPLIB_VER)
#include <utility>
#if !defined(_YVALS) && !defined(_CPPLIB_VER)
#error This is not the Dinkumware lib!
#endif
#endif


#if defined(_CPPLIB_VER) && (_CPPLIB_VER >= 306)
   // full dinkumware 3.06 and above
   // fully conforming provided the compiler supports it:
#  if !(defined(_GLOBAL_USING) && (_GLOBAL_USING+0 > 0)) && !defined(__BORLANDC__) && !defined(_STD) && !(defined(__ICC) && (__ICC >= 700))   // can be defined in yvals.h
#     define SC_BOOST_NO_STDC_NAMESPACE
#  endif
#  if !(defined(_HAS_MEMBER_TEMPLATES_REBIND) && (_HAS_MEMBER_TEMPLATES_REBIND+0 > 0)) && !(defined(_MSC_VER) && (_MSC_VER > 1300)) && defined(SC_BOOST_MSVC)
#     define SC_BOOST_NO_STD_ALLOCATOR
#  endif
#  define SC_BOOST_HAS_PARTIAL_STD_ALLOCATOR
#  if defined(SC_BOOST_MSVC) && (SC_BOOST_MSVC < 1300)
      // if this lib version is set up for vc6 then there is no std::use_facet:
#     define SC_BOOST_NO_STD_USE_FACET
#     define SC_BOOST_HAS_TWO_ARG_USE_FACET
      // C lib functions aren't in namespace std either:
#     define SC_BOOST_NO_STDC_NAMESPACE
      // and nor is <exception>
#     define SC_BOOST_NO_EXCEPTION_STD_NAMESPACE
#  endif
// There's no numeric_limits<long long> support unless _LONGLONG is defined:
#  if !defined(_LONGLONG) && (_CPPLIB_VER <= 310)
#     define SC_BOOST_NO_MS_INT64_NUMERIC_LIMITS
#  endif
// 3.06 appears to have (non-sgi versions of) <hash_set> & <hash_map>,
// and no <slist> at all
#else
#  define SC_BOOST_MSVC_STD_ITERATOR 1
#  define SC_BOOST_NO_STD_ITERATOR
#  define SC_BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
#  define SC_BOOST_NO_STD_ALLOCATOR
#  define SC_BOOST_NO_STDC_NAMESPACE
#  define SC_BOOST_NO_STD_USE_FACET
#  define SC_BOOST_NO_STD_OUTPUT_ITERATOR_ASSIGN
#  define SC_BOOST_HAS_MACRO_USE_FACET
#  ifndef _CPPLIB_VER
      // Updated Dinkum library defines this, and provides
      // its own min and max definitions.
#     define SC_BOOST_NO_STD_MIN_MAX
#     define SC_BOOST_NO_MS_INT64_NUMERIC_LIMITS
#  endif
#endif

//
// std extension namespace is stdext for vc7.1 and later, 
// the same applies to other compilers that sit on top
// of vc7.1 (Intel and Comeau):
//
#if defined(_MSC_VER) && (_MSC_VER >= 1310) && !defined(__BORLANDC__)
#  define SC_BOOST_STD_EXTENSION_NAMESPACE stdext
#endif


#if (defined(_MSC_VER) && (_MSC_VER <= 1300) && !defined(__BORLANDC__)) || !defined(_CPPLIB_VER) || (_CPPLIB_VER < 306)
   // if we're using a dinkum lib that's
   // been configured for VC6/7 then there is
   // no iterator traits (true even for icl)
#  define SC_BOOST_NO_STD_ITERATOR_TRAITS
#endif

#if defined(__ICL) && (__ICL < 800) && defined(_CPPLIB_VER) && (_CPPLIB_VER <= 310)
// Intel C++ chokes over any non-trivial use of <locale>
// this may be an overly restrictive define, but regex fails without it:
#  define SC_BOOST_NO_STD_LOCALE
#endif

#ifdef _CPPLIB_VER
#  define SC_BOOST_DINKUMWARE_STDLIB _CPPLIB_VER
#else
#  define SC_BOOST_DINKUMWARE_STDLIB 1
#endif

#ifdef _CPPLIB_VER
#  define SC_BOOST_STDLIB "Dinkumware standard library version " SC_BOOST_STRINGIZE(_CPPLIB_VER)
#else
#  define SC_BOOST_STDLIB "Dinkumware standard library version 1.x"
#endif









