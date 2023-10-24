// COMPLETE TEST
//
// If the execution log matches the golden log this example compiles and
// executes correctly.

#include <systemc.h>

SC_MODULE(M)
{
    SC_CTOR(M) {
        SC_THREAD(T);
    }
    void T() {
        try {
	    // . . .
	    SC_REPORT_ERROR("msg_type", "msg");
	} catch(sc_core::sc_report c) {
	    std::cout << "Caught " << c.what() << std::endl;
	}
    }
};

int sc_main( int argc, char* argv[] ) { 

    M m("m");

    sc_start();

    std::cout << std::endl << "program completed" << std::endl;
    return 0;
}
