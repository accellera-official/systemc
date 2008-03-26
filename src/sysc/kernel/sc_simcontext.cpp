/****************************************************************************

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

  sc_simcontext.cpp -- Provides a simulation context for use with multiple
                       simulations.

  Original Author: Stan Y. Liao, Synopsys, Inc.
                   Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Ali Dasdan, Synopsys, Inc.
  Description of Modification: - Added sc_stop() detection into initial_crunch
                                 and crunch. This makes it possible to exit out
                                 of a combinational loop using sc_stop().

      Name, Affiliation, Date: Andy Goodrich, Forte Design Systems 20 May 2003
  Description of Modification: - sc_stop mode
                               - phase callbacks

      Name, Affiliation, Date: Bishnupriya Bhattacharya, Cadence Design Systems,
                               25 August 2003
  Description of Modification: - support for dynamic process
                               - support for sc export registry
                               - new member methods elaborate(), 
				 prepare_to_simulate(), and initial_crunch()
				 that are invoked by initialize() in that order
                               - implement sc_get_last_created_process_handle() for use
                                 before simulation starts
                               - remove "set_curr_proc(handle)" from 
                                 register_method_process and 
                                 register_thread_process - led to bugs
                               
      Name, Affiliation, Date: Andy Goodrich, Forte Design Systems 04 Sep 2003
  Description of Modification: - changed process existence structures to
				 linked lists to eliminate exponential 
				 execution problem with using sc_pvector.
 *****************************************************************************/
// $Log: sc_simcontext.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.17  2006/04/11 23:13:21  acg
//   Andy Goodrich: Changes for reduced reset support that only includes
//   sc_cthread, but has preliminary hooks for expanding to method and thread
//   processes also.
//
// Revision 1.16  2006/03/21 00:00:34  acg
//   Andy Goodrich: changed name of sc_get_current_process_base() to be
//   sc_get_current_process_b() since its returning an sc_process_b instance.
//
// Revision 1.15  2006/03/13 20:26:50  acg
//  Andy Goodrich: Addition of forward class declarations, e.g.,
//  sc_reset, to keep gcc 4.x happy.
//
// Revision 1.14  2006/02/02 23:42:41  acg
//  Andy Goodrich: implemented a much better fix to the sc_event_finder
//  proliferation problem. This new version allocates only a single event
//  finder for each port for each type of event, e.g., pos(), neg(), and
//  value_change(). The event finder persists as long as the port does,
//  which is what the LRM dictates. Because only a single instance is
//  allocated for each event type per port there is not a potential
//  explosion of storage as was true in the 2.0.1/2.1 versions.
//
// Revision 1.13  2006/02/02 21:29:10  acg
//  Andy Goodrich: removed the call to sc_event_finder::free_instances() that
//  was in end_of_elaboration(), leaving only the call in clean(). This is
//  because the LRM states that sc_event_finder instances are persistent as
//  long as the sc_module hierarchy is valid.
//
// Revision 1.12  2006/02/02 21:09:50  acg
//  Andy Goodrich: added call to sc_event_finder::free_instances in the clean()
//  method.
//
// Revision 1.11  2006/02/02 20:43:14  acg
//  Andy Goodrich: Added an existence linked list to sc_event_finder so that
//  the dynamically allocated instances can be freed after port binding
//  completes. This replaces the individual deletions in ~sc_bind_ef, as these
//  caused an exception if an sc_event_finder instance was used more than
//  once, due to a double freeing of the instance.
//
// Revision 1.10  2006/01/31 21:43:26  acg
//  Andy Goodrich: added comments in constructor to highlight environmental
//  overrides section.
//
// Revision 1.9  2006/01/26 21:04:54  acg
//  Andy Goodrich: deprecation message changes and additional messages.
//
// Revision 1.8  2006/01/25 00:31:19  acg
//  Andy Goodrich: Changed over to use a standard message id of
//  SC_ID_IEEE_1666_DEPRECATION for all deprecation messages.
//
// Revision 1.7  2006/01/24 20:49:05  acg
// Andy Goodrich: changes to remove the use of deprecated features within the
// simulator, and to issue warning messages when deprecated features are used.
//
// Revision 1.6  2006/01/19 00:29:52  acg
// Andy Goodrich: Yet another implementation for signal write checking. This
// one uses an environment variable SC_SIGNAL_WRITE_CHECK, that when set to
// DISABLE will disable write checking on signals.
//
// Revision 1.5  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//
// Revision 1.4  2006/01/03 23:18:44  acg
// Changed copyright to include 2006.
//
// Revision 1.3  2005/12/20 22:11:10  acg
// Fixed $Log lines.
//
// Revision 1.2  2005/12/20 22:02:30  acg
// Changed where delta cycles are incremented to match IEEE 1666. Added the
// event_occurred() method to hide how delta cycle comparisions are done within
// sc_simcontext. Changed the boolean update_phase to an enum that shows all
// the phases.

