/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2011 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_thread_process.cpp -- Thread process implementation

  Original Author: Andy Goodrich, Forte Design Systems, 4 August 2005
               

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_thread_process.cpp,v $
// Revision 1.27  2011/02/19 08:30:53  acg
//  Andy Goodrich: Moved process queueing into trigger_static from
//  sc_event::notify.
//
// Revision 1.26  2011/02/18 20:27:14  acg
//  Andy Goodrich: Updated Copyrights.
//
// Revision 1.25  2011/02/17 19:54:33  acg
//  Andy Goodrich:
//    (1) Changed signature of trigger_dynamic() back to bool, and moved
//        run queue processing into trigger_dynamic.
//    (2) Simplified process control usage.
//
// Revision 1.24  2011/02/16 22:37:31  acg
//  Andy Goodrich: clean up to remove need for ps_disable_pending.
//
// Revision 1.23  2011/02/14 17:51:40  acg
//  Andy Goodrich: proper pushing an poppping of the module hierarchy for
//  start_of_simulation() and end_of_simulation.
//
// Revision 1.22  2011/02/13 23:09:58  acg
//  Andy Goodrich: only remove dynamic events for asynchronous resets.
//
// Revision 1.21  2011/02/13 21:47:38  acg
//  Andy Goodrich: update copyright notice.
//
// Revision 1.20  2011/02/13 21:37:13  acg
//  Andy Goodrich: removed temporary diagnostic. Also there is
//  remove_dynamic_events() call in reset code.
//
// Revision 1.19  2011/02/13 21:35:09  acg
//  Andy Goodrich: added error messages for throws before the simulator is
//  initialized.
//
// Revision 1.18  2011/02/11 13:25:24  acg
//  Andy Goodrich: Philipp A. Hartmann's changes:
//    (1) Removal of SC_CTHREAD method overloads.
//    (2) New exception processing code.
//
// Revision 1.17  2011/02/08 08:18:16  acg
//  Andy Goodrich: removed obsolete code.
//
// Revision 1.16  2011/02/07 19:17:20  acg
//  Andy Goodrich: changes for IEEE 1666 compatibility.
//
// Revision 1.15  2011/02/04 15:27:36  acg
//  Andy Goodrich: changes for suspend-resume semantics.
//
// Revision 1.14  2011/02/01 23:01:53  acg
//  Andy Goodrich: removed dead code.
//
// Revision 1.13  2011/02/01 21:16:36  acg
//  Andy Goodrich:
//  (1) New version of trigger_dynamic() to implement new return codes and
//      proper processing of events with new dynamic process rules.
//  (2) Recoding of kill_process(), throw_user() and reset support to
//      consolidate preemptive thread execution in sc_simcontext::preempt_with().
//
// Revision 1.12  2011/01/25 20:50:37  acg
//  Andy Goodrich: changes for IEEE 1666 2011.
//
// Revision 1.11  2011/01/20 16:52:20  acg
//  Andy Goodrich: changes for IEEE 1666 2011.
//
// Revision 1.10  2011/01/19 23:21:50  acg
//  Andy Goodrich: changes for IEEE 1666 2011
//
// Revision 1.9  2011/01/18 20:10:45  acg
//  Andy Goodrich: changes for IEEE1666_2011 semantics.
//
// Revision 1.8  2011/01/06 18:02:16  acg
//  Andy Goodrich: added check for disabled thread to trigger_dynamic().
//
// Revision 1.7  2010/11/20 17:10:57  acg
//  Andy Goodrich: reset processing changes for new IEEE 1666 standard.
//
// Revision 1.6  2010/07/22 20:02:33  acg
//  Andy Goodrich: bug fixes.
//
// Revision 1.5  2009/07/28 01:10:53  acg
//  Andy Goodrich: updates for 2.3 release candidate.
//
// Revision 1.4  2009/05/22 16:06:29  acg
//  Andy Goodrich: process control updates.
//
// Revision 1.3  2008/05/22 17:06:06  acg
//  Andy Goodrich: formatting and comments.
//
// Revision 1.2  2007/09/20 20:32:35  acg
//  Andy Goodrich: changes to the semantics of throw_it() to match the
//  specification. A call to throw_it() will immediately suspend the calling
//  thread until all the throwees have executed. At that point the calling
//  thread will be restarted before the execution of any other threads.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.8  2006/04/20 17:08:17  acg
//  Andy Goodrich: 3.0 style process changes.
//
// Revision 1.7  2006/04/11 23:13:21  acg
//   Andy Goodrich: Changes for reduced reset support that only includes
//   sc_cthread, but has preliminary hooks for expanding to method and thread
//   processes also.
//
// Revision 1.6  2006/03/21 00:00:34  acg
//   Andy Goodrich: changed name of sc_get_current_process_base() to be
//   sc_get_current_process_b() since its returning an sc_process_b instance.
//
// Revision 1.5  2006/01/26 21:04:55  acg
//  Andy Goodrich: deprecation message changes and additional messages.
//
// Revision 1.4  2006/01/24 20:49:05  acg
// Andy Goodrich: changes to remove the use of deprecated features within the
// simulator, and to issue warning messages when deprecated features are used.
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

