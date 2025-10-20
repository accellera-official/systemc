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

if (NOT ENABLE_EXAMPLES)
  return()
endif()

set(TEST_CATEGORY examples)
include(SystemCTesting)

###############################################################################

# If possible, find a more powerful platform-specific diff tool than "cmake -E compare_files"
find_program (DIFF_COMMAND
              NAMES diff diff.exe fc fc.exe
              PATHS ENV PATH)
if (DIFF_COMMAND MATCHES diff)
  set (DIFF_OPTIONS -u CACHE STRING "Diff command options.")
elseif (DIFF_COMMAND MATCHES fc)
  set (DIFF_OPTIONS /N CACHE STRING "Diff command options.")
else (DIFF_COMMAND MATCHES diff)
  set (DIFF_COMMAND ${CMAKE_COMMAND} CACHE FILEPATH "Path to diff command.")
  set (DIFF_OPTIONS -E compare_files CACHE FILEPATH "Diff command options.")
endif (DIFF_COMMAND MATCHES diff)
mark_as_advanced (DIFF_COMMAND DIFF_OPTIONS)

# configure_and_add_test(<NAME> [INPUT <input-file>] [GOLDEN <golden-log>] [FILTER <filter>])
function (configure_and_add_test TEST_PROG)
  set (oneValueArgs INPUT GOLDEN FILTER FOLDER)
  cmake_parse_arguments(TEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

  if (${TEST_UNPARSED_ARGS})
    message (FATAL_ERROR "Invalid options passed to configure_and_add_test(<NAME> [INPUT <input-file>] [GOLDEN <golden-log>] [FILTER <filter>]): ${TEST_UNPARSED_ARGS}")
  endif (${TEST_UNPARSED_ARGS})

  string (REPLACE "${CMAKE_SOURCE_DIR}/" "" TEST_NAME
                  "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_PROG}")

  if (NOT "${TEST_INPUT}" STREQUAL "")
    set (TEST_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_INPUT})
    if (NOT EXISTS ${TEST_INPUT})
      message (SEND_ERROR "The specified input file ${TEST_INPUT} for test ${TEST_NAME} does not exist.")
      unset (TEST_INPUT)
    endif (NOT EXISTS ${TEST_INPUT})
  endif (NOT "${TEST_INPUT}" STREQUAL "")

  if ("${TEST_GOLDEN}" STREQUAL "")
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/golden.log)
      set (TEST_GOLDEN ${CMAKE_CURRENT_SOURCE_DIR}/golden.log)
    elseif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/results/expected.log)
      set (TEST_GOLDEN ${CMAKE_CURRENT_SOURCE_DIR}/results/expected.log)
    else (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/golden.log)
      unset (TEST_GOLDEN)
    endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/golden.log)
  else ("${TEST_GOLDEN}" STREQUAL "")
    set (TEST_GOLDEN ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_GOLDEN})
    if (NOT EXISTS ${TEST_GOLDEN})
      message (SEND_ERROR "The specified golden reference file ${TEST_GOLDEN} for test ${TEST_NAME} does not exist.")
      unset (TEST_GOLDEN)
    endif (NOT EXISTS ${TEST_GOLDEN})
  endif ("${TEST_GOLDEN}" STREQUAL "")

  set(TEST_NAME ${TEST_NAME}${TEST_SUFFIX})
  add_test (NAME ${TEST_NAME}
            COMMAND ${CMAKE_COMMAND} "-DTEST_EXE=$<TARGET_FILE:${TEST_PROG}>"
                                     "-DTEST_DIR=${CMAKE_CURRENT_BINARY_DIR}"
                                     "-DTEST_INPUT=${TEST_INPUT}"
                                     "-DTEST_GOLDEN=${TEST_GOLDEN}"
                                     "-DTEST_FILTER=${TEST_FILTER}"
                                     "-DDIFF_COMMAND=${DIFF_COMMAND}"
                                     "-DDIFF_OPTIONS=${DIFF_OPTIONS}"
                                     -P ${PROJECT_SOURCE_DIR}/cmake/run-example.cmake)
  add_dependencies(all-examples ${TEST_PROG})
  set_tests_properties (${TEST_NAME}
                        PROPERTIES FAIL_REGULAR_EXPRESSION "^[*][*][*]ERROR")
  set_target_properties(${TEST_PROG} PROPERTIES FOLDER "${TEST_FOLDER}")
endfunction (configure_and_add_test)


########################################################################
# Add the SystemC and TLM examples.
########################################################################

add_subdirectory (sysc)
add_subdirectory (tlm)
