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
#
# SystemCTesting.cmake -- Some ctest settings and helpers
#
# Original Author: Philipp A. Hartmann, Apple Inc.
#
###############################################################################

# Only built under {examples,tests} / check-{examples,tests} target
#  - see https://cmake.org/Wiki/CMakeEmulateMakeCheck
add_custom_target(check-${TEST_CATEGORY}
  COMMAND ${CMAKE_CTEST_COMMAND} -C "$<CONFIG>"
  DEPENDS all-${TEST_CATEGORY}
)
add_custom_target(all-${TEST_CATEGORY})
enable_testing()

# combined target to build/run all enabled categories (examples, regression tests)
if(NOT TARGET check)
  add_custom_target(check)
endif()
add_dependencies(check check-${TEST_CATEGORY})

set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)
set_directory_properties(PROPERTIES EXCLUDE_FROM_ALL TRUE)
set_target_properties(check-${TEST_CATEGORY} PROPERTIES FOLDER "${TEST_CATEGORY}")
set_target_properties(all-${TEST_CATEGORY}   PROPERTIES FOLDER "${TEST_CATEGORY}")

if(NOT DEFINED TEST_SUFFIX)
  set(TEST_SUFFIX)
  if(DEFINED CMAKE_CXX_STANDARD AND CMAKE_CXX_STANDARD)
    set(TEST_SUFFIX "${TEST_SUFFIX}/cxx${CMAKE_CXX_STANDARD}")
  endif()
  if (NOT GENERATOR_IS_MULTI_CONFIG)
    set(TEST_SUFFIX "${TEST_SUFFIX}/${CMAKE_BUILD_TYPE}")
  endif()
endif(NOT DEFINED TEST_SUFFIX)

########################################################################
# Common definitions for all tests.
########################################################################

if (DEBUG_SYSTEMC)
  add_definitions(-DDEBUG_SYSTEMC)
endif (DEBUG_SYSTEMC)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wno-unused -Wno-unused-parameter -Wno-unused-variable -Wno-sign-compare)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  add_compile_options(/W3 /wd4244 /wd4267 /wd4996)
endif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

# Always enable assertions for the regression tests
add_definitions(-DSC_ENABLE_ASSERTIONS)
