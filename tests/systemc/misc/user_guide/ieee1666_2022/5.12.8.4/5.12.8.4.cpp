// COMPLETE TEST
//
// If the binding was done correctly there should be no error messages. Note that the
// LRM contains no definition for bus_interface, and a null body was added to properly
// compile.

#include <systemc>


class bus_interface {};

class peripheral_interface : virtual public sc_core::sc_interface {
  public:
    virtual void peripheral_write( int addr, int data ) =  0;
    virtual void peripheral_read( int addr, int& data ) =  0;
};

class bus_channel : public bus_interface, public sc_core::sc_module {
  public:
    sc_core::sc_port<peripheral_interface,0> peripheral_port;

    SC_CTOR(bus_channel) {
        SC_THREAD(action);
    }

  private:
    void action() {
        for ( size_t i = 0; i < peripheral_port.size(); ++i ) {
	    peripheral_port[i]->peripheral_write(0,i);
	    int j;
	    peripheral_port[i]->peripheral_read(0,j);
	    if ( i != j ) {
	        std::cout << "memory read/write mismatch " << j << " != " << i << std::endl;
	    }
	}
    }
};

class memory : public peripheral_interface, public sc_core::sc_module {
  public:
    SC_CTOR(memory) {}

    virtual void peripheral_write( int addr, int data ) { storage[addr] = data; }
    virtual void peripheral_read( int addr, int& data ) { data = storage[addr]; }
    int storage[20];
};

SC_MODULE(top_level) {
    bus_channel bus;
    memory ram0, ram1, ram2, ram3;

    SC_CTOR(top_level) : bus("bus"), ram0("ram0"), ram1("ram1"),  ram2("ram2"), ram3("ram3")
    {
        bus.peripheral_port(ram0);
        bus.peripheral_port(ram1);
        bus.peripheral_port(ram2);
        bus.peripheral_port(ram3);
    }
};
   
int sc_main( int argc, char* argv[] ) { 

    top_level tlm("tlm");
    sc_core::sc_start(sc_core::SC_ZERO_TIME);

    std::cout << "program completed" << std::endl;
    return 0;
}
