// COMPLETE TEST
//
// If the log file matches the golden log the test was successful.

#include <systemc>


SC_MODULE(Mod) {
    sc_core::sc_port<sc_core::sc_signal_in_if<int> > p;

    SC_CTOR(Mod) : p("p") {
        std::cout << "p.name() = " << p.name() << std::endl;
        std::cout << "p.basename() = " << p.basename() << std::endl;
        std::cout << "p.kind() = " << p.kind() << std::endl;
    }
};

SC_MODULE(Top) {
    Mod* mod;
    sc_core::sc_signal<int> sig;
    SC_CTOR(Top) : sig("sig") {
        mod = new Mod("mod");
	mod->p(sig);
	std::cout << "mod->name = " << mod->name() << std::endl;
	std::cout << "sig.name = " << sig.name() << std::endl;
    }
};

int sc_main( int argc, char* argv[] ) { 

    Top top("top");
    std::cout << "top.name() = " << top.name() << std::endl;

    sc_core::sc_start(sc_core::SC_ZERO_TIME);

    std::cout << "program completed" << std::endl;
    return 0;
}
