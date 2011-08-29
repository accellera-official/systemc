/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2011 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_except.cpp -- kill/reset exception handling

  Original Author: Philipp A. Hartmann, OFFIS

 CHANGE LOG APPEARS AT THE END OF THE FILE
 *****************************************************************************/

#include "sysc/kernel/sc_cmnhdr.h"
//
#include "sysc/kernel/sc_except.h"
#include "sysc/kernel/sc_process.h"
//
#include "sysc/utils/sc_report.h"

namespace sc_core {

sc_unwind_exception::sc_unwind_exception( sc_process_b* proc_p, bool is_reset )
  : m_proc_p(proc_p), m_is_reset( is_reset )
{
  sc_assert( m_proc_p );
  m_proc_p->start_unwinding();
}

bool
sc_unwind_exception::active() const
{
  return m_proc_p && m_proc_p->is_unwinding();
}

void
sc_unwind_exception::clear() const
{
  sc_assert( m_proc_p );
  m_proc_p->clear_unwinding();
}

const char*
sc_unwind_exception::what() const throw()
{
  return ( m_is_reset ) ? "RESET" : "KILL";
}

sc_unwind_exception::~sc_unwind_exception() throw()
{
  if( active() ) {
      // can't throw an exception, since we're already throwing
      // -> abort instead
      SC_REPORT_FATAL( SC_ID_RETHROW_UNWINDING_, m_proc_p->name() );
  }
}

// handle and translate uncaught exceptions here
//
// These exceptions can either escape from sc_main() directly,
// indirectly from an SC_METHOD(), or are thrown from within
// an SC_(C)THREAD()
//
// returns a pointer to a dynamically allocated sc_report object,
// containing the caught message

sc_report*
sc_handle_exception()
{
    try {

        // re-throw exception here
        try { throw; }

        catch( sc_report & ) // to be on the safe side
        {
            throw; // continue
        }
        catch( sc_unwind_exception const & )
        {
            sc_assert( false && "Unhandled kill/reset, should never happen" );
        }
        catch( std::exception const & x )
        {
            SC_REPORT_ERROR( SC_ID_SIMULATION_UNCAUGHT_EXCEPTION_, x.what() );
        }
        catch( char const * x )
        {
            SC_REPORT_ERROR( SC_ID_SIMULATION_UNCAUGHT_EXCEPTION_, x );
        }
        catch( ... )
        {
            SC_REPORT_ERROR( SC_ID_SIMULATION_UNCAUGHT_EXCEPTION_,
                             "UNKNOWN EXCEPTION" );
        }
    }
    // everything is an sc_report now
    catch( sc_report & rpt )
    {
        sc_report* rpt_p = new sc_report;
        rpt_p->swap( rpt );
        return rpt_p;
    }
    return 0;
}

} // namespace sc_core

// $Log: sc_except.cpp,v $
// Revision 1.4  2011/08/26 20:46:09  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
// Revision 1.3  2011/05/09 04:07:48  acg
//  Philipp A. Hartmann:
//    (1) Restore hierarchy in all phase callbacks.
//    (2) Ensure calls to before_end_of_elaboration.
//
// Revision 1.2  2011/02/13 21:47:37  acg
//  Andy Goodrich: update copyright notice.
//
// Revision 1.1  2011/02/10 22:47:38  acg
//  Andy Goodrich: first check in of Philipp A. Hartmann's new exception
//  processing code.
//

// Taf!
