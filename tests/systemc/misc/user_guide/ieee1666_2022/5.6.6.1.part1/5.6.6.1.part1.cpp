#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>


struct M1 : sc_core::sc_module {
    M1(sc_core::sc_module_name name)
    {
        SC_THREAD(ticker);
	SC_THREAD(calling);
	SC_THREAD(target);
	t = sc_core::sc_get_current_process_handle();
    }

    sc_core::sc_process_handle t;
    sc_core::sc_event ev;

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

	t.suspend();
	wait(10.0, SC_NS);

	t.resume();

	wait(10.0, SC_NS);

	t.disable();
	wait(10.0, SC_NS);

	t.enable();
	wait(10.0, SC_NS);

	sc_stop();
    }

    void target() {
        for(;;) {
	    wait(ev);
	    std::cout << "Target awoke at " << sc_core::sc_time_stamp() << std::endl;
	}
    }
};



int sc_main( int argc, char* argv[] ) { 

    M1 m1("m1");
    sc_start();
    std::cout << "program completed" << std::endl;
}
