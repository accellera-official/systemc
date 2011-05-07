#include "systemc.h"

SC_MODULE(DUT)
{
    SC_CTOR(DUT)
    {
        SC_THREAD(thread);
    }
    void thread()
    {
        for (;;)
        {
	    signal.write(true);
	    wait(1, SC_NS);
            assert(signal.event()==false);
        }
    }
    sc_signal<bool> signal;
};

int sc_main(int argc, char* argv[])
{
    DUT             dut("dut");

    sc_signal<bool> signal1;
    sc_start(0,SC_NS);
    signal1.write(true);
    sc_start(0,SC_NS);
    assert(signal1.event()==true);
    sc_start(1,SC_NS);
    assert(signal1.event()==false);
    sc_start(5,SC_NS);

    cout << "Program completed" << endl;
    return 0;
}
