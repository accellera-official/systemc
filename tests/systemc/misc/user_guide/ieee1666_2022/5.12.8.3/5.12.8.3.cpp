#include <systemc.h>

struct iface : virtual sc_core::sc_interface {
    virtual int read() const = 0;
};

struct chan : iface, sc_core::sc_prim_channel {
    virtual int read() const;
};

int chan::read() const { return 42; }

SC_MODULE(modu) {
    sc_core::sc_port<iface> SC_NAMED(P);

    SC_CTOR(modu) {
        SC_THREAD(thread);
    }

    void thread() {
        int i = P->read();
	std::cout << "i = " << i << std::endl;
    }
};

SC_MODULE(top) {
    modu* modu_p;
    chan* chan_p;

    SC_CTOR(top) {
        chan_p = new chan;
	modu_p = new modu("mo");
	modu_p->P(*chan_p);
    }
};

int sc_main( int argc, char* argv[] ) { 

    top top_module("top");

    sc_start(SC_ZERO_TIME);

    std::cout << "program completed" << std::endl;
}
