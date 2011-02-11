/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2010 by all Contributors.
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

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_except.cpp,v $
// Revision 1.1  2011/02/10 22:47:38  acg
//  Andy Goodrich: first check in of Philipp A. Hartmann's new exception
//  processing code.
//

#include "sysc/kernel/sc_cmnhdr.h"
//
#include "sysc/kernel/sc_except.h"
#include "sysc/kernel/sc_process.h"
//
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

} // namespace sc_core

// Taf!
