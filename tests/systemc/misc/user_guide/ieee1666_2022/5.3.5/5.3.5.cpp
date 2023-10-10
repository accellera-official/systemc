// COMPLETE TEST
//
// If the execution log matches the golden log, the compilation and execution validate the
// example.

#include <systemc>


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

void scan_hierarchy( sc_core::sc_object* obj ) {
    std::vector<sc_core::sc_object*> children = obj->get_child_objects();
    std::cout << "object " << obj->name() << std::endl;
    for ( size_t i = 0; i < children.size(); ++i ) {
        if ( children[i] ) {
            scan_hierarchy(children[i]);
        }
    }
}

int sc_main( int argc, char* argv[] ) { 

    Top top("top");

    sc_core::sc_start(sc_core::SC_ZERO_TIME);

    std::vector<sc_core::sc_object*> tops = sc_core::sc_get_top_level_objects();
    for ( size_t i = 0; i < tops.size(); ++i ) {
        if ( tops[i] ) {
            scan_hierarchy(tops[i]);
        }
    }

    std::cout << "program completed" << std::endl;
    return 0;
}