#include "sysc/kernel/sc_cor_fiber.h"
#include "sysc/kernel/sc_cor_pthread.h"
#include "sysc/kernel/sc_cor_qt.h"
#include "sysc/kernel/sc_event.h"
#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_macros_int.h"
#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_module_registry.h"
#include "sysc/kernel/sc_name_gen.h"
#include "sysc/kernel/sc_object_manager.h"
#include "sysc/kernel/sc_cthread_process.h"
#include "sysc/kernel/sc_method_process.h"
#include "sysc/kernel/sc_thread_process.h"
#include "sysc/kernel/sc_process_handle.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_simcontext_int.h"
#include "sysc/kernel/sc_reset.h"
#include "sysc/kernel/sc_ver.h"
#include "sysc/communication/sc_port.h"
#include "sysc/communication/sc_export.h"
#include "sysc/communication/sc_prim_channel.h"
#include "sysc/tracing/sc_trace.h"
#include "sysc/utils/sc_mempool.h"
#include "sysc/utils/sc_utils_ids.h"

namespace sc_core {

sc_stop_mode stop_mode = SC_STOP_FINISH_DELTA;

// ----------------------------------------------------------------------------
//  CLASS : sc_process_table
//
//  Container class that keeps track of all method processes,
//  thread processes, and cthread processes.
// ----------------------------------------------------------------------------

class sc_process_table
{
  public:

    sc_process_table();
    ~sc_process_table();
    void push_front( sc_method_handle );
    void push_front( sc_thread_handle );
    void push_front( sc_cthread_handle );
    sc_cthread_handle cthread_q_head();
    sc_method_handle method_q_head();
    sc_cthread_handle remove( sc_cthread_handle );
    sc_method_handle remove( sc_method_handle );
    sc_thread_handle remove( sc_thread_handle );
    sc_thread_handle thread_q_head();


  private:

