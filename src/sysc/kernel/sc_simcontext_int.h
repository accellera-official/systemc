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

  sc_simcontext_int.h -- For inline definitions of some utility functions.
                         DO NOT EXPORT THIS INCLUDE FILE. Include this file
                         after "sc_process_int.h" so that we can get the base
                         class right.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_simcontext_int.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.5  2006/01/19 00:29:52  acg
// Andy Goodrich: Yet another implementation for signal write checking. This
// one uses an environment variable SC_SIGNAL_WRITE_CHECK, that when set to
// DISABLE will disable write checking on signals.
//
// Revision 1.4  2006/01/18 21:42:37  acg
// Andy Goodrich: Changes for check writer support.
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

#ifndef SC_SIMCONTEXT_INT_H
#define SC_SIMCONTEXT_INT_H

#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_runnable.h"
#include "sysc/kernel/sc_runnable_int.h"

namespace sc_core {

// We use m_current_writer rather than m_curr_proc_info.process_handle to
// return the active process for sc_signal<T>::check_write since that lets
// us turn it off a library compile time, and only incur the overhead at
// the time of process switches rather than having to interrogate an
// additional switch every time a signal is written.

inline
void
sc_simcontext::set_curr_proc( sc_process_b* process_h )
{
    m_curr_proc_info.process_handle = process_h;
    m_curr_proc_info.kind           = process_h->proc_kind();
	m_current_writer = m_write_check ? process_h : (sc_object*)0;
}

inline
void
sc_simcontext::reset_curr_proc()
{
    m_curr_proc_info.process_handle = 0;
    m_curr_proc_info.kind           = SC_NO_PROC_;
    m_current_writer                = 0;
    sc_process_b::m_last_created_process_p = 0; 
}

inline
void
sc_simcontext::push_runnable_method( sc_method_handle method_h )
{
	m_runnable->push_back_method( method_h );
}

inline
void
sc_simcontext::push_runnable_method_front( sc_method_handle method_h )
{
    m_runnable->push_front_method( method_h );
}

inline
void
sc_simcontext::push_runnable_thread( sc_thread_handle thread_h )
{
    m_runnable->push_back_thread( thread_h );
}

inline
void
sc_simcontext::push_runnable_thread_front( sc_thread_handle thread_h )
{
    m_runnable->push_front_thread( thread_h );
}


inline
sc_method_handle
sc_simcontext::pop_runnable_method()
{
    sc_method_handle method_h = m_runnable->pop_method();
    if( method_h == 0 ) {
	reset_curr_proc();
	return 0;
    }
    set_curr_proc( (sc_process_b*)method_h );
    return method_h;
}

inline
sc_thread_handle
sc_simcontext::pop_runnable_thread()
{
    sc_thread_handle thread_h = m_runnable->pop_thread();
    if( thread_h == 0 ) {
	reset_curr_proc();
	return 0;
    }
    set_curr_proc( (sc_process_b*)thread_h );
    return thread_h;
}

inline
void
sc_simcontext::remove_runnable_method( sc_method_handle method_h )
{
    m_runnable->remove_method( method_h );
}

inline
void
sc_simcontext::remove_runnable_thread( sc_thread_handle thread_h )
{
    m_runnable->remove_thread( thread_h );
}



// ----------------------------------------------------------------------------

extern void sc_defunct_process_function( sc_module* );


} // namespace sc_core

#endif
