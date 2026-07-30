find_package(GTest)
include(GoogleTest)

enable_testing()

function(add_systemc_test_suite PATH)
  get_filename_component(FILE_NAME ${PATH} NAME_WE)
  set(TEST_NAME "gtest-${FILE_NAME}")

  add_executable(${TEST_NAME} ${PATH})
  target_link_libraries(${TEST_NAME} GTest::gtest GTest::gmock SystemC::systemc)
  add_dependencies(all-tests ${TEST_NAME})

  gtest_discover_tests(${TEST_NAME})
endfunction(add_systemc_test_suite)

add_systemc_test_suite(sc_event_cons.cpp)
add_systemc_test_suite(sc_event_cons_setup.cpp)
