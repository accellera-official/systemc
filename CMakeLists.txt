###############################################################################
#
# The following code is derived, directly or indirectly, from the SystemC
# source code Copyright (c) 1996-2011 by all Contributors.
# All Rights reserved.
#
# The contents of this file are subject to the restrictions and limitations
# set forth in the SystemC Open Source License Version 3.0 (the "License");
# You may not use this file except in compliance with such restrictions and
# limitations. You may obtain instructions on how to receive a copy of the
# License at http://www.systemc.org/. Software distributed by Contributors
# under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
# ANY KIND, either express or implied. See the License for the specific
# language governing rights and limitations under the License.
#
###############################################################################

###############################################################################
#
# CMakeLists.txt --
# Top-level CMake script to configure the SystemC sources and to generate
# native Makefiles and project workspaces for your compiler environment.
#
# Original Author: Torsten Maehne, Université Pierre et Marie Curie, Paris,
#                  2013-06-11
#
###############################################################################

###############################################################################
#
# MODIFICATION LOG - modifiers, enter your name, affiliation, date and
# changes you are making here.
#
#     Name, Affiliation, Date:
# Description of Modification:
#
###############################################################################

###############################################################################
#
# This CMake build script <http://www.cmake.org/> was developed taking the
# original autotools-based build system of the Accellera SystemC distribution
# as a reference. However, it is less restrictive regarding the supported
# OS/processor/compiler combinations by detecting the actual features provided
# by the target (Unix or Windows) platform. On Unix (including OS X),
# shared libraries can be built. If the QuickThreads library provides support
# for the target processor, it will be automatically used. Otherwise, we rely
# on Pthreads on Unix and Fiber on Windows. By default, the SystemC library
# installation will follow the GNU standard installation layout so that also
# other SystemC libraries (SystemC, SCV, TLM, AMS extensions) can be installed
# into the same directory hierarchy (Unix: /opt/systemc/;
# Windows: $ENV{ProgramFiles}/SystemC/). The target platform's conventions are
# respected meaning usually include/ for the headers and lib/, lib64/, or
# lib/<multiarch-tuple>/ for the libraries. The lib-${SYSTEMC_TARGET_ARCH})/
# convention is not used by default, as ${SYSTEMC_TARGET_ARCH} does not
# reliably encode the OS/processor/compiler tuple.
#
# The CMake build scripts are compatible with CMake >2.8.5 and have been tested
# on the following OS/processor/compiler platforms:
#
# - Linux (GCC, Clang): i386, x86_64
#
# - Mac OS X >= 10.5 (GCC, Clang): i386, x86_64, ppc
#
# - Windows (MingW32 GCC + optionally MSYS): x86
#
# Currently untested, but considered by the build script (based on the
# autotools scripts) are:
#
# - *BSD (GCC, Clang): i386, x86_64
#
# - Linux (GCC, Clang): sparc, powerpc
#
# - Mac OS X <= 10.5 (GCC): ppc, ppc64
#
# - Windows (Cygwin GCC, MingW32 GCC, Visual C++): x86, AMD64
#
# - Solaris (GCC and native compiler): sparc
#
# - HP-UX (GCC and native compiler): hppa
#
# The build process can be configured through the configuration variables
# offered to the user in the CMake console and GUI (ccmake and cmake-gui,
# respectively). Each variable is annotated with a brief documentation string.
# The most important variables are:
#
# BUILD_EXAMPLES                Build the SystemC examples (default: OFF).
#
# BUILD_SHARED_LIBS             Build shared instead of static libraries
#                               (default: ON).
#
# BUILD_SOURCE_DOC              Build source code documentation using Doxygen
#                               (default: OFF).
#
# CMAKE_BUILD_TYPE              Build type (default: Release).
#
# CMAKE_OSX_ARCHITECTURES       Architectures for cross-compilation on Mac OS X
#                               (default: empty, i.e., only for the system
#                               processor).
#
# CMAKE_VERBOSE_MAKEFILE        Generate a verbose Makefile (default: OFF).
#
# QT_ARCH                       QuickThreads architecture (auto-detected:
#                               hppa, x86_64, i386, powerpc_mach,
#                               powerpc_sys5, IGNORE).
#
# SystemC_DIR                   Root directory of the SystemC libraries
#                               installation (default $SystemC_DIR,
#                               /opt/systemc/ (Unix, Cygwin, MSYS), or
#                               $ENV{ProgramFiles}/SystemC/ (MinGW and
#                               MS Visual Studio)). CMAKE_INSTALL_PREFIX will
#                               be set to its value.
#
# SYSTEMC_TARGET_ARCH           Target architecture according to the
#                               Accellera SystemC conventions set either from
#                               $ENV{SYSTEMC_TARGET_ARCH} or detected by CMake.
#
# SET_LIBDIR_WITH_TARGET_ARCH   Set the CMAKE_INSTALL_LIBDIR to
#                               lib-${SYSTEMC_TARGET_ARCH}/ and update
#                               CMAKE_INSTALL_FULL_LIBDIR accordingly.
#
# SUPPORT_ASYNC_REQUEST_UPDATE  Support async_request_update() of
#                               primitive channels (default: TRUE).
#
# WANT_PTHREADS_THREADS         Use POSIX threads to implement the SystemC
#                               coroutines instead of QuickThreads on Unix or
#                               Fiber on Windows (default: OFF).
#
# Other configuration variables will be proposed by CMake depending on the OS
# and detected processor, compiler, and libraries. For more information, please
# refer to the CMake documentation or <http://www.cmake.org/>.
#
###############################################################################
#
# TODO:
#
# - Generate CMake project configuration files so that other CMake projects
#   (e.g., SCV, SystemC AMS) can automatically find the SystemC library.
#   <http://www.cmake.org/Wiki/CMake/Tutorials/How_to_create_a_ProjectConfig.cmake_file>
#
# - Configure the SystemC version from within the build system.
#
# - Configure the TLM version from within the build system.
#
# - Extract the release dates from the commit date.
#
# - Check whether preprocessor definitions WIN32, _LIB, _DEBUG, _CONSOLE, and
#   NOGDI are necessary on WIN32 platforms (MSVC and MinGW).
#
# - Don't generate source documentation with Doxygen in the source tree.
#
# - Replace the shell script wrappers config/run_test.sh.in and
#   config/run_test_with_input.sh.in, used for the TLM examples execution, with
#   a platform-independent solution.
#
# - Turn on the compiler warnings by default once SystemC is supposed to
#   compile without any warnings on recent compiler versions.
#
###############################################################################


