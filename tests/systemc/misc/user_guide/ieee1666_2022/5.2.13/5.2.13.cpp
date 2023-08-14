// COMPLETE TEST
//
// If the execution log matches the golden log the example has been tested.

#include <systemc.h>

SC_MODULE(M)
{
    sc_in<bool> SC_NAMED(clock);
    sc_in<bool> SC_NAMED(reset);
    // .  .  .

    SC_CTOR(M) {
        SC_METHOD(rtl_proc);
        sensitive << clock.pos();
        async_reset_signal_is( reset, true );
        // .  .  .
    }
    void rtl_proc() {
        if ( reset.read() ) {
	    std::cout << "asynchronous" << std::endl;
	}
	else {
	    std::cout << "synchronous" << std::endl;
	}
    }
};

int sc_main( int argc, char* argv[] ) { 

    sc_clock        clock;
    sc_signal<bool> reset;
    M               m("m");

    m.clock(clock);
    m.reset(reset);

    reset = true;
    sc_start(1, SC_NS);
    reset = false;
    sc_start(1, SC_NS);

    std::cout << "program completed" << std::endl;
    return 0;
}
