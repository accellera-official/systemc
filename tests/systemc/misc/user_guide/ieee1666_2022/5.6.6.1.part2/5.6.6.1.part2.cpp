#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>


struct M2 : sc_core::sc_module {
    M2(sc_core::sc_module_name name) {
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

        t.sync_reset_on();
        wait(10.0, SC_NS);
        wait(10.0, SC_NS);

        t.sync_reset_off();

        wait(10.0, SC_NS);

        sc_stop();
    }

    void target() {
	std::cout << "Target called reset at " << sc_core::sc_time_stamp() << std::endl;
        for(;;) {
            wait(ev);
            std::cout << "Target awoke at " << sc_core::sc_time_stamp() << std::endl;
        }
    }
};

        
int sc_main( int argc, char* argv[] ) { 

    M2 m2("m2");
    sc_start();
    std::cout << "program completed" << std::endl;
}
