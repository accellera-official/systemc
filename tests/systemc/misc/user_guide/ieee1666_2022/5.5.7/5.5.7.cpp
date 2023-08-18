// COMPLETE TEST
//
// If the execution log matches the golden log the example compiles and executes
// correclty.

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc>

int f1() { std::cout << "f1()" << std::endl; return 0; }
int f2() { std::cout << "f2()" << std::endl; return 0; }
int f3() { std::cout << "f3()" << std::endl; return 0; }

SC_MODULE(M)
{
    SC_CTOR(M)
    {
        SC_THREAD(spawner);
    }
    void spawner()
    {
	using namespace sc_core;
	sc_process_handle h1, h2, h3;

	std::cout << "spawner() entry" << std::endl;
	SC_FORK
	    h1 = sc_spawn(&f1),
	    h2 = sc_spawn(&f2),
	    h3 = sc_spawn(&f3)
	SC_JOIN
	std::cout << "spawner() exit" << std::endl;
    }
};

int sc_main( int argc, char* argv[] ) { 

    M m("m");

    sc_start(20, sc_core::SC_NS);
    std::cout << "program completed" << std::endl;
    return 0;
}
