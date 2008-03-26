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
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
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
        catch( const sc_report& ex ) {
            std::cout << "\n" << ex.what() << std::endl;
            thread_h->simcontext()->set_error();
        }
        break;
    }

    // SCHEDULE THREAD PACKAGE FOR DESTRUCTION:
    //
    // If control reaches this point the process semantics have returned
    // so the process should die.

    thread_h->kill_process();
}


//------------------------------------------------------------------------------
//"sc_thread_process::kill_process"
//
// This method removes this object instance from use. It calls the
// sc_process_b::kill_process() method to perform low level clean up. Then
// it aborts this process if it is the active process.
//------------------------------------------------------------------------------
void sc_thread_process::kill_process()
{
    // SIGNAL ANY MONITORS WAITING FOR THIS THREAD TO EXIT:

    int mon_n = m_monitor_q.size();
    if ( mon_n )
    {   
        for ( int mon_i = 0; mon_i < mon_n; mon_i++ )
            m_monitor_q[mon_i]->signal( this, sc_process_monitor::spm_exit);
    }

    // CLEAN UP THE LOW LEVEL STUFF ASSOCIATED WITH THIS DATA STRUCTURE:

    sc_process_b::kill_process();


    // IF THIS IS THE ACTIVE PROCESS THEN ABORT IT AND SWITCH TO A NEW ONE:
    //
    // Note we do not use an sc_process_handle, by making a call to
    // sc_get_current_process_handle(), since we don't want to increment the 
    // usage count in case it is already zero. (We are being deleted, and this
    // call to kill_process() may have been the result of the usage count going
    // to zero.) So we get a raw sc_process_b pointer instead.
 
    sc_process_b* active_p = sc_get_current_process_b();
    sc_simcontext*    simc_p = simcontext();
    if ( active_p == (sc_process_b*)this )
    {
        simc_p->cor_pkg()->abort( simc_p->next_cor() );
    }

    // IF THIS WAS NOT THE ACTIVE PROCESS REMOVE IT FROM ANY RUN QUEUES:

    else
    {
        simc_p->remove_runnable_thread( this );
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
