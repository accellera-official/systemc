###############################################################################
#
# Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
# more contributor license agreements.  See the NOTICE file distributed
# with this work for additional information regarding copyright ownership.
# Accellera licenses this file to you under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with the
# License.  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied.  See the License for the specific language governing
# permissions and limitations under the License.
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
#     Name, Affiliation, Date: Guillaume Delbergue, GreenSocs, 08 Mar, 2016
# Description of Modification: Extract SystemC/TLM version and date
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
# lib/<multiarch-tuple>/ for the libraries. The lib-${SystemC_TARGET_ARCH})/
# convention is not used by default, as ${SystemC_TARGET_ARCH} does not
# reliably encode the OS/processor/compiler tuple.
#
# The CMake build scripts are compatible with CMake >=2.8.5 and have been tested
# on the following OS/processor/compiler platforms:
#
# - Linux (GCC, Clang): i386, x86_64
#
# - Linux (GCC): aarch64
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
# - Linux (Clang): aarch64
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
# BUILD_SHARED_LIBS             Build shared instead of static libraries
#                               (default: ON if not targetting Windows).
#
# BUILD_SOURCE_DOCUMENTATION    Build source code documentation using Doxygen
#                               (default: OFF).
#
# CMAKE_BUILD_TYPE              Specifies the build type on single-configuration
#                               generators. (default: Release).
#
# CMAKE_CXX_STANDARD            C++ standard to build all targets. Supported
#                               values are 98, 11, 14, and 17. (default: 98)
#
# CMAKE_CXX_STANDARD_REQUIRED   The with CMAKE_CXX_STANDARD selected C++ standard
#                               is a requirement. (default: ON)
#
# CMAKE_INSTALL_PREFIX          Root directory of the SystemC libraries
#                               installation (defaults to $ENV{SYSTEMC_HOME}
#                               if set to an absolute path and otherwise to either
#                               /opt/systemc/ (Unix-like platforms including
#                               CYGWIN), $ENV{ProgramFiles}/SystemC/ (on Windows
#                               systems), or ${CMAKE_INSTALL_PREFIX}/systemc.
#
# CMAKE_OSX_ARCHITECTURES       Architectures for cross-compilation on Mac OS X
#                               (default: empty, i.e., only for the system
#                               processor).
#
# CMAKE_VERBOSE_MAKEFILE        Generate a verbose Makefile (default: OFF).
#
# DISABLE_ASYNC_UPDATES         Disable async_request_update() support.
#
# DISABLE_COPYRIGHT_MESSAGE     Do not print the copyright message when starting
#                               the application. (default: OFF)
#
# DISABLE_VIRTUAL_BIND          Disable the definition of bind() member
#                               functions of ports and exports as "virtual",
#                               which is incompatible with old TLM library
#                               implementations (< 2.0.2).
#
# ENABLE_ASSERTIONS             Always enable the `sc_assert' expressions
#                               (default: ON)
#
# ENABLE_EARLY_MAXTIME_CREATION Allow creation of sc_time objects with a value
#                               of sc_max_time() before finalizing the time
#                               resolution.
#
# ENABLE_IMMEDIATE_SELF_NOTIFICATIONS  Enable immediate self-notification of
#                               processes, which is no longer supported due to
#                               changes in IEEE Std 1666-2011 (see
#                               sc_event::notify, 5.10.6).
#
# ENABLE_PHASE_CALLBACKS        Enable the simulation phase callbacks (experimental).
#
# ENABLE_PHASE_CALLBACKS_TRACING  Enable usage of the (experimental) simulation
#                               phase callbacks for the implementation of
#                               sc_trace().
#
# ENABLE_PTHREADS               Use POSIX threads for SystemC processes instead
#                               of QuickThreads on Unix or Fiber on Windows.
#
# OVERRIDE_DEFAULT_STACK_SIZE   Define the default stack size used for SystemC
#                               (thread) processes. (> 0)
#
# SystemC_TARGET_ARCH           Target architecture according to the
#                               Accellera SystemC conventions set either from
#                               $ENV{SYSTEMC_TARGET_ARCH}, $ENV{SYSTEMC_ARCH},
#                               or detected by CMake.
#
# INSTALL_TO_LIB_BUILD_TYPE_DIR Install the libraries to a
#                               lib-${CMAKE_BUILD_TYPE}/ to enable parallel
#                               installation of the different build variants.
#                               (default: OFF)
#
# INSTALL_TO_LIB_TARGET_ARCH_DIR  Install the libraries to lib-<target-arch>
#                               to facilitate linking applications, which
#                               build systems assume to find SystemC in
#                               lib-<target-arch>. (default: OFF)
#
# INSTALL_LIB_TARGET_ARCH_SYMLINK  On Unix, install a symlink lib-<target-arch>
#                               to lib-${CMAKE_BUILD_TYPE} facilitating the
#                               linking of user code, which build system assumes
#                               to find the SystemC libraries under
#                               lib-<target-arch>. (default: OFF)
#
# Other configuration variables will be proposed by CMake depending on the OS
# and detected processor, compiler, and libraries. For more information, please
# refer to the CMake documentation or <http://www.cmake.org/>.
#
###############################################################################
#
# TODO:
#
# - Enable the compilation of SystemC as a DLL on Windows! To make it work, the
#   SystemC library has to be at least split into libsystemc-core.dll and a
#   static libsystemc.lib.
#
# - Check whether preprocessor definitions WIN32, _CONSOLE, _LIB, _DEBUG, NDEBUG,
#   and NOGDI are necessary on WIN32 platforms (MSVC and MinGW).
#
# - Turn on more compiler warnings by default once SystemC is supposed to
#   compile without any warnings on recent compiler versions. Currently, the
#   compiler warning flags are set to reduce the number of generated compiler
#   warnings (especially with recent Clang versions, e.g., Apple Clang 6.0) to
#   an acceptable amount. With the chosen flags, the SystemC library compiles
#   without warnings, but some examples still produce warnings.
#
# - Test on as many different hardware / OS platforms as possible.
#
# - Configure and install systemc.pc and tlm.pc for pkg-config
#
###############################################################################


