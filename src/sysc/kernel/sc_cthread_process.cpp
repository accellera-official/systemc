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

  sc_cthread_process.cpp -- Clocked thread implementation.

  Original Author: Andy Goodrich, Forte Design Systems, 4 August 2005
               

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_cthread_process.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.5  2006/04/11 23:13:20  acg
//   Andy Goodrich: Changes for reduced reset support that only includes
//   sc_cthread, but has preliminary hooks for expanding to method and thread
//   processes also.
//
// Revision 1.4  2006/01/24 20:49:04  acg
// Andy Goodrich: changes to remove the use of deprecated features within the
// simulator, and to issue warning messages when deprecated features are used.
//
// Revision 1.3  2006/01/13 18:44:29  acg
// Added $Log to record CVS changes into the source.
//

#include "sysc/kernel/sc_cthread_process.h"

namespace sc_core {

//------------------------------------------------------------------------------
//"sc_cthread_cor_fn"
// 
// This function invokes the coroutine for the supplied object instance.
//------------------------------------------------------------------------------
void sc_cthread_cor_fn( void* arg )
{
    sc_cthread_handle cthread_h = RCAST<sc_cthread_handle>( arg );

    // EXECUTE THE CTHREAD AND PROCESS ANY EXCEPTIONS THAT ARE THROWN:
    //
    // We set the wait state to unknown before invoking the semantics
    // in case we are reset, since the wait state will not be cleared, 
    // since that happens only if we are not reset.

    while( true ) {

        try {
            cthread_h->semantics();
        }
        catch( sc_user ) {
            continue;
        }
        catch( sc_halt ) {
            ::std::cout << "Terminating process "
                      << cthread_h->name() << ::std::endl;
        }
        catch( const sc_report& ex ) {
            ::std::cout << "\n" << ex.what() << ::std::endl;
            cthread_h->simcontext()->set_error();
        }

        break;
    }

    // SCHEDULE THREAD FOR DESTRUCTION:
    //
    // If control reaches this point the process semantics have returned
    // so the process should die.

    cthread_h->kill_process();

}

//------------------------------------------------------------------------------
//"sc_cthread_process::dont_initialize"
//
// This virtual method sets the initialization switch for this object instance.
//------------------------------------------------------------------------------
void sc_cthread_process::dont_initialize( bool dont )
{
    SC_REPORT_WARNING( SC_ID_DONT_INITIALIZE_, 0 );
}


//------------------------------------------------------------------------------
//"sc_cthread_process::prepare_for_simulation"
//
// This method prepares this object instance for simulation. It calls the
// coroutine package to create the actual thread.
//------------------------------------------------------------------------------
void sc_cthread_process::prepare_for_simulation()
{
    m_cor_p = simcontext()->cor_pkg()->create( m_stack_size,
                         sc_cthread_cor_fn, this );
    m_cor_p->stack_protect( true );
}


//------------------------------------------------------------------------------
//"sc_cthread_process::sc_cthread_process"
//
// This is the object instance constructor for this class.
//------------------------------------------------------------------------------
sc_cthread_process::sc_cthread_process( const char* name_p, 
    bool free_host, SC_ENTRY_FUNC method_p, 
    sc_process_host* host_p, const sc_spawn_options* opt_p 
):
    sc_thread_process(name_p, free_host, method_p, host_p, opt_p)
{
    m_dont_init = false;
    m_process_kind = SC_CTHREAD_PROC_;
}

//------------------------------------------------------------------------------
//"sc_cthread_process::~sc_cthread_process"
//
// This is the object instance constructor for this class.
//------------------------------------------------------------------------------
sc_cthread_process::~sc_cthread_process()
{
}

} // namespace sc_core 
