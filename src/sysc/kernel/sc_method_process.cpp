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

  sc_method_process.cpp -- Method process implementation

  Original Author: Andy Goodrich, Forte Design Systems, 4 August 2005
               

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_method_process.cpp,v $
// Revision 1.3  2009/05/22 16:06:29  acg
//  Andy Goodrich: process control updates.
//
// Revision 1.2  2008/05/22 17:06:25  acg
//  Andy Goodrich: updated copyright notice to include 2008.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.7  2006/04/20 17:08:16  acg
//  Andy Goodrich: 3.0 style process changes.
//
// Revision 1.6  2006/04/11 23:13:20  acg
//   Andy Goodrich: Changes for reduced reset support that only includes
//   sc_cthread, but has preliminary hooks for expanding to method and thread
//   processes also.
//
// Revision 1.5  2006/01/26 21:04:54  acg
//  Andy Goodrich: deprecation message changes and additional messages.
//
// Revision 1.4  2006/01/24 20:49:05  acg
// Andy Goodrich: changes to remove the use of deprecated features within the
// simulator, and to issue warning messages when deprecated features are used.
//
// Revision 1.3  2006/01/13 18:44:29  acg
// Added $Log to record CVS changes into the source.
//

#include "sysc/kernel/sc_method_process.h"
#include "sysc/kernel/sc_simcontext_int.h"
#include "sysc/kernel/sc_module.h"

namespace sc_core {

//------------------------------------------------------------------------------
//"sc_method_process::clear_trigger"
//
// This method clears any pending trigger for this object instance.
//------------------------------------------------------------------------------
void sc_method_process::clear_trigger()
{
    switch( m_trigger_type ) {
      case STATIC: 
        return;
      case EVENT: 
        m_event_p->remove_dynamic( this );
        m_event_p = 0;
        break;
      case OR_LIST: 
        m_event_list_p->remove_dynamic( this, 0 );
        m_event_list_p->auto_delete();
        m_event_list_p = 0;
        break;
      case AND_LIST: 
        m_event_list_p->remove_dynamic( this, 0 );
        m_event_list_p->auto_delete();
        m_event_list_p = 0;
        m_event_count = 0;
        break;
      case TIMEOUT: 
        m_timeout_event_p->cancel();
        m_timeout_event_p->reset();
        break;
      case EVENT_TIMEOUT: 
        m_timeout_event_p->cancel();
        m_timeout_event_p->reset();
        m_event_p->remove_dynamic( this );
        m_event_p = 0;
        break;
      case OR_LIST_TIMEOUT: 
        m_timeout_event_p->cancel();
        m_timeout_event_p->reset();
        m_event_list_p->remove_dynamic( this, 0 );
        m_event_list_p->auto_delete();
        m_event_list_p = 0;
      break;
      case AND_LIST_TIMEOUT: 
        m_timeout_event_p->cancel();
        m_timeout_event_p->reset();
        m_event_list_p->remove_dynamic( this, 0 );
        m_event_list_p->auto_delete();
        m_event_list_p = 0;
        m_event_count = 0;
        break;
    }
    m_trigger_type = STATIC;
}

//------------------------------------------------------------------------------
//"sc_method_process::disable_process"
//
// This virtual method disables this process and its children if requested to.
//     descendants = indicator of whether this process' children should also
//                   be suspended
//------------------------------------------------------------------------------
void sc_method_process::disable_process(
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
            if ( child_p ) child_p->disable_process(descendants);
        }
    }

    // SUSPEND OUR OBJECT INSTANCE:

    switch( m_state )
    {
      case ps_normal:
        m_state = (next_runnable() == 0) ? ps_disabled : 
                                           ps_disabled_pending;
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
//"sc_method_process::enable_process"
//
// This method enables the execution of this process, and if requested, its
// descendants. If the process was suspended and has a resumption pending it 
// will be dispatched in the next delta cycle. Otherwise the state will be
// adjusted to indicate it is no longer suspended, but no immediate execution
// will occur.
//------------------------------------------------------------------------------
void sc_method_process::enable_process(
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
            if ( child_p ) child_p->enable_process(descendants);
        }
    }

    // RESUME OBJECT INSTANCE:

    switch( m_state )
    {
      case ps_disabled:
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
//"sc_method_process::kill_process"
//
// This method removes throws a kill for this object instance. It calls the
// sc_process_b::kill_process() method to perform low level clean up. 
//------------------------------------------------------------------------------
void sc_method_process::kill_process(sc_descendant_inclusion_info descendants)
{
    int                              child_i;    // Index of child accessing.
    int                              child_n;    // Number of children.
    sc_process_b*                    child_p;    // Child accessing.
    const ::std::vector<sc_object*>* children_p; // Vector of children.

    // IF NEEDED, PROPOGATE THE KILL REQUEST THROUGH OUR DESCENDANTS:

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

    // REMOVE OUR PROCESS FROM EVENTS, ETC., AND QUEUE IT SO IT CAN 
    // THROW ITS KILL.

    disconnect_process();
    if ( next_runnable() == 0 ) simcontext()->push_runnable_method( this );
    // @@@@#### should we do this? if ( next_runnable() != 0 ) simcontext()->remove_runnable_method( this );
    m_throw_type = THROW_KILL;
}

//------------------------------------------------------------------------------
//"sc_method_process::sc_method_process"
//
// This is the object instance constructor for this class.
//------------------------------------------------------------------------------
sc_method_process::sc_method_process( const char* name_p, 
    bool free_host, SC_ENTRY_FUNC method_p, 
    sc_process_host* host_p, const sc_spawn_options* opt_p 
):
    sc_process_b(
        name_p && name_p[0] ? name_p : sc_gen_unique_name("method_p"), 
        free_host, method_p, host_p, opt_p)
{

    // CHECK IF THIS IS AN sc_module-BASED PROCESS AND SIMUALTION HAS STARTED:

    if ( DCAST<sc_module*>(host_p) != 0 && sc_is_running() )
    {
        SC_REPORT_ERROR( SC_ID_MODULE_METHOD_AFTER_START_, "" );
    }

    // INITIALIZE VALUES:
    //
    // If there are spawn options use them.

    m_process_kind = SC_METHOD_PROC_;
    if (opt_p) {
        m_dont_init = opt_p->m_dont_initialize;

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
    }

    else
    {
        m_dont_init = false;
    }
}

//------------------------------------------------------------------------------
//"sc_method_process::sc_method_process"
//
// This is the object instance destructor for this class.
//------------------------------------------------------------------------------
sc_method_process::~sc_method_process()
{
}


//------------------------------------------------------------------------------
//"sc_method_process::suspend_process"
//
// This virtual method suspends this process and its children if requested to.
//     descendants = indicator of whether this process' children should also
//                   be suspended
//------------------------------------------------------------------------------
void sc_method_process::suspend_process(
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
    // (1) If we are on the runnable queue then we are also pending.
    // (2) If this is self-suspension we are also pending.

    switch( m_state )
    {
      case ps_normal:
        m_state = ( (next_runnable() != 0) ||
            ( sc_get_current_process_b() == DCAST<sc_process_b*>(this) ) ) ?
            ps_suspended_pending : ps_suspended;
        break;
      case ps_disabled:
        m_state = ps_disabled_suspended;
        break;
      default:
        break;
    }
}

//------------------------------------------------------------------------------
//"sc_method_process::resume_process"
//
// This method resumes the execution of this process, and if requested, its
// descendants. If the process was suspended and has a resumption pending it 
// will be dispatched in the next delta cycle. Otherwise the state will be
// adjusted to indicate it is no longer suspended, but no immediate execution
// will occur.
//------------------------------------------------------------------------------
void sc_method_process::resume_process(
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
	    if ( m_resume_event_p == 0 ) 
	    {
	        m_resume_event_p = new sc_event;
	        add_static_event( *m_resume_event_p );
	    }
	    m_resume_event_p->notify(SC_ZERO_TIME);
        break;
      case ps_disabled_suspended:
        m_state = ps_disabled;
        break;
      default:
        break;
    }
}


