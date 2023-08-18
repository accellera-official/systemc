// THIS TEST NEEDS WORK
//
// It tests compilation but not execution.
//
// @@@@ ISSUES @@@@

#include <systemc>


SC_MODULE(synchronous_module)
{
    sc_core::sc_in<bool> SC_NAMED(clock);

    SC_CTOR(synchronous_module) {
        SC_THREAD(thread);
	sensitive << clock.pos();
    }
    void thread() {
        for (;;) {
	    wait();
	    // . . .
	}
    }
    // . . .
};

int sc_main( int argc, char* argv[] ) { 
    synchronous_module sm("sm");

    std::cout << "program completed" << std::endl;
    return 0;
}
