//  (C) Copyright John Maddock 2003.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         auto_link.hpp
  *   VERSION      see <sysc/packages/boost/version.hpp>
  *   DESCRIPTION: Automatic library inclusion for Borland/Microsoft compilers.
  */

/*************************************************************************

USAGE:
~~~~~~

Before including this header you must define one or more of define the following macros:

SC_BOOST_LIB_NAME:           Required: A string containing the basename of the library,
                          for example boost_regex.
SC_BOOST_LIB_TOOLSET:        Optional: the base name of the toolset.
SC_BOOST_DYN_LINK:           Optional: when set link to dll rather than static library.
SC_BOOST_LIB_DIAGNOSTIC:     Optional: when set the header will print out the name
                          of the library selected (useful for debugging).
SC_BOOST_AUTO_LINK_NOMANGLE: Specifies that we should link to SC_BOOST_LIB_NAME.lib,
                          rather than a mangled-name version.

These macros will be undef'ed at the end of the header, further this header
has no include guards - so be sure to include it only once from your library!

Algorithm:
~~~~~~~~~~

Libraries for Borland and Microsoft compilers are automatically
selected here, the name of the lib is selected according to the following
formula:

SC_BOOST_LIB_PREFIX
   + SC_BOOST_LIB_NAME
   + "_"
   + SC_BOOST_LIB_TOOLSET
   + SC_BOOST_LIB_THREAD_OPT
   + SC_BOOST_LIB_RT_OPT
   "-"
   + SC_BOOST_LIB_VERSION

These are defined as:

SC_BOOST_LIB_PREFIX:     "lib" for static libraries otherwise "".

SC_BOOST_LIB_NAME:       The base name of the lib ( for example boost_regex).

SC_BOOST_LIB_TOOLSET:    The compiler toolset name (vc6, vc7, bcb5 etc).

SC_BOOST_LIB_THREAD_OPT: "-mt" for multithread builds, otherwise nothing.

SC_BOOST_LIB_RT_OPT:     A suffix that indicates the runtime library used,
                      contains one or more of the following letters after
                      a hiphen:

                      s      static runtime (dynamic if not present).
                      d      debug build (release if not present).
                      g      debug/diagnostic runtime (release if not present).
                      p      STLPort Build.

SC_BOOST_LIB_VERSION:    The Boost version, in the form x_y, for Boost version x.y.


***************************************************************************/

#ifdef __cplusplus
#  ifndef SC_BOOST_CONFIG_HPP
#     include <sysc/packages/boost/config.hpp>
#  endif
#elif defined(_MSC_VER) && !defined(__MWERKS__) && !defined(__EDG_VERSION__)
//
// C language compatability (no, honestly)
//
#  define SC_BOOST_MSVC _MSC_VER
#  define SC_BOOST_STRINGIZE(X) SC_BOOST_DO_STRINGIZE(X)
#  define SC_BOOST_DO_STRINGIZE(X) #X
#endif
//
// Only include what follows for known and supported compilers:
//
#if defined(SC_BOOST_MSVC) \
    || defined(__BORLANDC__) \
    || (defined(__MWERKS__) && defined(_WIN32) && (__MWERKS__ >= 0x3000)) \
    || (defined(__ICL) && defined(_MSC_EXTENSIONS) && (_MSC_VER >= 1200))

#ifndef SC_BOOST_VERSION_HPP
#  include <sysc/packages/boost/version.hpp>
#endif

#ifndef SC_BOOST_LIB_NAME
#  error "Macro SC_BOOST_LIB_NAME not set (internal error)"
#endif

//
// error check:
//
#if defined(__MSVC_RUNTIME_CHECKS) && !defined(_DEBUG)
#  pragma message("Using the /RTC option without specifying a debug runtime will lead to linker errors")
#  pragma message("Hint: go to the code generation options and switch to one of the debugging runtimes")
#  error "Incompatible build options"
#endif
//
// select toolset if not defined already:
//
#ifndef SC_BOOST_LIB_TOOLSET
#if defined(SC_BOOST_MSVC) && (SC_BOOST_MSVC == 1200)

   // vc6:
#  define SC_BOOST_LIB_TOOLSET "vc6"

#elif defined(SC_BOOST_MSVC) && (SC_BOOST_MSVC == 1300)

   // vc7:
#  define SC_BOOST_LIB_TOOLSET "vc7"

