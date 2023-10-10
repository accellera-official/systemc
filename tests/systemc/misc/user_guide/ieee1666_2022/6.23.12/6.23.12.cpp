// COMPLETE TEST
//
// If the execution log matches the golden log this example compiled and
// executed correctly.

#include <systemc>

SC_MODULE(M)
{
    sc_core::sc_fifo<int> fifo;

    SC_CTOR(M) : fifo(4) {
        SC_THREAD(T);
    }

    void T() {
        int d;
	fifo.write(1);
	fifo.print(std::cout);
	fifo.write(2);
	fifo.print(std::cout);
	fifo.write(3);
	fifo.print(std::cout);
	std::cout << fifo.num_available() << std::endl;
	std::cout << fifo.num_free() << std::endl;
	std::cout << std::endl;

	fifo.read(d);
	fifo.print(std::cout);
	std::cout << fifo.num_available() << std::endl;
	std::cout << fifo.num_free() << std::endl;
	std::cout << std::endl;

	fifo.read(d);
	fifo.print(std::cout);
	fifo.read(d);
	std::cout << fifo.num_available() << std::endl;
	std::cout << fifo.num_free() << std::endl;
	std::cout << std::endl;

	wait(sc_core::SC_ZERO_TIME);
	std::cout << fifo.num_free() << std::endl;
    }
};

int sc_main( int argc, char* argv[] ) { 

    M m("m");

    sc_core::sc_start();
    std::cout << "program completed" << std::endl;
    return 0;
}
