/****************************************************************************

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


#include "sysc/kernel/sc_cor_fiber.h"
#include "sysc/kernel/sc_cor_pthread.h"
#include "sysc/kernel/sc_cor_qt.h"
#include "sysc/kernel/sc_event.h"
#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_lambda.h"
#include "sysc/kernel/sc_macros_int.h"
#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_module_registry.h"
#include "sysc/kernel/sc_name_gen.h"
#include "sysc/kernel/sc_object_manager.h"
#include "sysc/kernel/sc_process_int.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_simcontext_int.h"
#include "sysc/kernel/sc_ver.h"
#include "sysc/communication/sc_port.h"
#include "sysc/communication/sc_export.h"
#include "sysc/communication/sc_prim_channel.h"
#include "sysc/tracing/sc_trace.h"
#include "sysc/utils/sc_mempool.h"

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

    ::std::cout << ::std::endl 
         << "WATCH OUT!! In sc_process_table destructor. "
         << "Threads and cthreads are not actually getting deleted here. "
	 << "Some memory may leak. Look at the comments here in "
	 << "kernel/sc_simcontext.cpp for more details."
	 << ::std::endl;

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
    m_object_manager = new sc_object_manager;
    m_module_registry = new sc_module_registry( *this );
    m_port_registry = new sc_port_registry( *this );
    m_export_registry = new sc_export_registry( *this );
    m_prim_channel_registry = new sc_prim_channel_registry( *this );
    m_name_gen = new sc_name_gen;
    m_process_table = new sc_process_table;
    reset_curr_proc();
    m_next_proc_id = -1;
    m_timed_events = new sc_ppq<sc_event_timed*>( 128,
						  sc_notify_time_compare );
    m_something_to_trace = false;
    m_runnable = new sc_runnable;
    m_time_params = new sc_time_params;
    m_curr_time = SC_ZERO_TIME;
    m_delta_count = 0;
    m_forced_stop = false;
    m_ready_to_simulate = false;
    m_elaboration_done = false;
    m_update_phase = false;
    m_error = false;
    m_until_event = 0;
    m_cor_pkg = 0;
    m_cor = 0;
    m_watching_fn = watching_before_simulation;
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
    m_child_objects.erase_all();
    m_delta_events.erase_all();
    delete m_timed_events;
    for( int i = m_trace_files.size() - 1; i >= 0; -- i ) {
	delete m_trace_files[i];
    }
    m_trace_files.erase_all();
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
sc_simcontext::crunch()
{
#ifdef DEBUG_SYSTEMC
    int num_deltas = 0;  // number of delta cycles
#endif

    m_delta_count ++;

    while( true ) {

	// EVALUATE PHASE
	
	while( true ) {

	    // execute method processes

	    sc_method_handle method_h = pop_runnable_method();
	    while( method_h != 0 ) {
		try {
		    method_h->execute();
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
	    while( thread_h != 0 && ! thread_h->ready_to_run() ) {
		thread_h = pop_runnable_thread();
	    }
	    if( thread_h != 0 ) {
		m_cor_pkg->yield( thread_h->m_cor );
	    }
	    if( m_error ) {
		return;
	    }

	    // check for call(s) to sc_stop
	    if( m_forced_stop ) {
		if ( stop_mode == SC_STOP_IMMEDIATE ) break;
	    }

	    if( m_runnable->is_empty() ) {
		// no more runnable processes
		break;
	    }
	    m_runnable->toggle();
	}

	// UPDATE PHASE

	m_update_phase = true;
	m_prim_channel_registry->perform_update();
	m_update_phase = false;
	
	if( m_something_to_trace ) {
	    trace_cycle( /* delta cycle? */ true );
	}

	m_delta_count ++;

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

	// PROCESS DELTA NOTIFICATIONS

        int size;
	if( ( size = m_delta_events.size() ) == 0 && m_runnable->is_empty() ) {
	    break;
	}

        sc_event** l_events = m_delta_events.raw_data();
        int i = size - 1;
        do {
	    l_events[i]->trigger();
        } while( -- i >= 0 );
	m_delta_events.erase_all();
	
	if( m_runnable->is_empty() ) {
	    // no more runnable processes
	    break;
	}
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

    m_port_registry->elaboration_done();
    m_export_registry->elaboration_done();
    m_prim_channel_registry->elaboration_done();
    m_module_registry->elaboration_done();

    m_elaboration_done = true;

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

    m_watching_fn = watching_during_simulation;

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

    m_update_phase = true;
    m_prim_channel_registry->perform_update();
    m_update_phase = false;

    int size;

    // make all method processes runnable

    for ( method_p = m_process_table->method_q_head(); 
	  method_p; method_p = method_p->next_exist() )
    {
        if( method_p->do_initialize() ) {
            push_runnable_method_front( method_p );
        }
    }

    // make all thread processes runnable

    for ( thread_p = m_process_table->thread_q_head(); 
	  thread_p; thread_p = thread_p->next_exist() )
    {
        if( thread_p->do_initialize() ) {
            push_runnable_thread_front( thread_p );
        }
    }


    // process delta notifications

    if( ( size = m_delta_events.size() ) != 0 ) {
        sc_event** l_delta_events = m_delta_events.raw_data();
        int i = size - 1;
        do {
            l_delta_events[i]->trigger();
        } while( -- i >= 0 );
        m_delta_events.erase_all();
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
    m_in_simulator_control = true;

    sc_time until_t = m_curr_time + duration;

    m_until_event->cancel();  // to be on the safe side
    m_until_event->notify_delayed( duration );

    sc_time t;

    // IF DURATION WAS ZERO WE ONLY CRUNCH:
    //
    // We duplicate the code so that we don't add the overhead of the
    // check to each loop in the do below.
    if ( duration == SC_ZERO_TIME ) 
    {
        m_runnable->toggle();
  	crunch();
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
	m_curr_time = t;

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
    return m_object_manager->find_object( name );
}

// to generate unique names for objects in an MT-Safe way

const char*
sc_simcontext::gen_unique_name( const char* basename_, bool preserve_first )
{
    return m_name_gen->gen_unique_name( basename_, preserve_first );
}


sc_method_handle
sc_simcontext::register_method_process( const char* name,
				        SC_ENTRY_FUNC entry_fn,
				        sc_module* module )
{
    // SC_METHOD macro can only be called before simulation starts
    if (m_ready_to_simulate) {
        SC_REPORT_ERROR( SC_ID_MODULE_METHOD_AFTER_START_, "" );
	return 0;
    }
    sc_method_handle handle = new sc_method_process( name,
						     entry_fn,
						     module );
    m_process_table->push_front( handle );
    return handle;
}

sc_method_handle
sc_simcontext::create_dynamic_method_process( const char* name,
				              SC_ENTRY_FUNC entry_fn,
				              sc_process_host* host,
					      bool dont_initialize )
{
    sc_method_handle handle = new sc_method_process( name,
						     entry_fn,
						     host );
    // sc_spawn() can be called both before and after simulation starts
    if ( m_ready_to_simulate ) {
        if ( !dont_initialize ) {
	    push_runnable_method( handle );
        }
    } else {
        m_process_table->push_front( handle );
	if (dont_initialize) {
	    handle->do_initialize( false );
        }
    }
    return handle;
}

sc_thread_handle
sc_simcontext::register_thread_process( const char* name,
					SC_ENTRY_FUNC entry_fn,
					sc_module* module )
{
    // SC_THREAD macro can only be called before simulation starts
    if (m_ready_to_simulate) {
        SC_REPORT_ERROR( SC_ID_MODULE_THREAD_AFTER_START_, "" );
	return 0;
    }
    sc_thread_handle handle = new sc_thread_process( name,
						     entry_fn,
						     module );
    m_process_table->push_front( handle );
    return handle;
}

sc_thread_handle
sc_simcontext::create_dynamic_thread_process( const char* name,
					      SC_ENTRY_FUNC entry_fn,
					      sc_process_host* host ,
					      size_t stk_size,
					      bool dont_initialize )
{
    sc_thread_handle handle = new sc_thread_process( name,
						     entry_fn,
						      host );
    if (stk_size) {
	handle->set_stack_size(stk_size);
    }
    // sc_spawn() can be called both before and after simulation starts
    if (m_ready_to_simulate) {
	handle->prepare_for_simulation();
	if (!dont_initialize) {
	    push_runnable_thread(handle);
	}
    } else {
        m_process_table->push_front( handle );
	if (dont_initialize) {
	    handle->do_initialize( false );
        }
    }
    return handle;
}

sc_cthread_handle
sc_simcontext::register_cthread_process( const char* name,
					 SC_ENTRY_FUNC entry_fn,
					 sc_module* module )
{
    if (m_ready_to_simulate) {
        SC_REPORT_ERROR( SC_ID_MODULE_CTHREAD_AFTER_START_, "" );
	return 0;
    }
    sc_cthread_handle handle = new sc_cthread_process( name,
						       entry_fn,
						       module );
    m_process_table->push_front( handle );
    return handle;
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
    while( thread_h != 0 && ! thread_h->ready_to_run() ) {
	thread_h = pop_runnable_thread();
    }
    
    if( thread_h != 0 ) {
	return thread_h->m_cor;
    } else {
	return m_cor;
    }
}


const ::std::vector<sc_object*>&
sc_simcontext::get_child_objects() const
{
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
	    m_child_objects.decr_count();
	    return;
	}
    }
    // no check if object_ is really in the set
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
    int i = e->m_delta;
    int j = m_delta_events.size() - 1;
    assert( i >= 0 && i <= j );
    if( i != j ) {
	sc_event** l_delta_events = m_delta_events.raw_data();
	l_delta_events[i] = l_delta_events[j];
	l_delta_events[i]->m_delta = i;
    }
    m_delta_events.decr_count();
    e->m_delta = -1;
}