#elif defined(SC_BOOST_MSVC) && (SC_BOOST_MSVC == 1310)

   // vc71:
#  define SC_BOOST_LIB_TOOLSET "vc71"

#elif defined(SC_BOOST_MSVC) && (SC_BOOST_MSVC >= 1400)

   // vc80:
#  define SC_BOOST_LIB_TOOLSET "vc80"

#elif defined(__BORLANDC__)

   // CBuilder 6:
#  define SC_BOOST_LIB_TOOLSET "bcb"

#elif defined(__ICL)

   // Intel C++, no version number:
#  define SC_BOOST_LIB_TOOLSET "iw"

#elif defined(__MWERKS__) && (__MWERKS__ <= 0x31FF )

   // Metrowerks CodeWarrior 8.x
#  define SC_BOOST_LIB_TOOLSET "cw8"

#elif defined(__MWERKS__) && (__MWERKS__ <= 0x32FF )

   // Metrowerks CodeWarrior 9.x
#  define SC_BOOST_LIB_TOOLSET "cw9"

#endif
#endif // SC_BOOST_LIB_TOOLSET

//
// select thread opt:
//
#if defined(_MT) || defined(__MT__)
#  define SC_BOOST_LIB_THREAD_OPT "-mt"
#else
#  define SC_BOOST_LIB_THREAD_OPT
#endif

#if defined(_MSC_VER) || defined(__MWERKS__)

#  ifdef _DLL

#     if (defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION)) && (defined(_STLP_OWN_IOSTREAMS) || defined(__STL_OWN_IOSTREAMS))

#        if defined(_DEBUG) && (defined(__STL_DEBUG) || defined(_STLP_DEBUG))
#            define SC_BOOST_LIB_RT_OPT "-gdp"
#        elif defined(_DEBUG)
#            define SC_BOOST_LIB_RT_OPT "-gdp"
#            pragma message("warning: STLPort debug versions are built with /D_STLP_DEBUG=1")
#            error "Build options aren't compatible with pre-built libraries"
#        else
#            define SC_BOOST_LIB_RT_OPT "-p"
#        endif

#     elif defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION)

#        if defined(_DEBUG) && (defined(__STL_DEBUG) || defined(_STLP_DEBUG))
#            define SC_BOOST_LIB_RT_OPT "-gdpn"
#        elif defined(_DEBUG)
#            define SC_BOOST_LIB_RT_OPT "-gdpn"
#            pragma message("warning: STLPort debug versions are built with /D_STLP_DEBUG=1")
#            error "Build options aren't compatible with pre-built libraries"
#        else
#            define SC_BOOST_LIB_RT_OPT "-pn"
#        endif

#     else

#        if defined(_DEBUG)
#            define SC_BOOST_LIB_RT_OPT "-gd"
#        else
#            define SC_BOOST_LIB_RT_OPT
#        endif

#     endif

#  else

#     if (defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION)) && (defined(_STLP_OWN_IOSTREAMS) || defined(__STL_OWN_IOSTREAMS))

#        if defined(_DEBUG) && (defined(__STL_DEBUG) || defined(_STLP_DEBUG))
#            define SC_BOOST_LIB_RT_OPT "-sgdp"
#        elif defined(_DEBUG)
#             define SC_BOOST_LIB_RT_OPT "-sgdp"
#            pragma message("warning: STLPort debug versions are built with /D_STLP_DEBUG=1")
#            error "Build options aren't compatible with pre-built libraries"
#        else
#            define SC_BOOST_LIB_RT_OPT "-sp"
#        endif

#     elif defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION)

#        if defined(_DEBUG) && (defined(__STL_DEBUG) || defined(_STLP_DEBUG))
#            define SC_BOOST_LIB_RT_OPT "-sgdpn"
#        elif defined(_DEBUG)
#             define SC_BOOST_LIB_RT_OPT "-sgdpn"
#            pragma message("warning: STLPort debug versions are built with /D_STLP_DEBUG=1")
#            error "Build options aren't compatible with pre-built libraries"
#        else
#            define SC_BOOST_LIB_RT_OPT "-spn"
#        endif

#     else

#        if defined(_DEBUG)
#             define SC_BOOST_LIB_RT_OPT "-sgd"
#        else
#            define SC_BOOST_LIB_RT_OPT "-s"
#        endif

#     endif

#  endif

#elif defined(__BORLANDC__)

