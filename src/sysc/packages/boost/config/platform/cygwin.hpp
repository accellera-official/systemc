//  (C) Copyright John Maddock 2001 - 2003. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  cygwin specific config options:

#define SC_BOOST_PLATFORM "Cygwin"
#define SC_BOOST_NO_CWCTYPE
#define SC_BOOST_NO_CWCHAR
#define SC_BOOST_NO_SWPRINTF
#define SC_BOOST_HAS_DIRENT_H

//
// Threading API:
// See if we have POSIX threads, if we do use them, otherwise
// revert to native Win threads.
#define SC_BOOST_HAS_UNISTD_H
#include <unistd.h>
#if defined(_POSIX_THREADS) && (_POSIX_THREADS+0 >= 0) && !defined(SC_BOOST_HAS_WINTHREADS)
#  define SC_BOOST_HAS_PTHREADS
#  define SC_BOOST_HAS_SCHED_YIELD
#  define SC_BOOST_HAS_GETTIMEOFDAY
#  define SC_BOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE
#  define SC_BOOST_HAS_SIGACTION
#else
#  if !defined(SC_BOOST_HAS_WINTHREADS)
#     define SC_BOOST_HAS_WINTHREADS
#  endif
#  define SC_BOOST_HAS_FTIME
#endif

//
// find out if we have a stdint.h, there should be a better way to do this:
//
#include <sys/types.h>
#ifdef _STDINT_H
#define SC_BOOST_HAS_STDINT_H
#endif

// boilerplate code:
#include <sysc/packages/boost/config/posix_features.hpp>
 