    sc_cthread_handle m_cthread_q; // Queue of existing cthread processes.
    sc_method_handle  m_method_q;  // Queue of existing method processes.
    sc_thread_handle  m_thread_q;  // Queue of existing thread processes.
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

sc_process_table::sc_process_table() :
    m_cthread_q(0), m_method_q(0), m_thread_q(0)
{}

sc_process_table::~sc_process_table()
{

    sc_method_handle  method_next_p;	// Next method to delete.
    sc_method_handle  method_now_p;	// Method now deleting.

    for( method_now_p = m_method_q; method_now_p; method_now_p = method_next_p )
    {
	method_next_p = method_now_p->next_exist();
	delete method_now_p;
    }

    if ( m_thread_q || m_cthread_q )
    {
        ::std::cout << ::std::endl 
             << "WATCH OUT!! In sc_process_table destructor. "
             << "Threads and cthreads are not actually getting deleted here. "
	     << "Some memory may leak. Look at the comments here in "
	     << "kernel/sc_simcontext.cpp for more details."
	     << ::std::endl;
    }

    // don't delete threads and cthreads. If a (c)thread
    // has died, then it has already been deleted. Only (c)threads created
    // before simulation-start are in this table. Due to performance
    // reasons, we don't look up the dying thread in the process table
    // and remove it from there. simcontext::reset and ~simcontext invoke this
    // destructor. At present none of these routines are ever invoked. 
    // We can delete threads and cthreads here if a dying thread figured out
    // it was created before simulation-start and took itself off the 
    // process_table. 

#if 0
    sc_cthread_handle cthread_next_p;	// Next cthread to delete.
    sc_cthread_handle cthread_now_p;	// Cthread now deleting.
    sc_thread_handle  thread_next_p;	// Next thread to delete.
    sc_thread_handle  thread_now_p;	// Thread now deleting.

    for(cthread_now_p=m_cthread_q; cthread_now_p; cthread_now_p=cthread_next_p)
    {
	cthread_next_p = cthread_now_p->next_exist();
	delete cthread_now_p;
    }

    for( thread_now_p=m_thread_q; thread_now_p; thread_now_p=thread_next_p )
    {
	thread_next_p = thread_now_p->next_exist();
	delete thread_now_p;
    }
#endif // 0
}

inline
sc_cthread_handle 
sc_process_table::cthread_q_head()
{
    return m_cthread_q;
}

inline
sc_method_handle 
sc_process_table::method_q_head()
{
    return m_method_q;
}

inline
void
sc_process_table::push_front( sc_method_handle handle_ )
{
    handle_->set_next_exist(m_method_q);
    m_method_q = handle_;
}

inline
void
sc_process_table::push_front( sc_thread_handle handle_ )
{
    handle_->set_next_exist(m_thread_q);
    m_thread_q = handle_;
}

inline
void
sc_process_table::push_front( sc_cthread_handle handle_ )
{
    handle_->set_next_exist(m_cthread_q);
    m_cthread_q = handle_;
}


sc_cthread_handle
sc_process_table::remove( sc_cthread_handle handle_ )
{
    sc_cthread_handle now_p;	// Entry now examining.
    sc_cthread_handle prior_p;	// Entry prior to one now examining.

    prior_p = 0;
    for ( now_p = m_cthread_q; now_p; now_p = now_p->next_exist() )
    {
	if ( now_p == handle_ )
	{
	    if ( prior_p )
		prior_p->set_next_exist( now_p->next_exist() );
	    else
		m_cthread_q = now_p->next_exist();
	    return handle_;
	}
    }
    return 0;
}

sc_method_handle
sc_process_table::remove( sc_method_handle handle_ )
{
    sc_method_handle now_p;	// Entry now examining.
    sc_method_handle prior_p;	// Entry prior to one now examining.

    prior_p = 0;
    for ( now_p = m_method_q; now_p; now_p = now_p->next_exist() )
    {
	if ( now_p == handle_ )
	{
	    if ( prior_p )
		prior_p->set_next_exist( now_p->next_exist() );
	    else
		m_method_q = now_p->next_exist();
	    return handle_;
	}
    }
    return 0;
}

sc_thread_handle
sc_process_table::remove( sc_thread_handle handle_ )
{
    sc_thread_handle now_p;	// Entry now examining.
    sc_thread_handle prior_p;	// Entry prior to one now examining.

    prior_p = 0;
    for ( now_p = m_thread_q; now_p; now_p = now_p->next_exist() )
    {
	if ( now_p == handle_ )
	{
	    if ( prior_p )
		prior_p->set_next_exist( now_p->next_exist() );
	    else
		m_thread_q = now_p->next_exist();
	    return handle_;
	}
    }
    return 0;
}

inline
sc_thread_handle 
sc_process_table::thread_q_head()
{
    return m_thread_q;
}



// ----------------------------------------------------------------------------

void
pln()
{
    static bool lnp = false;
    if( ! lnp ) {
        ::std::cerr << ::std::endl;
	::std::cerr << sc_version() << ::std::endl;
	::std::cerr << sc_copyright() << ::std::endl;

	//  regressions check point
        if( getenv( "SYSTEMC_REGRESSION" ) != 0 ) {
            ::std::cerr << "SystemC Simulation" << ::std::endl;
        }

        lnp = true;
    }
}


int
sc_notify_time_compare( const void* p1, const void* p2 )
{
    const sc_event_timed* et1 = static_cast<const sc_event_timed*>( p1 );
    const sc_event_timed* et2 = static_cast<const sc_event_timed*>( p2 );

    const sc_time& t1 = et1->notify_time();
    const sc_time& t2 = et2->notify_time();
    
    if( t1 < t2 ) {
	return 1;
    } else if( t1 > t2 ) {
	return -1;
    } else {
	return 0;
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_simcontext
//
//  The simulation context.
// ----------------------------------------------------------------------------

void
sc_simcontext::init()
{

    // ALLOCATE VARIOUS MANAGERS AND REGISTRIES:

    m_object_manager = new sc_object_manager;
    m_module_registry = new sc_module_registry( *this );
    m_port_registry = new sc_port_registry( *this );
    m_export_registry = new sc_export_registry( *this );
    m_prim_channel_registry = new sc_prim_channel_registry( *this );
    m_name_gen = new sc_name_gen;
    m_process_table = new sc_process_table;
    m_current_writer = 0;


    // CHECK FOR ENVIRONMENT VARIABLES THAT MODIFY SIMULATOR EXECUTION:

    const char* write_check = std::getenv("SC_SIGNAL_WRITE_CHECK");
    m_write_check = ( (write_check==0) || strcmp(write_check,"DISABLE") ) ?
      true : false;


    // FINISH INITIALIZATIONS:

    reset_curr_proc();
    m_next_proc_id = -1;
    m_timed_events = new sc_ppq<sc_event_timed*>( 128, sc_notify_time_compare );
    m_something_to_trace = false;
    m_runnable = new sc_runnable;
    m_time_params = new sc_time_params;
    m_curr_time = SC_ZERO_TIME;
    m_delta_count = 0;
    m_forced_stop = false;
    m_ready_to_simulate = false;
    m_elaboration_done = false;
    m_execution_phase = phase_initialize;
    m_error = false;
    m_until_event = 0;
    m_cor_pkg = 0;
    m_cor = 0;
    m_in_simulator_control = false;
    m_start_of_simulation_called = false;
    m_end_of_simulation_called = false;
}

void
sc_simcontext::clean()
{
    delete m_object_manager;
    delete m_module_registry;
    delete m_port_registry;
    delete m_export_registry;
    delete m_prim_channel_registry;
    delete m_name_gen;
    delete m_process_table;
    m_child_objects.resize(0);
    m_delta_events.resize(0);
    delete m_timed_events;
    for( int i = m_trace_files.size() - 1; i >= 0; -- i ) {
	delete m_trace_files[i];
    }
    m_trace_files.resize(0);
    delete m_runnable;
    delete m_time_params;
    if( m_until_event != 0 ) {
        delete m_until_event;
    }
    if( m_cor_pkg != 0 ) {
	delete m_cor_pkg;
    }
}


sc_simcontext::sc_simcontext()
{
    init();
}

sc_simcontext::~sc_simcontext()
{
    clean();
}

inline void
sc_simcontext::crunch( bool once )
{
#ifdef DEBUG_SYSTEMC
    int num_deltas = 0;  // number of delta cycles
#endif

    while ( true ) {

	// EVALUATE PHASE
	
	m_execution_phase = phase_evaluate;
	while( true ) {


	    // execute method processes

	    sc_method_handle method_h = pop_runnable_method();
	    while( method_h != 0 ) {
		try {
		    method_h->semantics();
		}
		catch( const sc_report& ex ) {
		    ::std::cout << "\n" << ex.what() << ::std::endl;
		    m_error = true;
		    return;
		}
		method_h = pop_runnable_method();
	    }

	    // execute (c)thread processes

	    sc_thread_handle thread_h = pop_runnable_thread();
	    while( thread_h != 0 ) {
		if ( thread_h->ready_to_run() ) break;
		thread_h = pop_runnable_thread();
	    }
	    if( thread_h != 0 ) {
		m_cor_pkg->yield( thread_h->m_cor_p );
	    }
	    if( m_error ) {
		return;
	    }

	    // check for call(s) to sc_stop
	    if( m_forced_stop ) {
		if ( stop_mode == SC_STOP_IMMEDIATE ) return;
	    }

	    if( m_runnable->is_empty() ) {
		// no more runnable processes
		break;
	    }
	    m_runnable->toggle();
	}

	// UPDATE PHASE
	//
	// The delta count must be updated first so that event_occurred()
	// will work.

	m_execution_phase = phase_update;
	m_delta_count ++;
	m_prim_channel_registry->perform_update();
	m_execution_phase = phase_notify;
	
	if( m_something_to_trace ) {
	    trace_cycle( /* delta cycle? */ true );
	}

	// m_delta_count ++;

        // check for call(s) to sc_stop
        if( m_forced_stop ) {
            break;
        }

#ifdef DEBUG_SYSTEMC
        // check for possible infinite loops
        if( ++ num_deltas > SC_MAX_NUM_DELTA_CYCLES ) {
	    ::std::cerr << "SystemC warning: "
		 << "the number of delta cycles exceeds the limit of "
		 << SC_MAX_NUM_DELTA_CYCLES
		 << ", defined in sc_constants.h.\n"
		 << "This is a possible sign of an infinite loop.\n"
		 << "Increase the limit if this warning is invalid.\n";
	    break;
	}
#endif

	// PROCESS DELTA NOTIFICATIONS:

        int size = m_delta_events.size();
	if ( size != 0 )
	{
	    sc_event** l_events = &m_delta_events[0];
	    int i = size - 1;
	    do {
		l_events[i]->trigger();
	    } while( -- i >= 0 );
	    m_delta_events.resize(0);
	}
	
	if( m_runnable->is_empty() ) {
	    // no more runnable processes
	    break;
	}

	// IF ONLY DOING ONE CYCLE, WE ARE DONE. OTHERWISE GET NEW CALLBACKS

	if ( once ) break;

	m_runnable->toggle();
    } 
}

inline
void
sc_simcontext::cycle( const sc_time& t)
{
    sc_time next_event_time;

    m_in_simulator_control = true;
    m_runnable->toggle();
    crunch(); 
    trace_cycle( /* delta cycle? */ false );
    m_curr_time += t;
    next_event_time = next_time();
    if ( next_event_time != SC_ZERO_TIME && next_event_time <= m_curr_time) {
        SC_REPORT_WARNING(SC_ID_CYCLE_MISSES_EVENTS_, "");
    }
    m_in_simulator_control = false;
}

void
sc_simcontext::elaborate()
{
    if( m_elaboration_done || sim_status() != SC_SIM_OK ) {
        return;
    }

    m_port_registry->construction_done();
    m_export_registry->construction_done();
    m_prim_channel_registry->construction_done();
    m_module_registry->construction_done();

    // check for call(s) to sc_stop
    if( m_forced_stop ) {
        do_sc_stop_action();
        return;
    }

    // SIGNAL THAT ELABORATION IS DONE
    //
    // We set the switch before the calls in case someone creates a process 
    // in an end_of_elaboration callback. We need the information to flag 
    // the process as being dynamic.

    m_elaboration_done = true;

    m_port_registry->elaboration_done();
    m_export_registry->elaboration_done();
    m_prim_channel_registry->elaboration_done();
    m_module_registry->elaboration_done();
    sc_reset::reconcile_resets();

    // check for call(s) to sc_stop
    if( m_forced_stop ) {
        do_sc_stop_action();
        return;
    }
}

void
sc_simcontext::prepare_to_simulate()
{
    sc_cthread_handle cthread_p; // Pointer to cthread process accessing.
    sc_method_handle  method_p;  // Pointer to method process accessing.
    sc_thread_handle  thread_p;  // Pointer to thread process accessing.

    if( m_ready_to_simulate || sim_status() != SC_SIM_OK ) {
        return;
    }

    // instantiate the coroutine package
#   if defined(WIN32)
        m_cor_pkg = new sc_cor_pkg_fiber( this );
#   else
#       if defined(SC_USE_PTHREADS)
            m_cor_pkg = new sc_cor_pkg_pthread( this );
#       else
			m_cor_pkg = new sc_cor_pkg_qt( this );
#       endif
#   endif
    m_cor = m_cor_pkg->get_main();

    // NOTIFY ALL OBJECTS THAT SIMULATION IS ABOUT TO START:

    m_port_registry->start_simulation();
    m_export_registry->start_simulation();
    m_prim_channel_registry->start_simulation();
    m_module_registry->start_simulation();
    m_start_of_simulation_called = true;

    // CHECK FOR CALL(S) TO sc_stop 

    if( m_forced_stop ) {
        do_sc_stop_action();
        return;
    }

    // PREPARE ALL (C)THREAD PROCESSES FOR SIMULATION:

    for ( thread_p = m_process_table->thread_q_head(); 
	  thread_p; thread_p = thread_p->next_exist() )
    {
	thread_p->prepare_for_simulation();
    }

    for ( cthread_p = m_process_table->cthread_q_head(); 
	  cthread_p; cthread_p = cthread_p->next_exist() )
    {
	cthread_p->prepare_for_simulation();
    }

    m_ready_to_simulate = true;
    m_runnable->init();

    // update phase

    m_execution_phase = phase_update;
    m_prim_channel_registry->perform_update();
    m_execution_phase = phase_notify;

    int size;

    // make all method processes runnable

    for ( method_p = m_process_table->method_q_head(); 
	  method_p; method_p = method_p->next_exist() )
    {
        if( !method_p->dont_initialize() ) {
            push_runnable_method_front( method_p );
        }
    }

    // make all thread processes runnable

    for ( thread_p = m_process_table->thread_q_head(); 
	  thread_p; thread_p = thread_p->next_exist() )
    {
        if( !thread_p->dont_initialize() ) {
            push_runnable_thread_front( thread_p );
        }
    }


    // process delta notifications

    if( ( size = m_delta_events.size() ) != 0 ) {
        sc_event** l_delta_events = &m_delta_events[0];
        int i = size - 1;
        do {
            l_delta_events[i]->trigger();
        } while( -- i >= 0 );
        m_delta_events.resize(0);
    }

    // used in 'simulate()'
    m_until_event = new sc_event;

    if( m_runnable->is_empty() ) {
        m_delta_count++;
    }
}

void
sc_simcontext::initial_crunch( bool no_crunch )
{
    if( no_crunch || m_runnable->is_empty() ) {
        return;
    }
    m_runnable->toggle();

    // run the delta cycle loop

    crunch();
    if( m_error ) {
        return;
    }
    if( m_something_to_trace ) {
        trace_cycle( false );
    }
    // check for call(s) to sc_stop
    if( m_forced_stop ) {
        do_sc_stop_action();
    }
}

void
sc_simcontext::initialize( bool no_crunch )
{
    m_in_simulator_control = true;
    elaborate();
    prepare_to_simulate();
    initial_crunch(no_crunch);
    m_in_simulator_control = false;
}

void
sc_simcontext::simulate( const sc_time& duration )
{
    initialize( true );

    if (sim_status() != SC_SIM_OK) {
	return;
    }

    sc_time non_overflow_time = 
        sc_time(~sc_dt::UINT64_ZERO, false) - m_curr_time;
    if ( duration > non_overflow_time )
    {
	SC_REPORT_ERROR(SC_ID_SIMULATION_TIME_OVERFLOW_, "");
	return;
    }
    else if ( duration < SC_ZERO_TIME )
    {
	SC_REPORT_ERROR(SC_ID_NEGATIVE_SIMULATION_TIME_,"");
    }
    m_in_simulator_control = true;

    sc_time until_t = m_curr_time + duration;

    m_until_event->cancel();  // to be on the safe side
    m_until_event->notify_internal( duration );

    sc_time t;

    // IF DURATION WAS ZERO WE ONLY CRUNCH:
    //
    // We duplicate the code so that we don't add the overhead of the
    // check to each loop in the do below.
    if ( duration == SC_ZERO_TIME ) 
    {
        m_runnable->toggle();
  	crunch( true );
	if( m_error ) return;
	if( m_something_to_trace ) trace_cycle( /* delta cycle? */ false );
	if( m_forced_stop ) 
	    do_sc_stop_action(); 
	return;
    }


    // NON-ZERO DURATION: EXECUTE UP TO THAT TIME:
    do {
	m_runnable->toggle();

	crunch();
	if( m_error ) {
	    m_in_simulator_control = false;
	    return;
	}
	if( m_something_to_trace ) {
	    trace_cycle( false );
	}
	// check for call(s) to sc_stop
	if( m_forced_stop ) {
	    do_sc_stop_action();
	    return;
	}
	
	do {
            t = next_time();

	    // PROCESS TIMED NOTIFICATIONS

	    do {
		sc_event_timed* et = m_timed_events->extract_top();
		sc_event* e = et->event();
		delete et;
		if( e != 0 ) {
		    e->trigger();
		}
	    } while( m_timed_events->size() &&
		     m_timed_events->top()->notify_time() == t );

	} while( m_runnable->is_empty() && t != until_t );
	if ( t > m_curr_time ) m_curr_time = t;

    } while( t != until_t );
    m_in_simulator_control = false;
}

void
sc_simcontext::do_sc_stop_action()
{
    ::std::cout << "SystemC: simulation stopped by user." << ::std::endl;
    if (m_start_of_simulation_called) {
	end();
	m_in_simulator_control = false;
    }
}


//------------------------------------------------------------------------------
//"sc_simcontext::stop"
//
// This method stops the simulator after some amount of further processing.
// How much processing is done depends upon the value of the global variable
// stop_mode:
//     SC_STOP_IMMEDIATE - aborts the execution phase of the current delta
//                         cycle and performs whatever updates are pending.
//     SC_STOP_FINISH_DELTA - finishes the current delta cycle - both execution
//                            and updates.
// If sc_stop is called outside of the purview of the simulator kernel 
// (e.g., directly from sc_main), the end of simulation notifications 
// are performed. From within the purview of the simulator kernel, these
// will be performed at a later time.
//------------------------------------------------------------------------------

void
sc_simcontext::stop()
{
    static bool stop_warning_issued = false;
    if (m_forced_stop)
    {
        if ( !stop_warning_issued )
        {
            stop_warning_issued = true; // This must be before the WARNING!!!
            SC_REPORT_WARNING(SC_ID_SIMULATION_STOP_CALLED_TWICE_, "");
        }
        return;
    }
    if ( stop_mode == SC_STOP_IMMEDIATE ) m_runnable->init();
    m_forced_stop = true;
    if ( !m_in_simulator_control  )
    {
        do_sc_stop_action();
    } 
}

void
sc_simcontext::reset()
{
    clean();
    init();
}

void
sc_simcontext::end()
{
    m_ready_to_simulate = false;
    m_port_registry->simulation_done();
    m_export_registry->simulation_done();
    m_prim_channel_registry->simulation_done();
    m_module_registry->simulation_done();
    m_end_of_simulation_called = true;
}

void
sc_simcontext::hierarchy_push( sc_module* mod )
{
    m_object_manager->hierarchy_push( mod );
}

sc_module*
sc_simcontext::hierarchy_pop()
{
	return DCAST<sc_module*>( m_object_manager->hierarchy_pop() );
}

sc_module*
sc_simcontext::hierarchy_curr() const
{
    return DCAST<sc_module*>( m_object_manager->hierarchy_curr() );
}
    
sc_object*
sc_simcontext::first_object()
{
    return m_object_manager->first_object();
}

sc_object*
sc_simcontext::next_object()
{
    return m_object_manager->next_object();
}

sc_object*
sc_simcontext::find_object( const char* name )
{
    static bool warn_find_object=true;
    if ( warn_find_object )
    {
	warn_find_object = false;
	SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
	    "sc_simcontext::find_object() is deprecated,\n" \
            " use sc_find_object()" );
    }
    return m_object_manager->find_object( name );
}

// to generate unique names for objects in an MT-Safe way

const char*
sc_simcontext::gen_unique_name( const char* basename_, bool preserve_first )
{
    return m_name_gen->gen_unique_name( basename_, preserve_first );
}


sc_process_handle 
sc_simcontext::create_cthread_process( 
    const char* name_p, bool free_host, SC_ENTRY_FUNC method_p,         
    sc_process_host* host_p, const sc_spawn_options* opt_p )
{
    sc_cthread_handle handle = 
        new sc_cthread_process(name_p, free_host, method_p, host_p, opt_p);
    if ( m_ready_to_simulate ) 
    {
	handle->prepare_for_simulation();
    } else {
	m_process_table->push_front( handle );
    }
    return sc_process_handle(handle);
}


sc_process_handle 
sc_simcontext::create_method_process( 
    const char* name_p, bool free_host, SC_ENTRY_FUNC method_p,         
    sc_process_host* host_p, const sc_spawn_options* opt_p )
{
    sc_method_handle handle = 
        new sc_method_process(name_p, free_host, method_p, host_p, opt_p);
    if ( m_ready_to_simulate ) {
	if ( !handle->dont_initialize() ) {
	    push_runnable_method( handle );
	}
    } else {
	m_process_table->push_front( handle );
    }
    return sc_process_handle(handle);
}


sc_process_handle 
sc_simcontext::create_thread_process( 
    const char* name_p, bool free_host, SC_ENTRY_FUNC method_p,         
    sc_process_host* host_p, const sc_spawn_options* opt_p )
{
    sc_thread_handle handle = 
        new sc_thread_process(name_p, free_host, method_p, host_p, opt_p);
    if ( m_ready_to_simulate ) {
	handle->prepare_for_simulation();
	if ( !handle->dont_initialize() ) {
	    push_runnable_thread( handle );
	}
    } else {
	m_process_table->push_front( handle );
    }
    return sc_process_handle(handle);
}

void
sc_simcontext::add_trace_file( sc_trace_file* tf )
{
    m_trace_files.push_back( tf );
    m_something_to_trace = true;
}


sc_cor*
sc_simcontext::next_cor()
{
    if( m_error ) {
	return m_cor;
    }
    
    sc_thread_handle thread_h = pop_runnable_thread();
    while( thread_h != 0 ) {
        if ( thread_h->ready_to_run() ) break;
	thread_h = pop_runnable_thread();
    }
    
    if( thread_h != 0 ) {
	return thread_h->m_cor_p;
    } else {
	return m_cor;
    }
}


const ::std::vector<sc_object*>&
sc_simcontext::get_child_objects() const
{
    static bool warn_get_child_objects=true;
    if ( warn_get_child_objects )
    {
	warn_get_child_objects = false;
	SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
	    "sc_simcontext::get_child_objects() is deprecated,\n" \
            " use sc_get_top_level_objects()" );
    }
    return m_child_objects;
}

void
sc_simcontext::add_child_object( sc_object* object_ )
{
    // no check if object_ is already in the set
    m_child_objects.push_back( object_ );
}

void
sc_simcontext::remove_child_object( sc_object* object_ )
{
    int size = m_child_objects.size();
    for( int i = 0; i < size; ++ i ) {
	if( object_ == m_child_objects[i] ) {
	    m_child_objects[i] = m_child_objects[size - 1];
	    m_child_objects.resize(size-1);
	    return;
	}
    }
    // no check if object_ is really in the set
}

sc_dt::uint64
sc_simcontext::delta_count() const
{
    static bool warn_delta_count=true;
    if ( warn_delta_count )
    {
	warn_delta_count = false;
	SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
	    "sc_simcontext::delta_count() is deprecated, use sc_delta_count()" );
    }
    return m_delta_count;
}

bool
sc_simcontext::is_running() const
{
    static bool warn_is_running=true;
    if ( warn_is_running )
    {
	warn_is_running = false;
	SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
	    "sc_simcontext::is_running() is deprecated, use sc_is_running()" );
    }
    return m_ready_to_simulate;
}

