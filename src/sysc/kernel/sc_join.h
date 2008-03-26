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

  sc_join.h -- Join Process Synchronization Definition

  Original Author: Andy Goodrich, Forte Design Systems, 5 May 2003

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_JOIN_H
#define SC_JOIN_H

#include "sysc/kernel/sc_process_base.h"

namespace sc_core {

//==============================================================================
// CLASS sc_join
//
// This class provides a way of waiting for a set of threads to complete their
// execution. The threads whose completion is to be monitored are registered,
// and upon their completion an event notification will occur.
//==============================================================================
class sc_join : public sc_process_monitor {
    friend class sc_process_base;
  public:
    inline sc_join();
	virtual inline ~sc_join();
    void add_process( const sc_process_handle& process_h );
	inline int process_count();
    virtual void signal(sc_thread_handle thread_p, int type);
    inline void wait();
    inline void wait_clocked();

  protected: 
    void add_process( sc_process_base* process_p );

  protected:
    sc_event m_join_event;  // Event to notify when all threads have reported.
    int      m_threads_n;   // # of threads still need to wait for.
};

sc_join::sc_join() : m_threads_n(0) { }

sc_join::~sc_join() {}


int sc_join::process_count() { return m_threads_n; }

// suspend a thread that does not have a sensitivity list:

inline void sc_join::wait() { ::sc_core::wait(m_join_event); }

// suspend a thread that has a sensitivity list:

inline void sc_join::wait_clocked()
{
    do { ::sc_core::wait(); } while (m_threads_n != 0);
}

#define SC_CJOIN \
    }; \
    sc_join           join; \
	for ( unsigned int i = 0; i < sizeof(forkees)/sizeof(sc_thread_handle); \
		i++ ) \
		join.add_process(forkees[i]); \
	join.wait_clocked(); \
}

#define SC_FORK \
{ \
	sc_process_handle forkees[] = {

#define SC_JOIN \
    }; \
    sc_join           join; \
	for ( unsigned int i = 0; i < sizeof(forkees)/sizeof(sc_thread_handle); \
		i++ ) \
		join.add_process(forkees[i]); \
	join.wait(); \
}

} // namespace sc_core

#endif // SC_JOIN_H
