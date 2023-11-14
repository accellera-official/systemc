// COMPLETE TEST
//
// If the clock period is printed to the log file the before_end_of_elaboration() callback
// was executed successfully.

#include <systemc>

SC_MODULE(Top) {
    sc_core::sc_in<bool> clock;
    SC_CTOR(Top) {}
    void before_end_of_elaboration() {
        using namespace sc_core;
	sc_interface* i_f = clock.get_interface();
	sc_clock* clk = dynamic_cast<sc_clock*>(i_f);
	std::cout << "clock period is " << clk->period() << std::endl;
    }
};

int sc_main( int argc, char* argv[] ) { 

    sc_core::sc_clock clock;
    Top top("top");
    top.clock(clock);

    sc_core::sc_start(sc_core::SC_ZERO_TIME);

    std::cout << "program completed" << std::endl;

    return 0;
}
