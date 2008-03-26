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

  sc_runnable.h --

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Andy Goodrich, 30 June 2003, Forte Design Systems
  Description of Modification: Total rewrite using linked list rather than 
                               fixed vector.


      Name, Affiliation, Date: Bishnupriya Bhattacharya, Cadence Design Systems,
                               25 August, 2003
  Description of Modification: Add tail pointers for m_methods_push and
                               m_threads_push to maintain the same scheduler
                               ordering as 2.0.1

 *****************************************************************************/

// $Log: sc_runnable.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

#ifndef SC_RUNNABLE_H
#define SC_RUNNABLE_H


#include "sysc/kernel/sc_process.h"

namespace sc_core {

//=============================================================================
//  CLASS : sc_runnable
//
//  Class that manages the ready-to-run queues.
//=============================================================================

class sc_runnable
{

  public:
    sc_runnable();
    ~sc_runnable();

    inline void init();
    inline void toggle();

    inline void remove_method( sc_method_handle );
    inline void remove_thread( sc_thread_handle );

    inline void push_back_method( sc_method_handle );
    inline void push_back_thread( sc_thread_handle );
    inline void push_front_method( sc_method_handle );
    inline void push_front_thread( sc_thread_handle );

    inline bool is_empty() const;

    inline sc_method_handle pop_method();
    inline sc_thread_handle pop_thread();

  private:
    sc_method_handle m_methods_push_head;
    sc_method_handle m_methods_push_tail;
    sc_method_handle m_methods_pop;
    sc_thread_handle m_threads_push_head;
    sc_thread_handle m_threads_push_tail;
    sc_thread_handle m_threads_pop;

  private:
    // disabled
    sc_runnable( const sc_runnable& );
    sc_runnable& operator = ( const sc_runnable& );
};

} // namespace sc_core

#endif

// Taf!
