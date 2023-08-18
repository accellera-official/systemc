// COMPLETE TEST
//
// If the execution matches the golden log this test compile and executes
// correctly

#include <systemc>
	
class U {
  public:
    U(int val=0) {
        ptr = new int;
	*ptr = val;
    }

    int get() const { return *ptr; }
    void set(int i) { *ptr = i; }

    const U& operator = ( const U& arg ) {
        *(this->ptr) = *arg.ptr;
        return *this;
    }

  private:
    int* ptr;
};

std::ostream& operator << ( std::ostream& os, const U& arg ) { return os << arg.get(); }

SC_MODULE(M1) {
    sc_core::sc_fifo_out<U> fifo_out;

    SC_CTOR(M1) {
        SC_THREAD(producer);
    }

    void producer() {
        U u;
	for ( int i = 0; i < 4; ++i ) {
	    u.set(i);
	    bool status;
	    do {
	        wait(1.0, sc_core::SC_NS);
		status = fifo_out.nb_write(u);
	    } while (!status);
	}
    }
};

SC_MODULE(M2) {
    sc_core::sc_fifo_in<U> fifo_in;

    SC_CTOR(M2) {
        SC_THREAD(consumer);
    }

    void consumer() {
        for (;;) {
	    wait(fifo_in.data_written_event());
	    U u;
	    bool status = fifo_in.nb_read(u);
	    std::cout << u << " ";
	}
    } 
};

SC_MODULE(Top) {
    sc_core::sc_fifo<U> fifo;
    M1 m1;
    M2 m2;

    SC_CTOR(Top) : m1("m1"), m2("m2") {
        m1.fifo_out(fifo);
	m2.fifo_in(fifo);
    }
};

int sc_main( int argc, char* argv[] ) { 

    Top top("top");
    sc_core::sc_start();

    std::cout << std::endl << "program completed" << std::endl;
    return 0;
}
