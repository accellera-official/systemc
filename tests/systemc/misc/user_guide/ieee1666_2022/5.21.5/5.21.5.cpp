//  COMPLETE TEST
//
// If the execution log matches the golden log, the example s valid.

#include <systemc.h>

SC_MODULE(TopModule) { SC_CTOR(TopModule) {} };

SC_MODULE(SomeModule)
{
    sc_in<bool> SC_NAMED(in);
    SC_CTOR(SomeModule) 
    {
        {
	    sc_core::sc_hierarchy_scope scope( sc_core::sc_hierarchy_scope::get_root() );
	    TopModule* mod = new TopModule("TopModule");
	}
    }
};

int sc_main( int argc, char* argv[] ) { 

    SomeModule sm("sm");

    std::vector<sc_object*> tops = sc_get_top_level_objects();

    for ( size_t i = 0; i < tops.size(); ++i ) {
        std::cout << tops[i]->name() << std::endl;
    }

    std::cout << "program completed" << std::endl;
    return 0;
}
