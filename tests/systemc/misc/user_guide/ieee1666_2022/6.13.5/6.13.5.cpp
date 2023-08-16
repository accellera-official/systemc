// COMPLETE TEST
//
// If the execution file matches the golden file this example compiled and
// executed correctly.

#include <systemc.h>

SC_MODULE(M) 
{
    sc_core::sc_signal_resolved sig;

    SC_CTOR(M) {
        SC_METHOD(snooper);
	sensitive << sig;
        SC_THREAD(T1);
	SC_THREAD(T2);
	SC_THREAD(T3);
    }

    void T1() {
        wait(10, sc_core::SC_NS);
	sig = sc_dt::SC_LOGIC_0;
        wait(20, sc_core::SC_NS);
	sig = sc_dt::SC_LOGIC_Z;
    }

    void T2() {
        wait(20, sc_core::SC_NS);
	sig = sc_dt::SC_LOGIC_Z;
        wait(30, sc_core::SC_NS);
	sig = sc_dt::SC_LOGIC_0;
    }

    void T3() {
        wait(40, sc_core::SC_NS);
	sig = sc_dt::SC_LOGIC_1;
    }

    void snooper() {
        std::cout << sc_core::sc_time_stamp() << " " << sig << std::endl;
    }
};

int sc_main( int argc, char* argv[] ) { 

    M m("m");

    sc_start();
    std::cout << "program completed" << std::endl;
    return 0;
}
