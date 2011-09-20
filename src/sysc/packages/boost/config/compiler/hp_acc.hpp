//  (C) Copyright John Maddock 2001 - 2003. 
//  (C) Copyright Jens Maurer 2001 - 2003. 
//  (C) Copyright Aleksey Gurtovoy 2002. 
//  (C) Copyright David Abrahams 2002 - 2003. 
//  (C) Copyright Toon Knapen 2003. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  HP aCC C++ compiler setup:

#if (__HP_aCC <= 33100)
#    define SC_BOOST_NO_INTEGRAL_INT64_T
#    define SC_BOOST_NO_OPERATORS_IN_NAMESPACE
#  if !defined(_NAMESPACE_STD)
#     define SC_BOOST_NO_STD_LOCALE
#     define SC_BOOST_NO_STRINGSTREAM
#  endif
#endif

#if (__HP_aCC <= 33300)
// member templates are sufficiently broken that we disable them for now
#    define SC_BOOST_NO_MEMBER_TEMPLATES
#    define SC_BOOST_NO_DEPENDENT_NESTED_DERIVATIONS
#    define SC_BOOST_NO_USING_DECLARATION_OVERLOADS_FROM_TYPENAME_BASE
#endif

#if (__HP_aCC <= 33900) || !defined(SC_BOOST_STRICT_CONFIG)
#    define SC_BOOST_NO_UNREACHABLE_RETURN_DETECTION
#    define SC_BOOST_NO_TEMPLATE_TEMPLATES
#    define SC_BOOST_NO_SWPRINTF
#    define SC_BOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS
#    define SC_BOOST_NO_IS_ABSTRACT
//     std lib config should set this one already:
//#    define SC_BOOST_NO_STD_ALLOCATOR
#endif 

// optional features rather than defects:
#if (__HP_aCC >= 33900)
#    define SC_BOOST_HAS_LONG_LONG
#    define SC_BOOST_HAS_PARTIAL_STD_ALLOCATOR
#endif

#if (__HP_aCC >= 50000 ) && (__HP_aCC <= 53800 ) || (__HP_aCC < 31300 )
#    define SC_BOOST_NO_MEMBER_TEMPLATE_KEYWORD
#endif

#define SC_BOOST_NO_MEMBER_TEMPLATE_FRIENDS

#define SC_BOOST_COMPILER "HP aCC version " SC_BOOST_STRINGIZE(__HP_aCC)

//
// versions check:
// we don't support HP aCC prior to version 0:
#if __HP_aCC < 33000
#  error "Compiler not supported or configured - please reconfigure"
#endif
//
// last known and checked version is 0:
#if (__HP_aCC > 53800)
#  if defined(SC_BOOST_ASSERT_CONFIG)
#     error "Unknown compiler version - please run the configure tests and report the results"
#  endif
#endif




