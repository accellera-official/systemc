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

  sc_process_base.cpp -- Base for dynamic process support implementation.

  Original Authors: Andy Goodrich, Forte Design Systems, 17 June 2003
            Stuart Swan, Cadence
                    Bishnupriya Bhattacharya, Cadence Design Systems,
                    25 August, 2003

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
#include "sysc/kernel/sc_process_int.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_simcontext_int.h"
#include "sysc/kernel/sc_sensitive.h"
#include "sysc/kernel/sc_process_base.h"
#include "sysc/kernel/sc_join.h"
#include "sysc/kernel/sc_kernel_ids.h"

namespace sc_core {

// QUEUE OF FREED DYNAMIC PROCESSES:

sc_process_base* sc_process_base::m_delete_q = 0;


//------------------------------------------------------------------------------
//"sc_process_base::sc_process_base"
//
// This is the object instance construtor for this class. It allocates a 
// thread with the supplied stack size and registers this object instance
// as a monitor of that thread.
//------------------------------------------------------------------------------
sc_process_base::sc_process_base( 
    const char* name_p, const sc_spawn_options* opt_p 
) : sc_process_host(0), m_delete_p(0)
{
    bool             dont_initialize=0; // True dont init process.
    sc_process_base* next_p;            // Next delete process to free.
    sc_process_base* now_p;             // Process to free now.
    bool             spawn_method=0;    // True if method not thread.
    int              stk_size = 0;      // Size of stack to allocate.
    sc_process_b*    spawned_proc = 0;  // Process as base object.

    if (opt_p) {
        spawn_method = opt_p->m_spawn_method;
        dont_initialize = opt_p->m_dont_initialize;
        stk_size = opt_p->m_stack_size;
    }

    if ( name_p && name_p[0] ) {
        m_name_p = strdup(name_p); 
    } else {
        if ( spawn_method )
        {
            m_name_p = strdup( sc_gen_unique_name("method_p") );
        }
        else
        {
            m_name_p = strdup( sc_gen_unique_name("thread_p") );
        }
    }

    if (spawn_method) {
        m_is_method = true;
        m_method_p = sc_create_dynamic_method( 
            m_name_p, this, &sc_process_base::semantics, dont_initialize);
        m_thread_p = 0;
        m_references_n = 1; // the method will never die and hence the
                            // sc_process_base will never die
        spawned_proc = (sc_process_b*)m_method_p;
    } else {
        m_is_method = false;
        m_method_p = 0;
        m_thread_p = sc_create_dynamic_thread( 
            m_name_p, this, &sc_process_base::semantics, 
            stk_size, dont_initialize);
        m_thread_p->add_monitor(this);
        m_references_n = 1;
        spawned_proc = (sc_process_b*)m_thread_p;
    }

    if (opt_p) {
        // traverse event sensitivity list
        for (int i = 0; i < opt_p->m_sensitive_events.size(); i++) {
            sc_sensitive::make_static_sensitivity(
            spawned_proc, *opt_p->m_sensitive_events[i]);
        }

        // traverse port base sensitivity list
        for (int i = 0; i < opt_p->m_sensitive_port_bases.size(); i++) {
            sc_sensitive::make_static_sensitivity(
            spawned_proc, *opt_p->m_sensitive_port_bases[i]);
        }

        // traverse interface sensitivity list
        for (int i = 0; i < opt_p->m_sensitive_interfaces.size(); i++) {
            sc_sensitive::make_static_sensitivity(
            spawned_proc, *opt_p->m_sensitive_interfaces[i]);
        }

        // traverse event finder sensitivity list
        for (int i = 0; i < opt_p->m_sensitive_event_finders.size(); i++) {
            sc_sensitive::make_static_sensitivity(
            spawned_proc, *opt_p->m_sensitive_event_finders[i]);
        }
    }

    // DELETE ANY PROCESSES WHICH HAVE BEEN QUEUED FOR DELETION:

    now_p = m_delete_q;
    if ( now_p )
    {
        m_delete_q = 0;
        for ( ; now_p; now_p = next_p )
        {
            next_p = now_p->m_delete_p;
            delete now_p;
        }
    }
}


//------------------------------------------------------------------------------
//"sc_process_base::~sc_process_base"
//
// This is the object instance destrutor for this class. 
// ISSUE: does thread clean up properly?
//------------------------------------------------------------------------------
sc_process_base::~sc_process_base()
{
    if ( m_name_p ) free(m_name_p);
}


//------------------------------------------------------------------------------
//"sc_process_base::signal"
//
// This virtual method provides a monitor callback from this object instance's
// thread. Specifically, we wait for the exit call back so we can queue
// this object instance for deletion when its reference count goes to zero.
//------------------------------------------------------------------------------
void sc_process_base::signal(sc_thread_handle thread_p, int type)
{
    switch (type)
    {
      case spm_exit:
        m_thread_p = 0;
        reference_decrement();
        break;
      default:
        break;
    }
}


//------------------------------------------------------------------------------
//"sc_process_base::wait"
//
// This method suspends the current thread until the thread for this
// object instance terminates. It uses the join mechanism to do this.
//------------------------------------------------------------------------------
void sc_process_base::wait()
{
    sc_join join;

    if (m_is_method) {
    SC_REPORT_ERROR( SC_ID_WAIT_ON_METHOD_HANDLE_, 0);
        return;
    }

    if ( m_thread_p != 0 )
    {
        join.add_process(this);
        join.wait();
    }
}

} // namespace sc_core
