// $Log: deprecation.cpp,v $
// Revision 1.3  2006/01/25 00:39:49  acg
//  Andy Goodrich: changed code to test new compliance message scheme.
//

// This program tests suppression of IEEE 1666 compliance messages via 
// setting the actions associated with SC_INFO.

#include "systemc.h"

int sc_main(int argc, char* argv[] )
{
	sc_clock clock;

	sc_initialize();
    sc_report_handler::set_actions( SC_INFO, SC_DO_NOTHING );
	sc_cycle(0);

	cerr << "Program completed" << endl;
	return 0;
}