const sc_time
sc_simcontext::next_time()
{
    while( m_timed_events->size() ) {
	sc_event_timed* et = m_timed_events->top();
	if( et->event() != 0 ) {
	    return et->notify_time();
	}
	delete m_timed_events->extract_top();
    }
    return SC_ZERO_TIME;
}


void
sc_simcontext::remove_delta_event( sc_event* e )
{
    int i = e->m_delta_event_index;
    int j = m_delta_events.size() - 1;
    assert( i >= 0 && i <= j );
    if( i != j ) {
	sc_event** l_delta_events = &m_delta_events[0];
	l_delta_events[i] = l_delta_events[j];
	l_delta_events[i]->m_delta_event_index = i;
    }
    m_delta_events.resize(m_delta_events.size()-1);
    e->m_delta_event_index = -1;
}


void
sc_simcontext::trace_cycle( bool delta_cycle )
{
    int size;
    if( ( size = m_trace_files.size() ) != 0 ) {
	sc_trace_file** l_trace_files = &m_trace_files[0];
	int i = size - 1;
	do {
	    l_trace_files[i]->cycle( delta_cycle );
	} while( -- i >= 0 );
    }
}

// ----------------------------------------------------------------------------

#if 1
#ifdef PURIFY
	static sc_simcontext sc_default_global_context;
	sc_simcontext* sc_curr_simcontext = &sc_default_global_context;
