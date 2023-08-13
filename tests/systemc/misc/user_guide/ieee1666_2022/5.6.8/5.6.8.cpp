#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>

struct wait_on_exit {
    ~wait_on_exit() {
        if ( !sc_core::sc_is_unwinding() ) {
	    std::cout << "not unwinding" << std::endl;
	    wait(10.0, sc_core::SC_NS);
	}
	else {
	    std::cout << "unwinding" << std::endl;
	}
    }
};

SC_MODULE(M) 
{
    SC_CTOR(M) {
	SC_THREAD(some_process);
    }
    void some_process()
    {
	while(true) {
	    try {
		wait_on_exit w;
	    }
	    catch( const sc_core::sc_unwind_exception& ) {
		throw;
	    }

	}
    }
};

int sc_main( int argc, char* argv[] ) { 

    M m("m");

    sc_start(11, sc_core::SC_NS);

    std::cout << "program completed" << std::endl;
}
