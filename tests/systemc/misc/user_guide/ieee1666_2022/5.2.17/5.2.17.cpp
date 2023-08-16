// NEEDS EXECUTION WORK
//
// This tests compilation, but needs execution work.
//
// @@@@ ISSUES @@@@

#include <systemc.h>

SC_MODULE(M) {
    SC_CTOR(M) {
        SC_METHOD(entry);
	sensitive << sig;
    }

    void entry() {
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
    std::cout << "program completed" << std::endl;
    return 0;
}
