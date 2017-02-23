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


if (NOT BUILD_EXAMPLES)
  set_directory_properties (PROPERTIES EXCLUDE_FROM_ALL 1)
endif (NOT BUILD_EXAMPLES)


########################################################################
# Common definitions for all examples.
########################################################################

if (DEBUG_SYSTEMC)
  add_definitions (-DDEBUG_SYSTEMC)
endif (DEBUG_SYSTEMC)
if (ENABLE_ASSERTIONS)
  add_definitions (-DSC_ENABLE_ASSERTIONS)
endif (ENABLE_ASSERTIONS)

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

# TODO: Set MSVC warning level to 3 and 4 for the individual examples
# and disable selected warnings (e.g., 4996). See the individual
# *.vcxproj files for details.
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wno-unused-parameter -Wno-unused-variable)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  add_compile_options(/W3 /wd4244 /wd4267 /wd4996)
endif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

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

# configure_and_add_test (NAME [INPUT] [GOLDEN])
macro (configure_and_add_test TEST_PROG)
  if (${ARGC} GREATER 3)
    message (FATAL_ERROR "Wrong number of arguments for configure_and_add_test (NAME [INPUT] [GOLDEN]).")
  endif (${ARGC} GREATER 3)
  string (REPLACE "${CMAKE_SOURCE_DIR}/" "" TEST_NAME
                  "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_PROG}")
  #string(REGEX REPLACE "[^A-Za-z0-9_]" "_" TEST_NAME ${TEST_NAME})
  if (${ARGC} LESS 2)
    unset (TEST_INPUT)
  else (${ARGC} LESS 2)
    set (TEST_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/${ARGV1})
    if (NOT EXISTS ${TEST_INPUT})
      message (SEND_ERROR "The specified input file ${TEST_INPUT} for test ${TEST_NAME} does not exist.")
      unset (TEST_INPUT)
    endif (NOT EXISTS ${TEST_INPUT})
  endif (${ARGC} LESS 2)
  if (${ARGC} LESS 3)
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/golden.log)
      set (TEST_GOLDEN ${CMAKE_CURRENT_SOURCE_DIR}/golden.log)
    elseif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/results/expected.log)
      set (TEST_GOLDEN ${CMAKE_CURRENT_SOURCE_DIR}/results/expected.log)
    else (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/golden.log)
      unset (TEST_GOLDEN)
    endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/golden.log)
  else (${ARGC} LESS 3)
    set (TEST_GOLDEN ${CMAKE_CURRENT_SOURCE_DIR}/${ARGV2})
    if (NOT EXISTS ${TEST_GOLDEN})
      message (SEND_ERROR "The specified golden reference file ${TEST_GOLDEN} for test ${TEST_NAME} does not exist.")
      unset (TEST_GOLDEN)
    endif (NOT EXISTS ${TEST_GOLDEN})
  endif (${ARGC} LESS 3)
  add_test (NAME ${TEST_NAME}
            COMMAND ${CMAKE_COMMAND} "-DTEST_EXE=$<TARGET_FILE:${TEST_PROG}>"
                                     "-DTEST_DIR=${CMAKE_CURRENT_BINARY_DIR}"
                                     "-DTEST_INPUT=${TEST_INPUT}"
                                     "-DTEST_GOLDEN=${TEST_GOLDEN}"
                                     "-DDIFF_COMMAND=${DIFF_COMMAND}"
                                     "-DDIFF_OPTIONS=${DIFF_OPTIONS}"
                                     -P ${PROJECT_SOURCE_DIR}/cmake/run_test.cmake)
  set_tests_properties (${TEST_NAME}
                        PROPERTIES FAIL_REGULAR_EXPRESSION "^[*][*][*]ERROR")
endmacro (configure_and_add_test)


########################################################################
# Add the SystemC and TLM examples.
########################################################################

add_subdirectory (sysc)
add_subdirectory (tlm)
