// COMPLETE TEST
//
// If the execution log matches the golden log this test compiled and
// executes successfully.

#include <systemc>

class bus_interface : virtual public sc_core::sc_interface {
  public:
    virtual void write( int addr, int data ) = 0;
    virtual void read( int addr, int& data ) = 0;
};

class bus_adapter : public bus_interface, public sc_core::sc_channel {
  public:
    virtual void write(int addr, int data) {}
    virtual void read(int addr, int& data) {}

    sc_core::sc_in<bool>  SC_NAMED(clock);
    sc_core::sc_out<bool> SC_NAMED(wr), SC_NAMED(rd);
    sc_core::sc_out<int>  SC_NAMED(addr_bus);
    sc_core::sc_out<int>  SC_NAMED(data_out);
    sc_core::sc_in<int>   SC_NAMED(data_in);

    SC_CTOR(bus_adapter) { }
};

int sc_main( int argc, char* argv[] ) { 

    bus_adapter ba("ba");
    sc_core::sc_clock clock;
    sc_core::sc_signal<bool> wr, rd;
    sc_core::sc_signal<int> addr_bus;
    sc_core::sc_signal<int> data_out;
    sc_core::sc_signal<int> data_in;

    ba.clock(clock);
    ba.wr(wr);
    ba.rd(rd);
    ba.addr_bus(addr_bus);
    ba.data_out(data_out);
    ba.data_in(data_in);

    sc_core::sc_start(sc_core::SC_ZERO_TIME);

    std::cout << "program completed" << std::endl;
    return 0;
}
