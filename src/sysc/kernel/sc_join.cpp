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

  sc_join.cpp -- Join Process Synchronization Implementation

  Original Author: Andy Goodrich, Forte Design Systems, 5 May 2003

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_join.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:44:29  acg
// Added $Log to record CVS changes into the source.
//

#include <cassert>
#include <cstdlib>
#include <cstddef>

#include "sysc/kernel/sc_process_handle.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_simcontext_int.h"
#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_thread_process.h"
#include "sysc/kernel/sc_join.h"

namespace sc_core {

//------------------------------------------------------------------------------
//"sc_join::add_process - sc_process_b*"
//
// This method adds a process to this join object instance. This consists of
// incrementing the count of processes in the join process and adding this 
// object instance to the supplied thread's monitoring queue.
//     process_p -> thread to be monitored.
//------------------------------------------------------------------------------
void sc_join::add_process( sc_process_b* process_p )
{
	sc_thread_handle handle = DCAST<sc_thread_handle>(process_p);
	assert( handle != 0 );
	m_threads_n++;
	handle->add_monitor( this );
}


//------------------------------------------------------------------------------
//"sc_join::add_process - sc_process_handle"
//
// This method adds a process to this join object instance. This consists of
// incrementing the count of processes in the join process and adding this 
// object instance to the supplied thread's monitoring queue.
//     process_h = handle for process to be monitored.
//------------------------------------------------------------------------------
void sc_join::add_process( sc_process_handle process_h )
{
	sc_thread_handle thread_p; // Thread within process_h.

	thread_p = process_h.operator sc_thread_handle();
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