cmake_minimum_required (VERSION 2.8.5)


###############################################################################
# Project attributes
###############################################################################

project (SystemC CXX C)
set (SystemC_VERSION_MAJOR 2)
set (SystemC_VERSION_MINOR 3)
set (SystemC_VERSION_PATCH 0)
set (SystemC_VERSION_IS_PRERELEASE 1)
set (SystemC_VERSION_PRERELEASE "pub_rev")
set (SystemC_VERSION_RELEASE_DATE 20120701) # TODO: Extract date from commit.
set (SystemC_VERSION "${SystemC_VERSION_MAJOR}.${SystemC_VERSION_MINOR}.${SystemC_VERSION_PATCH}")
if (SystemC_IS_PRERELEASE)
  set (SystemC_VERSION "${SystemC_VERSION}_${SystemC_VERSION_PRERELEASE}_${SystemC_VERSION_RELEASE_DATE}")
endif (SystemC_IS_PRERELEASE)
set (SystemC_SOVERSION "${SystemC_VERSION_MAJOR}.${SystemC_VERSION_MINOR}")


set (SystemCTLM_VERSION_MAJOR 2)
set (SystemCTLM_VERSION_MINOR 0)
set (SystemCTLM_VERSION_PATCH 2)
set (SystemCTLM_VERSION_IS_PRERELEASE 1)
set (SystemCTLM_VERSION_PRERELEASE "pub_rev")
set (SystemCTLM_VERSION_RELEASE_DATE "20120511") # TODO: Extract date from commit.
set (SystemCTLM_VERSION "${SystemCTLM_VERSION_MAJOR}.${SystemCTLM_VERSION_MINOR}.${SystemCTLM_VERSION_PATCH}")
if (SystemCTLM_IS_PRERELEASE)
  set (SystemCTLM_VERSION "${SystemCTLM_VERSION}_${SystemCTLM_VERSION_PRERELEASE}_${SystemCTLM_VERSION_RELEASE_DATE}")
