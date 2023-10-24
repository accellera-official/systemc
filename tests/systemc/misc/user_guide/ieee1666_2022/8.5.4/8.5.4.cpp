// COMPLETE TEST
//
// If the execution log matches the golden log this example compiles and
// executes correctly.
//
// Note this test differs from the example in the LRM, but still tests the same
// statements, the difference being that the signal vector has been moved to
// sc_main() from my_module in order to check the operation of sc_vector.

#include <systemc.h>

typedef sc_core::sc_signal_out_if<int> i_f;

SC_MODULE(my_module) {
    sc_core::sc_vector<sc_core::sc_port<i_f> >    ports;

    SC_CTOR(my_module) 
    : ports("ports",4) {
        SC_METHOD(display);
    }

    void display() {
	for ( size_t i = 0; i < 4; ++i ) {
	    std::cout << i << " " << ports[i]->read() << endl;
	}
	std::cout << std::endl;
    }
};


int sc_main( int argc, char* argv[] ) { 

    sc_core::sc_vector<sc_core::sc_signal<int> > signals;
    signals.init(4);

    my_module mm("mm");
    for ( size_t i = 0; i < 4; ++i ) {
	mm.ports[i](signals[i]);
	signals[i] = i;
    }
    sc_core::sc_start(sc_core::SC_ZERO_TIME);

    std::cout << std::endl << "program completed" << std::endl;
    return 0;
}
