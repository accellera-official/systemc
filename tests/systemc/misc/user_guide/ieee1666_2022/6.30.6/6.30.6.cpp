// COMPLETE TEST
//
// If the execution log matches the golden log this test compiles and executes 
// correctly.

#include <systemc>


SC_MODULE(Mod) {
    sc_core::sc_event_queue EQ;

    SC_CTOR(Mod) {
        SC_THREAD(T);
	SC_METHOD(M);
	sensitive << EQ;
	dont_initialize();
    }

    void T() {
        EQ.notify(2.0, sc_core::SC_NS);
        EQ.notify(1.0, sc_core::SC_NS);
	EQ.notify(sc_core::SC_ZERO_TIME);
        EQ.notify(1.0, sc_core::SC_NS);
    }

    void M() {
        std::cout << sc_core::sc_time_stamp() << std::endl;
    }
};

int sc_main( int argc, char* argv[] ) { 

    Mod m("m");

    sc_core::sc_start();
    std::cout << "program completed" << std::endl;
    return 0;
}
