// COMPLETE TEST
//
// If this test compiles it verifies the SC_NAMED macro example.

#include <systemc>

SC_MODULE(M)
{
    sc_core::sc_in<bool>     port_i;
    sc_core::sc_out<bool>    SC_NAMED(port_o);
    sc_core::sc_signal<bool> sig;

    SC_CTOR(M) 
        : SC_NAMED(port_i),
	  SC_NAMED(sig,true)
    {
    }
    // . . .
};

int sc_main( int argc, char* argv[] ) { 

    M m("m");

    std::cout << "program completed" << std::endl;
    return 0;
}
