// COMPLETE TEST
//
// If the execution log matches the execution log this test compiled and
// executed correctly.

#include <systemc>
using namespace sc_core;

SC_MODULE(TB)
{
    SC_CTOR(TB)
    {
        SC_THREAD(tb_thread);
    }
    void tb_thread()
    {
        for (size_t i = 0; i < 10; ++i )
        {
	    std::cout << " tb   " << i << "   " << sc_time_stamp() << std::endl;
            wait(1.0, SC_NS);
        }
    }
};

int sc_main( int argc, char* argv[] ) {

    sc_signal<bool>    reset;
    TB                 tb("tb");

    sc_start( SC_ZERO_TIME ); // Run the initialization phase to create pending activity

    while ( sc_pending_activity() ) {
        sc_start( sc_time_to_pending_activity() ); // Run up to the next activity
    }
    return 0;
}