#else
	sc_simcontext* sc_curr_simcontext = 0;
	sc_simcontext* sc_default_global_context = 0;
#endif
#else
// Not MT-safe!
static sc_simcontext* sc_curr_simcontext = 0;


sc_simcontext*
sc_get_curr_simcontext()
{
    if( sc_curr_simcontext == 0 ) {
#ifdef PURIFY
        static sc_simcontext sc_default_global_context;
        sc_curr_simcontext = &sc_default_global_context;
#else
        static sc_simcontext* sc_default_global_context = new sc_simcontext;
        sc_curr_simcontext = sc_default_global_context;
#endif
    }
    return sc_curr_simcontext;
}
#endif // 0

// Generates unique names within each module.

const char*
sc_gen_unique_name( const char* basename_, bool preserve_first )
{
    sc_simcontext* simc = sc_get_curr_simcontext();
    sc_module* curr_module = simc->hierarchy_curr();
    if( curr_module != 0 ) {
	return curr_module->gen_unique_name( basename_, preserve_first );
    } else {
        sc_process_b* curr_proc_p = sc_get_current_process_b();
	if ( curr_proc_p )
	{
	    return curr_proc_p->gen_unique_name( basename_, preserve_first );
	}
	else
	{
	    return simc->gen_unique_name( basename_, preserve_first );
	}
    }
}

