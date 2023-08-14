// NEEDS EXECUTION WORK
#include <systemc.h>


struct M : sc_core::sc_module {
    sc_core::sc_port<sc_core::sc_signal_in_if<int>,0> p;

    M(sc_core::sc_module_name name) {
        SC_THREAD(T);
    }

    sc_core::sc_event_or_list all_events() const {
        sc_core::sc_event_or_list or_list;
	for ( size_t i = 0; i < p.size(); ++i ) {
	    or_list |= p[i]->default_event();
	}
	return or_list;
    }

    sc_core::sc_event event1, event2;

    void T() {
        using namespace sc_core;
	for (;;) {
	    wait( all_events() );
	    sc_event_and_list list;
	    sc_assert(list.size() == 0);
	    list = list & event1;
	    list &= event2;
	    wait(list);
	    sc_assert(list.size() == 2);
	}
    }
};

int sc_main( int argc, char* argv[] ) { 

    sc_signal<int> s;

    M m("m");
    m.p(s);


    sc_start(SC_ZERO_TIME);
    std::cout << "program completed" << std::endl;
    return 0;
}
