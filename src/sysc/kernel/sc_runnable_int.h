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

  sc_runnable_int.h -- For inline definitions of some utility functions.
                       DO NOT EXPORT THIS INCLUDE FILE. Include this file
                       after "sc_process_int.h" so that we can get the base
                       class right.

  Original Author: Bishnupriya Bhattacharya , Cadence Design, 28th July, 2003

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
      Andy Goodrich, Forte Design Systems, 2 September 2003
      Changed queue heads to instances to eliminate the checks for null heads.

 *****************************************************************************/

// $Log: sc_runnable_int.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.4  2006/04/20 17:08:17  acg
//  Andy Goodrich: Changed loop end checks to use non-zero unique values
//                 rather than erroneous check for zero values.
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

#ifndef SC_RUNNABLE_INT_H
#define SC_RUNNABLE_INT_H


#include "sysc/kernel/sc_runnable.h"
#include "sysc/kernel/sc_method_process.h"
#include "sysc/kernel/sc_thread_process.h"

namespace sc_core {

// The values below are used to indicate when a queue is empty. A non-zero     
// non-legal pointer value is used for this so that a zero value in the
// m_execute_p field of an sc_process_b instance can be used to indicate      
// that is has not been queued for run. (If we did not use a non-zero
// queue empty indicator then a sc_process_b instance that was queued
// twice in a row might end up on the queue twice if it were the first
// one that was queued!)

#define SC_NO_METHODS ((sc_method_handle)0xdb)
#define SC_NO_THREADS ((sc_thread_handle)0xdb)


//------------------------------------------------------------------------------
//"sc_runnable::init"
//
// This method initializes this object instance. Note we allocate the queue
// heads if necessary. This is done here rather than in the constructor for
// this class to eliminate CTOR processing errors with gcc.
//------------------------------------------------------------------------------
inline void sc_runnable::init()
{
    m_methods_pop = SC_NO_METHODS;
    if ( !m_methods_push_head )
    {
        m_methods_push_head = 
            new sc_method_process((const char*)0, true, (SC_ENTRY_FUNC)0, 0, 0);
        m_methods_push_head->dont_initialize(true);
    }
    m_methods_push_tail = m_methods_push_head;
	m_methods_push_head->set_next_runnable(SC_NO_METHODS);

    m_threads_pop = SC_NO_THREADS;
    if ( !m_threads_push_head )
    {
        m_threads_push_head = 
            new sc_thread_process((const char*)0, true, (SC_ENTRY_FUNC)0, 0, 0);
        m_threads_push_head->dont_initialize(true);
    }
	m_threads_push_head->set_next_runnable(SC_NO_THREADS);
    m_threads_push_tail = m_threads_push_head;
}


//------------------------------------------------------------------------------
//"sc_runnable::is_empty"
//
// This method returns true if the push queue is empty, or false if not.
//------------------------------------------------------------------------------
inline bool sc_runnable::is_empty() const
{
    return m_methods_push_head->next_runnable() == SC_NO_METHODS && 
		m_threads_push_head->next_runnable() == SC_NO_THREADS;
}


//------------------------------------------------------------------------------
//"sc_runnable::push_back_method"
//
// This method pushes the supplied method process onto the back of the queue of 
// runnable method processes.
//     method_h -> method process to add to the queue.
//------------------------------------------------------------------------------
inline void sc_runnable::push_back_method( sc_method_handle method_h )
{
    // assert( method_h->next_runnable() == 0 ); // Can't queue twice.
    method_h->set_next_runnable(SC_NO_METHODS);
    m_methods_push_tail->set_next_runnable(method_h);
    m_methods_push_tail = method_h;
}


//------------------------------------------------------------------------------
//"sc_runnable::push_back_thread"
//
// This method pushes the supplied thread process onto the back of the queue of 
// runnable thread processes.
//     thread_h -> thread process to add to the queue.
//------------------------------------------------------------------------------
inline void sc_runnable::push_back_thread( sc_thread_handle thread_h )
{
    // assert( thread_h->next_runnable() == 0 ); // Can't queue twice.
    thread_h->set_next_runnable(SC_NO_THREADS);
    m_threads_push_tail->set_next_runnable(thread_h);
    m_threads_push_tail = thread_h;
}


//------------------------------------------------------------------------------
//"sc_runnable::push_front_method"
//
// This method pushes the supplied method process onto the front of the queue of
// runnable method processes. If the queue is empty the process is the tail 
// also.
//     method_h -> method process to add to the queue.
//------------------------------------------------------------------------------
inline void sc_runnable::push_front_method( sc_method_handle method_h )
{
    // assert( method_h->next_runnable() == 0 ); // Can't queue twice.
    method_h->set_next_runnable(m_methods_push_head->next_runnable());
    if ( m_methods_push_tail == m_methods_push_head ) // Empty queue.
    {
        m_methods_push_tail->set_next_runnable(method_h);
	m_methods_push_tail = method_h;
    }
    else                                               // Non-empty queue.
    {
	m_methods_push_head->set_next_runnable(method_h);
    }
}


//------------------------------------------------------------------------------
//"sc_runnable::push_front_thread"
//
// This method pushes the supplied thread process onto the front of the queue of
// runnable thread processes. If the queue is empty the process is the tail 
// also.
//     thread_h -> thread process to add to the queue.
//------------------------------------------------------------------------------
inline void sc_runnable::push_front_thread( sc_thread_handle thread_h )
{
    // assert( thread_h->next_runnable() == 0 ); // Can't queue twice.
    thread_h->set_next_runnable(m_threads_push_head->next_runnable());
    if ( m_threads_push_tail == m_threads_push_head ) // Empty queue.
    {
        m_threads_push_tail->set_next_runnable(thread_h);
	m_threads_push_tail = thread_h;
    }
    else                                               // Non-empty queue.
    {
	m_threads_push_head->set_next_runnable(thread_h);
    }
}


//------------------------------------------------------------------------------
//"sc_runnable::pop_method"
//
// This method pops the next method process to be executed, or returns a null 
// if no method processes are available for execution.
//------------------------------------------------------------------------------
inline sc_method_handle sc_runnable::pop_method()
{
    sc_method_handle result_p;

    result_p = m_methods_pop;
    if ( result_p != SC_NO_METHODS ) 
    {
        m_methods_pop = result_p->next_runnable();
        result_p->set_next_runnable(0);
    }
	else
	{
		result_p = 0;
	}
    return result_p;

}

//------------------------------------------------------------------------------
//"sc_runnable::pop_thread"
//
// This method pops the next thread process to be executed, or returns a null 
// if no thread processes are available for execution.
//------------------------------------------------------------------------------
inline sc_thread_handle sc_runnable::pop_thread()
{
    sc_thread_handle result_p;

    result_p = m_threads_pop;
    if ( result_p != SC_NO_THREADS ) 
    {
        m_threads_pop = result_p->next_runnable();
        result_p->set_next_runnable(0);
    }
	else
	{
		result_p = 0;
	}
    return result_p;
}


//------------------------------------------------------------------------------
//"sc_runnable::remove_method"
//
// This method removes the supplied method process from the push queue if it is
// present. Note we clear the method's next pointer so that it may be queued 
// again.
//     remove_p -> method process to remove from the run queue.
//------------------------------------------------------------------------------
inline void sc_runnable::remove_method( sc_method_handle remove_p )
{
    sc_method_handle now_p;     // Method now checking.
    sc_method_handle prior_p;   // Method prior to now_p.

    prior_p = m_methods_push_head;
    for ( now_p = m_methods_push_head; now_p!= SC_NO_METHODS; 
	    now_p = now_p->next_runnable() )
    {
        if ( remove_p == now_p )
        {
            prior_p->set_next_runnable( now_p->next_runnable() );
            if (now_p == m_methods_push_tail) {
                m_methods_push_tail = prior_p;
            }
            now_p->set_next_runnable(0);
            break;
        }
        prior_p = now_p;
    }
}


//------------------------------------------------------------------------------
//"sc_runnable::remove_thread"
//
// This method removes the supplied thread process from the push queue if it is
// present. Note we clear the thread's next pointer so that it may be queued 
// again.
//     remove_p -> thread process to remove from the run queue.
//------------------------------------------------------------------------------
inline void sc_runnable::remove_thread( sc_thread_handle remove_p )
{
    sc_thread_handle now_p;     // Thread now checking.
    sc_thread_handle prior_p;   // Thread prior to now_p.

    prior_p = m_threads_push_head;
    for ( now_p = m_threads_push_head; now_p != SC_NO_THREADS; 
	    now_p = now_p->next_runnable() )
    {
        if ( remove_p == now_p )
        {
            prior_p->set_next_runnable( now_p->next_runnable() );
            if (now_p == m_threads_push_tail) {
                m_threads_push_tail = prior_p;
            }
            now_p->set_next_runnable(0);
            break;
        }
        prior_p = now_p;
    }
}

//------------------------------------------------------------------------------
//"sc_runnable::sc_runnable"
//
// This is the object instance constructor for this class.
//------------------------------------------------------------------------------
inline sc_runnable::sc_runnable() 
{
    m_methods_pop = 0;
    m_methods_push_head = 0;
    m_methods_push_tail = 0;
    m_threads_pop = 0;
    m_threads_push_head = 0;
    m_threads_push_tail = 0;
}


//------------------------------------------------------------------------------
//"sc_runnable::~sc_runnable"
//
// This is the object instance destructor for this class.
//------------------------------------------------------------------------------
inline sc_runnable::~sc_runnable()
{
    delete m_methods_push_head;
    delete m_threads_push_head;
}


//------------------------------------------------------------------------------
//"sc_runnable::toggle"
//
// This method moves the push queue to the pop queue and zeros the push
// queue.
//------------------------------------------------------------------------------
inline void sc_runnable::toggle()
{
    m_methods_pop = m_methods_push_head->next_runnable();
    m_methods_push_head->set_next_runnable(SC_NO_METHODS);
    m_methods_push_tail = m_methods_push_head;
    m_threads_pop = m_threads_push_head->next_runnable();
    m_threads_push_head->set_next_runnable(SC_NO_THREADS);
    m_threads_push_tail = m_threads_push_head;
}

#undef SC_NO_METHODS
#undef SC_NO_THREADS

} // namespace sc_core

#endif // SC_RUNNABLE_INT_H

// Taf!
