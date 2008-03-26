/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
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
Revision 1.5  2004/09/27 20:49:10  acg
Andy Goodrich, Forte Design Systems, Inc.
   - Added a $Log comment so that CVS checkin comments appear in the
     checkout source.

*/


#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_process_int.h"
#include "sysc/kernel/sc_simcontext_int.h"
#include "sysc/kernel/sc_wait_cthread.h"
#include "sysc/communication/sc_port.h"

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
	sprintf( msg, "n = %d", n );
	SC_REPORT_ERROR( SC_ID_WAIT_N_INVALID_, msg );
    }
    switch( cpi->kind ) {
      case SC_CTHREAD_PROC_: 
	RCAST<sc_cthread_handle>( cpi->process_handle )->wait_clock( n );
        break;
      default:
        do { wait(); } while (--n);
        break;
    }
}


void
wait_until( const sc_lambda_ptr& lambda, sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    switch( cpi->kind ) {
    case SC_CTHREAD_PROC_: {
	RCAST<sc_cthread_handle>( cpi->process_handle )->wait_lambda( lambda );
        break;
    }
    default:
	SC_REPORT_ERROR( SC_ID_WAIT_UNTIL_NOT_ALLOWED_, 0 );
        break;
    }
}


void
at_posedge( const sc_signal_in_if<bool>& s, sc_simcontext* simc )
{
    if( s.read() ) {
        wait_until( s.delayed() == false, simc );
        wait_until( s.delayed(), simc );
    } else {
        wait_until( s.delayed(), simc );
    }
}

void
at_posedge( const sc_signal_in_if<sc_dt::sc_logic>& s, sc_simcontext* simc )
{
    if( s.read() == '1' ) {
        wait_until( s.delayed() == '0', simc );
        wait_until( s.delayed() == '1', simc );
    } else {
        wait_until( s.delayed() == '1', simc );
    }
}

void
at_negedge( const sc_signal_in_if<bool>& s, sc_simcontext* simc )
{
    if( ! s.read() ) {
        wait_until( s.delayed(), simc );
        wait_until( s.delayed() == false, simc );
    } else {
        wait_until( s.delayed() == false, simc );
    }
}

void
at_negedge( const sc_signal_in_if<sc_dt::sc_logic>& s, sc_simcontext* simc )
{
    if( s.read() == '0' ) {
        wait_until( s.delayed() == '1', simc );
        wait_until( s.delayed() == '0', simc );
    } else {
        wait_until( s.delayed() == '0', simc );
    }
}


void
watching_before_simulation( const sc_lambda_ptr& lambda, sc_simcontext* simc )
{
    simc->get_port_registry()->add_lambda_for_resolution( lambda );
    sc_process_b* last_proc = sc_get_last_created_process_handle();
    
    if (!last_proc) return;
    if ( last_proc->is_cthread() ) {
	RCAST<sc_cthread_handle>( last_proc )->add_lambda( lambda );
    } else {
	SC_REPORT_ERROR( SC_ID_WATCHING_NOT_ALLOWED_, 0 );
    }
}

void
watching_during_simulation( const sc_lambda_ptr& lambda, sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    switch( cpi->kind ) {
    case SC_CTHREAD_PROC_: {
	RCAST<sc_cthread_handle>( cpi->process_handle )->add_lambda( lambda );
        break;
    }
    default:
	SC_REPORT_ERROR( SC_ID_WATCHING_NOT_ALLOWED_, 0 );
        break;
    }
}
 


void
__reset_watching( sc_cthread_handle cthread_h )
{
    cthread_h->__reset_watching();
}

void
__open_watching( sc_cthread_handle cthread_h )
{
    cthread_h->__open_watching();
}

void
__close_watching( sc_cthread_handle cthread_h )
{
    cthread_h->__close_watching();
    assert( cthread_h->__watch_level() >= 0 );
}

int
__watch_level( sc_cthread_handle cthread_h )
{
    return cthread_h->__watch_level();
}

void
__watching_first( sc_cthread_handle cthread_h )
{
    if( cthread_h->eval_watchlist_curr_level() ) {
        throw cthread_h->m_exception_level;
    }
}

void
__sanitycheck_watchlists( sc_cthread_handle cthread_h )
{
    assert( cthread_h->m_watchlists[cthread_h->__watch_level()]->empty() );
}

} // namespace sc_core

// Taf!
