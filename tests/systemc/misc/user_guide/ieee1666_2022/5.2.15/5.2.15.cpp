// COMPLETE TEST
//
// If the execution log mathes the golden log this test compiled and
// executed correctly.

#include <systemc>

#define CLEAR(X) { std::cout  << "clearing " << #X << std::endl; mod.X = false; }
#define ENABLE(X) { std::cout  << "setting " << #X << std::endl; mod.X = true; }
#define STATUS \
    std::cout << sc_core::sc_time_stamp() << "| " << mod.A << " " << mod.B << " " << mod.C \
              << " " << mod.D << " " << mod.E << " ";


SC_MODULE(Mod) {
    sc_core::sc_signal<bool> SC_NAMED(A), SC_NAMED(B), SC_NAMED(C),
                             SC_NAMED(D), SC_NAMED(E);
    
    SC_CTOR(Mod) {
        sensitive << A;

	SC_THREAD(T);
	sensitive << B << C;

	SC_METHOD(M);
	f();
	sensitive << E;
    }

    void f() { sensitive << D; }
    void T() { 
        for (;;) {
	    wait();
	    std::cout << sc_core::sc_time_stamp() << "| " << A << " " << B << " " << C << " " 
	              << D << " " << E << " invoking thread T " << std::endl; 
	}
    }
    void M() { 
	std::cout << sc_core::sc_time_stamp() << "| " << A << " " << B << " " << C << " " 
	          << D << " " << E << " invoking method M " << std::endl; 
    }
};

int sc_main( int argc, char* argv[] ) { 

    Mod mod("mod");

    std::cout << "Time  A B C D E" << std::endl;
    std::cout << "      - - - - -" << std::endl;

    sc_core::sc_start(sc_core::SC_ZERO_TIME);

    STATUS ENABLE(A)
    sc_start(2, sc_core::SC_NS);
    STATUS CLEAR(A)

    STATUS ENABLE(B)
    sc_start(2, sc_core::SC_NS);
    STATUS CLEAR(B)

    STATUS ENABLE(C)
    sc_start(2, sc_core::SC_NS);
    STATUS CLEAR(C)

    STATUS ENABLE(D)
    sc_start(2, sc_core::SC_NS);
    STATUS CLEAR(D)

    STATUS ENABLE(E)
    sc_start(2, sc_core::SC_NS);
    STATUS CLEAR(E)

    std::cout << "program completed" << std::endl;
    return 0;
}