#include "sysc/kernel/sc_thread_process.h"
#include "sysc/kernel/sc_process_handle.h"
#include "sysc/kernel/sc_simcontext_int.h"
#include "sysc/kernel/sc_module.h"

namespace sc_core {

//------------------------------------------------------------------------------
//"sc_thread_cor_fn"
// 
// This function invokes the coroutine for the supplied object instance.
//------------------------------------------------------------------------------
void sc_thread_cor_fn( void* arg )
{
    sc_simcontext*   simc_p = sc_get_curr_simcontext();
    sc_thread_handle thread_h = RCAST<sc_thread_handle>( arg );

    // PROCESS THE THREAD AND PROCESS ANY EXCEPTIONS THAT ARE THROWN:

    while( true ) {

        try {
            thread_h->semantics();
        }
        catch( sc_user ) {
            continue;
        }
        catch( sc_halt ) {
            ::std::cout << "Terminating process "
                      << thread_h->name() << ::std::endl;
        }
        catch( const sc_unwind_exception& ex ) {
	    ex.clear();
            if ( ex.is_reset() ) continue;
        }
        catch( const sc_report& ex ) {
            std::cout << "\n" << ex.what() << std::endl;
            thread_h->simcontext()->set_error();
        }
        break;
    }

    sc_process_b*    active_p = sc_get_current_process_b();

    // REMOVE ALL TRACES OF OUR THREAD FROM THE SIMULATORS DATA STRUCTURES:

    thread_h->disconnect_process();

    // IF WE AREN'T ACTIVE MAKE SURE WE WON'T EXECUTE:

    if ( thread_h->next_runnable() != 0 )
    {
	simc_p->remove_runnable_thread(thread_h);
    }

    // IF WE ARE THE ACTIVE PROCESS ABORT OUR EXECUTION:


    if ( active_p == (sc_process_b*)thread_h )
    {
     
        sc_core::sc_cor* x = simc_p->next_cor();
	simc_p->cor_pkg()->abort( x );
    }

}


//------------------------------------------------------------------------------
//"sc_thread_process::disable_process"
//
// This virtual method suspends this process and its children if requested to.
//     descendants = indicator of whether this process' children should also
//                   be suspended
//------------------------------------------------------------------------------
void sc_thread_process::disable_process(
    sc_descendant_inclusion_info descendants )
{     
    int                              child_i;    // Index of child accessing.
    int                              child_n;    // Number of children.
    sc_process_b*                    child_p;    // Child accessing.
    const ::std::vector<sc_object*>* children_p; // Vector of children.

    // IF NEEDED PROPOGATE THE DISABLE REQUEST THROUGH OUR DESCENDANTS:

    if ( descendants == SC_INCLUDE_DESCENDANTS )
    {
        children_p = &get_child_objects();
        child_n = children_p->size();
        for ( child_i = 0; child_i < child_n; child_i++ )
        {
            child_p = DCAST<sc_process_b*>((*children_p)[child_i]);
            if ( child_p ) child_p->disable_process(descendants);
        }
    }

    // DISABLE OUR OBJECT INSTANCE:

    m_state = m_state | ps_bit_disabled; 
}

//------------------------------------------------------------------------------
//"sc_thread_process::enable_process"
//
// This method resumes the execution of this process, and if requested, its
// descendants. If the process was suspended and has a resumption pending it 
// will be dispatched in the next delta cycle. Otherwise the state will be
// adjusted to indicate it is no longer suspended, but no immediate execution
// will occur.
//------------------------------------------------------------------------------
void sc_thread_process::enable_process(
    sc_descendant_inclusion_info descendants )
{
    int                              child_i;    // Index of child accessing.
    int                              child_n;    // Number of children.
    sc_process_b*                    child_p;    // Child accessing.
    const ::std::vector<sc_object*>* children_p; // Vector of children.

    // IF NEEDED PROPOGATE THE ENABLE REQUEST THROUGH OUR DESCENDANTS:

    if ( descendants == SC_INCLUDE_DESCENDANTS )
    {
        children_p = &get_child_objects();
        child_n = children_p->size();
        for ( child_i = 0; child_i < child_n; child_i++ )
        {
            child_p = DCAST<sc_process_b*>((*children_p)[child_i]);
            if ( child_p ) child_p->enable_process(descendants);
        }
    }

    // ENABLE THIS OBJECT INSTANCE:
    //
    // If it was disabled and ready to run then put it on the run queue.

    m_state = m_state & ~ps_bit_disabled; 
    if ( m_state == ps_bit_ready_to_run )
    {
        m_state = ps_normal;
	if ( next_runnable() == 0 )
	    simcontext()->push_runnable_thread(this);
    }
}


//------------------------------------------------------------------------------
//"sc_thread_process::kill_process"
//
// This method removes this object instance from use. It calls the
// sc_process_b::kill_process() method to perform low level clean up. Then
// it aborts this process if it is the active process.
//------------------------------------------------------------------------------
void sc_thread_process::kill_process(sc_descendant_inclusion_info descendants )
{
    int                              child_i;    // index of child accessing.
    int                              child_n;    // number of children.
    sc_process_b*                    child_p;    // child accessing.
    const ::std::vector<sc_object*>* children_p; // vector of children.
    sc_simcontext*                   context_p;  // current simulation context.

    // IF THE SIMULATION HAS NOT BEEN INITIALIZED YET THAT IS AN ERROR:

    if ( sc_get_status() == SC_ELABORATION )
    {
        SC_REPORT_ERROR( SC_KILL_PROCESS_WHILE_UNITIALIZED_, "" );
    }

    context_p = simcontext();

    // IF NEEDED PROPOGATE THE KILL REQUEST THROUGH OUR DESCENDANTS:

    if ( descendants == SC_INCLUDE_DESCENDANTS )
    {
        children_p = &get_child_objects();
        child_n = children_p->size();
        for ( child_i = 0; child_i < child_n; child_i++ )
        {
            child_p = DCAST<sc_process_b*>((*children_p)[child_i]);
            if ( child_p ) child_p->kill_process(descendants);
        }
    }

    // SET UP TO KILL THE PROCESS IF SIMULATION HAS STARTED:

    if ( sc_is_running() )
    {
        m_throw_status = THROW_KILL;
        m_wait_cycle_n = 0;
        context_p->preempt_with(this);
    }

    // IF THE SIMULATION HAS NOT STARTED REMOVE TRACES OF OUR PROCESS FROM 
    // EVENT QUEUES, ETC.:

    else
    {
        disconnect_process();
    }
}

//------------------------------------------------------------------------------
//"sc_thread_process::prepare_for_simulation"
//
// This method prepares this object instance for simulation. It calls the
// coroutine package to create the actual thread.
//------------------------------------------------------------------------------
void sc_thread_process::prepare_for_simulation()
{
    m_cor_p = simcontext()->cor_pkg()->create( m_stack_size,
                         sc_thread_cor_fn, this );
    m_cor_p->stack_protect( true );
}


//------------------------------------------------------------------------------
//"sc_thread_process::resume_process"
//
// This method resumes the execution of this process, and if requested, its
// descendants. If the process was suspended and has a resumption pending it 
// will be dispatched in the next delta cycle. Otherwise the state will be
// adjusted to indicate it is no longer suspended, but no immediate execution
// will occur.
//------------------------------------------------------------------------------
void sc_thread_process::resume_process(
    sc_descendant_inclusion_info descendants )
{
    int                              child_i;    // Index of child accessing.
    int                              child_n;    // Number of children.
    sc_process_b*                    child_p;    // Child accessing.
    const ::std::vector<sc_object*>* children_p; // Vector of children.

    // IF NEEDED PROPOGATE THE RESUME REQUEST THROUGH OUR DESCENDANTS:

    if ( descendants == SC_INCLUDE_DESCENDANTS )
    {
        children_p = &get_child_objects();
        child_n = children_p->size();
        for ( child_i = 0; child_i < child_n; child_i++ )
        {
            child_p = DCAST<sc_process_b*>((*children_p)[child_i]);
            if ( child_p ) child_p->resume_process(descendants);
        }
    }

    // RESUME OBJECT INSTANCE:
    //
    // Even if it is disabled we cancel the suspension.

    m_state = m_state & ~ps_bit_suspended;
    if ( m_state == ps_bit_ready_to_run )
    {
	m_state = ps_normal;
	if ( next_runnable() == 0 )  
	    simcontext()->push_runnable_thread(this);
	remove_dynamic_events();  // order important.
    }
}

//------------------------------------------------------------------------------
//"sc_thread_process::sc_thread_process"
//
// This is the object instance constructor for this class.
//------------------------------------------------------------------------------
sc_thread_process::sc_thread_process( const char* name_p, bool free_host,
    SC_ENTRY_FUNC method_p, sc_process_host* host_p, 
    const sc_spawn_options* opt_p 
):
    sc_process_b(
        name_p && name_p[0] ? name_p : sc_gen_unique_name("thread_p"), 
        true, free_host, method_p, host_p, opt_p),
        m_cor_p(0), m_stack_size(SC_DEFAULT_STACK_SIZE), 
        m_wait_cycle_n(0)
{

    // CHECK IF THIS IS AN sc_module-BASED PROCESS AND SIMUALTION HAS STARTED:

    if ( DCAST<sc_module*>(host_p) != 0 && sc_is_running() )
    {
        SC_REPORT_ERROR( SC_ID_MODULE_THREAD_AFTER_START_, "" );
    }

    // INITIALIZE VALUES:
    //
    // If there are spawn options use them.

    m_process_kind = SC_THREAD_PROC_;

    if (opt_p) {
        m_dont_init = opt_p->m_dont_initialize;
        if ( opt_p->m_stack_size ) m_stack_size = opt_p->m_stack_size;

        // traverse event sensitivity list
        for (unsigned int i = 0; i < opt_p->m_sensitive_events.size(); i++) {
            sc_sensitive::make_static_sensitivity(
                this, *opt_p->m_sensitive_events[i]);
        }

        // traverse port base sensitivity list
        for ( unsigned int i = 0; i < opt_p->m_sensitive_port_bases.size(); i++)
        {
            sc_sensitive::make_static_sensitivity(
                this, *opt_p->m_sensitive_port_bases[i]);
        }

        // traverse interface sensitivity list
        for ( unsigned int i = 0; i < opt_p->m_sensitive_interfaces.size(); i++)
        {
            sc_sensitive::make_static_sensitivity(
                this, *opt_p->m_sensitive_interfaces[i]);
        }

        // traverse event finder sensitivity list
        for ( unsigned int i = 0; i < opt_p->m_sensitive_event_finders.size();
            i++)
        {
            sc_sensitive::make_static_sensitivity(
                this, *opt_p->m_sensitive_event_finders[i]);
        }

        // process any reset signal specification:

	opt_p->specify_resets();

    }

    else
    {
        m_dont_init = false;
    }

}

//------------------------------------------------------------------------------
//"sc_thread_process::~sc_thread_process"
//
// This is the object instance constructor for this class.
//------------------------------------------------------------------------------
sc_thread_process::~sc_thread_process()
{

    // DESTROY THE COROUTINE FOR THIS THREAD:

    if( m_cor_p != 0 ) {
        m_cor_p->stack_protect( false );
        delete m_cor_p;
        m_cor_p = 0;
    }
}


//------------------------------------------------------------------------------
//"sc_thread_process::signal_monitors"
//
// This methods signals the list of monitors for this object instance.
//------------------------------------------------------------------------------
void sc_thread_process::signal_monitors(int type)
{       
    int mon_n;  // # of monitors present.
        
    mon_n = m_monitor_q.size();
    for ( int mon_i = 0; mon_i < mon_n; mon_i++ )
        m_monitor_q[mon_i]->signal(this, type);
}   


//------------------------------------------------------------------------------
//"sc_thread_process::suspend_process"
//
// This virtual method suspends this process and its children if requested to.
//     descendants = indicator of whether this process' children should also
//                   be suspended
//------------------------------------------------------------------------------
void sc_thread_process::suspend_process(
    sc_descendant_inclusion_info descendants )
{     
    int                              child_i;    // Index of child accessing.
    int                              child_n;    // Number of children.
    sc_process_b*                    child_p;    // Child accessing.
    const ::std::vector<sc_object*>* children_p; // Vector of children.

    // IF NEEDED PROPOGATE THE SUSPEND REQUEST THROUGH OUR DESCENDANTS:

    if ( descendants == SC_INCLUDE_DESCENDANTS )
    {
        children_p = &get_child_objects();
        child_n = children_p->size();
        for ( child_i = 0; child_i < child_n; child_i++ )
        {
            child_p = DCAST<sc_process_b*>((*children_p)[child_i]);
            if ( child_p ) child_p->suspend_process(descendants);
        }
    }

    // SUSPEND OUR OBJECT INSTANCE:
    //
    // (1) If we are on the runnable queue then set suspended and ready_to_run,
    //     and remove ourselves from the run queue.
    // (2) If this is a self-suspension then a resume should cause immediate
    //     scheduling of the process, and we need to call suspend_me() here.

    m_state = m_state | ps_bit_suspended;
    if ( next_runnable() != 0 ) 
    {
	m_state = m_state | ps_bit_ready_to_run;
	simcontext()->remove_runnable_thread( this );
    }
    if ( sc_get_current_process_b() == DCAST<sc_process_b*>(this)  )
    {
	m_state = m_state | ps_bit_ready_to_run;
	suspend_me();
    }
}

//------------------------------------------------------------------------------
//"sc_thread_process::throw_reset"
//
// This virtual method is invoked when an reset is to be thrown. The
// method will cancel any dynamic waits. If the reset is asynchronous it will 
// queue this object instance to be executed. 
//------------------------------------------------------------------------------
void sc_thread_process::throw_reset( bool async )
{     
    // If the thread to be reset is dead ignore the call.

    if ( m_state & ps_bit_zombie ) return;


    // Set the throw type and clear any pending dynamic events: 

    m_throw_status = async ? THROW_ASYNC_RESET : THROW_SYNC_RESET;
    m_wait_cycle_n = 0;

    // If this is an asynchronous reset cancel any dynamic events and
    // execute it immediately:

    if ( async ) 
    {
        m_state = m_state & ~ps_bit_ready_to_run;
        remove_dynamic_events();
        simcontext()->preempt_with( this );
    }
}


//------------------------------------------------------------------------------
//"sc_thread_process::throw_user"
//
// This virtual method is invoked when a user exception is to be thrown.
// If requested it will also throw the exception to the children of this 
// object instance. The order of dispatch for the processes that are 
// thrown the exception is from youngest child to oldest child and then
// this process instance. This means that this instance will be pushed onto
// the front of the simulator's runnable queue and then the children will
// be processed recursively.
//     helper_p    =  helper object to use to throw the exception.
//     descendants =  indicator of whether this process' children should also
//                    be suspended
//------------------------------------------------------------------------------
void sc_thread_process::throw_user( const sc_throw_it_helper& helper,
    sc_descendant_inclusion_info descendants )
{     
    int                              child_i;    // Index of child accessing.
    int                              child_n;    // Number of children.
    sc_process_b*                    child_p;    // Child accessing.
    const ::std::vector<sc_object*>* children_p; // Vector of children.

    // IF THE SIMULATION HAS NOT BEEN INITIALIZED YET THAT IS AN ERROR:

    if ( sc_get_status() == SC_ELABORATION )
    {
        SC_REPORT_ERROR( SC_KILL_PROCESS_WHILE_UNITIALIZED_, "" );
    }


    // SET UP THE THROW REQUEST FOR THIS OBJECT INSTANCE AND QUEUE IT FOR
    // EXECUTION:

    if ( m_state & ps_bit_zombie ) return;

    m_throw_status = THROW_USER;
    if ( m_throw_helper_p != 0 ) delete m_throw_helper_p;
    m_throw_helper_p = helper.clone();
    simcontext()->preempt_with( this );

    // IF NEEDED PROPOGATE THE THROW REQUEST THROUGH OUR DESCENDANTS:

    if ( descendants == SC_INCLUDE_DESCENDANTS )
    {
        children_p = &get_child_objects();
        child_n = children_p->size();
        for ( child_i = 0; child_i < child_n; child_i++ )
        {
            child_p = DCAST<sc_process_b*>((*children_p)[child_i]);
            if ( child_p ) child_p->throw_user(helper, descendants);
        }
    }
}


//------------------------------------------------------------------------------
//"sc_thread_process::trigger_dynamic"
//
// This method returns the status of this object instance with respect to 
// the supplied event. There are 3 potential values to return:
//   dt_rearm      - don't execute the thread and don't remove it from the
//                   event's queue.
//   dt_remove     - the thread should not be scheduled for execution and
//                   the process should be moved from the event's thread queue.
//   dt_run        - the thread should be scheduled for execution but the
//                   the proces should stay on the event's thread queue.
//   dt_run_remove - the thread should be scheduled for execution and the
//                   process should be removed from the event's thread queue.
//
// Notes:
//   (1) This method is identical to sc_method_process::trigger_dynamic(), 
//       but they cannot be combined as sc_process_b::trigger_dynamic() 
//       because the signatures things like sc_event::remove_dynamic()
//       have different overloads for sc_thread_process* and sc_method_process*.
//       So if you change code here you'll also need to change it in 
//       sc_method_process.cpp.
// Result is true if this process should be removed from the event's list,
// false if not.
//------------------------------------------------------------------------------
bool sc_thread_process::trigger_dynamic( sc_event* e )
{
    // No time outs yet, and keep gcc happy.

    m_timed_out = false;

    // If this thread is already runnable can't trigger an event.

    if( is_runnable() ) 
    {
        return true;
    }

    // If a process is disabled then we ignore any events, leaving them enabled:
    //
    // But if this is a time out event we need to remove both it and the
    // event that was being waited for.

    if ( m_state & ps_bit_disabled )
    {
        if ( e == m_timeout_event_p )
	{
	    remove_dynamic_events( true );  
	    return true;
	}
	else
	{
	    return false;
	}
    }


    // Process based on the event type and current process state:
    //
    // Every case needs to set 'rc' and continue on to the end of
    // this method to allow suspend processing to work correctly.

    switch( m_trigger_type ) 
    {
      case EVENT: 
	m_event_p = 0;
	m_trigger_type = STATIC;
	break;

      case AND_LIST:
        -- m_event_count;
	if ( m_event_count == 0 )
	{
	    m_event_list_p->auto_delete();
	    m_event_list_p = 0;
	    m_trigger_type = STATIC;
	}
	else
	{
	    return true;
	}
	break;

      case OR_LIST:
	m_event_list_p->remove_dynamic( this, e );
	m_event_list_p->auto_delete();
	m_event_list_p = 0;
	m_trigger_type = STATIC;
	break;

      case TIMEOUT: 
	m_trigger_type = STATIC;
	break;

      case EVENT_TIMEOUT: 
        if ( e == m_timeout_event_p )
	{
	    m_timed_out = true;
	    m_event_p->remove_dynamic( this );
	    m_event_p = 0;
	    m_trigger_type = STATIC;
	}
	else
	{
	    m_timeout_event_p->cancel();
	    m_timeout_event_p->reset();
	    m_event_p = 0;
	    m_trigger_type = STATIC;
	}
	break;

      case OR_LIST_TIMEOUT:
        if ( e == m_timeout_event_p )
	{
            m_timed_out = true;
            m_event_list_p->remove_dynamic( this, e ); 
            m_event_list_p->auto_delete();
            m_event_list_p = 0; 
            m_trigger_type = STATIC;
	}

	else
	{
            m_timeout_event_p->cancel();
            m_timeout_event_p->reset();
	    m_event_list_p->remove_dynamic( this, e ); 
	    m_event_list_p->auto_delete();
	    m_event_list_p = 0; 
	    m_trigger_type = STATIC;
	}
	break;
      
      case AND_LIST_TIMEOUT:
        if ( e == m_timeout_event_p )
	{
            m_timed_out = true;
            m_event_list_p->remove_dynamic( this, e ); 
            m_event_list_p->auto_delete();
            m_event_list_p = 0; 
            m_trigger_type = STATIC;
	}

	else
	{
	    -- m_event_count;
	    if ( m_event_count == 0 )
	    {
		m_timeout_event_p->cancel();
		m_timeout_event_p->reset();
		// no need to remove_dynamic
		m_event_list_p->auto_delete();
		m_event_list_p = 0; 
		m_trigger_type = STATIC;
	    }
	    else
	    {
	        return true;
	    }
	}
	break;

      case STATIC: {
        // we should never get here
        assert( false );
      }
    }

    // If we get here then the thread is has satisfied its wait criteria, if 
    // its suspended mark its state as ready to run. If its not suspended then 
    // push it onto the runnable queue.

    if ( (m_state & ps_bit_suspended) )
    {
	m_state = m_state | ps_bit_ready_to_run;
    }
    else
    {
        simcontext()->push_runnable_thread(this);
    }

    return true;
}


//------------------------------------------------------------------------------
//"sc_set_stack_size"
//
//------------------------------------------------------------------------------
void
sc_set_stack_size( sc_thread_handle thread_h, std::size_t size )
{
    thread_h->set_stack_size( size );
}

} // namespace sc_core 
