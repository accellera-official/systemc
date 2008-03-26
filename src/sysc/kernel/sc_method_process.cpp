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
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
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
//"sc_method_process::kill_process"
//
// This method removes this object instance from use. It calls the
// sc_process_b::kill_process() method to perform low level clean up. Then
// it aborts this process if it is the active process.
//------------------------------------------------------------------------------
void sc_method_process::kill_process()
{
    // CLEAN UP THE LOW LEVEL STUFF ASSOCIATED WITH THIS DATA STRUCTURE:

    sc_process_b::kill_process();


    // REMOVE METHOD FROM RUN QUEUE:

    simcontext()->remove_runnable_method( this );

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
