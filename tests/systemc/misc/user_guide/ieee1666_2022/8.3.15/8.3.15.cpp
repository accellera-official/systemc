// COMPLETE TEST
//
// If the execution log matches the golden log this example compiles and
// executes correctly.

#include <systemc.h>

int sc_main( int argc, char* argv[] ) { 

    sc_core::sc_report_handler::set_log_file_name("foo");
    std::cout << "Name is'" << sc_core::sc_report_handler::get_log_file_name() << "'" << std::endl;

    if ( true == sc_core::sc_report_handler::set_log_file_name("bar") ) {
        std::cout << "ERROR: successive calls to set_log_file_name() with non-null strings ";
	std::cout << "should return false" << std::endl;
    }
    std::cout << "Name is'" << sc_core::sc_report_handler::get_log_file_name() << "'" << std::endl;

    if ( true == sc_core::sc_report_handler::set_log_file_name(0) ) {
        std::cout << "calling set_log_file_name() NULL should return false" << std::endl;
    }

    if ( sc_core::sc_report_handler::get_log_file_name() != NULL ) {
        std::cout << "ERROR: get_log_file_name() should have returned NULL" << std::endl;
    }

    std::cout << std::endl << "program completed" << std::endl;
    return 0;
}
