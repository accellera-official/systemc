// COMPLETE TEST
//
// This test just makes sure all the ports are bound. It does not check what is bound to what.
#include <systemc.h>

SC_MODULE(M) {
    sc_core::sc_inout<int> SC_NAMED(P), SC_NAMED(Q), SC_NAMED(R), SC_NAMED(S);
    sc_core::sc_inout<int>* T;

    SC_CTOR(M) {
        T = new sc_core::sc_inout<int>("T");
    }
};

SC_MODULE(Top) {
    sc_core::sc_inout<int> SC_NAMED(A), SC_NAMED(B);
    sc_core::sc_signal<int> SC_NAMED(C), SC_NAMED(D), SC_NAMED(E);

    M m;

    SC_CTOR(Top) :  m("m") {
        m.P(A);
	m.Q.bind(B);
	m.R(C);
	m.S.bind(D);
	m.T->bind(E);
    }
};


int sc_main( int argc, char* argv[] ) { 
    sc_core::sc_signal<int> sigA, sigB;

    Top t("top");
    t.A(sigA);
    t.B(sigB);

    sc_start(0, sc_core::SC_NS);

    std::cout << "program completed" << std::endl;
}
