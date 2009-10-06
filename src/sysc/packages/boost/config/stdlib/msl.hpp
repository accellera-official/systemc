//  (C) Copyright John Maddock 2001. 
//  (C) Copyright Darin Adler 2001. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  Metrowerks standard library:

#ifndef __MSL_CPP__
#  include <utility>
#  ifndef __MSL_CPP__
#     error This is not the MSL standard library!
#  endif
#endif

#if __MSL_CPP__ >= 0x6000  // Pro 6
#  define SC_BOOST_HAS_HASH
#  define SC_BOOST_STD_EXTENSION_NAMESPACE Metrowerks
#endif
#define SC_BOOST_HAS_SLIST

#if __MSL_CPP__ < 0x6209
#  define SC_BOOST_NO_STD_MESSAGES
#endif

// check C lib version for <stdint.h>
#include <cstddef>

#if defined(__MSL__) && (__MSL__ >= 0x5000)
#  define SC_BOOST_HAS_STDINT_H
#  if !defined(__PALMOS_TRAPS__)
#    define SC_BOOST_HAS_UNISTD_H
#  endif
   // boilerplate code:
#  include <sysc/packages/boost/config/posix_features.hpp>
#endif

#if defined(_MWMT) || _MSL_THREADSAFE
#  define SC_BOOST_HAS_THREADS
#endif

#ifdef _MSL_NO_EXPLICIT_FUNC_TEMPLATE_ARG
#  define SC_BOOST_NO_STD_USE_FACET
#  define SC_BOOST_HAS_TWO_ARG_USE_FACET
#endif


#define SC_BOOST_STDLIB "Metrowerks Standard Library version " SC_BOOST_STRINGIZE(__MSL_CPP__)









