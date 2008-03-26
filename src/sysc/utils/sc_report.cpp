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

  sc_report.h -- Run-time logging and reporting facilities

  Interface design by SystemC Verification Working Group.
  Implementation by Alex Riesen, Synopsys Inc.
  Original implementation by Martin Janssen, Synopsys Inc.
  Reference implementation by Cadence Design Systems, Inc., 2002-09-23:
  Norris Ip, Dean Shea, John Rose, Jasvinder Singh, William Paulsen,
  John Pierce, Rachida Kebichi, Ted Elkind, David Bailey.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Alex Riesen, Synopsys Inc., Jan 28, 2003
  Description of Modification: Implementation for SytemC 2.1

 *****************************************************************************/

#include "sysc/kernel/sc_process_int.h"
#include "sysc/kernel/sc_simcontext_int.h"
#include "sysc/utils/sc_stop_here.h"
#include "sysc/utils/sc_report.h"

namespace sc_core {

static char empty_str[] = "";
static inline char * empty_dup(const char * p)
{
    return p && *p ? strdup(p): empty_str;
}

sc_report::sc_report() 
: severity(SC_INFO),
  md(0),
  msg(empty_dup(0)),
  file(empty_dup(0)),
  line(0),
  timestamp(new sc_time(sc_time_stamp())),
  process(0),
  m_what(empty_dup(0))
{
}

sc_report::sc_report(sc_severity severity_,
		     const sc_msg_def* md_,
		     const char* msg_,
		     const char* file_,
		     int line_)
: severity(severity_),
  md(md_),
  msg(empty_dup(msg_)),
  file(empty_dup(file_)),
  line(line_),
  timestamp(new sc_time(sc_time_stamp())),
  process(sc_get_curr_process_handle()),
  m_what(strdup(sc_report_compose_message(*this).c_str()))
{
}

sc_report::sc_report(const sc_report& other)
: severity(other.severity),
  md(other.md),
  msg(empty_dup(other.msg)),
  file(empty_dup(other.file)),
  line(other.line),
  timestamp(new sc_time(*other.timestamp)),
  process(other.process),
  m_what(empty_dup(other.m_what))
{
}

sc_report & sc_report::operator=(const sc_report& other)
{
    severity = other.severity;
    md = other.md;

    if ( msg != empty_str ) free(msg);
    msg = empty_dup(other.msg);

    if ( file != empty_str ) free(file);
    file = empty_dup(other.file);

    line = other.line;
    delete timestamp;
    timestamp = new sc_time(*other.timestamp);
    process = other.process;

    if ( m_what != empty_str ) free(m_what);
	m_what = empty_dup(other.m_what);

    return *this;
}

sc_report::~sc_report() throw()
{
    if ( file != empty_str )
	free(file);
    if ( msg != empty_str )
	free(msg);
    delete timestamp;
    if ( m_what != empty_str )
    free(m_what);
}

const char * sc_report::get_msg_type() const
{
    return md->msg_type;
}

//
// backward compatibility with 2.0+
//

static bool warnings_are_errors = false;
static const char unknown_id[] = "unknown id";

void sc_report_handler::report(sc_severity severity_,
			       int         id_,
			       const char* msg_,
			       const char* file_,
			       int         line_ )
{
    sc_msg_def * md = sc_report_handler::mdlookup(id_);

    if ( !md )
    {
	md = sc_report_handler::add_msg_type(unknown_id);
	md->id = id_;
    }

    if ( severity_ == SC_WARNING && warnings_are_errors )
	severity_ = SC_ERROR;

    sc_actions actions = execute(md, severity_);
    sc_report rep(severity_, md, msg_, file_, line_);

    if ( actions & SC_CACHE_REPORT )
	cache_report(rep);

    if ( severity_ == SC_ERROR )
	actions |= SC_THROW;
    else if ( severity_ == SC_FATAL )
	actions |= SC_ABORT;

    handler(rep, actions);
}

void sc_report::register_id( int id, const char* msg )
{
    if( id < 0 ) {
	SC_REPORT_ERROR( SC_ID_REGISTER_ID_FAILED_,
			 "invalid report id" );
    }
    if( msg == 0 ) {
	SC_REPORT_ERROR( SC_ID_REGISTER_ID_FAILED_,
			 "invalid report message" );
    }
    sc_msg_def * md = sc_report_handler::mdlookup(id);

    if ( !md )
	md = sc_report_handler::add_msg_type(msg);

    if ( !md ) {
	SC_REPORT_ERROR( SC_ID_REGISTER_ID_FAILED_,
			 "report_map insertion error" );
    }

    if( md->id != -1 ) {
	if( strcmp( msg, md->msg_type ) != 0 ) {
	    SC_REPORT_ERROR( SC_ID_REGISTER_ID_FAILED_,
			     "report id already exists" );
	}
	return;
    }
    md->id = id;
}

const char* sc_report::get_message( int id )
{
    sc_msg_def* md = sc_report_handler::mdlookup(id);

    return md ? md->msg_type: unknown_id;
}

bool sc_report::is_suppressed( int id )
{
    sc_msg_def* md = sc_report_handler::mdlookup(id);

    return md ? md->actions == SC_DO_NOTHING: false; // only do-nothing set
}

void sc_report::suppress_id(int id_, bool suppress)
{
    sc_msg_def* md = sc_report_handler::mdlookup(id_);

    if ( md )
	md->actions = suppress ? SC_DO_NOTHING: SC_UNSPECIFIED;
}

void sc_report::suppress_infos(bool suppress)
{
    sc_report_handler::sev_actions[SC_INFO] =
	suppress ? SC_DO_NOTHING: SC_DEFAULT_INFO_ACTIONS;
}

void sc_report::suppress_warnings(bool suppress)
{
    sc_report_handler::sev_actions[SC_WARNING] =
	suppress ? SC_DO_NOTHING: SC_DEFAULT_WARNING_ACTIONS;
}

void sc_report::make_warnings_errors(bool flag)
{
    warnings_are_errors = flag;
}

int sc_report::get_id() const
{
    return md->id;
}

} // namespace sc_core
// taf
