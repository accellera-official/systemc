/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
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

  sc_wait_cthread.cpp -- Wait() and related functions for SC_CTHREADs.

  Original Author: Stan Y. Liao, Synopsys, Inc.
                   Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* 
$Log: sc_wait_cthread.cpp,v $
Revision 1.1.1.1  2006/12/15 20:31:37  acg
SystemC 2.2

Revision 1.3  2006/03/13 20:26:51  acg
 Andy Goodrich: Addition of forward class declarations, e.g.,
 sc_reset, to keep gcc 4.x happy.

Revision 1.2  2006/01/03 23:18:45  acg
Changed copyright to include 2006.

Revision 1.1.1.1  2005/12/19 23:16:44  acg
First check in of SystemC 2.1 into its own archive.

Revision 1.10  2005/09/15 23:02:18  acg
Added std:: prefix to appropriate methods and types to get around
issues with the Edison Front End.

Revision 1.9  2005/09/02 19:03:30  acg
Changes for dynamic processes. Removal of lambda support.

Revision 1.8  2005/04/04 00:16:08  acg
Changes for directory name change to sys from systemc.
Changes for sc_string going to std::string.
Changes for sc_pvector going to std::vector.
Changes for reference pools for bit and part selections.
Changes for const sc_concatref support.

Revision 1.5  2004/09/27 20:49:10  acg
Andy Goodrich, Forte Design Systems, Inc.
   - Added a $Log comment so that CVS checkin comments appear in the
     checkout source.

*/


#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_cthread_process.h"
#include "sysc/kernel/sc_simcontext_int.h"
#include "sysc/kernel/sc_wait_cthread.h"
#include "sysc/communication/sc_port.h"
#include "sysc/kernel/sc_wait.h"
namespace sc_core 
{

// for SC_CTHREADs

void
halt( sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    switch( cpi->kind ) {
    case SC_CTHREAD_PROC_: {
	RCAST<sc_cthread_handle>( cpi->process_handle )->wait_halt();
	break;
    }
    default:
	SC_REPORT_ERROR( SC_ID_HALT_NOT_ALLOWED_, 0 );
	break;
    }
}


void
wait( int n, sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    if( n <= 0 ) {
	char msg[BUFSIZ];
	std::sprintf( msg, "n = %d", n );
	SC_REPORT_ERROR( SC_ID_WAIT_N_INVALID_, msg );
    }
    switch( cpi->kind ) {
      case SC_THREAD_PROC_: 
      case SC_CTHREAD_PROC_: 
	RCAST<sc_cthread_handle>( cpi->process_handle )->wait_cycles( n );
        break;
      default:
        break;
    }
}


void
at_posedge( const sc_signal_in_if<bool>& s, sc_simcontext* simc )
{
    if( s.read() == true ) 
        do { wait(simc); } while ( s.read() == true );
    do { wait(simc); } while ( s.read() == false );
}

void
at_posedge( const sc_signal_in_if<sc_dt::sc_logic>& s, sc_simcontext* simc )
{
    if( s.read() == '1' ) 
        do { wait(simc); } while ( s.read() == '1' );
    do { wait(simc); } while ( s.read() == '0' );
}

void
at_negedge( const sc_signal_in_if<bool>& s, sc_simcontext* simc )
{
    if( s.read() == false ) 
        do { wait(simc); } while ( s.read() == false );
    do { wait(simc); } while ( s.read() == true );
}

void
at_negedge( const sc_signal_in_if<sc_dt::sc_logic>& s, sc_simcontext* simc )
{
    if( s.read() == '0' ) 
        do { wait(simc); } while ( s.read() == '0' );
    do { wait(simc); } while ( s.read() == '1' );
}


} // namespace sc_core

// Taf!
