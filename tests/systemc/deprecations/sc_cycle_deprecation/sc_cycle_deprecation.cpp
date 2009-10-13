#include "systemc.h"

SC_MODULE(X)
{
	SC_CTOR(X)
	{
		SC_CTHREAD(y, clk.pos());
	}
	void y()
	{
		wait();
		wait();
		wait();
		sc_stop();
	}
	sc_in_clk clk;
};

int sc_main(int argc, char* argv[])
{
	sc_clock clock;
	X        x("x");
	x.clk(clock);

	sc_start(1);


	// FORCE WARNING MESSAGE:

	sc_cycle(SC_ZERO_TIME);
	cout << "After first sc_cycle call" << endl;


	// REQUEST WARNING MESSAGE SUPRESSION AND SEE THAT IT IS SUPPRESSED:

	sc_report_handler::set_actions( SC_ID_SC_CYCLE_DEPRECATED_, SC_DO_NOTHING);
	sc_cycle(SC_ZERO_TIME);
	cout << "After second sc_cycle call" << endl;


	// REQUEST WARNING MESSAGE AGAIN:

	sc_report_handler::set_actions( SC_ID_SC_CYCLE_DEPRECATED_, 
		SC_DEFAULT_WARNING_ACTIONS);
	sc_cycle(SC_ZERO_TIME);

	cout << "After third sc_cycle call" << endl;


    cerr << "Program completed" << endl;
    return 0;
}

