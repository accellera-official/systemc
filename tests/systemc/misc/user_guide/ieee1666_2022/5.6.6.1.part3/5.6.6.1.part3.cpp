// COMPLETE TEST
//
// If Sexecution log matches the golden log this example compiled and executed
// correctly.

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>


struct M3 : sc_core::sc_module {
    M3(sc_core::sc_module_name name) {
        SC_THREAD(ticker);
        SC_THREAD(calling);
        SC_THREAD(target);
        t = sc_core::sc_get_current_process_handle();
    }

    sc_core::sc_process_handle t;
    sc_core::sc_event ev;
    int               count;

    void ticker()
    {
        for (;;) {
            wait( 10.0, sc_core::SC_NS );
            ev.notify();
        }
    }

    void calling()
    {
        using namespace sc_core;
        wait(15.0, SC_NS);
	sc_assert(count == 1);

        wait(10.0, SC_NS);
	sc_assert(count == 2);

        t.reset();
	sc_assert(count == 0);

        wait(10.0, SC_NS);
	sc_assert(count == 1);

	t.kill();
	sc_assert(t.terminated());

        sc_stop();
    }

    void target() {
	std::cout << "Target called reset at " << sc_core::sc_time_stamp() << std::endl;
	count = 0;
        for(;;) {
            wait(ev);
            std::cout << "Target awoke at " << sc_core::sc_time_stamp() << std::endl;
	    ++count;
        }
    }
};

        
int sc_main( int argc, char* argv[] ) { 

    M3 m3("m3");
    sc_start();
    std::cout << "program completed" << std::endl;
    return 0;
}
