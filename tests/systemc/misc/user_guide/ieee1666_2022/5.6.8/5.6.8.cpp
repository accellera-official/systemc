// COMPLETE EXAMPLE
//
// If the execution log matches the golden log this example compiles and
// exectues correctly.

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc>

struct wait_on_exit {
    ~wait_on_exit() noexcept(false) {
        if ( !sc_core::sc_is_unwinding() ) {
	    std::cout << sc_core::sc_time_stamp() << " waiting in ~wait_on_exit()" << std::endl;
	    wait(10.0, sc_core::SC_NS);
	}
	else {
	    std::cout << sc_core::sc_time_stamp() << " unwinding in ~wait_on_exit()" << std::endl;
	}
    }
};

SC_MODULE(M) 
{
    SC_CTOR(M) {
	SC_THREAD(controller);
	SC_THREAD(some_process);
    }

    void controller()
    {
        wait(25.0, sc_core::SC_NS);
	some_process_handle.kill();
    }

    void some_process()
    {
        some_process_handle = sc_core::sc_get_current_process_handle();
	while(true) {
	    try {
		wait_on_exit w;
	    }
	    catch( const sc_core::sc_unwind_exception& ) {
	        std::cout << sc_core::sc_time_stamp() << " caught exception" << std::endl;
		if ( !sc_core::sc_is_unwinding() ) {
		    std::cout << "unwinding" << std::endl;
		}
		throw;
	    }

	}
    }
    sc_core::sc_process_handle some_process_handle;
};

int sc_main( int argc, char* argv[] ) { 

    M m("m");

    sc_core::sc_start();

    std::cout << "program completed" << std::endl;
    return 0;
}
