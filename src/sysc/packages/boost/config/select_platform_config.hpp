//  Boost compiler configuration selection header file

//  (C) Copyright John Maddock 2001 - 2002. 
//  (C) Copyright Jens Maurer 2001. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

// locate which platform we are on and define SC_BOOST_PLATFORM_CONFIG as needed.
// Note that we define the headers to include using "header_name" not
// <header_name> in order to prevent macro expansion within the header
// name (for example "linux" is a macro on linux systems).

#if defined(linux) || defined(__linux) || defined(__linux__)
// linux:
#  define SC_BOOST_PLATFORM_CONFIG "sysc/packages/boost/config/platform/linux.hpp"

#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
// BSD:
#  define SC_BOOST_PLATFORM_CONFIG "sysc/packages/boost/config/platform/bsd.hpp"

#elif defined(sun) || defined(__sun)
// solaris:
#  define SC_BOOST_PLATFORM_CONFIG "sysc/packages/boost/config/platform/solaris.hpp"

#elif defined(__sgi)
// SGI Irix:
#  define SC_BOOST_PLATFORM_CONFIG "sysc/packages/boost/config/platform/irix.hpp"

#elif defined(__hpux)
// hp unix:
#  define SC_BOOST_PLATFORM_CONFIG "sysc/packages/boost/config/platform/hpux.hpp"

#elif defined(__CYGWIN__)
// cygwin is not win32:
#  define SC_BOOST_PLATFORM_CONFIG "sysc/packages/boost/config/platform/cygwin.hpp"

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
// win32:
#  define SC_BOOST_PLATFORM_CONFIG "sysc/packages/boost/config/platform/win32.hpp"

#elif defined(__BEOS__)
// BeOS
#  define SC_BOOST_PLATFORM_CONFIG "sysc/packages/boost/config/platform/beos.hpp"

#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
// MacOS
#  define SC_BOOST_PLATFORM_CONFIG "sysc/packages/boost/config/platform/macos.hpp"

#elif defined(__IBMCPP__) || defined(_AIX)
// IBM
#  define SC_BOOST_PLATFORM_CONFIG "sysc/packages/boost/config/platform/aix.hpp"

#elif defined(__amigaos__)
// AmigaOS
#  define SC_BOOST_PLATFORM_CONFIG "sysc/packages/boost/config/platform/amigaos.hpp"

#else

#  if defined(unix) \
      || defined(__unix) \
      || defined(_XOPEN_SOURCE) \
      || defined(_POSIX_SOURCE)

   // generic unix platform:

#  ifndef SC_BOOST_HAS_UNISTD_H
#     define SC_BOOST_HAS_UNISTD_H
#  endif

#  include <sysc/packages/boost/config/posix_features.hpp>

#  endif

#  if defined (SC_BOOST_ASSERT_CONFIG)
      // this must come last - generate an error if we don't
      // recognise the platform:
#     error "Unknown platform - please configure and report the results to boost.org"
#  endif

#endif



