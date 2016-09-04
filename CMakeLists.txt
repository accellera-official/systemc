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
# BUILD_SOURCE_DOCUMENTATION    Build source code documentation using Doxygen
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
# DISABLE_ASYNC_UPDATES         Disable async_request_update() support.
#
# DISABLE_VIRTUAL_BIND          Disable the definition of bind() member
#                               functions of ports and exports as "virtual",
#                               which is incompatible with old TLM library
#                               implementations (< 2.0.2).
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
# QT_ARCH                       QuickThreads architecture (auto-detected:
#                               hppa, x86_64, i386, powerpc_mach,
#                               powerpc_sys5, IGNORE).
#
# SystemC_ROOT_DIR              Root directory of the SystemC libraries
#                               installation (default $SYSTEMC_ROOT_DIR,
#                               $SYSTEMC_ROOT, $SYSTEMC_HOME, $SYSTEMC_PATH
#                               /opt/systemc/ (Unix, Cygwin, MSYS), or
#                               $ENV{ProgramFiles}/SystemC/ (MinGW and
#                               MS Visual Studio)). CMAKE_INSTALL_PREFIX will
#                               be set to its value.
#
# SystemC_TARGET_ARCH           Target architecture according to the
#                               Accellera SystemC conventions set either from
#                               $ENV{SYSTEMC_TARGET_ARCH}, $ENV{SYSTEMC_ARCH},
#                               or detected by CMake.
#
# INSTALL_TO_LIB_BUILD_TYPE_DIR Install the libraries to a
#                               lib-${CMAKE_BUILD_TYPE}/ to enable parallel
#                               installation of the different build variants.
#                               (default: ON)
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
# - Improve further the directory layout for the installation of SystemC and
#   libraries using it. The installed CMake project configuration files should
#   be automatically found by other CMake projects (e.g., SCV, SystemC AMS).
#   <http://www.cmake.org/Wiki/CMake/Tutorials/How_to_create_a_ProjectConfig.cmake_file>
#
# - Detect C++'11-compatible compiler and then pass the right flags
#   to use C++'11 language and library features.
#
# - Replace the shell script wrappers config/test.sh.in used for the examples
#   execution, with a platform-independent solution (on Windows, a Unix Shell
#   is required, e.g., using MSYS2 or Cygwin).
#
# - Check whether preprocessor definitions WIN32, _LIB, _DEBUG, _CONSOLE, and
#   NOGDI are necessary on WIN32 platforms (MSVC and MinGW).
#
# - Don't generate source documentation with Doxygen in the source tree.
#
# - Turn on more compiler warnings by default once SystemC is supposed to
#   compile without any warnings on recent compiler versions. Currently, the
#   compiler warning flags are set to reduce the number of generated compiler
#   warnings (especially with recent Clang versions, e.g., Apple Clang 6.0) to
#   an acceptable amount. With the chosen flags, the SystemC library compiles
#   without warnings, but some examples still produce warnings.
#
# - Fix Xcode build settings warning by properly initializing
#   CMAKE_OSX_ARCHITECTURES (cf. CMake issue 0014736). Ensure to build only
#   for the selected QuickThreads architecture!
#
# - Fix confusing "Warning: Did not find file Compiler/-ASM" due to no
#   specification of an Assembler dialect. The warning disappears if "ASM-ATT"
#   is used instead of "ASM" as argument to enable_language(). However, this
#   currently breaks compilation of QuickThreads with Xcode.
#
# - Test on as many different hardware / OS platforms as possible.
#
# - Set more reliably the needed compiler options, e.g., /vmg for MSVC.
#
# - Check whether SystemCConfig.cmake.in should define further compiler options
#   beyond those defined currently for MSVC (/vmg /MP).
#
# - Check whether SystemCConfig.cmake.in forwards all necessary
#   preprocessor definitions that need to be consistent for compilation of
#   SystemC libraries as well as applications (e.g., remove
#   SC_ENABLE_ASSERTIONS?).
#
# - Find a better way to make sure all code linking to the SystemC library is
#   also linking to ${CMAKE_THREAD_LIBS_INIT} if needed.
#
# - Fix execution of the unit tests from within Xcode:
#
#     [...]
#     make: *** [RUN_TESTS_buildpart_0] Error 8
#     Command /bin/sh failed with exit code 2
#
###############################################################################


