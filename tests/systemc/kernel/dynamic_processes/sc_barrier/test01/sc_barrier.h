/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2003 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_barrier.h -- Barrier Process Synchronization Definition

  Original Author: Andy Goodrich, Forte Design Systems, 5 May 2003

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_barrier.h,v $
// Revision 1.1.1.1  2006/12/15 20:26:03  acg
// systemc_tests-2.3
//
// Revision 1.2  2006/01/24 21:04:52  acg
//  Andy Goodrich: replacement of deprecated features with their non-deprecated
//  counterparts.
//

#ifndef SC_BARRIER_H
#define SC_BARRIER_H

namespace sc_dp {

// -----------------------------------------------------------------------------
// CLASS sc_barrier
//
// This class provides a way of synchronising a set of processes. Each process
// calls the wait() method and one all processes have called that method they
// will all be released for execution.
// -----------------------------------------------------------------------------

class sc_barrier {
  public:
    void initialize( int thread_n )
    {
        m_thread_n = thread_n;
    }

    void wait()
    {
        m_thread_n--;
        if ( m_thread_n )
        {
            ::sc_core::wait(m_barrier_event);
        }
        else
        {
            m_barrier_event.notify(SC_ZERO_TIME);
            ::sc_core::wait(m_barrier_event);
        }
    }

  protected:
    sc_event m_barrier_event;   // Event to wait on.
    int      m_thread_n;        // # of threads left to wait.
};

} // namespace sc_dp

#endif // SC_BARRIER_H
