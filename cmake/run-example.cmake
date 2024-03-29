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
# cmake/run-example.cmake --
# CMake script to run a compiled example and to optionally compare its output
# with a golden reference file.
#
# Original Author: Torsten Maehne, Berner Fachhochschule (BFH), 2016-09-12
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


cmake_minimum_required(VERSION 3.5)

if(NOT TEST_EXE)
  message(FATAL_ERROR "  Usage: cmake -DTEST_EXE=<executable> [-DTEST_INPUT=<input-file>] \\\n"
                      "               [-DTEST_GOLDEN=<reference-output-file>] \\\n"
                      "               [-DTEST_FILTER=<pattern-to-filter-from-log>] \\\n"
                      "               [-DDIFF_COMMAND=<diff-command>] \\\n"
                      "               [-DDIFF_OPTIONS=<diff-options>] \\\n"
                      "               -P ${CMAKE_CURRENT_LIST_FILE}")
endif(NOT TEST_EXE)

if(NOT EXISTS ${TEST_EXE})
  message(FATAL_ERROR "Can't find the executable: ${TEST_EXE}")
endif(NOT EXISTS ${TEST_EXE})

if (NOT TEST_DIR)
  get_filename_component(TEST_DIR ${TEST_EXE} DIRECTORY)
endif (NOT TEST_DIR)

if(NOT EXISTS ${TEST_DIR})
  message(FATAL_ERROR "Can't find the test execution directory: ${TEST_DIR}")
endif(NOT EXISTS ${TEST_DIR})

if(TEST_INPUT AND NOT EXISTS ${TEST_INPUT})
  message(FATAL_ERROR "Can't find the test input file: ${TEST_INPUT}")
endif(TEST_INPUT AND NOT EXISTS ${TEST_INPUT})

if(TEST_GOLDEN AND NOT EXISTS ${TEST_GOLDEN})
  message(FATAL_ERROR "Can't find the test's golden reference output file: ${TEST_GOLDEN}")
endif(TEST_GOLDEN AND NOT EXISTS ${TEST_GOLDEN})

if(TEST_INPUT)
  execute_process(COMMAND ${TEST_EXE}
                  WORKING_DIRECTORY ${TEST_DIR}
                  RESULT_VARIABLE TEST_EXIT_CODE
                  INPUT_FILE ${TEST_INPUT}
                  OUTPUT_FILE run.log
                  ERROR_VARIABLE TEST_ERROR)
else(TEST_INPUT)
  execute_process(COMMAND ${TEST_EXE}
                  WORKING_DIRECTORY ${TEST_DIR}
                  RESULT_VARIABLE TEST_EXIT_CODE
                  OUTPUT_FILE run.log
                  ERROR_VARIABLE TEST_ERROR)
endif(TEST_INPUT)

if(NOT TEST_GOLDEN)
  if(TEST_EXIT_CODE EQUAL 0)
    message ("OK")
    return()
  else(TEST_EXIT_CODE EQUAL 0)
    message (FATAL_ERROR "***ERROR:\n${TEST_ERROR}")
  endif(TEST_EXIT_CODE EQUAL 0)
endif(NOT TEST_GOLDEN)

# TODO: trim empty lines from run and expected log output

if ("${TEST_FILTER}" STREQUAL "")
  file (READ ${TEST_DIR}/run.log RUN_LOG)
else ("${TEST_FILTER}" STREQUAL "")
  file (STRINGS ${TEST_DIR}/run.log RUN_FILTERED_LOG REGEX "^.*${TEST_FILTER}.*$" )
  string(REPLACE ";" "\n"  RUN_LOG "${RUN_FILTERED_LOG}\n")
endif ("${TEST_FILTER}" STREQUAL "")
string(REGEX REPLACE "^.*stopped by user.*$" "" RUN_TRIMMED_LOG "${RUN_LOG}")
file(WRITE ${TEST_DIR}/run_trimmed.log "${RUN_TRIMMED_LOG}")

file(READ ${TEST_GOLDEN} EXPECTED_LOG)
string(REGEX REPLACE "^.*stopped by user.*$" "" EXPECTED_TRIMMED_LOG "${EXPECTED_LOG}")
file(WRITE ${TEST_DIR}/expected_trimmed.log "${EXPECTED_TRIMMED_LOG}")

if(NOT DIFF_COMMAND)
  set(DIFF_COMMAND ${CMAKE_COMMAND})
  set(DIFF_OPTIONS  -E compare_files)
endif(NOT DIFF_COMMAND)
execute_process(COMMAND ${DIFF_COMMAND} ${DIFF_OPTIONS}
                        run_trimmed.log expected_trimmed.log
                WORKING_DIRECTORY ${TEST_DIR}
                RESULT_VARIABLE DIFF_EXIT_CODE
                OUTPUT_FILE diff.log
                ERROR_FILE diff.log)

message("DIFF_EXIT_CODE = ${DIFF_EXIT_CODE}")

if(DIFF_EXIT_CODE EQUAL 0)
  message ("OK")
else(DIFF_EXIT_CODE EQUAL 0)
  file(READ ${TEST_DIR}/diff.log DIFF_LOG)
  message(FATAL_ERROR "***ERROR:\n${DIFF_LOG}")
endif(DIFF_EXIT_CODE EQUAL 0)

file(REMOVE ${TEST_DIR}/run.log
            ${TEST_DIR}/run_trimmed.log
            ${TEST_DIR}/expected_trimmed.log
            ${TEST_DIR}/diff.log)
