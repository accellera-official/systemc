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
# examples/tlm/CMakeLists.txt --
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


########################################################################
# Common definitions for the TLM examples.
########################################################################

add_definitions (-DSC_INCLUDE_DYNAMIC_PROCESSES)
include_directories (BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/common/include)
set (COMMON_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common/src)

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
  endif (${ARGC} LESS 2)
  if (${ARGC} LESS 3)
    set (TESTGOLDEN ${CMAKE_CURRENT_SOURCE_DIR}/results/expected.log)
  else (${ARGC} LESS 3)
    set (TESTGOLDEN ${CMAKE_CURRENT_SOURCE_DIR}/${ARGV2})
  endif (${ARGC} LESS 3)
  configure_file (${CMAKE_SOURCE_DIR}/config/test.sh.in
                  ${CMAKE_CURRENT_BINARY_DIR}/test.sh @ONLY)
  add_test (${TEST_NAME} ${SHELL} ${CMAKE_CURRENT_BINARY_DIR}/test.sh)
  set_tests_properties (${TEST_NAME}
                        PROPERTIES FAIL_REGULAR_EXPRESSION "^[*][*][*]ERROR")
endmacro (configure_and_add_test)


########################################################################
# Add the TLM examples.
########################################################################

add_subdirectory (at_1_phase)
add_subdirectory (at_2_phase)
add_subdirectory (at_4_phase)
add_subdirectory (at_extension_optional)
add_subdirectory (at_mixed_targets)
add_subdirectory (at_ooo)
add_subdirectory (lt)
add_subdirectory (lt_dmi)
add_subdirectory (lt_extension_mandatory)
add_subdirectory (lt_mixed_endian)
add_subdirectory (lt_temporal_decouple)