// Get a handle for the current process
//
// Note that this method should not be called if the current process is
// in the act of being deleted, it will mess up the reference count management
// of sc_process_b instance the handle represents. Instead, use the a 
// pointer to the raw sc_process_b instance, which may be acquired via
// sc_get_current_process_b().

sc_process_handle
sc_get_current_process_handle()
{
    return ( sc_is_running() ) ?
	sc_process_handle(sc_get_current_process_b()) : 
	sc_get_last_created_process_handle();
}

// THE FOLLOWING FUNCTION IS DEPRECATED IN 2.1
sc_process_b*
sc_get_curr_process_handle()
{
    static bool warn=true;
    if ( warn )
    {
        warn = false;
        SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
       "sc_get_curr_process_handle deprecated use sc_get_current_process_handle"
       );
    }

    return sc_get_curr_simcontext()->get_curr_proc_info()->process_handle;
}

// Return indication if there are more processes to execute in this delta phase

bool
sc_pending_activity_at_current_time()
{
    sc_simcontext* c_p = sc_get_curr_simcontext();
    return (c_p->m_delta_events.size() != 0) ||
    	    !c_p->m_runnable->is_empty() ||
	    c_p->m_prim_channel_registry->pending_updates();
}

// Set the random seed for controlled randomization -- not yet implemented

