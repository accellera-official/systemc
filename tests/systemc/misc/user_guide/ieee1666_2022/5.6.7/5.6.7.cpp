// COMPLETE TEST
//
// If Sexecution log matches the golden log this example compiled and executed
// correctly.

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc>



SC_MODULE(Mod) {
    sc_core::sc_in<bool> in;
    SC_CTOR(Mod) {
        SC_METHOD(run);
	sensitive << in;
	sc_core::sc_process_handle h1 = sc_core::sc_get_current_process_handle();
    }

    void run()
    {
        using namespace sc_core;

	sc_process_handle h2 = sc_core::sc_get_current_process_handle();

	if ( h2.proc_kind() == SC_METHOD_PROC_ ) {
	    std::cout << "running method process" << std::endl;
	}
	sc_object* parent = h2.get_parent_object();

	if ( parent ) {
	    sc_process_handle handle = sc_process_handle(parent);
	    if ( handle.valid() ) {
	        std::cout << "valid handle" << std::endl;
	    }
	    else {
	        std::cout << "invalid handle" << std::endl;
	    }
	}
    }
};
	        

int sc_main( int argc, char* argv[] ) { 

    sc_core::sc_clock clock;
    Mod               mod("mod");
    mod.in(clock);

    sc_core::sc_start(1, sc_core::SC_NS );

    std::cout << "program completed" << std::endl;
    return 0;
}