endif (SystemCTLM_IS_PRERELEASE)
set (SystemCTLM_SOVERSION "${SystemCTLM_MAJOR_VERSION}.${SystemCTLM_MINOR_VERSION}")


###############################################################################
# Build options
###############################################################################

if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Release CACHE STRING
       "Choose the type of build, options are: empty, Debug, Release, RelWithDebInfo, MinSizeRel."
       FORCE)
endif (NOT CMAKE_BUILD_TYPE)

if (NOT (WIN32 OR CYGWIN))
  option (BUILD_SHARED_LIBS "Build shared libraries." ON)
else (NOT (WIN32 OR CYGWIN))
  option (BUILD_SHARED_LIBS "Build shared libraries." OFF)
endif (NOT (WIN32 OR CYGWIN))
if (BUILD_SHARED_LIBS AND (WIN32 OR CYGWIN))
  message (WARNING "Only static libraries are supported on the Windows platform.")
  set (BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
endif (BUILD_SHARED_LIBS AND (WIN32 OR CYGWIN))

option (BUILD_SOURCE_DOC "Build source documentation with Doxygen." OFF)
option (BUILD_EXAMPLES "Build SystemC and TLM examples." OFF)
if (BUILD_EXAMPLES)
  enable_testing ()
endif (BUILD_EXAMPLES)

option (SUPPORT_ASYNC_REQUEST_UPDATE
        "Support async_request_update() of primitive channels (default: TRUE)."
        TRUE)

option (WANT_PTHREADS_THREADS
        "Use POSIX threads to implement the SystemC coroutines instead
of QuickThreads library on Unix or Fiber on Windows (default: FALSE)."
        FALSE)

###############################################################################
# Set SYSTEMC_TARGET_ARCH and QT_ARCH depending on OS and processor type
###############################################################################

message (STATUS "Checking OS and processor type.")

if (APPLE)
  list (LENGTH CMAKE_OSX_ARCHITECTURES N_OSX_ARCHITECTURES)
endif (APPLE)

if (NOT APPLE OR (APPLE AND (N_OSX_ARCHITECTURES LESS 2)))
  if (APPLE)
    if (N_OSX_ARCHITECTURES GREATER 0)
      message (STATUS "The target architecture(s) will be passed explicitly to the Apple compiler.")
      if (N_OSX_ARCHITECTURES EQUAL 1)
        set (CMAKE_REQUIRED_FLAGS "-arch ${CMAKE_OSX_ARCHITECTURES}")
      endif (N_OSX_ARCHITECTURES EQUAL 1)
    endif (N_OSX_ARCHITECTURES GREATER 0)
    unset (HAS__i386_DEFINED CACHE)
    unset (HAS__x86_64__DEFINED CACHE)
    unset (HAS__ppc__DEFINED CACHE)
  endif (APPLE)
  include (CheckSymbolExists)
  check_symbol_exists (__LP64__ "" HAS__LP64__DEFINED)
  check_symbol_exists (__sparc "" HAS__sparc_DEFINED)
  check_symbol_exists (__sparc__ "" HAS__sparc__DEFINED)
  check_symbol_exists (__hppa "" HAS__hppa_DEFINED)
  check_symbol_exists (__x86_64__ "" HAS__x86_64__DEFINED)
  check_symbol_exists (__i386 "" HAS__i386_DEFINED)
  check_symbol_exists (__ppc__ "" HAS__ppc__DEFINED)
  check_symbol_exists (__powerpc "" HAS__powerpc_DEFINED)
endif (NOT APPLE OR (APPLE AND (N_OSX_ARCHITECTURES LESS 2)))


# Set SYSTEMC_TARGET_ARCH according to the detected
# OS/processor/compiler platform
if (NOT SYSTEMC_TARGET_ARCH)
  if ($ENV{SYSTEMC_TARGET_ARCH})
    message (STATUS "Using SYSTEMC_TARGET_ARCH from the environment.")
    set (_TARGET_ARCH $ENV{SYSTEMC_TARGET_ARCH})
  elseif (APPLE)
    if (N_OSX_ARCHITECTURES GREATER 1)
      set (_TARGET_ARCH "macosxuniversal")
    elseif (HAS__i386_DEFINED)
      set (_TARGET_ARCH "macosx")
    elseif (HAS__x86_64__DEFINED)
      set (_TARGET_ARCH "macosx64")
    elseif (HAS__ppc__DEFINED)
      set (_TARGET_ARCH "macosxppc")
    elseif (HAS__ppc64__DEFINED)
      set (_TARGET_ARCH "macosxppc64")
    else (HAS__i386_DEFINED)
      set (_TARGET_ARCH "macosxunknown")
    endif (N_OSX_ARCHITECTURES GREATER 1)
  elseif (UNIX AND CMAKE_SYSTEM_NAME STREQUAL "Linux" AND HAS__i386_DEFINED)
    set (_TARGET_ARCH "linux")
  elseif (UNIX AND CMAKE_SYSTEM_NAME STREQUAL "Linux" AND HAS__x86_64__DEFINED)
    set (_TARGET_ARCH "linux64")
  elseif (UNIX AND CMAKE_SYSTEM_NAME STREQUAL "Linux" AND HAS__powerpc_DEFINED)
    # Untested platform.
    set (_TARGET_ARCH "linuxppc")
  elseif (UNIX AND CMAKE_SYSTEM_NAME STREQUAL "Linux" AND (HAS__sparc_DEFINED OR HAS__sparc__DEFINED))
    # Untested platform.
    set (_TARGET_ARCH "linuxsparc")
  elseif (UNIX AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Untested platform.
    set (_TARGET_ARCH "linuxunknown")
  elseif (UNIX AND CMAKE_SYSTEM_NAME MATCHES ".*BSD" AND HAS__i386_DEFINED)
    set (_TARGET_ARCH "bsd")
  elseif (UNIX AND CMAKE_SYSTEM_NAME MATCHES ".*BSD" AND HAS__x86_64__DEFINED)
    set (_TARGET_ARCH "bsd64")
  elseif (UNIX AND CMAKE_SYSTEM_NAME STREQUAL ".*BSD")
    # Untested platform.
    set (_TARGET_ARCH "bsdunknown")
  elseif (UNIX AND CMAKE_SYSTEM_NAME STREQUAL "SunOS" AND (HAS__sparc_DEFINED OR HAS__sparc__DEFINED))
    # Untested platform.
    set (_TARGET_ARCH "sparcOS5")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (_TARGET_ARCH "gcc${_TARGET_ARCH}")
    endif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  elseif (UNIX AND CMAKE_SYSTEM_NAME STREQUAL "HPUX" AND HAS__hppa_DEFINED)
    # Untested platform.
    set (_TARGET_ARCH "hpux11")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (_TARGET_ARCH "gcc${_TARGET_ARCH}")
    endif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (HAS__LP64__DEFINED)
      set (_TARGET_ARCH "${_TARGET_ARCH}64")
    endif (HAS__LP64__DEFINED)
  elseif (MSVC)
    set (_TARGET_ARCH "msvc")
    if (CMAKE_CL_64)
      set (_TARGET_ARCH "${_TARGET_ARCH}64")
    endif (CMAKE_CL_64)
  elseif (CYGWIN)
    if (HAS__i386_DEFINED)
      set (_TARGET_ARCH "cygwin")
    elseif (HAS__x86_64__DEFINED)
      set (_TARGET_ARCH "cygwin64")
    else (HAS__i386_DEFINED)
      set (_TARGET_ARCH "cygwinunknown")
    endif (HAS__i386_DEFINED)
  elseif (WIN32
          AND ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
               OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")))
    # MinGW platform, as CYGWIN already has been handled
    if (HAS__i386_DEFINED)
      set (_TARGET_ARCH "mingw")
    elseif (HAS__x86_64__DEFINED)
      set (_TARGET_ARCH "mingw64")
    else (HAS__i386_DEFINED)
      set (_TARGET_ARCH "mingwunknown")
    endif (HAS__i386_DEFINED)
  else ($ENV{SYSTEMC_TARGET_ARCH})
    # Unknown platform, so guess a SystemC target architecture value.
    string (TOLOWER "${CMAKE_SYSTEM_NAME}${CMAKE_PROCESSOR_NAME}" _TARGET_ARCH)
  endif ($ENV{SYSTEMC_TARGET_ARCH})
  set (SYSTEMC_TARGET_ARCH ${_TARGET_ARCH} CACHE STRING "SystemC Target Architecture")
  unset (_TARGET_ARCH)
endif (NOT SYSTEMC_TARGET_ARCH)


###############################################################################
# Configure QuickThreads
###############################################################################

# Check if chosen QuickThreads is valid.
if (DEFINED QT_ARCH AND (NOT QT_ARCH MATCHES "sparc|hppa|x86_64|i386|powerpc_mach|powerpc_sys5|IGNORE"))
  message (WARNING "Unknown QuickThreads architecture \"${QT_ARCH}\".")
  unset (QT_ARCH CACHE)
endif (DEFINED QT_ARCH AND (NOT QT_ARCH MATCHES "sparc|hppa|x86_64|i386|powerpc_mach|powerpc_sys5|IGNORE"))

if (APPLE AND CMAKE_OSX_ARCHITECTURES)
  # Make sure that the QuickThreads architecture always matches the target OS X architecture(s).
  unset (QT_ARCH CACHE)
endif (APPLE AND CMAKE_OSX_ARCHITECTURES)

# Set QuickThreads architecture based on the detected preprocessor symbols.
if (NOT DEFINED QT_ARCH)
  message (STATUS "Detect the target processor architecture for QuickThreads.")
  if (WANT_PTHREADS_THREADS)
    set (_QT_ARCH "IGNORE") # Pthreads will be used for the SystemC coroutines.
  elseif (MSVC)
    set (_QT_ARCH "IGNORE") # Fibers will be used for the SystemC coroutines.
  elseif (APPLE AND (N_OSX_ARCHITECTURES GREATER 1))
    set (_QT_ARCH "IGNORE") # For universal libraries, Pthreads will be used for the SystemC coroutines.
  elseif (HAS__sparc_DEFINED OR HAS__sparc__DEFINED)
    set (_QT_ARCH "sparc")
  elseif (HAS__hppa_DEFINED)
    set (_QT_ARCH "hppa")
  elseif (HAS__x86_64__DEFINED)
    set (_QT_ARCH "x86_64")
  elseif (HAS__i386_DEFINED)
    set (_QT_ARCH "i386")
  elseif (HAS__ppc__DEFINED)
    set (_QT_ARCH "powerpc_mach")
  elseif (HAS__powerpc__DEFINED)
    set (_QT_ARCH "powerpc_sys5")
  else (WANT_PTHREADS_THREADS)
    message (WARNING "QuickThreads is not supported on ${CMAKE_SYSTEM} on ${CMAKE_SYSTEM_PROCESSOR}.")
    set (_QT_ARCH "IGNORE")
  endif (WANT_PTHREADS_THREADS)
  set (QT_ARCH ${_QT_ARCH} CACHE STRING "QuickThreads architecture (IGNORE, sparc, hppa, x86_64, i386, powerpc_mach, powerpc_sys5).")
  unset (_QT_ARCH)
endif (NOT DEFINED QT_ARCH)

if (QT_ARCH)
  # To build QuickThreads, enable the assembler support.
  enable_language (ASM)
else (QT_ARCH)
  # Otherwise, fall back to Pthreads or Fiber.
  if (NOT MSVC)
    set (WANT_PTHREADS_THREADS TRUE CACHE BOOL
         "Use POSIX threads to implement the SystemC coroutines instead of QuickThreads library on Unix or Fiber on Windows (default: OFF)"
         FORCE)
  endif (NOT MSVC)
endif (QT_ARCH)


###############################################################################
# Configure Pthreads if necessary.
###############################################################################

if (WANT_PTHREADS_THREADS OR NOT QT_ARCH OR (SUPPORT_ASYNC_REQUEST_UPDATE AND NOT WIN32))
  set (CMAKE_THREAD_PREFER_PTHREAD)
  find_package (Threads REQUIRED)

  if (NOT CMAKE_USE_PTHREADS_INIT)
    message (SEND_ERROR "Failed to find the Pthreads library required to implement the SystemC coroutines and async_request_update() of primitive channels on Unix.")
  endif (NOT CMAKE_USE_PTHREADS_INIT)
  if (WANT_PTHREADS_THREADS)
    add_definitions (-DSC_USE_PTHREADS)
  endif (WANT_PTHREADS_THREADS)
  link_libraries (${CMAKE_THREAD_LIBS_INIT})
endif (WANT_PTHREADS_THREADS OR NOT QT_ARCH OR (SUPPORT_ASYNC_REQUEST_UPDATE AND NOT WIN32))


###############################################################################
# Set the platform-dependent compiler definitions.
###############################################################################

add_definitions (-DSC_INCLUDE_FX)

# # TODO: Turn on all compiler warnings once SystemC is supposed to compile without any.
# #       (SystemC 2.3.0 still causes some warnings during compilation.)
# # Set additional compiler flags for known compilers.
# if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
#   if (NOT CMAKE_C_FLAGS)
#     set (CMAKE_C_FLAGS "-Wall -Wextra" CACHE STRING "Flags used by the compiler during all build types." FORCE)
#   endif (NOT CMAKE_C_FLAGS)
#   if (NOT CMAKE_CXX_FLAGS)
#     set (CMAKE_CXX_FLAGS "-Wall -Wextra" CACHE STRING "Flags used by the compiler during all build types." FORCE)
#   endif (NOT CMAKE_CXX_FLAGS)
# endif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

if (APPLE)
  # On Mac OS X, the linker needs to be told that it's OK that _sc_main is an undefined symbol.
  set (CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -Wl,-U,_sc_main")
  set (CMAKE_SHARED_MODULE_CREATE_CXX_FLAGS "${CMAKE_SHARED_MODULE_CREATE_CXX_FLAGS} -Wl,-U,_sc_main")
endif (APPLE)

if (WIN32)
  add_definitions (-DWIN32) # Needed?
  if (MSVC)
    # Additional definition for Microsoft Visual Studio C++ compilers
    add_definitions(-D_LIB)               # Needed?
    # Is the definition of _DEBUG needed or already handled by CMake?
    if (CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
      add_definitions(-D_DEBUG)
    endif (CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
    add_definitions(-D_USE_MATH_DEFINES)  # Needed?
    # Suppress warnings about sprintf from Visual C++ 2005
    add_definitions (-D_CRT_SECURE_NO_DEPRECATE)
    # Suppress warnings from Visual C++ 2008
    add_definitions (-D_CRT_SECURE_NO_WARNINGS)
  endif (MSVC)
endif (WIN32)


###############################################################################
# Configure RPATH so that all targets find the libraries they link to
###############################################################################

# Use, i.e., don't skip, the full RPATH for the build tree.
set (CMAKE_SKIP_BUILD_RPATH  FALSE)
# For build, don't use the install RPATH already (but later on when
# installing).
set (CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
# Set the RPATH to be used when installing.
set (CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}")
# Add the automatically determined parts of the RPATH, which point to
# directories outside the build tree, to the install RPATH.
set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


###############################################################################
# Set the installation paths
###############################################################################

# If necessary, default-initialize SystemC_DIR and set
# CMAKE_INSTALL_PREFIX to its value.
if (NOT SystemC_DIR)
  if ($ENV{SystemC_DIR})
    set (_SystemC_DIR $ENV{SystemC_DIR})
  elseif ($ENV{SYSTEMC_ROOT})
    set (_SystemC_DIR $ENV{SYSTEMC_ROOT})
  elseif ($ENV{SYSTEMC_HOME})
    set (_SystemC_DIR $ENV{SYSTEMC_HOME})
  elseif (UNIX)
    set (_SystemC_DIR "/opt/systemc")
  elseif (WIN32)
    file (TO_CMAKE_PATH $ENV{ProgramFiles} _WIN32_INSTALL_PREFIX)
    set (_SystemC_DIR "${_WIN32_INSTALL_PREFIX}/SystemC")
    unset (_WIN32_INSTALL_PREFIX)
  else ($ENV{SystemC_DIR})
    set (_SystemC_DIR "${CMAKE_INSTALL_PREFIX}/systemc")
  endif ($ENV{SystemC_DIR})
  set (SystemC_DIR ${_SystemC_DIR} CACHE PATH "installation root for the SystemC libraries")
  unset (_SystemC_DIR)
endif (NOT SystemC_DIR)
set (CMAKE_INSTALL_PREFIX ${SystemC_DIR} CACHE PATH "Install directory used by install." FORCE)

# Force SYSTEMC_TARGET_ARCH library directory if requested.
option (SET_LIBDIR_WITH_TARGET_ARCH
        "Set the library installation directory to lib-${SYSTEMC_TARGET_ARCH}"
        OFF)
if (SET_LIBDIR_WITH_TARGET_ARCH)
  set(CMAKE_INSTALL_LIBDIR "lib-${SYSTEMC_TARGET_ARCH}" CACHE PATH
      "object code libraries (lib-<SYSTEMC_TARGET_ARCH>)" FORCE)
  set (SET_LIBDIR_WITH_TARGET_ARCH OFF CACHE BOOL
       "Set the library installation directory to lib-${SYSTEMC_TARGET_ARCH}"
       FORCE)
endif (SET_LIBDIR_WITH_TARGET_ARCH)

# Set the installation paths for headers, libraries, and documentation.
include (GNUInstallDirs)


###############################################################################
# Configure status
###############################################################################

message (STATUS "========================================================================")
message (STATUS "Settings to build SystemC ${SystemC_VERSION} (${SystemC_VERSION_RELEASE_DATE}) and TLM ${SystemCTLM_VERSION} (${SystemCTLM_VERSION_RELEASE_DATE})")
message (STATUS "------------------------------------------------------------------------")
message (STATUS "BUILD_EXAMPLES = ${BUILD_EXAMPLES}")
message (STATUS "BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}")
message (STATUS "BUILD_SOURCE_DOC = ${BUILD_SOURCE_DOC}")
message (STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message (STATUS "------------------------------------------------------------------------")
message (STATUS "CMAKE_SYSTEM = ${CMAKE_SYSTEM}")
message (STATUS "CMAKE_SYSTEM_PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}")
message (STATUS "SYSTEMC_TARGET_ARCH = ${SYSTEMC_TARGET_ARCH}")
message (STATUS "QT_ARCH = ${QT_ARCH}")
message (STATUS "WANT_PTHREADS_THREADS = ${WANT_PTHREADS_THREADS}")
if (APPLE)
  message (STATUS "CMAKE_OSX_ARCHITECTURES = ${CMAKE_OSX_ARCHITECTURES}")
  message (STATUS "CMAKE_OSX_DEPLOYMENT_TARGET = ${CMAKE_OSX_DEPLOYMENT_TARGET}")
  message (STATUS "CMAKE_OSX_SYSROOT = ${CMAKE_OSX_SYSROOT}")
endif(APPLE)
message (STATUS "Threads_FOUND = ${Threads_FOUND}")
message (STATUS "CMAKE_USE_PTHREADS_INIT = ${CMAKE_USE_PTHREADS_INIT}")
message (STATUS "------------------------------------------------------------------------")
message (STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message (STATUS "CMAKE_INSTALL_BINDIR = ${CMAKE_INSTALL_BINDIR}")
message (STATUS "CMAKE_INSTALL_DOCDIR = ${CMAKE_INSTALL_DOCDIR}")
message (STATUS "CMAKE_INSTALL_INCLUDEDIR = ${CMAKE_INSTALL_INCLUDEDIR}")
message (STATUS "CMAKE_INSTALL_LIBDIR = ${CMAKE_INSTALL_LIBDIR}")
message (STATUS "========================================================================")


###############################################################################
# Subdirectories
###############################################################################

add_subdirectory (docs)
add_subdirectory (examples)
add_subdirectory (src)


###############################################################################
# Install README files
###############################################################################

install (FILES AUTHORS
               COPYING
               ChangeLog
               INSTALL
               LICENSE
               NEWS
               README
               RELEASENOTES
         DESTINATION ${CMAKE_INSTALL_DOCDIR})