void
sc_simcontext::trace_cycle( bool delta_cycle )
{
    int size;
    if( ( size = m_trace_files.size() ) != 0 ) {
	sc_trace_file** l_trace_files = m_trace_files.raw_data();
	int i = size - 1;
	do {
	    l_trace_files[i]->cycle( delta_cycle );
	} while( -- i >= 0 );
    }
}

// ----------------------------------------------------------------------------

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

sc_process_b*
sc_get_last_created_process_handle()
{
    return sc_process_b::get_last_created_process();
}

// Generates unique names within each module.

const char*
sc_gen_unique_name( const char* basename_, bool preserve_first )
{
    sc_simcontext* simc = sc_get_curr_simcontext();
    sc_module* curr_module = simc->hierarchy_curr();
    if( curr_module != 0 ) {
	return curr_module->gen_unique_name( basename_, preserve_first );
    } else {
        sc_process_b* curr_proc_p = sc_get_curr_process_handle();
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

// for backward compatibility with 1.0
void
sc_start( double duration )  // in default time units
{
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


// The following function has been deprecated in favor of sc_start(0):

void
sc_initialize()
{
    static int warning_count = 0;

    if ( warning_count++ < 100 )
    {
	    SC_REPORT_WARNING( SC_ID_SC_INITIALIZE_DEPRECATED_, "" );
    }
    sc_get_curr_simcontext()->initialize();
}

// The following function has been deprecated in favor of sc_start(duration):

void
sc_cycle( const sc_time& duration )
{
    static int warning_count = 0;

    if ( warning_count++ < 100 )
    {
	    SC_REPORT_WARNING( SC_ID_SC_CYCLE_DEPRECATED_, "" );
    }
    sc_get_curr_simcontext()->cycle( duration );
}

sc_dt::uint64
sc_delta_count()
{
    return sc_get_curr_simcontext()->delta_count();
}

const sc_time&
sc_time_stamp()
{
    return sc_get_curr_simcontext()->time_stamp();
}

double
sc_simulation_time()
{
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
    if ( sc_get_curr_simcontext()->is_running() )
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
