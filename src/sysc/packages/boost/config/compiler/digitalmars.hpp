//  Copyright (C) Christof Meerwald 2003
//  Copyright (C) Dan Watkins 2003
//
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  Digital Mars C++ compiler setup:
#define SC_BOOST_COMPILER __DMC_VERSION_STRING__

#define SC_BOOST_HAS_LONG_LONG
#define SC_BOOST_HAS_PRAGMA_ONCE

#if (__DMC__ <= 0x833)
#define SC_BOOST_FUNCTION_SCOPE_USING_DECLARATION_BREAKS_ADL
#define SC_BOOST_NO_TEMPLATE_TEMPLATES
#define SC_BOOST_NEEDS_TOKEN_PASTING_OP_FOR_TOKENS_JUXTAPOSING
#define SC_BOOST_NO_ARRAY_TYPE_SPECIALIZATIONS
#define SC_BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS
#endif
#if (__DMC__ <= 0x840) || !defined(SC_BOOST_STRICT_CONFIG)
#define SC_BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS
#define SC_BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#define SC_BOOST_NO_OPERATORS_IN_NAMESPACE
#define SC_BOOST_NO_UNREACHABLE_RETURN_DETECTION
#define SC_BOOST_NO_SFINAE
#define SC_BOOST_NO_USING_TEMPLATE
#define SC_BOOST_FUNCTION_SCOPE_USING_DECLARATION_BREAKS_ADL
#endif

//
// has macros:
#if (__DMC__ >= 0x840)
#define SC_BOOST_HAS_DIRENT_H
#define SC_BOOST_HAS_STDINT_H
#define SC_BOOST_HAS_WINTHREADS
#endif


// check for exception handling support:
#ifndef _CPPUNWIND
#  define SC_BOOST_NO_EXCEPTIONS
#endif

#if (__DMC__ < 0x840)
#  if defined(SC_BOOST_ASSERT_CONFIG)
#     error "Unknown compiler version - please run the configure tests and report the results"
#  endif
#endif
