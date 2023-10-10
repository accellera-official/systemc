// COMPLETE TEST
//
// If the execution log matches the golden log this example compiles and
// executes correctly.

#include <systemc.h>

int sc_main( int argc, char* argv[] ) { 

    sc_core::sc_report_handler::stop_after(sc_core::SC_WARNING,1);
    sc_core::sc_report_handler::stop_after("/Acme_IP", 2);
    sc_core::sc_report_handler::stop_after("/Acme_IP", sc_core::SC_WARNING, 3);

    SC_REPORT_WARNING("/Acme_IP", "Overflow");
    SC_REPORT_WARNING("/Acme_IP", "Conflict");
    SC_REPORT_WARNING("/Acme_IP", "Misuse");

    std::cout << std::endl << "program completed" << std::endl;
    return 0;
}
