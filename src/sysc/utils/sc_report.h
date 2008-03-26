#ifndef SC_REPORT_H
#define SC_REPORT_H 1

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

#include <exception>
#include <string>

namespace sc_core {

// ----------------------------------------------------------------------------
//  ENUM : sc_severity
//
//  Enumeration of possible exception severity levels
// ----------------------------------------------------------------------------

enum sc_severity {
    SC_INFO = 0,        // informative only
    SC_WARNING, // indicates potentially incorrect condition
    SC_ERROR,   // indicates a definite problem
    SC_FATAL,   // indicates a problem from which we cannot recover
    SC_MAX_SEVERITY
};

typedef unsigned sc_actions;

// ----------------------------------------------------------------------------
//  ENUM : 
//
//  Enumeration of actions on an exception (implementation specific)
// ----------------------------------------------------------------------------

enum {
    SC_UNSPECIFIED  = 0x0000, // look for lower-priority rule
    SC_DO_NOTHING   = 0x0001, // take no action (ignore if other bits set)
    SC_THROW        = 0x0002, // throw an exception
    SC_LOG          = 0x0004, // add report to report log
    SC_DISPLAY      = 0x0008, // display report to screen
    SC_CACHE_REPORT = 0x0010, // save report to cache
    SC_INTERRUPT    = 0x0020, // call sc_interrupt_here(...)
    SC_STOP         = 0x0040, // call sc_stop()
    SC_ABORT        = 0x0080  // call abort()
};

class sc_object;
class sc_time;
struct sc_msg_def;
class sc_report;
const std::string sc_report_compose_message( const sc_report& );

// ----------------------------------------------------------------------------
//  CLASS : sc_report
//
//  Exception reporting
// ----------------------------------------------------------------------------

class sc_report : public std::exception
{
public:

    sc_report();

    sc_report(const sc_report&);

    sc_report & operator=(const sc_report&);

    virtual ~sc_report() throw();

    const char * get_msg_type() const;

    const char * get_msg() const
	{ return msg; }

    sc_severity get_severity() const
	{ return severity; }

    const char * get_file_name() const
	{ return file; }

    int get_line_number() const
	{ return line; }

    const sc_time & get_time() const
	{ return *timestamp; }

    const char* get_process_name() const;

    bool valid () const
        {
	    return process != 0;
	}

    virtual const char* what() const throw()
        { 
	    return m_what;
	}

protected:

    friend class sc_report_handler;


    sc_report(sc_severity,
	      const sc_msg_def*,
	      const char* msg,
	      const char* file,
	      int line);



    sc_severity        severity;
    const sc_msg_def*  md;
    char*              msg;
    char*              file;
    int                line;
    sc_time*           timestamp;
    sc_object*         process;
    char*              m_what;

public:  // backward compatibility with 2.0+

    static const char* get_message(int id);
    static bool is_suppressed(int id);
    static void make_warnings_errors(bool);
    static void register_id(int id, const char* msg);
    static void suppress_id(int id, bool); // only for info or warning
    static void suppress_infos(bool);
    static void suppress_warnings(bool);

    int get_id() const;
};
typedef std::exception sc_exception;

#define SC_DEFAULT_INFO_ACTIONS (SC_LOG | SC_DISPLAY)
#define SC_DEFAULT_WARNING_ACTIONS (SC_LOG | SC_DISPLAY)
#define SC_DEFAULT_ERROR_ACTIONS (SC_LOG | SC_CACHE_REPORT | SC_THROW)
#define SC_DEFAULT_FATAL_ACTIONS \
(SC_LOG | SC_DISPLAY | SC_CACHE_REPORT | SC_ABORT)


// ----------------------------------------------------------------------------
//  Report macros.
//
//  Use these macros to report an info, warning, error, or fatal.
// ----------------------------------------------------------------------------

#define SC_REPORT_INFO(id,msg) \
    sc_core::sc_report_handler::report( \
	    sc_core::SC_INFO, id, msg, __FILE__, __LINE__ )

#define SC_REPORT_WARNING(id,msg) \
    sc_core::sc_report_handler::report(\
	    sc_core::SC_WARNING, id, msg, __FILE__, __LINE__)

#define SC_REPORT_ERROR(id,msg) \
    sc_core::sc_report_handler::report( \
	    sc_core::SC_ERROR, id, msg, __FILE__, __LINE__ )

#define SC_REPORT_FATAL(id,msg) \
    sc_core::sc_report_handler::report( \
	    sc_core::SC_FATAL, id, msg, __FILE__, __LINE__ )

// ----------------------------------------------------------------------------
//  MACRO : sc_assert(expr)
//
//  Like assert(), but additionally prints the current process name
//  and simulation time, if the simulation is running.
// ----------------------------------------------------------------------------

#ifdef NDEBUG

#define sc_assert(expr)                                                       \
 ((void) 0)

#else

#define sc_assert(expr)                                                       \
 ((void) ((expr) ? 0 : (SC_REPORT_FATAL( SC_ID_ASSERTION_FAILED_ , #expr ), 0)))

#endif

extern const char SC_ID_UNKNOWN_ERROR_[];
extern const char SC_ID_WITHOUT_MESSAGE_[];
extern const char SC_ID_NOT_IMPLEMENTED_[];
extern const char SC_ID_INTERNAL_ERROR_[];
extern const char SC_ID_ASSERTION_FAILED_[];
extern const char SC_ID_OUT_OF_BOUNDS_[];

// backward compatibility with 2.0+
extern const char SC_ID_REGISTER_ID_FAILED_[];

} // namespace sc_core

#include "sysc/utils/sc_report_handler.h"

#endif // SC_REPORT_H
