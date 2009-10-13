#include "systemc.h"

SC_MODULE(X)
{
	SC_CTOR(X)
	{
		cout << "Creating able..." << endl;
		SC_THREAD(able);
		sensitive << clk.pos();
		cout << "Creating baker..." << endl;
		SC_THREAD(baker);
		sensitive << clk.pos();
		cout << "Creating charlie..." << endl;
		SC_THREAD(charlie);
		sensitive << clk.pos();
	}
		
	void able()
	{
		for (;;)
		{
			wait();
			cout << "able: " << sc_simulation_time() << endl;
			sc_stop();
		}
	}
	void baker()
	{
		for (;;)
		{
			wait();
			cout << "baker: " << sc_simulation_time() << endl;
			sc_stop();
		}
	}
	void charlie()
	{
		for (;;)
		{
			wait();
			cout << "charlie: " << sc_simulation_time() << endl;
			sc_stop();
		}
	}
	sc_in_clk clk;
};

int sc_main(int argc, char* argv[] )
{
	sc_clock clock;
	X        x("x");
	x.clk(clock);

	sc_set_stop_mode(SC_STOP_IMMEDIATE);
	sc_start(100);
	return 0;
}
