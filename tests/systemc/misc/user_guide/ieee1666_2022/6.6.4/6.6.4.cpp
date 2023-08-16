// COMPLETE TEST
//
// If the execution log matches the golden log this example compiles and executes
// correctly.

#include <systemc.h>


SC_MODULE(M) 
{
    sc_core::sc_buffer<int> buf;

    SC_CTOR(M) {
        SC_THREAD(writer);
	SC_METHOD(reader);
	sensitive << buf;
    }

    void writer() {
        buf.write(1);
	wait( sc_core::SC_ZERO_TIME);
	buf.write(1);
    }

    void reader() {
        std::cout << sc_core::sc_time_stamp() << " " << buf << std::endl;
    }
}; 

int sc_main( int argc, char* argv[] ) { 

    M m("m");

    sc_start();

    std::cout << "program completed" << std::endl;
    return 0;
}
