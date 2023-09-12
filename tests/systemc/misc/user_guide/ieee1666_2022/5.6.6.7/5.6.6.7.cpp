// NEEDS EXECUTION CODE
//
// At present this test will validate the compilation syntax, but needs an execution 
// component.
//
// @@@@ ISSUES @@@@

#include <systemc>

SC_MODULE(M) {
    SC_CTOR(M) {
        SC_THREAD(run);
    }

    void run()
    {
        for (;; ) {
	    try {
		sc_core::wait(1, sc_core::SC_NS);
	    } catch( const sc_core::sc_unwind_exception& ex ) {
		// perform clean up
		if ( ex.is_reset() ) {
		    std::cout << "Performing unwind reset" << std::endl;
		}
		else {
		    std::cout << "Not performing unwind reset" << std::endl;
		}
		throw ex;
	    }
	}
    }
};

int sc_main( int argc, char* argv[] ) { 

    M m("m");
    sc_core::sc_start(10, sc_core::SC_NS);

    std::cout << "program completed" << std::endl;
    return 0;
}
