//  (C) Copyright John Maddock 2001 - 2002. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  IBM/Aix specific config options:

#define SC_BOOST_PLATFORM "IBM Aix"

#define SC_BOOST_HAS_UNISTD_H
#define SC_BOOST_HAS_NL_TYPES_H
#define SC_BOOST_HAS_NANOSLEEP
#define SC_BOOST_HAS_CLOCK_GETTIME

// This needs support in "sysc/packages/boost/cstdint.hpp" exactly like FreeBSD.
// This platform has header named <inttypes.h> which includes all
// the things needed.
#define SC_BOOST_HAS_STDINT_H

// Threading API's:
#define SC_BOOST_HAS_PTHREADS
#define SC_BOOST_HAS_PTHREAD_DELAY_NP
#define SC_BOOST_HAS_SCHED_YIELD
//#define SC_BOOST_HAS_PTHREAD_YIELD

// boilerplate code:
#include <sysc/packages/boost/config/posix_features.hpp>




