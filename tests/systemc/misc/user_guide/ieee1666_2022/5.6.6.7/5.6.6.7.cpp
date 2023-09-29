// COMPLETE TEST
//
// If the execution log matches the golden log this test compiled
// and executed correctly.

#include <systemc>

SC_MODULE(M) {
    SC_CTOR(M) {
        SC_THREAD(run);
        SC_THREAD(controller);
    }

    void run()
    {
        run_process = sc_core::sc_get_current_process_handle();
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
    void controller()
    {
        sc_core::wait(10, sc_core::SC_NS);
	run_process.reset();
        sc_core::wait(10, sc_core::SC_NS);
	run_process.kill();
    }
    sc_core::sc_process_handle run_process;
};

int sc_main( int argc, char* argv[] ) { 

    M m("m");
    sc_core::sc_start();

    std::cout << "program completed" << std::endl;
    return 0;
}
