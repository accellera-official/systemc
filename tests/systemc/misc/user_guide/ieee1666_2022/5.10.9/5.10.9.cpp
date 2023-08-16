// COMPLETE TEST
// 
// If the execution log matches the golden log this test verifies compilation and
// execution.

#include <systemc.h>

#define NOTIFY1(ARG) { \
    std::cout << "FIRE: event.notify1(" << #ARG << ")" << std::endl; \
    event1.notify(ARG); \
}

#define NOTIFY2(ARG1,ARG2) { \
    std::cout << "FIRE: event.notify1(" << #ARG1 << "," << #ARG2 << ")" << std::endl; \
    event1.notify(ARG1,ARG2); \
}

using namespace sc_core;

SC_MODULE(M)
{
    sc_event event1;
    SC_CTOR(M)
    {
	SC_THREAD( firer );
        SC_THREAD( thread );
    }
    void firer()
    {
#if 0
	event1.notify(SC_ZERO_TIME);
	event1.notify(1.0,SC_NS);
	event1.notify();
	event1.notify(2.0,SC_NS);
	event1.notify(1.0,SC_NS);
	event1.notify(SC_ZERO_TIME);
#endif
	NOTIFY1(SC_ZERO_TIME);
	NOTIFY2(1.0,SC_NS);
	NOTIFY1();
	NOTIFY2(2.0,SC_NS);
	NOTIFY2(1.0,SC_NS);
	NOTIFY1(SC_ZERO_TIME);
    }

    void thread()
    {
	for (;;) {
	    wait(event1);
	    if ( event1.triggered()) std::cout << sc_time_stamp() << " event1" << std::endl;
        }
    }
};


int sc_main( int argc, char* argv[] ) { 

    M m("m");

    sc_start(20, SC_NS);

    std::cout << "program completed" << std::endl;
    return 0;
}