void
sc_set_random_seed( unsigned int )
{
    SC_REPORT_WARNING( SC_ID_NOT_IMPLEMENTED_,
		       "void sc_set_random_seed( unsigned int )" );
}


void
sc_start( const sc_time& duration )
{
    sc_simcontext* context;
    int status;

    context = sc_get_curr_simcontext();
    status = context->sim_status();
    if( status != SC_SIM_OK ) 
    {
	if ( status == SC_SIM_USER_STOP )
	{
	    SC_REPORT_ERROR(SC_ID_SIMULATION_START_AFTER_STOP_, "");        
	}
        return;
    }
    context->simulate( duration );
}

void
sc_start()  
{
	sc_start( sc_time(~sc_dt::UINT64_ZERO, false) - sc_time_stamp() );
}

// for backward compatibility with 1.0
void
sc_start( double duration )  // in default time units
{
    static bool warn_sc_start=true;
    if ( warn_sc_start )
    {
	warn_sc_start = false;
	SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
	    "sc_start(double) deprecated, use sc_start(sc_time) or sc_start()");
    }

    if( duration == -1 )  // simulate forever
    {
        sc_start( 
            sc_time(~sc_dt::UINT64_ZERO, false) - sc_time_stamp() );
    }
    else
    {
        sc_start( sc_time( duration, true ) );
    }
}

