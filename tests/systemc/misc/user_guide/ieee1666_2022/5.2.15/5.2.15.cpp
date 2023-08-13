#include <systemc.h>

SC_MODULE(Mod) {
    sc_core::sc_signal<bool> SC_NAMED(A), SC_NAMED(B), SC_NAMED(C),
                             SC_NAMED(D), SC_NAMED(E);
    
    SC_CTOR(Mod) {
        sensitive << A;

	SC_THREAD(T);
	sensitive << B << C;

	SC_METHOD(M);
	f();
	sensitive<< E;
    }

    void f() { sensitive << D; }
    void T() { }
    void M() { }
    // .  .  .
};

int sc_main( int argc, char* argv[] ) { 

    Mod mod("mod");
    std::cout << "program completed" << std::endl;
}
