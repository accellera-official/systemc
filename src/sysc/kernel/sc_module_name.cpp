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

  sc_module_name.cpp -- An object used to help manage object names 
                        and hierarchy

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include <cstdlib>

#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_module_name.h"
#include "sysc/kernel/sc_object_manager.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/utils/sc_iostream.h"

namespace sc_core {

sc_module_name::sc_module_name( const char* name_ )
: m_name( name_ ),
  m_module( 0 ),
  m_next( 0 ),
  m_simc( sc_get_curr_simcontext() ),
  m_pushed( true )
{
    m_simc->get_object_manager()->push_module_name( this );
}

sc_module_name::sc_module_name( const sc_module_name& name_ )
: m_name( name_.m_name ),
  m_module( 0 ),
  m_next( 0 ),
  m_simc( name_.m_simc ),
  m_pushed( false )
{}

sc_module_name::~sc_module_name()
{
    if( m_pushed ) {
        sc_module_name* smn = m_simc->get_object_manager()->pop_module_name();
        if( this != smn || 0 == m_module ) {
            SC_REPORT_ERROR( SC_ID_SC_MODULE_NAME_USE_, 0 );
        }
        m_module->end_module();
    }
}

sc_module_name::operator const char*() const
{
    return m_name;
}

} // namespace sc_core
