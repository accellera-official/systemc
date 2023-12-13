/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_signal.cpp -- The sc_signal<T> primitive channel class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

  CHANGE LOG IS AT THE END OF THE FILE
 *****************************************************************************/


#include "sysc/communication/sc_communication_ids.h"
#include "sysc/utils/sc_utils_ids.h"
#include "sysc/communication/sc_signal.h"
#include "sysc/kernel/sc_reset.h"
#include "sysc/kernel/sc_simcontext_int.h"

#include <sstream>

using sc_dt::sc_logic;

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
               "(" << first_writer->kind() << ")"
            << "\n second driver "
               "`" << second_writer->name() << "' "
               "(" << second_writer->kind() << ")";

        if( check_delta )
        {
            msg << "\n conflicting write in delta cycle "
                << sc_delta_count();
        }
        SC_REPORT_ERROR( SC_ID_MORE_THAN_ONE_SIGNAL_DRIVER_,
                         msg.str().c_str() );
    }
}

bool
sc_writer_policy_check_write::only_delta()
{
    return sc_get_curr_simcontext()->write_check_conflicts_only();
}

bool
sc_writer_policy_check_port::
  check_port( sc_object* target, sc_port_base * port_, bool is_output )
{
    if ( is_output && sc_get_curr_simcontext()->write_check() &&
         !sc_get_curr_simcontext()->write_check_conflicts_only() )
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

sc_signal_channel::~sc_signal_channel()
{
    delete m_change_event_p;
}

void
sc_signal_channel::deprecated_get_data_ref() const
{
    static bool warn_get_data_ref_deprecated=true;
    if ( warn_get_data_ref_deprecated )
    {
        warn_get_data_ref_deprecated=false;
	SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
	    "sc_signal<T>::get_data_ref() is deprecated, use read() instead" );
    }
}

void
sc_signal_channel::deprecated_get_new_value() const
{
    static bool warn_new_value=true;
    if ( warn_new_value )
    {
        warn_new_value=false;
	SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
	    "sc_signal<T>::get_new_value() is deprecated");
    }
}

void
sc_signal_channel::deprecated_trace() const
{
    static bool warn_trace_deprecated=true;
    if ( warn_trace_deprecated )
    {
        warn_trace_deprecated=false;
	SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_,
	    "sc_signal<T>::trace() is deprecated");
    }
}

// create a (kernel) event, if needed
sc_event*
sc_signal_channel::lazy_kernel_event( sc_event** ev, const char* name ) const
{
    if ( !*ev ) {
        sc_hierarchy_scope scope
          ( const_cast<sc_signal_channel*>(this)->get_hierarchy_scope() );
        *ev = new sc_event( sc_event::kernel_event, name );
    }
    return *ev;
}


// get the value changed event
const sc_event&
sc_signal_channel::value_changed_event() const
{
    return *lazy_kernel_event(&m_change_event_p,"value_changed_event");
}


// do the generic update actions
void
sc_signal_channel::do_update()
{
    notify_next_delta( m_change_event_p );
    m_change_stamp = simcontext()->change_stamp();
}

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

template< sc_writer_policy POL >
void
sc_signal<bool,POL>::update()
{
    policy_type::update();
    if( !( base_type::m_new_val == base_type::m_cur_val ) ) {
        do_update();
    }
}

template< sc_writer_policy POL >
void
sc_signal<bool,POL>::do_update()
{
    // order of execution below is important, the notify_processes() call
    // must come after the update of m_cur_val for things to work properly!
    base_type::do_update();
    if ( m_reset_p ) m_reset_p->notify_processes();

    sc_event* event_p = this->m_cur_val ? m_posedge_event_p : m_negedge_event_p;
    sc_signal_channel::notify_next_delta( event_p );
}

// edge event methods

template< sc_writer_policy POL >
const sc_event&
sc_signal<bool,POL>::posedge_event() const
{
    return *this->lazy_kernel_event(&m_posedge_event_p,"posedge_event");
}

template< sc_writer_policy POL >
const sc_event&
sc_signal<bool,POL>::negedge_event() const
{
    return *this->lazy_kernel_event(&m_negedge_event_p,"negedge_event");
}


// reset support

template< sc_writer_policy POL >
sc_reset*
sc_signal<bool,POL>::is_reset() const
{
    sc_reset* result_p;
    if ( !m_reset_p ) m_reset_p = new sc_reset( this );
    result_p = m_reset_p;
    return result_p;
}

// destructor

template< sc_writer_policy POL >
sc_signal<bool,POL>::~sc_signal()
{
    delete m_negedge_event_p;
    delete m_posedge_event_p;
    delete m_reset_p;
}

// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

template< sc_writer_policy POL >
void
sc_signal<sc_logic,POL>::update()
{
    policy_type::update();
    if( !( base_type::m_new_val == base_type::m_cur_val ) ) {
        do_update();
    }
}

template< sc_writer_policy POL >
void
sc_signal<sc_logic,POL>::do_update()
{
    base_type::do_update();

    if( this->m_cur_val == sc_dt::SC_LOGIC_1 ) {
        sc_signal_channel::notify_next_delta( m_posedge_event_p );
    }
    else if( this->m_cur_val == sc_dt::SC_LOGIC_0 ) {
        sc_signal_channel::notify_next_delta( m_negedge_event_p );
    }
}

// edge event methods

template< sc_writer_policy POL >
const sc_event&
sc_signal<sc_logic,POL>::posedge_event() const
{
    return *this->lazy_kernel_event(&m_posedge_event_p,"posedge_event");
}

template< sc_writer_policy POL >
const sc_event&
sc_signal<sc_logic,POL>::negedge_event() const
{
    return *this->lazy_kernel_event(&m_negedge_event_p,"negedge_event");
}

// destructor

template< sc_writer_policy POL >
sc_signal<sc_logic,POL>::~sc_signal()
{
    delete m_negedge_event_p;
    delete m_posedge_event_p;
}


// template instantiations for writer policies

template class SC_API sc_signal_t<bool,SC_ONE_WRITER>;
template class SC_API sc_signal_t<bool,SC_MANY_WRITERS>;
template class SC_API sc_signal_t<bool,SC_UNCHECKED_WRITERS>;

template class SC_API sc_signal<bool,SC_ONE_WRITER>;
template class SC_API sc_signal<bool,SC_MANY_WRITERS>;
template class SC_API sc_signal<bool,SC_UNCHECKED_WRITERS>;

template class SC_API sc_signal_t<sc_logic,SC_ONE_WRITER>;
template class SC_API sc_signal_t<sc_logic,SC_MANY_WRITERS>;
template class SC_API sc_signal_t<sc_logic,SC_UNCHECKED_WRITERS>;

template class SC_API sc_signal<sc_logic,SC_ONE_WRITER>;
template class SC_API sc_signal<sc_logic,SC_MANY_WRITERS>;
template class SC_API sc_signal<sc_logic,SC_UNCHECKED_WRITERS>;

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