//
// figure out whether we want the debug builds or not:
//
#if __BORLANDC__ > 0x561
#pragma defineonoption SC_BOOST_BORLAND_DEBUG -v
#endif
//
// sanity check:
//
#if defined(__STL_DEBUG) || defined(_STLP_DEBUG)
#error "Pre-built versions of the Boost libraries are not provided in STLPort-debug form"
#endif

#  ifdef _RTLDLL

#     ifdef SC_BOOST_BORLAND_DEBUG
#         define SC_BOOST_LIB_RT_OPT "-d"
#     else
#         define SC_BOOST_LIB_RT_OPT
#     endif

#  else

#     ifdef SC_BOOST_BORLAND_DEBUG
#         define SC_BOOST_LIB_RT_OPT "-sd"
#     else
#         define SC_BOOST_LIB_RT_OPT "-s"
#     endif

#  endif

#endif

//
// select linkage opt:
//
#if (defined(_DLL) || defined(_RTLDLL)) && defined(SC_BOOST_DYN_LINK)
#  define SC_BOOST_LIB_PREFIX
#elif defined(SC_BOOST_DYN_LINK)
#  error "Mixing a dll boost library with a static runtime is a really bad idea..."
#else
#  define SC_BOOST_LIB_PREFIX "lib"
#endif

//
// now include the lib:
//
#if defined(SC_BOOST_LIB_NAME) \
      && defined(SC_BOOST_LIB_PREFIX) \
      && defined(SC_BOOST_LIB_TOOLSET) \
      && defined(SC_BOOST_LIB_THREAD_OPT) \
      && defined(SC_BOOST_LIB_RT_OPT) \
      && defined(SC_BOOST_LIB_VERSION)

#ifndef SC_BOOST_AUTO_LINK_NOMANGLE
#  pragma comment(lib, SC_BOOST_LIB_PREFIX SC_BOOST_STRINGIZE(SC_BOOST_LIB_NAME) "-" SC_BOOST_LIB_TOOLSET SC_BOOST_LIB_THREAD_OPT SC_BOOST_LIB_RT_OPT "-" SC_BOOST_LIB_VERSION ".lib")
#  ifdef SC_BOOST_LIB_DIAGNOSTIC
#     pragma message ("Linking to lib file: " SC_BOOST_LIB_PREFIX SC_BOOST_STRINGIZE(SC_BOOST_LIB_NAME) "-" SC_BOOST_LIB_TOOLSET SC_BOOST_LIB_THREAD_OPT SC_BOOST_LIB_RT_OPT "-" SC_BOOST_LIB_VERSION ".lib")
#  endif
#else
#  pragma comment(lib, SC_BOOST_STRINGIZE(SC_BOOST_LIB_NAME) ".lib")
#  ifdef SC_BOOST_LIB_DIAGNOSTIC
#     pragma message ("Linking to lib file: " SC_BOOST_STRINGIZE(SC_BOOST_LIB_NAME) ".lib")
#  endif
#endif

#else
#  error "some required macros where not defined (internal logic error)."
#endif


#endif // _MSC_VER || __BORLANDC__

//
// finally undef any macros we may have set:
//
#ifdef SC_BOOST_LIB_PREFIX
#  undef SC_BOOST_LIB_PREFIX
#endif
#if defined(SC_BOOST_LIB_NAME)
#  undef SC_BOOST_LIB_NAME
#endif
// Don't undef this one: it can be set by the user and should be the 
// same for all libraries:
//#if defined(SC_BOOST_LIB_TOOLSET)
//#  undef SC_BOOST_LIB_TOOLSET
//#endif
#if defined(SC_BOOST_LIB_THREAD_OPT)
#  undef SC_BOOST_LIB_THREAD_OPT
#endif
#if defined(SC_BOOST_LIB_RT_OPT)
#  undef SC_BOOST_LIB_RT_OPT
#endif
#if defined(SC_BOOST_LIB_LINK_OPT)
#  undef SC_BOOST_LIB_LINK_OPT
#endif
#if defined(SC_BOOST_LIB_DEBUG_OPT)
#  undef SC_BOOST_LIB_DEBUG_OPT
#endif
#if defined(SC_BOOST_DYN_LINK)
#  undef SC_BOOST_DYN_LINK
#endif
#if defined(SC_BOOST_AUTO_LINK_NOMANGLE)
#  undef SC_BOOST_AUTO_LINK_NOMANGLE
#endif