cmake_minimum_required (VERSION 2.8.5)

project (SystemC CXX C)

set(SystemC_VERSION_FILE "${PROJECT_SOURCE_DIR}/src/sysc/kernel/sc_ver.h")
if(EXISTS ${SystemC_VERSION_FILE})
    file(READ ${SystemC_VERSION_FILE} SYSTEMC_VERSION_FILE_CONTENT)
    string(REGEX MATCH "SC_VERSION_MAJOR[ \t]+([0-9]+)" SystemC_VERSION_MAJOR ${SYSTEMC_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemC_VERSION_MAJOR ${SystemC_VERSION_MAJOR})
    string(REGEX MATCH "SC_VERSION_MINOR[ \t]+([0-9]+)" SystemC_VERSION_MINOR ${SYSTEMC_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemC_VERSION_MINOR ${SystemC_VERSION_MINOR})
    string(REGEX MATCH "SC_VERSION_PATCH[ \t]+([0-9]+)" SystemC_VERSION_PATCH ${SYSTEMC_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemC_VERSION_PATCH ${SystemC_VERSION_PATCH})
    string(REGEX MATCH "SC_IS_PRERELEASE[ \t]+([0-9]+)" SystemC_VERSION_IS_PRERELEASE ${SYSTEMC_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemC_VERSION_IS_PRERELEASE ${SystemC_VERSION_IS_PRERELEASE})
    string(REGEX MATCH "SC_VERSION_PRERELEASE[ \t]+\"([^\".]+)\"" SystemC_VERSION_PRERELEASE ${SYSTEMC_VERSION_FILE_CONTENT})
    string(REGEX REPLACE "(.*)\"(.*)\"" "\\2" SystemC_VERSION_PRERELEASE ${SystemC_VERSION_PRERELEASE})
    string(REGEX MATCH "SYSTEMC_VERSION[ \t]+([0-9]+)" SystemC_VERSION_RELEASE_DATE ${SYSTEMC_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemC_VERSION_RELEASE_DATE ${SystemC_VERSION_RELEASE_DATE})
else()
    message (FATAL_ERROR "Unable to read SystemC version file")
endif()

set (SystemC_VERSION "${SystemC_VERSION_MAJOR}.${SystemC_VERSION_MINOR}.${SystemC_VERSION_PATCH}")
if (SystemC_VERSION_IS_PRERELEASE)
  set (SystemC_VERSION "${SystemC_VERSION}_${SystemC_VERSION_PRERELEASE}_${SystemC_VERSION_RELEASE_DATE}")
endif (SystemC_VERSION_IS_PRERELEASE)
set (SystemC_SOVERSION "${SystemC_VERSION_MAJOR}.${SystemC_VERSION_MINOR}")

set(TLM_VERSION_FILE "${PROJECT_SOURCE_DIR}/src/tlm_core/tlm_2/tlm_version.h")
if(EXISTS ${TLM_VERSION_FILE})
    file(READ ${TLM_VERSION_FILE} TLM_VERSION_FILE_CONTENT)
    string(REGEX MATCH "TLM_VERSION_MAJOR[ \t]+([0-9]+)" TLM_VERSION_MAJOR ${TLM_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" TLM_VERSION_MAJOR ${TLM_VERSION_MAJOR})
    string(REGEX MATCH "TLM_VERSION_MINOR[ \t]+([0-9]+)" TLM_VERSION_MINOR ${TLM_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" TLM_VERSION_MINOR ${TLM_VERSION_MINOR})
    string(REGEX MATCH "TLM_VERSION_PATCH[ \t]+([0-9]+)" TLM_VERSION_PATCH ${TLM_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" TLM_VERSION_PATCH ${TLM_VERSION_PATCH})
    string(REGEX MATCH "TLM_IS_PRERELEASE[ \t]+([0-9]+)" TLM_VERSION_IS_PRERELEASE ${TLM_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" TLM_VERSION_IS_PRERELEASE ${TLM_VERSION_IS_PRERELEASE})
    string(REGEX MATCH "TLM_VERSION_PRERELEASE[ \t]+\"([^\".]+)\"" TLM_VERSION_PRERELEASE ${TLM_VERSION_FILE_CONTENT})
    string(REGEX REPLACE "(.*)\"(.*)\"" "\\2" TLM_VERSION_PRERELEASE ${TLM_VERSION_PRERELEASE})
    string(REGEX MATCH "TLM_VERSION_RELEASE_YEAR[ \t]+\"([0-9]+)\"" TLM_VERSION_RELEASE_YEAR ${TLM_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" TLM_VERSION_RELEASE_YEAR ${TLM_VERSION_RELEASE_YEAR})
    string(REGEX MATCH "TLM_VERSION_RELEASE_MONTH[ \t]+\"([0-9]+)\"" TLM_VERSION_RELEASE_MONTH ${TLM_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" TLM_VERSION_RELEASE_MONTH ${TLM_VERSION_RELEASE_MONTH})
    string(REGEX MATCH "TLM_VERSION_RELEASE_DAY[ \t]+\"([0-9]+)\"" TLM_VERSION_RELEASE_DAY ${TLM_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" TLM_VERSION_RELEASE_DAY ${TLM_VERSION_RELEASE_DAY})
else()
    message(FATAL_ERROR "Unable to read TLM version file")
endif()

set (TLM_VERSION_RELEASE_DATE "${TLM_VERSION_RELEASE_YEAR}${TLM_VERSION_RELEASE_MONTH}${TLM_VERSION_RELEASE_DAY}")
set (TLM_VERSION "${TLM_VERSION_MAJOR}.${TLM_VERSION_MINOR}.${TLM_VERSION_PATCH}")
if (TLM_IS_PRERELEASE)
  set (TLM_VERSION "${TLM_VERSION}_${TLM_VERSION_PRERELEASE}_${TLM_VERSION_RELEASE_DATE}")
endif (TLM_IS_PRERELEASE)
set (TLM_SOVERSION "${TLM_MAJOR_VERSION}.${TLM_MINOR_VERSION}")


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
  message (WARNING "DLL support on Windows is still experimental and requires more testing!")
endif (BUILD_SHARED_LIBS AND (WIN32 OR CYGWIN))

option (BUILD_SOURCE_DOCUMENTATION "Build source documentation with Doxygen." OFF)
option (BUILD_EXAMPLES "Build SystemC and TLM examples." OFF)
if (BUILD_EXAMPLES)
  enable_testing ()
endif (BUILD_EXAMPLES)

option (DISABLE_ASYNC_UPDATES "Disable async_request_update() support." OFF)

option (DISABLE_COPYRIGHT_MESSAGE "Do not print the copyright message when starting the application." OFF)

option (DISABLE_VIRTUAL_BIND "Disable the definition of bind() member functions of ports and exports as \"virtual\", which is incompatible with old TLM library implementations (< 2.0.2)." OFF)

option (ENABLE_ASSERTIONS "Always enable the `sc_assert' expressions." ON)

option (ENABLE_EARLY_MAXTIME_CREATION "Allow creation of sc_time objects with a value of sc_max_time() before finalizing the time resolution." ON)

option (ENABLE_IMMEDIATE_SELF_NOTIFICATIONS "Enable immediate self-notification of processes, which is no longer supported due to changes in IEEE Std 1666-2011 (see sc_event::notify, 5.10.6)." OFF)

option (ENABLE_PHASE_CALLBACKS "Enable the simulation phase callbacks (experimental)." OFF)

option (ENABLE_PHASE_CALLBACKS_TRACING "Enable the use of the (experimental) simulation phase callbacks for the sc_trace() implementation." ON)

option (ENABLE_PTHREADS
        "Use POSIX threads for SystemC processes instead of QuickThreads on Unix or Fiber on Windows."
        OFF)

mark_as_advanced(DISABLE_ASYNC_UPDATES
                 DISABLE_COPYRIGHT_MESSAGE
                 DISABLE_VIRTUAL_BIND
                 ENABLE_ASSERTIONS
                 ENABLE_EARLY_MAXTIME_CREATION
                 ENABLE_IMMEDIATE_SELF_NOTIFICATIONS
                 ENABLE_PHASE_CALLBACKS
                 ENABLE_PHASE_CALLBACKS_TRACING)

option (INSTALL_TO_LIB_BUILD_TYPE_DIR "Install the libraries to lib-${CMAKE_BUILD_TYPE} to enable parallel installation of the different build variants. (default: ON)" ON)

if (CMAKE_HOST_UNIX)
  option (INSTALL_LIB_TARGET_ARCH_SYMLINK "Install a symlink lib-<target-arch> to lib-${CMAKE_BUILD_TYPE} facilitating the linking of user code, which build system assumes to find the SystemC libraries under lib-<target-arch>." OFF)
endif (CMAKE_HOST_UNIX)

set (OVERRIDE_DEFAULT_STACK_SIZE 0 CACHE STRING "Define the default stack size used for SystemC (thread) processes. (> 0)")


###############################################################################
# Set SystemC_TARGET_ARCH and QT_ARCH depending on OS and processor type
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
  if (ENABLE_PTHREADS)
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
  else (ENABLE_PTHREADS)
    message (WARNING "QuickThreads is not supported on ${CMAKE_SYSTEM} on ${CMAKE_SYSTEM_PROCESSOR}.")
    set (_QT_ARCH "IGNORE")
  endif (ENABLE_PTHREADS)
  set (QT_ARCH ${_QT_ARCH} CACHE STRING "QuickThreads architecture (IGNORE, sparc, hppa, x86_64, i386, powerpc_mach, powerpc_sys5).")
  unset (_QT_ARCH)
endif (NOT DEFINED QT_ARCH)

if (QT_ARCH)
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

if (ENABLE_PTHREADS OR (QT_ARCH AND NOT DISABLE_ASYNC_UPDATES) OR NOT QT_ARCH)
  set (CMAKE_THREAD_PREFER_PTHREAD)
  find_package (Threads REQUIRED)
  if (ENABLE_PTHREADS)
    if (NOT CMAKE_USE_PTHREADS_INIT)
      message (SEND_ERROR "Failed to find the Pthreads library required to implement the SystemC coroutines and async_request_update() of primitive channels on Unix.")
    endif (NOT CMAKE_USE_PTHREADS_INIT)
    add_definitions (-DSC_USE_PTHREADS)
  endif (ENABLE_PTHREADS)
endif (ENABLE_PTHREADS OR (QT_ARCH AND NOT DISABLE_ASYNC_UPDATES) OR NOT QT_ARCH)


###############################################################################
# Set the preprocessor definitions and platform-dependent compiler flags.
###############################################################################

add_definitions (-DSC_INCLUDE_FX)

if (DISABLE_ASYNC_UPDATES)
  add_definitions (-DSC_DISABLE_ASYNC_UPDATES)
endif (DISABLE_ASYNC_UPDATES)

if (DISABLE_COPYRIGHT_MESSAGE)
  add_definitions (-DSC_DISABLE_COPYRIGHT_MESSAGE)
endif (DISABLE_COPYRIGHT_MESSAGE)

if (DISABLE_VIRTUAL_BIND)
  add_definitions (-DSC_DISABLE_VIRTUAL_BIND)
endif (DISABLE_VIRTUAL_BIND)

if (ENABLE_ASSERTIONS)
  add_definitions (-DSC_ENABLE_ASSERTIONS)
endif (ENABLE_ASSERTIONS)

if (ENABLE_EARLY_MAXTIME_CREATION)
  add_definitions (-DSC_ENABLE_EARLY_MAXTIME_CREATION)
endif (ENABLE_EARLY_MAXTIME_CREATION)

if (ENABLE_IMMEDIATE_SELF_NOTIFICATIONS)
  add_definitions (-DSC_ENABLE_IMMEDIATE_SELF_NOTIFICATIONS)
endif (ENABLE_IMMEDIATE_SELF_NOTIFICATIONS)

if (ENABLE_PHASE_CALLBACKS)
  add_definitions (-DSC_ENABLE_SIMULATION_PHASE_CALLBACKS)
endif (ENABLE_PHASE_CALLBACKS)

if (ENABLE_PHASE_CALLBACKS_TRACING)
  add_definitions (-DSC_ENABLE_SIMULATION_PHASE_CALLBACKS_TRACING)
endif (ENABLE_PHASE_CALLBACKS_TRACING)

if (OVERRIDE_DEFAULT_STACK_SIZE GREATER 0)
  add_definitions (-DSC_OVERRIDE_DEFAULT_STACK_SIZE=${OVERRIDE_DEFAULT_STACK_SIZE})
elseif (OVERRIDE_DEFAULT_STACK_SIZE LESS 0)
  message (SEND_ERROR "Negative default stack size requested for SystemC (thread) processes.")
endif (OVERRIDE_DEFAULT_STACK_SIZE GREATER 0)

if (BUILD_SHARED_LIBS AND (WIN32 OR CYGWIN))
  add_definitions (-DSC_WIN_DLL)
endif (BUILD_SHARED_LIBS AND (WIN32 OR CYGWIN))

# Set additional compiler flags for known compilers.
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  if (NOT CMAKE_C_FLAGS)
    set (CMAKE_C_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wno-unused-variable" CACHE STRING "Flags used by the compiler during all build types." FORCE)
  endif (NOT CMAKE_C_FLAGS)
  if (NOT CMAKE_CXX_FLAGS)
    set (CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wno-unused-variable" CACHE STRING "Flags used by the compiler during all build types." FORCE)
  endif (NOT CMAKE_CXX_FLAGS)
elseif (MSVC)
  if (NOT CMAKE_C_FLAGS MATCHES "/vmg")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /vmg" CACHE STRING "Flags used by the compiler during all build types." FORCE)
  endif (NOT CMAKE_C_FLAGS MATCHES "/vmg")
  if (NOT CMAKE_CXX_FLAGS MATCHES "/vmg")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /vmg" CACHE STRING "Flags used by the compiler during all build types." FORCE)
  endif (NOT CMAKE_CXX_FLAGS MATCHES "/vmg")
  if (NOT CMAKE_C_FLAGS MATCHES "/MP")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP" CACHE STRING "Flags used by the compiler during all build types." FORCE)
  endif (NOT CMAKE_C_FLAGS MATCHES "/MP")
  if (NOT CMAKE_CXX_FLAGS MATCHES "/MP")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP" CACHE STRING "Flags used by the compiler during all build types." FORCE)
  endif (NOT CMAKE_CXX_FLAGS MATCHES "/MP")
endif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

if (APPLE)
  # On Mac OS X, the linker needs to be told that it's OK that _sc_main is an undefined symbol.
  set (CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -Wl,-U,_sc_main")
  set (CMAKE_SHARED_MODULE_CREATE_CXX_FLAGS "${CMAKE_SHARED_MODULE_CREATE_CXX_FLAGS} -Wl,-U,_sc_main")
endif (APPLE)

# TODO: Check required preprocessor definitions for WIN32.
if (WIN32)
  add_definitions (-DWIN32) # Needed?
  if (MSVC)
    # Additional definition for Microsoft Visual Studio C++ compilers
    add_definitions(-D_LIB)               # Needed?
    # Is the definition of _DEBUG needed or already handled by CMake?
    if (CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
      add_definitions (-D_DEBUG) # Needed?
    else (CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
      add_definitions (-DNDEBUG) # Needed?
    endif (CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
    add_definitions(-D_USE_MATH_DEFINES)  # Needed?
    # Suppress warnings about sprintf from Visual C++ 2005
    add_definitions (-D_CRT_SECURE_NO_DEPRECATE) # Needed?
    # Suppress warnings from Visual C++ 2008
    add_definitions (-D_CRT_SECURE_NO_WARNINGS) # Needed?
  endif (MSVC)
endif (WIN32)


###############################################################################
# Set the installation paths
###############################################################################

# If necessary, default-initialize SystemC_ROOT_DIR and set
# CMAKE_INSTALL_PREFIX to its value.
if (NOT SystemC_ROOT_DIR)
  if (EXISTS $ENV{SYSTEMC_ROOT_DIR})
    set (_SystemC_ROOT_DIR $ENV{SYSTEMC_ROOT_DIR})
  elseif (EXISTS $ENV{SYSTEMC_ROOT})
    set (_SystemC_ROOT_DIR $ENV{SYSTEMC_ROOT})
  elseif (EXISTS $ENV{SYSTEMC_HOME})
    set (_SystemC_ROOT_DIR $ENV{SYSTEMC_HOME})
  elseif (EXISTS $ENV{SYSTEMC_PATH})
    set (_SystemC_ROOT_DIR $ENV{SYSTEMC_PATH})
  elseif (UNIX)
    set (_SystemC_ROOT_DIR "/opt/systemc")
  elseif (WIN32)
    file (TO_CMAKE_PATH $ENV{ProgramFiles} _WIN32_INSTALL_PREFIX)
    set (_SystemC_ROOT_DIR "${_WIN32_INSTALL_PREFIX}/SystemC")
    unset (_WIN32_INSTALL_PREFIX)
  else (EXISTS $ENV{SYSTEMC_ROOT_DIR})
    set (_SystemC_ROOT_DIR "${CMAKE_INSTALL_PREFIX}/systemc")
  endif (EXISTS $ENV{SYSTEMC_ROOT_DIR})
  set (SystemC_ROOT_DIR ${_SystemC_ROOT_DIR} CACHE PATH "installation root for the SystemC libraries")
  unset (_SystemC_ROOT_DIR)
endif (NOT SystemC_ROOT_DIR)
set (CMAKE_INSTALL_PREFIX ${SystemC_ROOT_DIR} CACHE PATH "Install directory used by install." FORCE)
mark_as_advanced (CMAKE_INSTALL_PREFIX)

# Set the installation paths for headers, libraries, and documentation.
set (CMAKE_INSTALL_DOCDIR "share/doc/systemc") # otherwise mixed-case
if (INSTALL_TO_LIB_BUILD_TYPE_DIR)
  # Install libraries to lib-${CMAKE_BUILD_TYPE} so that different build variants
  # of the library can be installed in parallel to the same SystemC root
  # directory. The find_package() mechanism in config-mode of CMake will then
  # automatically find the correct libraries matching the chosen
  # ${CMAKE_BUILD_TYPE}.
  set (_INSTALL_LIBDIR lib-${CMAKE_BUILD_TYPE})
  string (TOLOWER ${_INSTALL_LIBDIR} _INSTALL_LIBDIR)
  set (CMAKE_INSTALL_LIBDIR ${_INSTALL_LIBDIR})
  unset (_INSTALL_LIBDIR)
endif (INSTALL_TO_LIB_BUILD_TYPE_DIR)
include (GNUInstallDirs)

# Install symlink lib-<target-arch> to lib-${CMAKE_BUILD_TYPE}
if (INSTALL_LIB_TARGET_ARCH_SYMLINK AND CMAKE_HOST_UNIX)
  install (CODE "
    EXECUTE_PROCESS(COMMAND cmake -E create_symlink ${CMAKE_INSTALL_LIBDIR} lib-${SystemC_TARGET_ARCH}
                    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
  ")
endif (INSTALL_LIB_TARGET_ARCH_SYMLINK AND CMAKE_HOST_UNIX)


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
message (STATUS "Settings to build SystemC ${SystemC_VERSION} (${SystemC_VERSION_RELEASE_DATE}) and TLM ${TLM_VERSION} (${TLM_VERSION_RELEASE_DATE})")
message (STATUS "------------------------------------------------------------------------")
message (STATUS "BUILD_EXAMPLES = ${BUILD_EXAMPLES}")
message (STATUS "BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}")
message (STATUS "BUILD_SOURCE_DOCUMENTATION = ${BUILD_SOURCE_DOCUMENTATION}")
message (STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
if (DISABLE_ASYNC_UPDATES)
  message ("DISABLE_ASYNC_UPDATES = ${DISABLE_ASYNC_UPDATES}")
else (DISABLE_ASYNC_UPDATES)
  message (STATUS "DISABLE_ASYNC_UPDATES = ${DISABLE_ASYNC_UPDATES}")
endif (DISABLE_ASYNC_UPDATES)
if (DISABLE_VIRTUAL_BIND)
  message ("DISABLE_VIRTUAL_BIND = ${DISABLE_VIRTUAL_BIND}")
else (DISABLE_VIRTUAL_BIND)
  message (STATUS "DISABLE_VIRTUAL_BIND = ${DISABLE_VIRTUAL_BIND}")
endif (DISABLE_VIRTUAL_BIND)
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
message (STATUS "SystemC_ROOT_DIR = ${SystemC_ROOT_DIR}")
message (STATUS "SystemC_TARGET_ARCH = ${SystemC_TARGET_ARCH}")
message (STATUS "INSTALL_TO_LIB_BUILD_TYPE_DIR = ${INSTALL_TO_LIB_BUILD_TYPE_DIR}")
message (STATUS "INSTALL_LIB_TARGET_ARCH_SYMLINK = ${INSTALL_LIB_TARGET_ARCH_SYMLINK}")
message (STATUS "------------------------------------------------------------------------")
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
               cmake/INSTALL_USING_CMAKE
               LICENSE
               NEWS
               README
               RELEASENOTES
         DESTINATION ${CMAKE_INSTALL_DOCDIR}
         COMPONENT doc)


###############################################################################
# Provide package of the SystemC library to other programs using CMake
###############################################################################

set (INSTALL_CMAKEDIR cmake)
set (INSTALL_FULL_CMAKEDIR ${CMAKE_INSTALL_PREFIX}/${INSTALL_CMAKEDIR})

# Export the systemc library target for usage by other programs
# ... to local build tree
export (TARGETS systemc FILE SystemCTargets.cmake)
# ... to installation tree
install (EXPORT SystemCTargets DESTINATION ${INSTALL_CMAKEDIR}
                               COMPONENT dev)

# Register the SystemC package for use from the build tree.
# (This registers the build tree with the global CMake-registry.)
export (PACKAGE SystemC)
export (PACKAGE TLM)

# Create the SystemCConfig.cmake, and SystemCConfigVersion files
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")
configure_file(cmake/SystemCConfig.cmake.in
  "${PROJECT_BINARY_DIR}/SystemCConfig.cmake" @ONLY)
configure_file(cmake/TLMConfig.cmake.in
  "${PROJECT_BINARY_DIR}/TLMConfig.cmake" @ONLY)
# ... for the install tree
file (RELATIVE_PATH REL_INCLUDEDIR
                    "${INSTALL_FULL_CMAKEDIR}" "${CMAKE_INSTALL_FULL_INCLUDEDIR}")
set(CONF_INCLUDE_DIRS "\${SystemC_CMAKE_DIR}/${REL_INCLUDEDIR}")
configure_file(cmake/SystemCConfig.cmake.in
  "${PROJECT_BINARY_DIR}/cmake/SystemCConfig.cmake" @ONLY)
configure_file(cmake/TLMConfig.cmake.in
  "${PROJECT_BINARY_DIR}/cmake/TLMConfig.cmake" @ONLY)
# ... for both
configure_file(cmake/SystemCConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/SystemCConfigVersion.cmake" @ONLY)
configure_file(cmake/TLMConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/TLMConfigVersion.cmake" @ONLY)
unset (REL_INCLUDEDIR)
unset (CONF_INCLUDE_DIRS)

# Install the FooBarConfig.cmake and FooBarConfigVersion.cmake
install(FILES "${PROJECT_BINARY_DIR}/cmake/SystemCConfig.cmake"
              "${PROJECT_BINARY_DIR}/SystemCConfigVersion.cmake"
              "${PROJECT_BINARY_DIR}/cmake/TLMConfig.cmake"
              "${PROJECT_BINARY_DIR}/TLMConfigVersion.cmake"
        DESTINATION "${INSTALL_CMAKEDIR}"
        COMPONENT dev)
