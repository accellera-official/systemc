
set(SOURCE_FILES
        in_class_initialization.cpp
        adder_int_5_pimpl.h
        adder_int_5_pimpl.cpp
        adder.h
        )

add_executable (in_class_initialization ${SOURCE_FILES})
target_link_libraries (in_class_initialization SystemC::systemc)
configure_and_add_test (in_class_initialization)

add_executable (in_class_initialization_pimpl ${SOURCE_FILES})
target_compile_definitions(in_class_initialization_pimpl PRIVATE USE_PIMPL_ADDER)
target_link_libraries (in_class_initialization_pimpl SystemC::systemc)
configure_and_add_test (in_class_initialization_pimpl)