//------------------------------------------------------------------------------
//"sc_method_process::throw_reset"
//
// This virtual method is invoked to "throw" a reset. If the reset is 
// asynchronous the method will cancel any dynamic waits and queue this 
// object instance to be executed. If the reset is synchronous this is a no-op.
//------------------------------------------------------------------------------
void sc_method_process::throw_reset( bool async )
{
    if ( async )
    {
        if ( m_event_p ) 
            m_event_p->remove_dynamic( this );
        if ( m_event_list_p ) 
            m_event_list_p->remove_dynamic( this, 0 );
        if ( next_runnable() == 0 ) 
            simcontext()->push_runnable_method( this );
    }
}


//------------------------------------------------------------------------------
//"sc_method_process::throw_user"
//
// This virtual method is invoked when a user exception is to be thrown.
// If requested it will also throw the exception to the children of this 
// object instance. Since this is a method no throw will occur for this
// object instance. The children will be awakened from youngest child to
// eldest.
//     helper_p    -> object to use to throw the exception.
//     descendants =  indicator of whether this process' children should also
//                    be suspended
//------------------------------------------------------------------------------
void sc_method_process::throw_user( const sc_throw_it_helper& helper,
    sc_descendant_inclusion_info descendants )
{     
    int                              child_i;    // Index of child accessing.
    int                              child_n;    // Number of children.
    sc_process_b*                    child_p;    // Child accessing.
    const ::std::vector<sc_object*>* children_p; // Vector of children.

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
//"sc_method_process::trigger_dynamic"
//
// This method sets up a dynamic trigger on an event.
//------------------------------------------------------------------------------
bool
sc_method_process::trigger_dynamic( sc_event* e )
{
    if( is_runnable() ) {
        return false;
    }
    m_timed_out = false;
    switch( m_trigger_type ) {
      case EVENT: 
        m_event_p = 0;
        m_trigger_type = STATIC;
        return true;
      case OR_LIST: 
        m_event_list_p->remove_dynamic( this, e );
        m_event_list_p->auto_delete();
        m_event_list_p = 0;
        m_trigger_type = STATIC;
        return true;
      case AND_LIST:
        if( -- m_event_count == 0 ) {
            // no need to remove_dynamic
            m_event_list_p->auto_delete();
            m_event_list_p = 0;
            m_trigger_type = STATIC;
            return true;
        }
        return false;
      case TIMEOUT:
        m_trigger_type = STATIC;
        return true;
      case EVENT_TIMEOUT:
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
      case OR_LIST_TIMEOUT:
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
      case AND_LIST_TIMEOUT:
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
      case STATIC:
        // we should never get here
        assert( false );
    }
    return false;
}


} // namespace sc_core 