void
sc_stop()
{
    sc_get_curr_simcontext()->stop();
}


// The following function is deprecated in favor of sc_start(SC_ZERO_TIME):

void
sc_initialize()
{
    static bool warning_initialize = true;

    if ( warning_initialize )
    {
        warning_initialize = false;
        SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
	    "sc_initialize() is deprecated: use sc_start(SC_ZERO_TIME)" );
    }
    sc_get_curr_simcontext()->initialize();
}

// The following function has been deprecated in favor of sc_start(duration):

void
sc_cycle( const sc_time& duration )
{
    static bool warning_cycle = true;

    if ( warning_cycle )
    {
        warning_cycle = false;
        SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
	    "sc_cycle is deprecated: use sc_start(sc_time)" );
    }
    sc_get_curr_simcontext()->cycle( duration );
}

sc_object* sc_find_object( const char* name, sc_simcontext* simc_p )
{
    return simc_p->get_object_manager()->find_object( name );
}


const sc_time&
sc_time_stamp()
{
    return sc_get_curr_simcontext()->time_stamp();
}

double
sc_simulation_time()
{
    static bool warn_simulation_time=true;
    if ( warn_simulation_time )
    {
        warn_simulation_time=false;
        SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
	    "sc_simulation_time() is deprecated use sc_time_stamp()" );
    }
    return sc_get_curr_simcontext()->time_stamp().to_default_time_units();
}

void
sc_defunct_process_function( sc_module* )
{
    // This function is pointed to by defunct sc_thread_process'es and
    // sc_cthread_process'es. In a correctly constructed world, this
    // function should never be called; hence the assert.
    assert( false );
}

//------------------------------------------------------------------------------
//"sc_set_stop_mode"
//
// This function sets the mode of operation when sc_stop() is called.
//     mode = SC_STOP_IMMEDIATE or SC_STOP_FINISH_DELTA.
//------------------------------------------------------------------------------
void sc_set_stop_mode(sc_stop_mode mode)
{
    if ( sc_is_running() )
    {
        SC_REPORT_WARNING(SC_ID_STOP_MODE_AFTER_START_,"");
    }
    else
    {
        switch( mode )
        {
          case SC_STOP_IMMEDIATE:
          case SC_STOP_FINISH_DELTA:
              stop_mode = mode;
              break;
          default:
              break;
        }
    }
}

sc_stop_mode
sc_get_stop_mode()
{
    return stop_mode;
}

} // namespace sc_core
// Taf!
