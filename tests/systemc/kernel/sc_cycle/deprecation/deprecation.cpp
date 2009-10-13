// This program tests sc_cycle message generation and suppression:

#include "systemc.h"

int sc_main(int argc, char* argv[] )
{
	sc_clock clock;

    sc_start(1);
	sc_cycle(0);
    sc_report_handler::set_actions( SC_ID_SC_CYCLE_DEPRECATED_,
                                    SC_DO_NOTHING );
	sc_cycle(0);

	cerr << "Program completed" << endl;
	return 0;
}
