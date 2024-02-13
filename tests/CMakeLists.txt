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
# tests/CMakeLists.txt -- discover and build SystemC regression tests
#
# Original Author: Philipp A. Hartmann, Apple Inc.
#
###############################################################################

if (NOT ENABLE_REGRESSION)
  return()
endif()

set(TEST_CATEGORY tests)
include(SystemCTesting)

###############################################################################

function(configure_and_add_regression_test TEST_PATH)
  string(REGEX REPLACE "^([^/]+)/.*" "\\1" TEST_GROUP "${TEST_PATH}")
  get_filename_component(TEST_FOLDER "${TEST_PATH}" DIRECTORY)
  string(REPLACE "/" "-" TEST_NAME "${TEST_PATH}")

  # message(DEBUG "found ${TEST_GROUP} test: ${TEST_NAME} (${TEST_PATH})")
  add_executable(${TEST_NAME})
  set_target_properties(${TEST_NAME} PROPERTIES FOLDER "tests/${TEST_FOLDER}")
  add_dependencies(all-tests ${TEST_NAME})

  file(GLOB sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_PATH}/*.f")
  if(sources) # found a file list
    get_filename_component(TEST_LEAFNAME ${sources} NAME_WLE)
    file(STRINGS ${sources} sources_list)
    set(sources)
    foreach(src ${sources_list})
      string(REGEX REPLACE "^[^/]+/(.*)" "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_PATH}/\\1" src "${src}")
      string(STRIP "${src}" src)
      list(APPEND sources ${src})
    endforeach()
  else(sources)
    file(GLOB sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_PATH}/*.cpp")
    get_filename_component(TEST_LEAFNAME ${sources} NAME_WLE)
  endif(sources)
  # message(TRACE "found ${TEST_NAME} sources: ${sources}")
  target_sources(${TEST_NAME} PRIVATE ${sources})

  if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/${TEST_GROUP}")
    target_include_directories(${TEST_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/${TEST_GROUP}")
  endif()
  target_link_libraries(${TEST_NAME} PRIVATE SystemC::systemc)


  set(testname ${TEST_PATH}${TEST_SUFFIX})
  add_test(NAME ${testname}
           COMMAND ${PROJECT_SOURCE_DIR}/cmake/run-test.py  $<TARGET_FILE:${TEST_NAME}>)

  set(workdir ${CMAKE_CURRENT_BINARY_DIR}/Testing/${TEST_PATH})
  get_filename_component(leafdir ${TEST_PATH} NAME)
  file(MAKE_DIRECTORY ${workdir})
  execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
                  ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_PATH}
                  ${workdir}/${leafdir})

  set(env
    SYSTEMC_REGRESSION=1
    SYSTEMC_ARCH=${SystemC_TARGET_ARCH}
    TEST_FULLNAME=${TEST_PATH}
    TEST_LEAFNAME=${TEST_LEAFNAME}
    TEST_BUILD_TYPE=$<CONFIG>
  )

  set_tests_properties(${testname} PROPERTIES
      LABELS "${TEST_GROUP}"
      ENVIRONMENT "${env}"
      SKIP_RETURN_CODE 111
      WORKING_DIRECTORY ${workdir}
  )
endfunction(configure_and_add_regression_test)

function(skip_test TEST_PATH)
  set(testname ${TEST_PATH}${TEST_SUFFIX})
  get_test_property(${testname} ENVIRONMENT env)
  list(APPEND env TEST_SKIPPED=yes)
  set_tests_properties(${testname} PROPERTIES ENVIRONMENT "${env}")
endfunction(skip_test)

function(discover_regression_tests)
  # find all golden log directories
  file(GLOB_RECURSE tests
       LIST_DIRECTORIES TRUE
       RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
       CONFIGURE_DEPENDS "golden"
  )
  foreach(test ${tests})
    if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${test}/golden")
      configure_and_add_regression_test(${test})
    endif()
  endforeach()
endfunction()

discover_regression_tests()

###############################################################################
# Additional compile/link options for specific tests

target_link_libraries(systemc-kernel-sc_suspend PRIVATE Threads::Threads)
