// COMPLETE TEST
// 
// If the execution log matches the golden log this test verifies compilation and
// execution.

#include <systemc.h>

using namespace sc_core;

SC_MODULE(M)
{
    sc_event event1, event2;
    SC_CTOR(M)
    {
	SC_THREAD( firer );
        SC_THREAD( thread );
    }
    void firer()
    {
        wait(10, SC_NS);
	event1.notify();
        wait(10, SC_NS);
	event2.notify();
        wait(10, SC_NS);
	event1.notify();
	event2.notify();
        wait(10, SC_NS);
    }

    void thread()
    {
	for (;;) {
	    wait(event1 | event2);
	    if ( event1.triggered()) std::cout << sc_time_stamp() << " event1" << std::endl;
	    if ( event2.triggered()) std::cout << sc_time_stamp() << " event2" << std::endl;
        }
    }
};


int sc_main( int argc, char* argv[] ) { 

    M m("m");

    sc_start(50, SC_NS);

    std::cout << "program completed" << std::endl;
    return 0;
}
