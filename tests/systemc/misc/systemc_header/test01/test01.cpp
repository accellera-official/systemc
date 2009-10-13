#include "systemc"

SC_MODULE(DUT)
{
	SC_CTOR(DUT)
	{
		SC_METHOD(x);
		sensitive << m_clk.pos();
		SC_CTHREAD(z, m_clk.pos());
		SC_THREAD(y);
		sensitive << m_clk.pos();
	}
	void x()
	{
		std::cout << "x: Hello World" << std::endl;
	}
	void y()
	{
		for (;;)
		{
			wait();
		    std::cout << "y: Hello World" << std::endl;
		}
	}
	void z()
	{
		for (;;)
		{
			wait();
		    std::cout << "z: Hello World" << std::endl;
		}
	}
	::sc_core::sc_in<bool> m_clk;
};

int sc_main(int argc, char* argv[])
{
	sc_core::sc_clock clock;
	DUT               dut("dut");

    dut.m_clk(clock);
	sc_core::sc_start(10);
	return 0;
}
