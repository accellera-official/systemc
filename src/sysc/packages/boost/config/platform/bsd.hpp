//  (C) Copyright John Maddock 2001 - 2003. 
//  (C) Copyright Darin Adler 2001. 
//  (C) Copyright Douglas Gregor 2002. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  generic BSD config options:

#if !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__) && !defined(__DragonFly__)
#error "This platform is not BSD"
#endif

#ifdef __FreeBSD__
#define SC_BOOST_PLATFORM "FreeBSD " SC_BOOST_STRINGIZE(__FreeBSD__)
#elif defined(__NetBSD__)
#define SC_BOOST_PLATFORM "NetBSD " SC_BOOST_STRINGIZE(__NetBSD__)
#elif defined(__OpenBSD__)
#define SC_BOOST_PLATFORM "OpenBSD " SC_BOOST_STRINGIZE(__OpenBSD__)
#elif defined(__DragonFly__)
#define SC_BOOST_PLATFORM "DragonFly " SC_BOOST_STRINGIZE(__DragonFly__)
#endif

//
// is this the correct version check?
// FreeBSD has <nl_types.h> but does not
// advertise the fact in <unistd.h>:
//
#if (defined(__FreeBSD__) && (__FreeBSD__ >= 3)) || defined(__DragonFly__)
#  define SC_BOOST_HAS_NL_TYPES_H
#endif

//
// FreeBSD 3.x has pthreads support, but defines _POSIX_THREADS in <pthread.h>
// and not in <unistd.h>
//
#if defined(__FreeBSD__) && (__FreeBSD__ <= 3)
#  define SC_BOOST_HAS_PTHREADS
#endif

//
// No wide character support in the BSD header files:
//
#if !(defined(__FreeBSD__) && (__FreeBSD__ >= 5))
#  define SC_BOOST_NO_CWCHAR
#endif
//
// The BSD <ctype.h> has macros only, no functions:
//
#if !defined(__OpenBSD__)
#  define SC_BOOST_NO_CTYPE_FUNCTIONS
#endif

//
// thread API's not auto detected:
//
#define SC_BOOST_HAS_SCHED_YIELD
#define SC_BOOST_HAS_NANOSLEEP
#define SC_BOOST_HAS_GETTIMEOFDAY
#define SC_BOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE
#define SC_BOOST_HAS_SIGACTION

// boilerplate code:
#define SC_BOOST_HAS_UNISTD_H
#include <sysc/packages/boost/config/posix_features.hpp>






