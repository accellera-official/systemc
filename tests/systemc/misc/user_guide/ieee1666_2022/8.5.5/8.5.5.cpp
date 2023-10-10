// COMPLETE TEST
//
// If the execution log matches the golden log this test compiles and
// executes correctly.

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>

struct my_module : public sc_core::sc_module {
    my_module( sc_core::sc_module_name n, std::string extra_arg ) {}
};

struct Top : sc_core::sc_module {
    sc_core::sc_vector<my_module> vector1;
    sc_core::sc_vector<my_module> vector2;

    // Case 1: creator is a function object
    struct my_module_creator {
        my_module_creator( std::string arg ) : extra_arg(arg) {}

	my_module* operator()(const char* name, size_t) {
	    return new my_module(name, extra_arg);
	}
	std::string extra_arg;
    };

    // Case 2: creator is a member function
    my_module* my_module_creator_func( const char* name, size_t ) {
        return new my_module(name, "value_of_extra_arg" );
    }

    Top(sc_core::sc_module_name name, int N) {
        // Initialize vector passing through constructor arguments to my_module
	// Case 1: constructor and pass in function object
	vector1.init(N, my_module_creator("value of extra arg"));

	// Case 2: pass in a member function using Boost bind
	vector2.init(N,
	    sc_core::sc_bind(&Top::my_module_creator_func, this, sc_unnamed::_1, sc_unnamed::_2));
    }
};


int sc_main( int argc, char* argv[] ) { 

    Top top("top", 3);
    sc_core::sc_start(sc_core::SC_ZERO_TIME);

    std::cout << std::endl << "program completed" << std::endl;
    return 0;
}
