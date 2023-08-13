#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>

int fa() { return 0; }
int fb() { return 0; }

void test_handles()
{
    using namespace sc_core;
    sc_process_handle a, b;

    sc_assert(!a.valid() && !b.valid() );
    sc_assert( a != b );
    sc_assert( !(a < b) && !(b < a) );

    a = sc_spawn(&fa);
    b = sc_spawn(&fb);

    sc_process_handle c = b;

    sc_assert(c == b);
    sc_assert(!(b < c) && !(c < b));

    wait( a.terminated_event() & b.terminated_event() );

    sc_assert( (a < b) || (b < a) );

    if ( b.valid() ) 
        sc_assert( b == c );
    else
        sc_assert( b != c );
	
    sc_assert( b.valid() == c.valid() );

    sc_assert( !(b < c) && !(c < b) );

    sc_assert( c.terminated() );
}

void test_swaps()
{
    using namespace sc_core;
    sc_process_handle a,b = sc_get_current_process_handle();
    sc_assert( b.valid() );

    a.swap(b);
    sc_assert(a == sc_get_current_process_handle() );
    sc_assert(!b.valid());
}

int sc_main( int argc, char* argv[] ) { 

    std::cout << "program completed" << std::endl;
}
