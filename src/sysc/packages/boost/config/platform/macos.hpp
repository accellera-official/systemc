//  (C) Copyright John Maddock 2001 - 2003. 
//  (C) Copyright Darin Adler 2001 - 2002. 
//  (C) Copyright Bill Kempf 2002. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  Mac OS specific config options:

#define SC_BOOST_PLATFORM "Mac OS"

#if __MACH__ && !defined(_MSL_USING_MSL_C)

// Using the Mac OS X system BSD-style C library.

#  ifndef SC_BOOST_HAS_UNISTD_H
#    define SC_BOOST_HAS_UNISTD_H
#  endif
//
// Begin by including our boilerplate code for POSIX
// feature detection, this is safe even when using
// the MSL as Metrowerks supply their own <unistd.h>
// to replace the platform-native BSD one. G++ users
// should also always be able to do this on MaxOS X.
//
#  include <sysc/packages/boost/config/posix_features.hpp>
#  ifndef SC_BOOST_HAS_STDINT_H
#     define SC_BOOST_HAS_STDINT_H
#  endif

//
// BSD runtime has pthreads, sigaction, sched_yield and gettimeofday,
// of these only pthreads are advertised in <unistd.h>, so set the 
// other options explicitly:
//
#  define SC_BOOST_HAS_SCHED_YIELD
#  define SC_BOOST_HAS_GETTIMEOFDAY
#  define SC_BOOST_HAS_SIGACTION

#  if (__GNUC__ < 3) && !defined( __APPLE_CC__)

// GCC strange "ignore std" mode works better if you pretend everything
// is in the std namespace, for the most part.

#    define SC_BOOST_NO_STDC_NAMESPACE
#  endif

#else

// Using the MSL C library.

// We will eventually support threads in non-Carbon builds, but we do
// not support this yet.
#  if ( defined(TARGET_API_MAC_CARBON) && TARGET_API_MAC_CARBON ) || ( defined(TARGET_CARBON) && TARGET_CARBON )

#  if !defined(SC_BOOST_HAS_PTHREADS)
#    define SC_BOOST_HAS_MPTASKS
#  elif ( __dest_os == __mac_os_x )
// We are doing a Carbon/Mach-O/MSL build which has pthreads, but only the
// gettimeofday and no posix.
#  define SC_BOOST_HAS_GETTIMEOFDAY
#  endif

// The MP task implementation of Boost Threads aims to replace MP-unsafe
// parts of the MSL, so we turn on threads unconditionally.
#    define SC_BOOST_HAS_THREADS

// The remote call manager depends on this.
#    define SC_BOOST_BIND_ENABLE_PASCAL

#  endif

#endif



