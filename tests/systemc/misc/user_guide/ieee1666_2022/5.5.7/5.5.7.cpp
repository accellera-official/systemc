#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>

int f1() { return 0; }
int f2() { return 0; }
int f3() { return 0; }

void spawner()
{
    using namespace sc_core;
    sc_process_handle h1, h2, h3;

    SC_FORK
        h1 = sc_spawn(&f1),
        h2 = sc_spawn(&f2),
        h3 = sc_spawn(&f3)
    SC_JOIN
}

int sc_main( int argc, char* argv[] ) { 

    std::cout << "program completed" << std::endl;
}
