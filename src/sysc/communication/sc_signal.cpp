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

  sc_signal.cpp -- The sc_signal<T> primitive channel class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

  CHANGE LOG IS AT THE END OF THE FILE
 *****************************************************************************/


#include "sysc/communication/sc_communication_ids.h"
#include "sysc/utils/sc_utils_ids.h"
#include "sysc/communication/sc_signal.h"
#include "sysc/datatypes/int/sc_signed.h"
#include "sysc/datatypes/int/sc_unsigned.h"
#include "sysc/datatypes/bit/sc_lv_base.h"
#include "sysc/kernel/sc_reset.h"

#include <sstream>

using sc_dt::sc_lv_base;
using sc_dt::sc_signed;
using sc_dt::sc_unsigned;
using sc_dt::int64;
using sc_dt::uint64;

namespace sc_core {

// to avoid code bloat in sc_signal<T>

void
sc_signal_invalid_writer( sc_object* target, sc_object* first_writer,
                          sc_object* second_writer, bool check_delta )
{
    if ( second_writer )
    {   
        std::stringstream msg;

        msg
            << "\n signal "
               "`" << target->name() << "' "
               "(" << target->kind() << ")"
            << "\n first driver "
               "`" << first_writer->name() << "' "
              " (" << first_writer->kind() << ")"
            << "\n second driver "
               "`" << second_writer->name() << "' "
               "(" << second_writer->kind() << ")";

        if( check_delta )
        {
            msg << "\n first conflicting write in delta cycle "
                << sc_delta_count();
        }
        SC_REPORT_ERROR( SC_ID_MORE_THAN_ONE_SIGNAL_DRIVER_,
                         msg.str().c_str() );
    }
}

bool
sc_writer_policy_check_port::
  check_port( sc_object* target, sc_port_base * port_, bool is_output )
{
    if ( is_output && sc_get_curr_simcontext()->write_check() )
    {
        // an out or inout port; only one can be connected
        if( m_output != 0) {
            sc_signal_invalid_writer( target, m_output, port_, false );
            return false;
        } else {
            m_output = port_;
        }
    }
    return true;
}

void sc_deprecated_get_data_ref()
{
    static bool warn_get_data_ref_deprecated=true;
    if ( warn_get_data_ref_deprecated )
    {
        warn_get_data_ref_deprecated=false;
	SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
	    "get_data_ref() is deprecated, use read() instead" );
    }
}

void sc_deprecated_get_new_value()
{
    static bool warn_new_value=true;
    if ( warn_new_value )
    {
        warn_new_value=false;
	SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
	    "sc_signal<T>::get_new_value() is deprecated");
    }
}

void sc_deprecated_trace()
{
    static bool warn_trace_deprecated=true;
    if ( warn_trace_deprecated )
    {
        warn_trace_deprecated=false;
	SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
	    "sc_signal<T>::trace() is deprecated");
    }
}

} // namespace sc_core

/* 
$Log: sc_signal.cpp,v $
Revision 1.9  2011/08/26 20:45:42  acg
 Andy Goodrich: moved the modification log to the end of the file to
 eliminate source line number skew when check-ins are done.

Revision 1.8  2011/02/18 20:23:45  acg
 Andy Goodrich: Copyright update.

Revision 1.7  2011/02/18 20:08:14  acg
 Philipp A. Hartmann: addition of include for sstream for MSVC.

Revision 1.6  2011/01/25 20:50:37  acg
 Andy Goodrich: changes for IEEE 1666 2011.

Revision 1.5  2010/12/07 19:50:36  acg
 Andy Goodrich: addition of writer policies, courtesy of Philipp Hartmann.

Revision 1.3  2007/04/09 21:59:49  acg
 Andy Goodrich: fixed multiple write notification bug where writes
 done outside the simulator were being treated as multiple writes.

Revision 1.2  2007/04/02 17:24:01  acg
 Andy Goodrich: added check for null writer pointers in sc_signal invalid
 writer method.

Revision 1.1.1.1  2006/12/15 20:20:04  acg
SystemC 2.3

Revision 1.7  2006/04/11 23:11:57  acg
  Andy Goodrich: Changes for reset support that only includes
  sc_cthread_process instances.

Revision 1.6  2006/03/13 20:19:44  acg
 Andy Goodrich: changed sc_event instances into pointers to sc_event instances
 that are allocated as needed. This saves considerable storage for large
 numbers of signals, etc.

Revision 1.5  2006/01/25 00:31:11  acg
 Andy Goodrich: Changed over to use a standard message id of
 SC_ID_IEEE_1666_DEPRECATION for all deprecation messages.

Revision 1.4  2006/01/24 20:46:32  acg
Andy Goodrich: changes to eliminate use of deprecated features. For instance,
using notify(SC_ZERO_TIME) in place of notify_delayed().

Revision 1.3  2006/01/18 21:42:26  acg
Andy Goodrich: Changes for check writer support, and tightening up sc_clock
port usage.

Revision 1.2  2006/01/03 23:18:26  acg
Changed copyright to include 2006.

Revision 1.1.1.1  2005/12/19 23:16:43  acg
First check in of SystemC 2.1 into its own archive.

Revision 1.14  2005/09/15 23:01:51  acg
Added std:: prefix to appropriate methods and types to get around
issues with the Edison Front End.

Revision 1.13  2005/05/08 19:04:06  acg
Fix bug in concat_set(int64,off). Other changes from 2.1 examples usage.

Revision 1.12  2005/04/04 00:15:51  acg
Changes for directory name change to sys from systemc.
Changes for sc_string going to std::string.
Changes for sc_pvector going to std::vector.
Changes for reference pools for bit and part selections.
Changes for const sc_concatref support.

Revision 1.10  2005/03/21 22:31:32  acg
Changes to sc_core namespace.

Revision 1.9  2004/09/27 21:02:54  acg
Andy Goodrich - Forte Design Systems, Inc.
   - Added a $Log comment so that CVS checkin comments will appear in
     checked out source.

*/

// Taf!
