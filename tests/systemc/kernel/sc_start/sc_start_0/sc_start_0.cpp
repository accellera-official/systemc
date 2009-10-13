#include "systemc.h"

SC_MODULE(X)
{
	SC_CTOR(X)
	{
		SC_METHOD(comb);
		sensitive << tweak;
		SC_CTHREAD(sync, clk.pos());
	}
	void comb()
	{
		cout << "  X::comb() - " << 
			sc_time_stamp() << "  " <<
			tweak.read() << endl;
	}
	void sync()
	{
		for (;;)
		{
			wait();
			cout << "  X::sync() - " << 
				sc_time_stamp() << "  " <<
				tweak.read() << endl;
		}
	}
	sc_in_clk   clk;
	sc_in<bool> tweak;
};

#define ACTION(action,descr) \
{ \
	cout << descr << " - " << \
		sc_time_stamp() << "   " << \
		tweak.read() << endl; \
	action ; \
	cout << "                                        " << \
                sc_time_stamp() << "   " << \
		tweak.read() << endl; \
}


int sc_main(int argc, char* argv[])
{
	sc_clock clock;
	sc_signal<bool> tweak;
	X        x("x");
	x.clk(clock);
	x.tweak(tweak);

	cout << "Event         Start Parameters         End Parameters" << endl;
	cout << "------------- ----------------         --------------\n" << endl;

	ACTION(sc_start(1, SC_NS),"sc_start(1, SC_NS)")
	ACTION(tweak = !tweak,"~tweak     ")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")
	ACTION(sc_start(1, SC_NS),"sc_start(1, SC_NS)")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")

	ACTION(tweak = !tweak,"~tweak     ")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")

	ACTION(tweak = !tweak,"~tweak     ")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")

	ACTION(sc_start(1, SC_NS),"sc_start(1, SC_NS)")
	ACTION(sc_start(1, SC_NS),"sc_start(1, SC_NS)")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")
	ACTION(tweak = !tweak,"~tweak     ")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")
	ACTION(sc_start(0, SC_NS),"sc_start(0, SC_NS)")

    cerr << "Program completed" << endl;
    return 0;
}

