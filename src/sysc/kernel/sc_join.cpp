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

  sc_join.cpp -- Join Process Synchronization Implementation

  Original Author: Andy Goodrich, Forte Design Systems, 5 May 2003

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <assert.h>
#include <cstdlib>
#include <cstddef>

#include "sysc/kernel/sc_process_int.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_simcontext_int.h"
#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_join.h"

namespace sc_core {

//------------------------------------------------------------------------------
//"sc_join::add_process - sc_process_base*"
//
// This method adds a process to this join object instance. This consists of
// incrementing the count of processes in the join process and adding this 
// object instance to the supplied thread's monitoring queue.
//     process_p -> thread to be monitored.
//------------------------------------------------------------------------------
void sc_join::add_process( sc_process_base* process_p )
{
    m_threads_n++;
    process_p->operator sc_thread_handle()->add_monitor( this );
}


//------------------------------------------------------------------------------
//"sc_join::add_process - sc_process_handle"
//
// This method adds a process to this join object instance. This consists of
// incrementing the count of processes in the join process and adding this 
// object instance to the supplied thread's monitoring queue.
//     process_h = handle for process to be monitored.
//------------------------------------------------------------------------------
void sc_join::add_process( const sc_process_handle& process_h )
{
    sc_thread_handle thread_p; // Thread within process_h.

    thread_p = process_h.m_target_p->operator sc_thread_handle();
    if ( thread_p )
    {
        m_threads_n++;
        thread_p->add_monitor( this );
    }
    else
    {
        SC_REPORT_ERROR( SC_ID_JOIN_ON_METHOD_HANDLE_, 0 ); 
    }
}


//------------------------------------------------------------------------------
//"sc_join::signal"
//
// This virtual method is called when a process being monitored by this object
// instance sends a signal. If the signal type is spm_exit and the count of 
// threads that we are waiting to terminate on goes to zero we fire our join 
// event.
//     thread_p -> thread that is signalling.
//     type     =  type of signal being sent.
//------------------------------------------------------------------------------
void sc_join::signal(sc_thread_handle thread_p, int type)
{
    switch ( type )
    {
      case sc_process_monitor::spm_exit:
        thread_p->remove_monitor(this);
        if ( --m_threads_n == 0 ) m_join_event.notify();
        break;
    }
}

} // namespace sc_core
