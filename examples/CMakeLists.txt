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
# examples/CMakeLists.txt --
# CMake script to configure the SystemC sources and to generate native
# Makefiles and project workspaces for your compiler environment.
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


if (NOT BUILD_EXAMPLES)
  set_directory_properties (PROPERTIES EXCLUDE_FROM_ALL 1)
endif (NOT BUILD_EXAMPLES)


########################################################################
# Common definitions for all examples.
########################################################################

include_directories (BEFORE ${SystemC_SOURCE_DIR}/src)

if (WIN32)
  # Not sure, if these preprocessor definitions are strictly required to build
  # console applications with Visual Studio. Instead of _CONSOLE, _LIB may be
  # more appropriate. Maybe also WIN32, _WIN32, _WIN64 needs to be #defined to
  # generate correct VS projects.
  #
  # Resources:
  # <http://social.msdn.microsoft.com/Forums/en-US/vclanguage/thread/9e45a760-f2b8-47b2-8b8c-dfeeb1b24c92/>.
  # <http://www.itk.org/Bug/view.php?id=13292>
  # <http://www.sunlightd.com/Archive/Windows/FAQ.aspx>
  # <http://msdn.microsoft.com/en-us/library/9yb4317s(v=vs.80).aspx>
  # <http://www.windows-api.com/microsoft/VC-MFC/32308312/preprocessor-definitions-for-x64-projects.aspx>
  add_definitions (-D_CONSOLE -DNOGDI)
endif (WIN32)

# On many platforms (e.g., Linux), code that links to SystemC also needs to
# link to ${CMAKE_THREAD_LIBS_INIT}.
#
# TODO: Check why CMake doesn't add automatically ${CMAKE_THREAD_LIBS_INIT},
# as already target systemc depends on it.
set (SystemC_LIBRARIES systemc ${CMAKE_THREAD_LIBS_INIT})


# configure_and_add_test (NAME [INPUT] [GOLDEN])
macro (configure_and_add_test TEST_PROG)
  if (${ARGC} GREATER 3)
    message (FATAL_ERROR "Wrong number of arguments for configure_and_add_test (NAME [INPUT] [GOLDEN]).")
  endif (${ARGC} GREATER 3)
  string (REPLACE "${CMAKE_SOURCE_DIR}/" "" TEST_NAME
                  "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_PROG}")
  if (NOT SHELL)
    set (SHELL /bin/sh)
  endif (NOT SHELL)
  set (TEST ${CMAKE_CURRENT_BINARY_DIR}/${TEST_PROG})
  if (${ARGC} LESS 2)
    unset (TESTINPUT)
  else (${ARGC} LESS 2)
    set (TESTINPUT ${CMAKE_CURRENT_SOURCE_DIR}/${ARGV1})
    if (NOT EXISTS ${TESTINPUT})
      message (SEND_ERROR "The specified input file ${TESTINPUT} for test ${TEST_NAME} does not exist.")
      unset (TESTINPUT)
    endif (NOT EXISTS ${TESTINPUT})
  endif (${ARGC} LESS 2)
  if (${ARGC} LESS 3)
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/golden.log)
      set (TESTGOLDEN ${CMAKE_CURRENT_SOURCE_DIR}/golden.log)
    elseif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/results/expected.log)
      set (TESTGOLDEN ${CMAKE_CURRENT_SOURCE_DIR}/results/expected.log)
    else (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/golden.log)
      unset (TESTGOLDEN)
    endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/golden.log)
  else (${ARGC} LESS 3)
    set (TESTGOLDEN ${CMAKE_CURRENT_SOURCE_DIR}/${ARGV2})
    if (NOT EXISTS ${TESTGOLDEN})
      message (SEND_ERROR "The specified golden reference file ${TESTGOLDEN} for test ${TEST_NAME} does not exist.")
      unset (TESTGOLDEN)
    endif (NOT EXISTS ${TESTGOLDEN})
  endif (${ARGC} LESS 3)
  configure_file (${CMAKE_SOURCE_DIR}/config/test.sh.in
                  ${CMAKE_CURRENT_BINARY_DIR}/test.sh @ONLY)
  add_test (${TEST_NAME} ${SHELL} ${CMAKE_CURRENT_BINARY_DIR}/test.sh)
  set_tests_properties (${TEST_NAME}
                        PROPERTIES FAIL_REGULAR_EXPRESSION "^[*][*][*]ERROR")
endmacro (configure_and_add_test)


########################################################################
# Add the SystemC and TLM examples.
########################################################################

add_subdirectory (sysc)
add_subdirectory (tlm)
