//  (C) Copyright John Maddock 2001 - 2003. 
//  (C) Copyright Jens Maurer 2001 - 2003. 
//  (C) Copyright David Abrahams 2002. 
//  (C) Copyright Toon Knapen 2003. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  hpux specific config options:

#define SC_BOOST_PLATFORM "HP-UX"

// In principle, HP-UX has a nice <stdint.h> under the name <inttypes.h>
// However, it has the following problem:
// Use of UINT32_C(0) results in "0u l" for the preprocessed source
// (verifyable with gcc 2.95.3, assumed for HP aCC)
// #define SC_BOOST_HAS_STDINT_H

#define SC_BOOST_NO_SWPRINTF 
#define SC_BOOST_NO_CWCTYPE

#if defined(__GNUC__)
#  if (__GNUC__ < 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ < 3))
      // GNU C on HP-UX does not support threads (checked up to gcc 3.3)
#     define SC_BOOST_DISABLE_THREADS
#  elif !defined(SC_BOOST_DISABLE_THREADS)
      // threads supported from gcc-3.3 onwards:
#     define SC_BOOST_HAS_THREADS
#     define SC_BOOST_HAS_PTHREADS
#  endif
#endif

// boilerplate code:
#define SC_BOOST_HAS_UNISTD_H
#include <sysc/packages/boost/config/posix_features.hpp>

// the following are always available:
#ifndef SC_BOOST_HAS_GETTIMEOFDAY
#  define SC_BOOST_HAS_GETTIMEOFDAY
#endif
#ifndef SC_BOOST_HAS_SCHED_YIELD
#    define SC_BOOST_HAS_SCHED_YIELD
#endif
#ifndef SC_BOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE
#    define SC_BOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE
#endif
#ifndef SC_BOOST_HAS_NL_TYPES_H
#    define SC_BOOST_HAS_NL_TYPES_H
#endif
#ifndef SC_BOOST_HAS_NANOSLEEP
#    define SC_BOOST_HAS_NANOSLEEP
#endif
#ifndef SC_BOOST_HAS_GETTIMEOFDAY
#    define SC_BOOST_HAS_GETTIMEOFDAY
#endif
#ifndef SC_BOOST_HAS_DIRENT_H
#    define SC_BOOST_HAS_DIRENT_H
#endif
#ifndef SC_BOOST_HAS_CLOCK_GETTIME
#    define SC_BOOST_HAS_CLOCK_GETTIME
#endif
#ifndef SC_BOOST_HAS_SIGACTION
#  define SC_BOOST_HAS_SIGACTION
#endif


