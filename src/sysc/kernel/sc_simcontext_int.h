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

#ifndef SC_SIMCONTEXT_INT_H
#define SC_SIMCONTEXT_INT_H

#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_runnable.h"
#include "sysc/kernel/sc_runnable_int.h"

namespace sc_core {

inline
void
sc_simcontext::set_curr_proc( sc_method_handle method_h )
{
    m_curr_proc_info.process_handle = method_h;
    m_curr_proc_info.kind           = SC_METHOD_PROC_;
}

inline void
sc_simcontext::set_curr_proc( sc_thread_handle thread_h )
{
    m_curr_proc_info.process_handle = thread_h;
    if( thread_h->is_cthread() ) {
	m_curr_proc_info.kind = SC_CTHREAD_PROC_;
    } else {
	m_curr_proc_info.kind = SC_THREAD_PROC_;
    }
}

inline
void
sc_simcontext::reset_curr_proc()
{
    m_curr_proc_info.process_handle = 0;
    m_curr_proc_info.kind           = SC_NO_PROC_;
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
    set_curr_proc( method_h );
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
    set_curr_proc( thread_h );
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

extern void watching_before_simulation( const sc_lambda_ptr&,
					sc_simcontext* );
extern void watching_during_simulation( const sc_lambda_ptr&,
					sc_simcontext* );

} // namespace sc_core

#endif
