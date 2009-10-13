// test case

#include "systemc.h"

SC_MODULE(X)
{
	SC_CTOR(X)
	{
		cout << "Creating able..." << endl;
		SC_THREAD(able);
		sensitive << immediate_event;
		cout << "Creating baker..." << endl;
		SC_THREAD(baker);
	}

	void able()
	{
		wait();
		cout << "able: " << sc_simulation_time() << endl;
	}
	void baker()
	{
		cout << "baker: " << sc_simulation_time()
			 << ": issuing sc_stop()" <<  endl;
		sc_stop();
		immediate_event.notify();
	}
	sc_event immediate_event;
};

int sc_main(int argc, char* argv[] )
{
	X        x("x");

	//sc_set_stop_mode(SC_STOP_IMMEDIATE);
	sc_start(100);
	return 0;
}


