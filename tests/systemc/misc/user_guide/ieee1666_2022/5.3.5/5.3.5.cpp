#include <systemc.h>


struct A : sc_core::sc_module {
    A(sc_core::sc_module_name) {}
};

struct B : sc_core::sc_module {
    B( sc_core::sc_module_name n ) : sc_core::sc_module(n) {}
};

struct C : B {
    C( sc_core::sc_module_name n ) : B(n) {}
};

struct Top : sc_core::sc_module {
    A a;
    C c;

    Top(sc_core::sc_module_name n) : sc_core::sc_module(n) ,
                                  a("a"),
				  c("c")
    {}
};

int sc_main( int argc, char* argv[] ) { 

    Top top("top");

    std::cout << "program completed" << std::endl;
}
