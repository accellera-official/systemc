#include <systemc.h>

SC_MODULE(M1) 
{
    SC_CTOR(M1) : i(0) {}

    int i;
    // ....
};

SC_MODULE(M2) 
{
    SC_CTOR(M2);

    int i;
    // ....
};

M2::M2(sc_core::sc_module_name) : i(0) {}


int  sc_main(int argc, char* argv[] ) {
    M1 m1("m1");
    M2 m2("m2");

    std::cout << "program complete" << std::endl;
}