cmake_minimum_required (VERSION 3.1)
cmake_policy(SET CMP0001 NEW)

project (SystemCLanguage CXX C)

set(SystemCLanguage_VERSION_FILE "${PROJECT_SOURCE_DIR}/src/sysc/kernel/sc_ver.h")
if(EXISTS ${SystemCLanguage_VERSION_FILE})
    file(READ ${SystemCLanguage_VERSION_FILE} SYSTEMC_VERSION_FILE_CONTENT)
    string(REGEX MATCH "SC_VERSION_MAJOR[ \t]+([0-9]+)" SystemCLanguage_VERSION_MAJOR ${SYSTEMC_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCLanguage_VERSION_MAJOR ${SystemCLanguage_VERSION_MAJOR})
    string(REGEX MATCH "SC_VERSION_MINOR[ \t]+([0-9]+)" SystemCLanguage_VERSION_MINOR ${SYSTEMC_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCLanguage_VERSION_MINOR ${SystemCLanguage_VERSION_MINOR})
    string(REGEX MATCH "SC_VERSION_PATCH[ \t]+([0-9]+)" SystemCLanguage_VERSION_PATCH ${SYSTEMC_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCLanguage_VERSION_PATCH ${SystemCLanguage_VERSION_PATCH})
    string(REGEX MATCH "SC_IS_PRERELEASE[ \t]+([0-9]+)" SystemCLanguage_VERSION_IS_PRERELEASE ${SYSTEMC_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCLanguage_VERSION_IS_PRERELEASE ${SystemCLanguage_VERSION_IS_PRERELEASE})
    string(REGEX MATCH "SC_VERSION_PRERELEASE[ \t]+\"([^\".]+)\"" SystemCLanguage_VERSION_PRERELEASE ${SYSTEMC_VERSION_FILE_CONTENT})
    string(REGEX REPLACE "(.*)\"(.*)\"" "\\2" SystemCLanguage_VERSION_PRERELEASE ${SystemCLanguage_VERSION_PRERELEASE})
    string(REGEX MATCH "SYSTEMC_VERSION[ \t]+([0-9]+)" SystemCLanguage_VERSION_RELEASE_DATE ${SYSTEMC_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCLanguage_VERSION_RELEASE_DATE ${SystemCLanguage_VERSION_RELEASE_DATE})
else(EXISTS ${SystemCLanguage_VERSION_FILE})
    message (FATAL_ERROR "Unable to read SystemC version file")
endif(EXISTS ${SystemCLanguage_VERSION_FILE})

set (SystemCLanguage_VERSION "${SystemCLanguage_VERSION_MAJOR}.${SystemCLanguage_VERSION_MINOR}.${SystemCLanguage_VERSION_PATCH}")
if (SystemCLanguage_VERSION_IS_PRERELEASE)
  set (SystemCLanguage_VERSION "${SystemCLanguage_VERSION}_${SystemCLanguage_VERSION_PRERELEASE}_${SystemCLanguage_VERSION_RELEASE_DATE}")
endif (SystemCLanguage_VERSION_IS_PRERELEASE)
set (SystemCLanguage_SOVERSION "${SystemCLanguage_VERSION_MAJOR}.${SystemCLanguage_VERSION_MINOR}")

set(SystemCTLM_VERSION_FILE "${PROJECT_SOURCE_DIR}/src/tlm_core/tlm_2/tlm_version.h")
if(EXISTS ${SystemCTLM_VERSION_FILE})
    file(READ ${SystemCTLM_VERSION_FILE} SystemCTLM_VERSION_FILE_CONTENT)
    string(REGEX MATCH "TLM_VERSION_MAJOR[ \t]+([0-9]+)" SystemCTLM_VERSION_MAJOR ${SystemCTLM_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCTLM_VERSION_MAJOR ${SystemCTLM_VERSION_MAJOR})
    string(REGEX MATCH "TLM_VERSION_MINOR[ \t]+([0-9]+)" SystemCTLM_VERSION_MINOR ${SystemCTLM_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCTLM_VERSION_MINOR ${SystemCTLM_VERSION_MINOR})
    string(REGEX MATCH "TLM_VERSION_PATCH[ \t]+([0-9]+)" SystemCTLM_VERSION_PATCH ${SystemCTLM_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCTLM_VERSION_PATCH ${SystemCTLM_VERSION_PATCH})
    string(REGEX MATCH "TLM_IS_PRERELEASE[ \t]+([0-9]+)" SystemCTLM_VERSION_IS_PRERELEASE ${SystemCTLM_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCTLM_VERSION_IS_PRERELEASE ${SystemCTLM_VERSION_IS_PRERELEASE})
    string(REGEX MATCH "TLM_VERSION_PRERELEASE[ \t]+\"([^\".]+)\"" SystemCTLM_VERSION_PRERELEASE ${SystemCTLM_VERSION_FILE_CONTENT})
    string(REGEX REPLACE "(.*)\"(.*)\"" "\\2" SystemCTLM_VERSION_PRERELEASE ${SystemCTLM_VERSION_PRERELEASE})
    string(REGEX MATCH "TLM_VERSION_RELEASE_YEAR[ \t]+\"([0-9]+)\"" SystemCTLM_VERSION_RELEASE_YEAR ${SystemCTLM_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCTLM_VERSION_RELEASE_YEAR ${SystemCTLM_VERSION_RELEASE_YEAR})
    string(REGEX MATCH "TLM_VERSION_RELEASE_MONTH[ \t]+\"([0-9]+)\"" SystemCTLM_VERSION_RELEASE_MONTH ${SystemCTLM_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCTLM_VERSION_RELEASE_MONTH ${SystemCTLM_VERSION_RELEASE_MONTH})
    string(REGEX MATCH "TLM_VERSION_RELEASE_DAY[ \t]+\"([0-9]+)\"" SystemCTLM_VERSION_RELEASE_DAY ${SystemCTLM_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCTLM_VERSION_RELEASE_DAY ${SystemCTLM_VERSION_RELEASE_DAY})
else(EXISTS ${SystemCTLM_VERSION_FILE})
    message(FATAL_ERROR "Unable to read TLM version file")
endif(EXISTS ${SystemCTLM_VERSION_FILE})

set (SystemCTLM_VERSION_RELEASE_DATE "${SystemCTLM_VERSION_RELEASE_YEAR}${SystemCTLM_VERSION_RELEASE_MONTH}${SystemCTLM_VERSION_RELEASE_DAY}")
set (SystemCTLM_VERSION "${SystemCTLM_VERSION_MAJOR}.${SystemCTLM_VERSION_MINOR}.${SystemCTLM_VERSION_PATCH}")
if (TLM_IS_PRERELEASE)
  set (SystemCTLM_VERSION "${SystemCTLM_VERSION}_${SystemCTLM_VERSION_PRERELEASE}_${SystemCTLM_VERSION_RELEASE_DATE}")
endif (TLM_IS_PRERELEASE)
set (SystemCTLM_SOVERSION "${SystemCTLM_VERSION_MAJOR}.${SystemCTLM_VERSION_MINOR}")


###############################################################################
# Build options
###############################################################################

if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Release CACHE STRING
       "Choose the type of build, options are: empty, Debug, Release, RelWithDebInfo, MinSizeRel."
       FORCE)
endif (NOT CMAKE_BUILD_TYPE)

set (CMAKE_CXX_STANDARD 98 CACHE STRING
     "C++ standard to build all targets. Supported values are 98, 11, 14, and 17.")
set (CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL
     "The with CMAKE_CXX_STANDARD selected C++ standard is a requirement.")
mark_as_advanced (CMAKE_CXX_STANDARD_REQUIRED)

if (NOT (WIN32 OR CYGWIN))
  option (BUILD_SHARED_LIBS "Build shared libraries." ON)
else (NOT (WIN32 OR CYGWIN))
  option (BUILD_SHARED_LIBS "Build shared libraries." OFF)
endif (NOT (WIN32 OR CYGWIN))
if (BUILD_SHARED_LIBS AND (WIN32 OR CYGWIN))
  message (WARNING "The compilation of SystemC as a DLL on Windows is currently not supported!")
  set (BUILD_SHARED_LIBS CACHE BOOL "Build shared libraries." OFF FORCE)
endif (BUILD_SHARED_LIBS AND (WIN32 OR CYGWIN))

option (BUILD_SOURCE_DOCUMENTATION "Build source documentation with Doxygen." OFF)

option (DISABLE_ASYNC_UPDATES "Disable async_request_update() support." OFF)

option (DISABLE_COPYRIGHT_MESSAGE "Do not print the copyright message when starting the application." OFF)

option (DISABLE_VCD_SCOPES "Put all traces in a single dummy scope. For compatibility with SystemC 2.3.1." OFF)

option (DISABLE_VIRTUAL_BIND "Disable the definition of bind() member functions of ports and exports as \"virtual\", which is incompatible with old TLM library implementations (< 2.0.2)." OFF)

option (ENABLE_ASSERTIONS "Always enable the `sc_assert' expressions." ON)

option (ENABLE_EARLY_MAXTIME_CREATION "Allow creation of sc_time objects with a value of sc_max_time() before finalizing the time resolution." ON)

option (ENABLE_IMMEDIATE_SELF_NOTIFICATIONS "Enable immediate self-notification of processes, which is no longer supported due to changes in IEEE Std 1666-2011 (see sc_event::notify, 5.10.6)." OFF)

option (ENABLE_PHASE_CALLBACKS "Enable the simulation phase callbacks (experimental)." OFF)

option (ENABLE_PHASE_CALLBACKS_TRACING "Enable the use of the (experimental) simulation phase callbacks for the sc_trace() implementation." ON)

option (ENABLE_PTHREADS
        "Use POSIX threads for SystemC processes instead of QuickThreads on Unix or Fiber on Windows."
        OFF)

option (INSTALL_TO_LIB_BUILD_TYPE_DIR
        "Install the libraries to lib-${CMAKE_BUILD_TYPE} to enable parallel installation of the different build variants. (default: OFF)"
        OFF)

if (NOT INSTALL_TO_LIB_BUILD_TYPE_DIR)
  option (INSTALL_TO_LIB_TARGET_ARCH_DIR "Install the libraries to lib-<target-arch> to facilitate linking applications, which build systems assume to find SystemC in lib-<target-arch>. (default: OFF)" OFF)
else (NOT INSTALL_TO_LIB_BUILD_TYPE_DIR)
  unset (INSTALL_TO_LIB_TARGET_ARCH_DIR CACHE)
endif (NOT INSTALL_TO_LIB_BUILD_TYPE_DIR)

if (CMAKE_HOST_UNIX AND NOT INSTALL_TO_LIB_TARGET_ARCH_DIR)
  option (INSTALL_LIB_TARGET_ARCH_SYMLINK "Install a symlink lib-<target-arch> to lib-${CMAKE_BUILD_TYPE} facilitating the linking of user code, which build system assumes to find the SystemC libraries under lib-<target-arch>." OFF)
else (CMAKE_HOST_UNIX AND NOT INSTALL_TO_LIB_TARGET_ARCH_DIR)
  unset (INSTALL_LIB_TARGET_ARCH_SYMLINK CACHE)
endif (CMAKE_HOST_UNIX AND NOT INSTALL_TO_LIB_TARGET_ARCH_DIR)

set (OVERRIDE_DEFAULT_STACK_SIZE 0 CACHE STRING "Define the default stack size used for SystemC (thread) processes. (> 0)")
if (OVERRIDE_DEFAULT_STACK_SIZE LESS 0)
  message (SEND_ERROR "Negative default stack size requested for SystemC (thread) processes.")
endif (OVERRIDE_DEFAULT_STACK_SIZE LESS 0)

mark_as_advanced(DISABLE_ASYNC_UPDATES
                 DISABLE_COPYRIGHT_MESSAGE
                 DISABLE_VIRTUAL_BIND
                 ENABLE_ASSERTIONS
                 ENABLE_EARLY_MAXTIME_CREATION
                 ENABLE_IMMEDIATE_SELF_NOTIFICATIONS
                 ENABLE_PHASE_CALLBACKS
                 ENABLE_PHASE_CALLBACKS_TRACING
                 OVERRIDE_DEFAULT_STACK_SIZE
                 DISABLE_VCD_SCOPES)


###############################################################################
# Set SystemC_TARGET_ARCH depending on OS and processor type
###############################################################################

message (STATUS "Checking OS and processor type.")

if (APPLE)
  list (LENGTH CMAKE_OSX_ARCHITECTURES N_OSX_ARCHITECTURES)
endif (APPLE)

if (NOT APPLE OR (APPLE AND (N_OSX_ARCHITECTURES LESS 2)))
  if (APPLE)
    if (N_OSX_ARCHITECTURES EQUAL 1)
      message (STATUS "The target architecture will be passed explicitly to the Apple compiler.")
      set (CMAKE_REQUIRED_FLAGS "-arch ${CMAKE_OSX_ARCHITECTURES}")
    endif (N_OSX_ARCHITECTURES EQUAL 1)
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
  check_symbol_exists (__aarch64__ "" HAS__aarch64__DEFINED)
  check_symbol_exists (__ppc__ "" HAS__ppc__DEFINED)
  check_symbol_exists (__powerpc "" HAS__powerpc_DEFINED)
endif (NOT APPLE OR (APPLE AND (N_OSX_ARCHITECTURES LESS 2)))


# Set SystemC_TARGET_ARCH according to the detected
# OS/processor/compiler platform
if (NOT SystemC_TARGET_ARCH)
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
  elseif (UNIX AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (HAS__i386_DEFINED)
      set (_TARGET_ARCH "linux")
    elseif (HAS__x86_64__DEFINED)
      set (_TARGET_ARCH "linux64")
    elseif (HAS__aarch64__DEFINED)
      set (_TARGET_ARCH "linuxaarch64")
    elseif (HAS__powerpc_DEFINED)
      # Untested platform.
      set (_TARGET_ARCH "linuxppc")
    elseif (HAS__sparc_DEFINED OR HAS__sparc__DEFINED)
      # Untested platform.
      set (_TARGET_ARCH "linuxsparc")
    else (HAS__i386_DEFINED)
      # Untested platform.
      set (_TARGET_ARCH "linuxunknown")
    endif (HAS__i386_DEFINED)
  elseif (UNIX AND CMAKE_SYSTEM_NAME MATCHES ".*BSD")
    if (HAS__i386_DEFINED)
      set (_TARGET_ARCH "bsd")
    elseif (HAS__x86_64__DEFINED)
      set (_TARGET_ARCH "bsd64")
    else (HAS__i386_DEFINED)
      # Untested platform.
      set (_TARGET_ARCH "bsdunknown")
    endif (HAS__i386_DEFINED)
  elseif (UNIX AND CMAKE_SYSTEM_NAME STREQUAL "SunOS"
          AND (HAS__sparc_DEFINED OR HAS__sparc__DEFINED))
    # Untested platform.
    set (_TARGET_ARCH "sparcOS5")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (_TARGET_ARCH "gcc${_TARGET_ARCH}")
    endif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  elseif (UNIX AND CMAKE_SYSTEM_NAME STREQUAL "HP-UX" AND HAS__hppa_DEFINED)
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
  set (SystemC_TARGET_ARCH ${_TARGET_ARCH} CACHE STRING "SystemC Target Architecture")
  unset (_TARGET_ARCH)
endif (NOT SystemC_TARGET_ARCH)


###############################################################################
# Configure QuickThreads
###############################################################################

# Set QuickThreads architecture based on the detected preprocessor symbols.
message (STATUS "Detect the target processor architecture for QuickThreads.")
if (ENABLE_PTHREADS)
  set (QT_ARCH "IGNORE") # Pthreads will be used for the SystemC coroutines.
elseif (MSVC)
  set (QT_ARCH "IGNORE") # Fibers will be used for the SystemC coroutines.
elseif (APPLE AND (N_OSX_ARCHITECTURES GREATER 1))
  set (QT_ARCH "IGNORE") # For universal libraries, Pthreads will be used for the SystemC coroutines.
elseif (HAS__sparc_DEFINED OR HAS__sparc__DEFINED)
  set (QT_ARCH "sparc")
elseif (HAS__hppa_DEFINED)
  set (QT_ARCH "hppa")
elseif (HAS__x86_64__DEFINED)
  set (QT_ARCH "x86_64")
elseif (HAS__i386_DEFINED)
  set (QT_ARCH "i386")
elseif (HAS__aarch64__DEFINED)
  set (QT_ARCH "aarch64")
elseif (HAS__ppc__DEFINED)
  set (QT_ARCH "powerpc_mach")
elseif (HAS__powerpc__DEFINED)
  set (QT_ARCH "powerpc_sys5")
else (ENABLE_PTHREADS)
  message (WARNING "QuickThreads is not supported on ${CMAKE_SYSTEM} on ${CMAKE_SYSTEM_PROCESSOR}.")
  set (QT_ARCH "IGNORE")
endif (ENABLE_PTHREADS)

if (QT_ARCH)
  # QuickThreads stack protection can benefit from posix_memalign
  include(CheckSymbolExists)
  check_symbol_exists(posix_memalign "stdlib.h" HAVE_POSIX_MEMALIGN)

  # To build QuickThreads, enable the assembler support.
  enable_language (ASM)
else (QT_ARCH)
  # Otherwise, fall back to Pthreads or Fiber.
  if (NOT MSVC)
    set (ENABLE_PTHREADS TRUE CACHE BOOL
         "Use POSIX threads for SystemC processes instead of QuickThreads on Unix or Fiber on Windows."
         FORCE)
  endif (NOT MSVC)
endif (QT_ARCH)


###############################################################################
# Configure Pthreads or Fiber if necessary.
###############################################################################
if (WIN32 AND ENABLE_PTHREADS)
  message (FATAL_ERROR "Pthreads is not supported on ${CMAKE_SYSTEM}.")
endif (WIN32 AND ENABLE_PTHREADS)

if (ENABLE_PTHREADS OR (QT_ARCH AND NOT DISABLE_ASYNC_UPDATES) OR NOT QT_ARCH)
  set (THREADS_PREFER_PTHREAD_FLAG ON)
  find_package (Threads REQUIRED)
  if (ENABLE_PTHREADS AND NOT CMAKE_USE_PTHREADS_INIT)
    message (SEND_ERROR "Failed to find the Pthreads library required to implement the SystemC coroutines and async_request_update() of primitive channels on Unix.")
  endif (ENABLE_PTHREADS AND NOT CMAKE_USE_PTHREADS_INIT)
endif (ENABLE_PTHREADS OR (QT_ARCH AND NOT DISABLE_ASYNC_UPDATES) OR NOT QT_ARCH)


###############################################################################
# Set the installation paths
###############################################################################

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  if (IS_ABSOLUTE $ENV{SYSTEMC_HOME})
    set (_CMAKE_INSTALL_PREFIX $ENV{SYSTEMC_HOME})
  elseif (UNIX)
    set (_CMAKE_INSTALL_PREFIX "/opt/systemc")
  elseif (WIN32)
    file (TO_CMAKE_PATH $ENV{ProgramFiles} _WIN32_INSTALL_PREFIX)
    set (_CMAKE_INSTALL_PREFIX "${_WIN32_INSTALL_PREFIX}/SystemC")
    unset (_WIN32_INSTALL_PREFIX)
  else (EXISTS $ENV{SYSTEMC_ROOT_DIR})
    set (_CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/systemc")
  endif (IS_ABSOLUTE $ENV{SYSTEMC_HOME})
  set (CMAKE_INSTALL_PREFIX ${_CMAKE_INSTALL_PREFIX} CACHE PATH "Installation root directory" FORCE)
  unset (_CMAKE_INSTALL_PREFIX)
endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# Set the installation paths for headers, libraries, and documentation.
set (CMAKE_INSTALL_DOCDIR "share/doc/systemc" CACHE PATH
     "Documentation installation directory") # otherwise mixed-case
if (INSTALL_TO_LIB_BUILD_TYPE_DIR)
  # Install libraries to lib-${CMAKE_BUILD_TYPE} so that different build variants
  # of the library can be installed in parallel to the same SystemC root
  # directory. The find_package() mechanism in config-mode of CMake will then
  # automatically find the correct libraries matching the chosen
  # ${CMAKE_BUILD_TYPE}.
  set (_INSTALL_LIBDIR lib-${CMAKE_BUILD_TYPE})
  string (TOLOWER ${_INSTALL_LIBDIR} _INSTALL_LIBDIR)
  set (CMAKE_INSTALL_LIBDIR ${_INSTALL_LIBDIR} CACHE PATH
       "Object code libraries installation directory" FORCE)
  unset (_INSTALL_LIBDIR)
elseif (INSTALL_TO_LIB_TARGET_ARCH_DIR)
  # Some build systems assume to find SystemC in lib-<target-arch>
  set (CMAKE_INSTALL_LIBDIR lib-${SystemC_TARGET_ARCH} CACHE PATH
       "Object code libraries installation directory" FORCE)
endif (INSTALL_TO_LIB_BUILD_TYPE_DIR)
include (GNUInstallDirs)

# Install symlink lib-<target-arch> to lib-${CMAKE_BUILD_TYPE}
if (INSTALL_LIB_TARGET_ARCH_SYMLINK AND CMAKE_HOST_UNIX)
  install (CODE "
    EXECUTE_PROCESS(COMMAND cmake -E create_symlink ${CMAKE_INSTALL_LIBDIR} lib-${SystemC_TARGET_ARCH}
                    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
  ")
endif (INSTALL_LIB_TARGET_ARCH_SYMLINK AND CMAKE_HOST_UNIX)

if (INSTALL_TO_LIB_BUILD_TYPE_DIR OR INSTALL_TO_LIB_TARGET_ARCH_DIR)
  set (SystemCLanguage_INSTALL_CMAKEDIR share/cmake/SystemCLanguage CACHE PATH
       "CMake package configuration installation directory" FORCE)
  set (SystemCTLM_INSTALL_CMAKEDIR share/cmake/SystemCTLM CACHE PATH
       "CMake package configuration installation directory" FORCE)
else (INSTALL_TO_LIB_BUILD_TYPE_DIR OR INSTALL_TO_LIB_TARGET_ARCH_DIR)
  set (SystemCLanguage_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/SystemCLanguage CACHE PATH
       "CMake package configuration installation directory for the SystemCLanguage package.")
  set (SystemCTLM_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/SystemCTLM CACHE PATH
       "CMake package configuration installation directory for the SystemCTLM package.")
endif (INSTALL_TO_LIB_BUILD_TYPE_DIR OR INSTALL_TO_LIB_TARGET_ARCH_DIR)
mark_as_advanced(SystemCLanguage_INSTALL_CMAKEDIR SystemCTLM_INSTALL_CMAKEDIR)
set(SystemCLanguage_INSTALL_FULL_CMAKEDIR "${CMAKE_INSTALL_PREFIX}/${SystemCLanguage_INSTALL_CMAKEDIR}")
set(SystemCTLM_INSTALL_FULL_CMAKEDIR "${CMAKE_INSTALL_PREFIX}/${SystemCTLM_INSTALL_CMAKEDIR}")


###############################################################################
# Configure RPATH so that all targets find the libraries they link to
###############################################################################

# Starting from CMake 2.8.12 use CMake's full RPATH support on Mac OS X
set (CMAKE_MACOSX_RPATH TRUE)

# Use, i.e., don't skip, the full RPATH for the build tree.
set (CMAKE_SKIP_BUILD_RPATH  FALSE)
# For build, don't use the install RPATH already (but later on when
# installing).
set (CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
# Set the RPATH to be used when installing.
set (CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
# Add the automatically determined parts of the RPATH, which point to
# directories outside the build tree, to the install RPATH.
set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Set the RPATH to be used when installing, but only if it's not a system directory
list (FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_FULL_LIBDIR}" isSystemDir)
if ("${isSystemDir}" STREQUAL "-1")
  set (CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
endif ("${isSystemDir}" STREQUAL "-1")


###############################################################################
# Configure status
###############################################################################

message (STATUS "========================================================================")
message (STATUS "Settings to build SystemC ${SystemCLanguage_VERSION} (${SystemCLanguage_VERSION_RELEASE_DATE}) and TLM ${SystemCTLM_VERSION} (${SystemCTLM_VERSION_RELEASE_DATE})")
message (STATUS "------------------------------------------------------------------------")
message (STATUS "BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}")
message (STATUS "BUILD_SOURCE_DOCUMENTATION = ${BUILD_SOURCE_DOCUMENTATION}")
message (STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
if (DISABLE_ASYNC_UPDATES)
  message ("DISABLE_ASYNC_UPDATES = ${DISABLE_ASYNC_UPDATES}")
else (DISABLE_ASYNC_UPDATES)
  message (STATUS "DISABLE_ASYNC_UPDATES = ${DISABLE_ASYNC_UPDATES}")
endif (DISABLE_ASYNC_UPDATES)
message (STATUS "DISABLE_COPYRIGHT_MESSAGE = ${DISABLE_COPYRIGHT_MESSAGE}")
message (STATUS "DISABLE_VCD_SCOPES = ${DISABLE_VCD_SCOPES}")
if (DISABLE_VIRTUAL_BIND)
  message ("DISABLE_VIRTUAL_BIND = ${DISABLE_VIRTUAL_BIND}")
else (DISABLE_VIRTUAL_BIND)
  message (STATUS "DISABLE_VIRTUAL_BIND = ${DISABLE_VIRTUAL_BIND}")
endif (DISABLE_VIRTUAL_BIND)
message (STATUS "ENABLE_ASSERTIONS = ${ENABLE_ASSERTIONS}")
message (STATUS "ENABLE_EARLY_MAXTIME_CREATION = ${ENABLE_EARLY_MAXTIME_CREATION}")
if (ENABLE_IMMEDIATE_SELF_NOTIFICATIONS)
  message ("ENABLE_IMMEDIATE_SELF_NOTIFICATIONS = ${ENABLE_IMMEDIATE_SELF_NOTIFICATIONS}")
else (ENABLE_IMMEDIATE_SELF_NOTIFICATIONS)
  message (STATUS "ENABLE_IMMEDIATE_SELF_NOTIFICATIONS = ${ENABLE_IMMEDIATE_SELF_NOTIFICATIONS}")
endif (ENABLE_IMMEDIATE_SELF_NOTIFICATIONS)
message (STATUS "ENABLE_PHASE_CALLBACKS = ${ENABLE_PHASE_CALLBACKS}")
message (STATUS "ENABLE_PHASE_CALLBACKS_TRACING = ${ENABLE_PHASE_CALLBACKS_TRACING}")
if (ENABLE_PTHREADS)
  message ("ENABLE_PTHREADS = ${ENABLE_PTHREADS}")
else (ENABLE_PTHREADS)
  message (STATUS "ENABLE_PTHREADS = ${ENABLE_PTHREADS}")
endif (ENABLE_PTHREADS)
if (OVERRIDE_DEFAULT_STACK_SIZE GREATER 0)
  message ("OVERRIDE_DEFAULT_STACK_SIZE = ${OVERRIDE_DEFAULT_STACK_SIZE}")
endif (OVERRIDE_DEFAULT_STACK_SIZE GREATER 0)
message (STATUS "SystemC_TARGET_ARCH = ${SystemC_TARGET_ARCH}")
message (STATUS "SystemCLanguage_VERSION = ${SystemCLanguage_VERSION}")
message (STATUS "SystemCTLM_VERSION = ${SystemCTLM_VERSION}")
message (STATUS "INSTALL_TO_LIB_BUILD_TYPE_DIR = ${INSTALL_TO_LIB_BUILD_TYPE_DIR}")
message (STATUS "INSTALL_TO_LIB_TARGET_ARCH_DIR = ${INSTALL_TO_LIB_TARGET_ARCH_DIR}")
message (STATUS "INSTALL_LIB_TARGET_ARCH_SYMLINK = ${INSTALL_LIB_TARGET_ARCH_SYMLINK}")
message (STATUS "------------------------------------------------------------------------")
message (STATUS "CMAKE_CXX_STANDARD = ${CMAKE_CXX_STANDARD}")
message (STATUS "CMAKE_CXX_STANDARD_REQUIRED = ${CMAKE_CXX_STANDARD_REQUIRED}")
message (STATUS "CMAKE_SYSTEM = ${CMAKE_SYSTEM}")
message (STATUS "CMAKE_SYSTEM_PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}")
message (STATUS "QT_ARCH = ${QT_ARCH}")
if (DEFINED CMAKE_OSX_ARCHITECTURES)
  message (STATUS "CMAKE_OSX_ARCHITECTURES = ${CMAKE_OSX_ARCHITECTURES}")
endif (DEFINED CMAKE_OSX_ARCHITECTURES)
if (DEFINED CMAKE_OSX_DEPLOYMENT_TARGET)
  message (STATUS "CMAKE_OSX_DEPLOYMENT_TARGET = ${CMAKE_OSX_DEPLOYMENT_TARGET}")
endif (DEFINED CMAKE_OSX_DEPLOYMENT_TARGET)
if (DEFINED CMAKE_OSX_SYSROOT)
  message (STATUS "CMAKE_OSX_SYSROOT = ${CMAKE_OSX_SYSROOT}")
endif(DEFINED CMAKE_OSX_SYSROOT)
if (DEFINED Threads_FOUND)
  message (STATUS "Threads_FOUND = ${Threads_FOUND}")
endif (DEFINED Threads_FOUND)
if (DEFINED CMAKE_USE_PTHREADS_INIT)
  message (STATUS "CMAKE_USE_PTHREADS_INIT = ${CMAKE_USE_PTHREADS_INIT}")
endif (DEFINED CMAKE_USE_PTHREADS_INIT)
if (DEFINED CMAKE_USE_WIN32_THREADS_INIT)
  message (STATUS "CMAKE_USE_WIN32_THREADS_INIT = ${CMAKE_USE_WIN32_THREADS_INIT}")
endif (DEFINED CMAKE_USE_WIN32_THREADS_INIT)
if (DEFINED CMAKE_THREAD_LIBS_INIT)
  message (STATUS "CMAKE_THREAD_LIBS_INIT = ${CMAKE_THREAD_LIBS_INIT}")
endif (DEFINED CMAKE_THREAD_LIBS_INIT)
message (STATUS "------------------------------------------------------------------------")
message (STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message (STATUS "CMAKE_INSTALL_BINDIR = ${CMAKE_INSTALL_BINDIR}")
message (STATUS "CMAKE_INSTALL_DOCDIR = ${CMAKE_INSTALL_DOCDIR}")
message (STATUS "CMAKE_INSTALL_INCLUDEDIR = ${CMAKE_INSTALL_INCLUDEDIR}")
message (STATUS "CMAKE_INSTALL_LIBDIR = ${CMAKE_INSTALL_LIBDIR}")
message (STATUS "INSTALL_CMAKEDIR = ${INSTALL_CMAKEDIR}")
message (STATUS "========================================================================")


###############################################################################
# Subdirectories
###############################################################################

add_subdirectory (docs)
add_subdirectory (src)
add_subdirectory (examples)


###############################################################################
# Install README files
###############################################################################

install (FILES AUTHORS.md
               INSTALL.md
               cmake/INSTALL_USING_CMAKE
               LICENSE
               NOTICE
               README.md
               RELEASENOTES
         DESTINATION ${CMAKE_INSTALL_DOCDIR}
         COMPONENT doc)


###############################################################################
# Provide package of the SystemC library to other programs using CMake
###############################################################################

# Export the systemc library target for usage by other programs
# ... to local build tree
export (TARGETS systemc
        NAMESPACE SystemC::
        FILE SystemCLanguageTargets.cmake)
# ... to installation tree
install (EXPORT SystemCLanguageTargets
         NAMESPACE SystemC::
         DESTINATION ${SystemCLanguage_INSTALL_CMAKEDIR}
         COMPONENT dev)

# Register the SystemCLanguage and SystemCTLM packages for use from the
# build tree. (This registers the build tree with the global CMake-registry.)
export (PACKAGE SystemCLanguage)
export (PACKAGE SystemCTLM)

# Create the <Package>Config.cmake and <Package>ConfigVersion files
include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/SystemCLanguageConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/SystemCLanguageConfig.cmake
  INSTALL_DESTINATION ${SystemCLanguage_INSTALL_CMAKEDIR})
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/SystemCLanguageConfigVersion.cmake"
  VERSION ${SystemCLanguage_VERSION_MAJOR}.${SystemCLanguage_VERSION_MINOR}.${SystemCLanguage_VERSION_PATCH}.${SystemCLanguage_VERSION_RELEASE_DATE}
  COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(cmake/SystemCTLMConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/SystemCTLMConfig.cmake
  INSTALL_DESTINATION ${SystemCTLM_INSTALL_CMAKEDIR})
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/SystemCTLMConfigVersion.cmake"
  VERSION ${SystemCTLM_VERSION_MAJOR}.${SystemCTLM_VERSION_MINOR}.${SystemCTLM_VERSION_PATCH}.${SystemCTLM_VERSION_RELEASE_DATE}
  COMPATIBILITY AnyNewerVersion
)

# Install the <Package>Config.cmake and <Package>ConfigVersion.cmake
install(FILES "${PROJECT_BINARY_DIR}/SystemCLanguageConfig.cmake"
              "${PROJECT_BINARY_DIR}/SystemCLanguageConfigVersion.cmake"
        DESTINATION "${SystemCLanguage_INSTALL_CMAKEDIR}"
        COMPONENT dev)
install(FILES "${PROJECT_BINARY_DIR}/SystemCTLMConfig.cmake"
              "${PROJECT_BINARY_DIR}/SystemCTLMConfigVersion.cmake"
        DESTINATION "${SystemCTLM_INSTALL_CMAKEDIR}"
        COMPONENT dev)
