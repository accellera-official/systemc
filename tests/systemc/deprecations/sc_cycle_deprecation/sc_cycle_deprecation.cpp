#include "systemc.h"
// #include "sysc/utils/sc_utils_ids.h"

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
	sc_event event;
	X        x("x");
	x.clk(clock);

	// FORCE WARNING MESSAGE:

	sc_initialize();
	cout << "After initialize call" << endl;


	// REQUEST WARNING MESSAGE SUPRESSION AND SEE THAT IT IS SUPPRESSED:

	sc_report_handler::set_actions( SC_ID_IEEE_1666_DEPRECATION_, 
	    SC_DO_NOTHING);
	sc_cycle(SC_ZERO_TIME);
	cout << "After sc_cycle call" << endl;


	// REQUEST WARNING MESSAGE AGAIN:

	sc_report_handler::set_actions( SC_ID_IEEE_1666_DEPRECATION_, 
		SC_DEFAULT_WARNING_ACTIONS);
	notify(0, SC_NS, event);

	cout << "After notify() call" << endl;


    cerr << "Program completed" << endl;
    return 0;
}

