#include <systemc.h>

SC_MODULE(M) {
    sc_core::sc_in<bool> SC_NAMED(clock);
    sc_core::sc_in<bool> SC_NAMED(reset);
    sc_core::sc_in<bool> SC_NAMED(async_reset);

    SC_CTOR(M) {
        SC_CTHREAD(CT1, clock.pos());
	reset_signal_is(reset, true);
	SC_CTHREAD(CT2, clock.pos());
	async_reset_signal_is(async_reset, true);}
    void CT1() {
        if (reset.read()) {
	    // ... Reset actions}
	    while(true) {
	        wait(true); // Wait for 1 clock cycle
		// . . .
	    }
	}
    }

    void CT2() {
	// ... Reset actions
	while(true) {
	        try {
		while (true) {wait(); // Wait for 1 clock cycle
		    // ... // Regular behavior}
		}
	    } catch (...) { 
		// Some exception has been thrown
	    }
	    // ... handle exception and go back to wait
	}
    }
};

int sc_main( int argc, char* argv[] ) { 

    M m("m");
    std::cout << "program completed" << std::endl;
}
