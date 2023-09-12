// OOMPLETE TEST
//
// If the execution log matches the golden log this test compiled
// and executed correctly.

#include <systemc>

SC_MODULE(M) {
    SC_CTOR(M) {
        SC_METHOD(entry);
	sensitive << sig;
    }

    void entry() {
        std::cout << sc_core::sc_time_stamp() << " " << sig << std::endl;
        if ( sig == 0 ) next_trigger(e1 | e2);
        else if ( sig == 1 ) next_trigger(1, sc_core::SC_NS);
        else next_trigger();
    }
    sc_core::sc_signal<int> SC_NAMED(sig);
    sc_core::sc_event       SC_NAMED(e1), SC_NAMED(e2);
    // .  .  .
};

int sc_main( int argc, char* argv[] ) { 

    M m("m");
    m.sig = 1;
    sc_core::sc_start(2, sc_core::SC_NS);
    m.sig = 0;
    sc_core::sc_start(5, sc_core::SC_NS);
    m.e1.notify();
    sc_core::sc_start(5, sc_core::SC_NS);
    m.e2.notify();
    sc_core::sc_start(5, sc_core::SC_NS);
    m.e1.notify();
    m.e2.notify();
    sc_core::sc_start(5, sc_core::SC_NS);

    std::cout << "program completed" << std::endl;
    return 0;
}
