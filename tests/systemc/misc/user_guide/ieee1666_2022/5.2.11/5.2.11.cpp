// THIS TEST NEEDS WORK
//
// It tests compilation but not execution.

#include <systemc.h>


SC_MODULE(synchronous_module)
{
    sc_in<bool> SC_NAMED(clock);

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
