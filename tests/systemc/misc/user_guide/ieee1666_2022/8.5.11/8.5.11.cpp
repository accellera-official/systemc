// COMPLETE TEST
//
// If the execution log matches its golden log the test compiles.

#include <systemc.h>

struct i_f : virtual sc_core::sc_interface { };

struct Init : sc_core::sc_module {
    sc_core::sc_port<i_f> port;

    Init(sc_core::sc_module_name name) : port("port") { }
};

struct Targ: public sc_core::sc_module, private i_f {
    sc_export<i_f> xp;

    Targ(sc_core::sc_module_name name) : xp("xp") {}
};

struct Top : public sc_core::sc_module {
    sc_core::sc_vector<Init> init_vec;
    sc_core::sc_vector<Targ> targ_vec;

    Top(sc_core::sc_module_name name, int N) 
    : init_vec("init_vec",N), targ_vec("targ_vec", N) {
        // Vector-to_vector bind from vector-of-ports to vector-of-exports
	sc_core::sc_assemble_vector(init_vec, &Init::port).bind(
	    sc_core::sc_assemble_vector(targ_vec, &Targ::xp));
    }
};

int sc_main( int argc, char* argv[] ) { 

    Top top("top",4);

    std::cout << std::endl << "program completed" << std::endl;
    return 0;
}
