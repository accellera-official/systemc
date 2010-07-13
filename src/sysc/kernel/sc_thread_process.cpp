/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
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
        catch( sc_kill ) {
            ::std::cout << "Killing process "
                      << thread_h->name() << ::std::endl;
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

	// simc_p->cor_pkg()->abort( simc_p->next_cor() );
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

    // SUSPEND OUR OBJECT INSTANCE:

    switch( m_state )
    {
      case ps_normal:
        m_state = ( next_runnable() == 0 ) ? ps_disabled : ps_disabled_pending;
        break;
      case ps_suspended:
      case ps_suspended_pending:
        m_state = ps_disabled_suspended;
        break;
      default:
        break;
    }
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

    // RESUME OBJECT INSTANCE:

    switch( m_state )
    {
      case ps_disabled:
      case ps_disabled_pending:
        m_state = ps_normal;
        break;
      case ps_disabled_suspended:
        m_state = ps_suspended;
        break;
      default:
        break;
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
    int                              child_i;    // Index of child accessing.
    int                              child_n;    // Number of children.
    sc_process_b*                    child_p;    // Child accessing.
    const ::std::vector<sc_object*>* children_p; // Vector of children.

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

    // REMOVE TRACES OF OUR PROCESS FROM EVENT QUEUES AND THE LIKE:

    disconnect_process();

    // SET UP TO KILL THE PROCESS IF SIMULATION HAS STARTED:
 
    if ( sc_is_running() )
    {
        m_throw_type = THROW_KILL;
        m_wait_cycle_n = 0;
        if ( (next_runnable() == 0) ) simcontext()->push_runnable_thread(this);
    }

    // REMOVE TRACES OF OUR PROCESS FROM EVENT QUEUES, ETC.:

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

    switch( m_state )
    {
      case ps_suspended:
        m_state = ps_normal;
        break;
      case ps_suspended_pending:
        m_state = ps_normal;
        if ( next_runnable() == 0 ) 
	{
	    if ( m_resume_event_p == 0 ) 
	    {
	        m_resume_event_p = new sc_event;
	        add_static_event( *m_resume_event_p );
	    }
	    m_resume_event_p->notify(SC_ZERO_TIME);
	}
        break;
      case ps_disabled_suspended:
        m_state = ps_disabled;
        break;
      default:
        break;
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
        free_host, method_p, host_p, opt_p),
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
        if ( opt_p->m_areset_iface_p )
        {
            sc_reset::reset_signal_is(
                true, *opt_p->m_areset_iface_p, opt_p->m_areset_level );
        }
        if ( opt_p->m_areset_port_p )
        {
            sc_reset::reset_signal_is(
                true, *opt_p->m_areset_port_p, opt_p->m_areset_level );
        }
        if ( opt_p->m_reset_iface_p )
        {
            sc_reset::reset_signal_is(
                false, *opt_p->m_reset_iface_p, opt_p->m_reset_level );
        }
        if ( opt_p->m_reset_port_p )
        {
            sc_reset::reset_signal_is(
                false, *opt_p->m_reset_port_p, opt_p->m_reset_level );
        }

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
    // (1) If we are on the runnable queue then set suspended and pending.
    // (2) If this is a self-suspension then a resume should cause immediate
    //     scheduling of the process.

    switch( m_state )
    {
      case ps_normal:
        m_state = (next_runnable() == 0) ? 
            ps_suspended : ps_suspended_pending;
        if ( sc_get_current_process_b() == DCAST<sc_process_b*>(this) )
        {
            m_state = ps_suspended_pending;
            suspend_me();
        }
        break;
      case ps_disabled:
        m_state = ps_disabled_suspended;
        break;
      default:
        break;
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
    sc_thread_handle active_h; // handle for active thread if its a thread.

    // IF THIS INSTANCE IS DEAD IGNORE THE REQUEST

    if ( m_state == ps_zombie ) return;


    // SET THE THROW TYPE AND CLEAR ANY PENDING DYNAMIC EVENTS:

    m_throw_type = THROW_RESET;
    m_wait_cycle_n = 0;
    remove_dynamic_events();

    // IF REQUESTED TO, IMMEDIATELY SCHEDULE THE RESET:

    if ( async && (next_runnable() == 0) )
            simcontext()->push_runnable_thread( this );

    // IF THE ACTIVE THREAD IS NOT THIS OBJECT INSTANCE WE NEED TO RESCHEDULE
    // IT AFTER WE PERFORM OUR RESET, AND SUSPEND IT SO WE DO GET OUR RESET:

    active_h = RCAST<sc_thread_handle>(sc_get_current_process_b());
    if ( this != active_h && active_h )
    {
	simcontext()->push_runnable_thread( active_h );
	suspend_me();
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


    // SET UP THE THROW REQUEST FOR THIS OBJECT INSTANCE AND QUEUE IT FOR
    // EXECUTION:

    if ( m_state == ps_zombie ) return;

    m_throw_type = THROW_USER;
    if ( m_throw_helper_p != 0 ) delete m_throw_helper_p;
    m_throw_helper_p = helper.clone();
    if ( next_runnable() == 0 ) simcontext()->push_runnable_thread_front(this);

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
//------------------------------------------------------------------------------
bool sc_thread_process::trigger_dynamic( sc_event* e )
{
    if( is_runnable() ) {
        return false;
    }
    m_timed_out = false;
    switch( m_trigger_type ) {
    case EVENT: {
        m_trigger_type = STATIC;
        m_event_p = 0;
        return true;
    }
    case OR_LIST: {
        m_event_list_p->remove_dynamic( this, e );
        m_event_list_p->auto_delete();
        m_event_list_p = 0;
        m_trigger_type = STATIC;
        return true;
    }
    case AND_LIST: {
        if( -- m_event_count == 0 ) {
            // no need to remove_dynamic
            m_event_list_p->auto_delete();
            m_event_list_p = 0;
            m_trigger_type = STATIC;
            return true;
        }
        return false;
    }
    case TIMEOUT: {
        m_trigger_type = STATIC;
        return true;
    }
    case EVENT_TIMEOUT: {
        if( e == m_event_p ) {
            m_timeout_event_p->cancel();
            m_timeout_event_p->reset();
        } else {
            m_timed_out = true;
            m_event_p->remove_dynamic( this );
        }
        m_event_p = 0;
        m_trigger_type = STATIC;
        return true;
    }
    case OR_LIST_TIMEOUT: {
        if( e != m_timeout_event_p ) {
            m_timeout_event_p->cancel();
            m_timeout_event_p->reset();
        } else {
            m_timed_out = true;
        }
        m_event_list_p->remove_dynamic( this, e );
        m_event_list_p->auto_delete();
        m_event_list_p = 0;
        m_trigger_type = STATIC;
        return true;
    }
    case AND_LIST_TIMEOUT: {
        if( e == m_timeout_event_p ) {
            m_timed_out = true;
            m_event_list_p->remove_dynamic( this, e );
            m_event_list_p->auto_delete();
            m_event_list_p = 0;
            m_trigger_type = STATIC;
            return true;
        } else if( -- m_event_count == 0 ) {
            m_timeout_event_p->cancel();
            m_timeout_event_p->reset();
            // no need to remove_dynamic
            m_event_list_p->auto_delete();
            m_event_list_p = 0;
            m_trigger_type = STATIC;
            return true;
        }
        return false;
    }
    case STATIC: {
        // we should never get here
        assert( false );
    }
    }
    return false;
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
