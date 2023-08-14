// COMPLETE TEST
//
// If the log file matches the golden log the object scanner operated correctly.
#include <systemc.h>

void scan_hierarchy( sc_core::sc_object* obj ) {
    std::vector<sc_core::sc_object*> children = obj->get_child_objects();
    std::cout << "object " << obj->name() << std::endl;
    for ( size_t i = 0; i < children.size(); ++i ) {
        if ( children[i] ) {
	    scan_hierarchy(children[i]);
	}
    }
}

void snoop()
{
    std::vector<sc_core::sc_object*> tops = sc_core::sc_get_top_level_objects();
    for ( size_t i = 0; i < tops.size(); ++i ) {
        if ( tops[i] ) {
	    scan_hierarchy(tops[i]);
	}
    }

    sc_core::sc_object* obj = sc_core::sc_find_object("foo.foobar");
    if ( obj ) {
        std::cout << obj->name() << std::endl;
    }

    sc_core::sc_module* m;
    if ( (m = dynamic_cast<sc_core::sc_module*>(obj)) ) {
        std::cout << obj->name() << " is an sc_module" << std::endl;
    }

    sc_core::sc_object* parent = obj->get_parent_object();
    if ( parent ) {
        std::cout << "parent of " << obj->name() << " is " << parent->name() 
	          << " " << parent->kind() << std::endl;
    }
}


SC_MODULE(Bottom) {
    sc_signal<int> SC_NAMED(sig1), SC_NAMED(sig2);

    SC_CTOR(Bottom) {}
};

SC_MODULE(Top) {
    Bottom fb;
    SC_CTOR(Top) : fb("foobar") {}
};

int sc_main( int argc, char* argv[] ) { 

    Top top("foo");

    snoop();

    std::cout << "program completed" << std::endl;
    return 0;
}
