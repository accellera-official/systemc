// NEEDS EXECUTION WORK
//
// This test currently verifies compilation, but not execution. The issue with
// execution is missing port binds, e.g., top2.E

#include <systemc.h>

SC_MODULE(M1) {
    sc_core::sc_inout<int> SC_NAMED(P), SC_NAMED(Q), SC_NAMED(R);

    SC_CTOR(M1) { }
};

SC_MODULE(Top1)
{
    sc_core::sc_inout<int>  SC_NAMED(A), SC_NAMED(B);
    sc_core::sc_signal<int> SC_NAMED(C);

    M1 m1;

    SC_CTOR(Top1) : m1("m1") {
        m1(A,B,C);
    }
};

SC_MODULE(M2) {
    sc_core::sc_inout<int> SC_NAMED(S);
    sc_core::sc_inout<int>* T;
    sc_core::sc_inout<int> SC_NAMED(U);

    SC_CTOR(M2) { T = new sc_inout<int>("T"); }
};

SC_MODULE(Top2) {
    sc_core::sc_inout<int> SC_NAMED(D), SC_NAMED(E);
    sc_core::sc_signal<int> SC_NAMED(F);
    M2 m2;

    SC_CTOR(Top2) : m2("m2") {
        m2(D,E,F);
    }
};
        
int sc_main( int argc, char* argv[] ) { 

    Top1 top1("top1");
    Top2 top2("top2");

    // sc_start(SC_ZERO_TIME);
    std::cout << "program completed" << std::endl;
    return 0;
}
