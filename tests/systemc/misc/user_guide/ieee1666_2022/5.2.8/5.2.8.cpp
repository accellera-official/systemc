// COMPLETE TEST
//
// If the execution log matches the golden log this example is verified.
#include <systemc>

SC_MODULE(M) {
    sc_core::sc_in<bool> clk;
    SC_CTOR(M) : clk("clk") {
        SC_METHOD(a_method);
        SC_THREAD(a_thread);
        SC_CTHREAD(a_cthread, clk.pos() );
    }
    void a_method() { std::cout << "a_method at " << sc_core::sc_time_stamp() << std::endl; }
    void a_thread() { std::cout << "a_thread at " << sc_core::sc_time_stamp() << std::endl; }
    void a_cthread() { std::cout << "a_cthread at " << sc_core::sc_time_stamp() << std::endl; }
};

int sc_main( int argc, char* argv[] ) { 

    sc_core::sc_clock clock;
    M m("m");
    m.clk(clock);

    sc_core::sc_start(1, sc_core::SC_NS);

    std::cout << "program completed" << std::endl;
    return 0;
}
