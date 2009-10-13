/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2003 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_report -- test error reporting, sepcifically log file

  That that messages are printed into the log file.

  Original Author: Ulli Holtmann, Synopsys, Inc., Jan 2003

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <systemc.h>

bool sc_core::sc_report_close_default_log();

SC_MODULE( M )
{
    sc_event trigger_t, trigger_m;
    SC_CTOR( M ) {
	SC_THREAD( thread1 );
	sensitive << trigger_t;
	dont_initialize();

	SC_METHOD( method1 );
	sensitive << trigger_m;
	dont_initialize();
    }
    void method1() {
	sc_report_handler::report( SC_INFO,    "ID1", "after log is opened", "method", 101);
	sc_report_handler::report( SC_WARNING, "ID2", "after log is opened", "method", 102);
	sc_report_handler::report( SC_ERROR,   "ID1", "after log is opened", "method", 103);
	sc_report_handler::report( SC_FATAL,   "ID2", "after log is opened", "method", 104);
    }
    void thread1() {
	while(1) {
	    sc_report_handler::report( SC_INFO,    "ID2", "after log is opened", "thread", 201);
	    sc_report_handler::report( SC_WARNING, "ID1", "after log is opened", "thread", 202);
	    sc_report_handler::report( SC_ERROR,   "ID2", "after log is opened", "thread", 203);
	    sc_report_handler::report( SC_FATAL,   "ID1", "after log is opened", "thread", 204);
	    wait();
	}
    }
};

int sc_main(int,char**)
{
    const char fname[] = "./sc_report.log";
    M uut("uut");

    // do not abort when on error, fatal
    sc_report_handler::stop_after( SC_ERROR, 0 );
    sc_report_handler::stop_after( SC_FATAL, 0 );
    sc_report_handler::suppress( SC_STOP | SC_ABORT );
    sc_report_handler::set_actions( "ID1", SC_LOG | SC_DISPLAY );
    sc_report_handler::set_actions( "ID2", SC_LOG | SC_DISPLAY );    

    // produces messages before the log file is opened
    sc_report_handler::report( SC_INFO,    "ID1", "before log is opened", "no_file", 0);
    sc_report_handler::report( SC_WARNING, "ID1", "before log is opened", "no_file", 0);

    // open log file
    sc_report_handler::set_log_file_name( fname );
    assert( strcmp(fname,sc_report_handler::get_log_file_name()) == 0 );

    // emit report from global context at t=0
    sc_report_handler::report( SC_INFO,    "ID1", "after log is opened", "file1", 1);
    sc_report_handler::report( SC_WARNING, "ID2", "after log is opened", "file2", 2);
    sc_report_handler::report( SC_ERROR,   "ID1", "after log is opened", "file3", 3);
    sc_report_handler::report( SC_FATAL,   "ID2", "after log is opened", "file4", 4);

    // emit report from global context at 10ns, method at 20ns, thread at 30ns
    uut.trigger_m.notify( 20, SC_NS );
    uut.trigger_t.notify( 30, SC_NS );
    sc_start( 10,SC_NS );
    sc_report_handler::report( SC_ERROR,   "ID1", "after log is opened", "file3", 5);
    sc_start( 100, SC_NS );

    // close log file and more messages
    sc_report_close_default_log();

    sc_report_handler::report( SC_INFO,    "ID1", "after log is closed", "no_file", 0);
    sc_report_handler::report( SC_WARNING, "ID1", "after log is closed", "no_file", 0);

    // open file again and dump into cout so we cover it by regression
    cout << "\n\nDump the logfile\n";
    FILE* log = fopen( fname,"r" );
    if ( !log ) {
	SC_REPORT_FATAL( "Can not open the sc_report log file: ", fname );
	return 1;
    }
    char c;
    while ( 1 ) {
	c = fgetc(log);
	if (c==EOF)
	    break;
	cout << c;
    }
    fclose( log) ;

    return 0;
}
