// COMPLETE TEST
//
// This example illustrates SC_CTOR signatures and if it compiles it is complete.

#include <systemc.h>

SC_MODULE(M1)
{
    SC_CTOR(M1,int a, int b) {}
};

SC_MODULE(M2)
{
    M2(sc_core::sc_module_name nm, int a, int b ) : sc_core::sc_module(nm) {}
};

int sc_main( int argc, char* argv[] )
{
    int a = 0;
    int b = 1;

    M1 m1("m1", a, b);

    M2 m2("m2", 5, 10);

    std::cout << "program completed" << std::endl;
    return 0;
}
