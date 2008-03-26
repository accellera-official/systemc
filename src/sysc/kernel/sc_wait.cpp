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

  sc_wait.cpp -- Wait() and related functions.

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
$Log: sc_wait.cpp,v $
Revision 1.5  2004/09/27 20:49:10  acg
Andy Goodrich, Forte Design Systems, Inc.
   - Added a $Log comment so that CVS checkin comments appear in the
     checkout source.

*/


#include "sysc/kernel/sc_except.h"
#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_process_int.h"
#include "sysc/kernel/sc_simcontext_int.h"
#include "sysc/kernel/sc_wait.h"

namespace sc_core {

// static sensitivity for SC_THREADs and SC_CTHREADs

void
wait( sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    switch( cpi->kind ) {
    case SC_THREAD_PROC_: {
	sc_switch_thread( simc );
        break;
    }
    case SC_CTHREAD_PROC_: {
	RCAST<sc_cthread_handle>( cpi->process_handle )->wait_clock();
        break;
    }
    default:
	SC_REPORT_ERROR( SC_ID_WAIT_NOT_ALLOWED_, "\n        "
			 "in SC_METHODs use next_trigger() instead" );
        break;
    }
}


// dynamic sensitivity for SC_THREADs and SC_CTHREADs

void
wait( const sc_event& e, sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    switch( cpi->kind ) {
    case SC_THREAD_PROC_: {
	RCAST<sc_thread_handle>( cpi->process_handle )->wait( e );
	break;
    }
    case SC_CTHREAD_PROC_: {
	sc_cthread_handle cthread_h =
            RCAST<sc_cthread_handle>( cpi->process_handle );
	cthread_h->wait( e );
	cthread_h->wait_clock();
	break;
    }
    default:
	SC_REPORT_ERROR( SC_ID_WAIT_NOT_ALLOWED_, "\n        "
			 "in SC_METHODs use next_trigger() instead" );
        break;
    }
}

void
wait( sc_event_or_list& el, sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    switch( cpi->kind ) {
    case SC_THREAD_PROC_: {
	RCAST<sc_thread_handle>( cpi->process_handle )->wait( el );
	break;
    }
    case SC_CTHREAD_PROC_: {
	sc_cthread_handle cthread_h =
            RCAST<sc_cthread_handle>( cpi->process_handle );
	cthread_h->wait( el );
	cthread_h->wait_clock();
	break;
    }
    default:
	SC_REPORT_ERROR( SC_ID_WAIT_NOT_ALLOWED_, "\n        "
			 "in SC_METHODs use next_trigger() instead" );
        break;
    }
}

void
wait( sc_event_and_list& el, sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    switch( cpi->kind ) {
    case SC_THREAD_PROC_: {
	RCAST<sc_thread_handle>( cpi->process_handle )->wait( el );
	break;
    }
    case SC_CTHREAD_PROC_: {
	sc_cthread_handle cthread_h =
            RCAST<sc_cthread_handle>( cpi->process_handle );
	cthread_h->wait( el );
	cthread_h->wait_clock();
	break;
    }
    default:
	SC_REPORT_ERROR( SC_ID_WAIT_NOT_ALLOWED_, "\n        "
			 "in SC_METHODs use next_trigger() instead" );
        break;
    }
}

void
wait( const sc_time& t, sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    switch( cpi->kind ) {
    case SC_THREAD_PROC_: {
	RCAST<sc_thread_handle>( cpi->process_handle )->wait( t );
	break;
    }
    case SC_CTHREAD_PROC_: {
	sc_cthread_handle cthread_h =
            RCAST<sc_cthread_handle>( cpi->process_handle );
	cthread_h->wait( t );
	cthread_h->wait_clock();
	break;
    }
    default:
	SC_REPORT_ERROR( SC_ID_WAIT_NOT_ALLOWED_, "\n        "
			 "in SC_METHODs use next_trigger() instead" );
        break;
    }
}

void
wait( const sc_time& t, const sc_event& e, sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    switch( cpi->kind ) {
    case SC_THREAD_PROC_: {
	RCAST<sc_thread_handle>( cpi->process_handle )->wait( t, e );
	break;
    }
    case SC_CTHREAD_PROC_: {
	sc_cthread_handle cthread_h =
            RCAST<sc_cthread_handle>( cpi->process_handle );
	cthread_h->wait( t, e );
	cthread_h->wait_clock();
	break;
    }
    default:
	SC_REPORT_ERROR( SC_ID_WAIT_NOT_ALLOWED_, "\n        "
			 "in SC_METHODs use next_trigger() instead" );
        break;
    }
}

void
wait( const sc_time& t, sc_event_or_list& el, sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    switch( cpi->kind ) {
    case SC_THREAD_PROC_: {
	RCAST<sc_thread_handle>( cpi->process_handle )->wait( t, el );
	break;
    }
    case SC_CTHREAD_PROC_: {
	sc_cthread_handle cthread_h =
            RCAST<sc_cthread_handle>( cpi->process_handle );
	cthread_h->wait( t, el );
	cthread_h->wait_clock();
	break;
    }
    default:
	SC_REPORT_ERROR( SC_ID_WAIT_NOT_ALLOWED_, "\n        "
			 "in SC_METHODs use next_trigger() instead" );
        break;
    }
}

void
wait( const sc_time& t, sc_event_and_list& el, sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    switch( cpi->kind ) {
    case SC_THREAD_PROC_: {
	RCAST<sc_thread_handle>( cpi->process_handle )->wait( t, el );
	break;
    }
    case SC_CTHREAD_PROC_: {
	sc_cthread_handle cthread_h =
            RCAST<sc_cthread_handle>( cpi->process_handle );
	cthread_h->wait( t, el );
	cthread_h->wait_clock();
	break;
    }
    default:
	SC_REPORT_ERROR( SC_ID_WAIT_NOT_ALLOWED_, "\n        "
			 "in SC_METHODs use next_trigger() instead" );
        break;
    }
}


// static sensitivity for SC_METHODs

void
next_trigger( sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    if( cpi->kind == SC_METHOD_PROC_ ) {
	RCAST<sc_method_handle>( cpi->process_handle )->clear_trigger();
    } else {
	SC_REPORT_ERROR( SC_ID_NEXT_TRIGGER_NOT_ALLOWED_, "\n        "
			 "in SC_THREADs and SC_CTHREADs use wait() instead" );
    }
}


// dynamic sensitivity for SC_METHODs

void
next_trigger( const sc_event& e, sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    if( cpi->kind == SC_METHOD_PROC_ ) {
	RCAST<sc_method_handle>( cpi->process_handle )->next_trigger( e );
    } else {
	SC_REPORT_ERROR( SC_ID_NEXT_TRIGGER_NOT_ALLOWED_, "\n        "
			 "in SC_THREADs and SC_CTHREADs use wait() instead" );
    }
}

void
next_trigger( sc_event_or_list& el, sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    if( cpi->kind == SC_METHOD_PROC_ ) {
	RCAST<sc_method_handle>( cpi->process_handle )->next_trigger( el );
    } else {
	SC_REPORT_ERROR( SC_ID_NEXT_TRIGGER_NOT_ALLOWED_, "\n        "
			 "in SC_THREADs and SC_CTHREADs use wait() instead" );
    }
}

void
next_trigger( sc_event_and_list& el, sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    if( cpi->kind == SC_METHOD_PROC_ ) {
	RCAST<sc_method_handle>( cpi->process_handle )->next_trigger( el );
    } else {
	SC_REPORT_ERROR( SC_ID_NEXT_TRIGGER_NOT_ALLOWED_, "\n        "
			 "in SC_THREADs and SC_CTHREADs use wait() instead" );
    }
}

void
next_trigger( const sc_time& t, sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    if( cpi->kind == SC_METHOD_PROC_ ) {
	RCAST<sc_method_handle>( cpi->process_handle )->next_trigger( t );
    } else {
	SC_REPORT_ERROR( SC_ID_NEXT_TRIGGER_NOT_ALLOWED_, "\n        "
			 "in SC_THREADs and SC_CTHREADs use wait() instead" );
    }
}

void
next_trigger( const sc_time& t, const sc_event& e, sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    if( cpi->kind == SC_METHOD_PROC_ ) {
	RCAST<sc_method_handle>( cpi->process_handle )->next_trigger( t, e );
    } else {
	SC_REPORT_ERROR( SC_ID_NEXT_TRIGGER_NOT_ALLOWED_, "\n        "
			 "in SC_THREADs and SC_CTHREADs use wait() instead" );
    }
}

void
next_trigger( const sc_time& t, sc_event_or_list& el, sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    if( cpi->kind == SC_METHOD_PROC_ ) {
	RCAST<sc_method_handle>( cpi->process_handle )->next_trigger( t, el );
    } else {
	SC_REPORT_ERROR( SC_ID_NEXT_TRIGGER_NOT_ALLOWED_, "\n        "
			 "in SC_THREADs and SC_CTHREADs use wait() instead" );
    }
}

void
next_trigger( const sc_time& t, sc_event_and_list& el, sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    if( cpi->kind == SC_METHOD_PROC_ ) {
	RCAST<sc_method_handle>( cpi->process_handle )->next_trigger( t, el );
    } else {
	SC_REPORT_ERROR( SC_ID_NEXT_TRIGGER_NOT_ALLOWED_, "\n        "
			 "in SC_THREADs and SC_CTHREADs use wait() instead" );
    }
}


// for SC_METHODs and SC_THREADs and SC_CTHREADs

bool
timed_out( sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    return cpi->process_handle->timed_out();
}



// misc.

void
sc_set_location( const char* file, int lineno, sc_simcontext* simc )
{
    sc_curr_proc_handle cpi = simc->get_curr_proc_info();
    sc_process_b* handle = cpi->process_handle;
    handle->file = file;
    handle->lineno = lineno;
}

} // namespace sc_core

// Taf!
