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

  sc_report.h -- Run-time logging and reporting facilities

  Interface design by SystemC Verification Working Group.
  Implementation by Alex Riesen, Synopsys Inc.
  Original implementation by Martin Janssen, Synopsys Inc.
  Reference implementation by Cadence Design Systems, Inc., 2002-09-23:
  Norris Ip, Dean Shea, John Rose, Jasvinder Singh, William Paulsen,
  John Pierce, Rachida Kebichi, Ted Elkind, David Bailey.

  CHANGE LOG AT END OF FILE
 *****************************************************************************/

#ifndef SC_REPORT_H
#define SC_REPORT_H 1

#include <exception>
#include <string>
#include "sysc/kernel/sc_cmnhdr.h"

#if defined(_MSC_VER) && !defined(SC_WIN_DLL_WARN)
# pragma warning(push)
# pragma warning(disable:4275) // ignore missing std::exception DLL export
#endif

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

// ----------------------------------------------------------------------------
//  ENUM : sc_verbosity
//
//  Enumeration of message verbosity.
// ----------------------------------------------------------------------------

 enum sc_verbosity {
     SC_NONE = 0,
     SC_LOW = 100,
     SC_MEDIUM = 200,
     SC_HIGH = 300,
     SC_FULL = 400,
     SC_DEBUG = 500
 };

// ----------------------------------------------------------------------------
//  ENUM :
//
//  Enumeration of actions on an exception (implementation specific)
// ----------------------------------------------------------------------------

typedef unsigned sc_actions;

enum {
    SC_UNSPECIFIED  = 0x0000, // look for lower-priority rule
    SC_DO_NOTHING   = 0x0001, // take no action (ignore if other bits set)
    SC_THROW        = 0x0002, // throw an exception
    SC_LOG          = 0x0004, // add report to report log
    SC_DISPLAY      = 0x0008, // display report to screen
    SC_CACHE_REPORT = 0x0010, // save report to cache
    SC_INTERRUPT    = 0x0020, // call sc_interrupt_here(...)
    SC_STOP         = 0x0040, // call sc_stop()
    SC_ABORT        = 0x0080, // call abort()

    // default action constants
    SC_DEFAULT_INFO_ACTIONS    = SC_LOG | SC_DISPLAY,
    SC_DEFAULT_WARNING_ACTIONS = SC_LOG | SC_DISPLAY,
    SC_DEFAULT_ERROR_ACTIONS   = SC_LOG | SC_CACHE_REPORT | SC_THROW,
    SC_DEFAULT_FATAL_ACTIONS   = SC_LOG | SC_DISPLAY | SC_CACHE_REPORT | SC_ABORT,
    SC_DEFAULT_CATCH_ACTIONS   = SC_DISPLAY
};

class sc_object;
class sc_time;
struct sc_msg_def;
class sc_report;
class sc_report_handler;
SC_API const std::string sc_report_compose_message( const sc_report& );

// ----------------------------------------------------------------------------
//  CLASS : sc_report
//
//  Exception reporting
// ----------------------------------------------------------------------------

class SC_API sc_report : public std::exception
{
    friend class sc_report_handler;
    friend SC_API sc_report* sc_handle_exception();

    sc_report(); // used internally by sc_handle_exception

public:

    sc_report(const sc_report&);

    sc_report & operator=(const sc_report&);

    virtual ~sc_report() SC_NOEXCEPT_;

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

    int get_verbosity() const { return m_verbosity_level; }

    bool valid () const;

    virtual const char* what() const SC_NOEXCEPT_
        {
	    return m_what;
	}

    void swap( sc_report& );

protected:

    sc_report(sc_severity,
	      const sc_msg_def*,
	      const char* msg,
	      const char* file,
	      int line,
	      int verbosity_level=SC_MEDIUM);

    sc_severity        severity;
    const sc_msg_def*  md;
    char*              msg;
    char*              file;
    int                line;
    sc_time*           timestamp;
    char*              process_name;
    int                m_verbosity_level;
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

// ----------------------------------------------------------------------------
//  Report macros.
//
//  Use these macros to report an info, warning, error, or fatal.
// ----------------------------------------------------------------------------

#define SC_REPORT_INFO( msg_type, msg )    \
    ::sc_core::sc_report_handler::report(  \
            ::sc_core::SC_INFO, msg_type, msg, __FILE__, __LINE__ )

#define SC_REPORT_INFO_VERB( msg_type, msg, verbosity )   \
    ::sc_core::sc_report_handler::report(                 \
            ::sc_core::SC_INFO, msg_type, msg, verbosity, \
                               __FILE__ , __LINE__ )

#define SC_REPORT_WARNING( msg_type, msg ) \
    ::sc_core::sc_report_handler::report(  \
            ::sc_core::SC_WARNING, msg_type, msg, __FILE__, __LINE__ )

#define SC_REPORT_ERROR( msg_type, msg )  \
    ::sc_core::sc_report_handler::report( \
            ::sc_core::SC_ERROR, msg_type, msg, __FILE__, __LINE__ )

#define SC_REPORT_FATAL( msg_type, msg )  \
    ::sc_core::sc_report_handler::report( \
            ::sc_core::SC_FATAL, msg_type, msg, __FILE__, __LINE__ )


// SC_NORETURN_ macro, indicating that a function does not return
#if SC_CPLUSPLUS >= 201103L && (!defined(_MSC_VER) || _MSC_VER >= 1900)
// C++11: use standard C++ attribute
# define SC_NORETURN_ [[noreturn]]
#else
# if defined(_MSC_VER)
#    define SC_NORETURN_ __declspec(noreturn)
# elif defined(__GNUC__) || defined(__MINGW32__) || defined(__clang__)
#    define SC_NORETURN_ __attribute__((noreturn))
# else
#    define SC_NORETURN_ /* nothing */
# endif
#endif // SC_NORETURN_

// ----------------------------------------------------------------------------
//  FUNCTION : sc_abort()
//
//  Like abort(), never returns and aborts the current program immediately,
//  but may print additional information.
// ----------------------------------------------------------------------------

SC_NORETURN_ SC_API void sc_abort();

// ----------------------------------------------------------------------------
//  MACRO : sc_assert(expr)
//
//  Like assert(), but additionally prints the current process name
//  and simulation time, if the simulation is running.
// ----------------------------------------------------------------------------

#if defined(NDEBUG) && !defined(SC_ENABLE_ASSERTIONS) // disable assertions

#define sc_assert(expr) \
 ((void) 0)

#else // enable assertions

#define sc_assert(expr) \
 ((void)((expr) ? 0 : \
   (::sc_core::sc_assertion_failed(#expr,__FILE__,__LINE__),0)))

#endif // defined(NDEBUG) && !defined(SC_ENABLE_ASSERTIONS)

SC_NORETURN_ SC_API  void
sc_assertion_failed(const char* msg, const char* file, int line);

extern SC_API const char SC_ID_UNKNOWN_ERROR_[];
extern SC_API const char SC_ID_WITHOUT_MESSAGE_[];
extern SC_API const char SC_ID_NOT_IMPLEMENTED_[];
extern SC_API const char SC_ID_INTERNAL_ERROR_[];
extern SC_API const char SC_ID_ASSERTION_FAILED_[];
extern SC_API const char SC_ID_OUT_OF_BOUNDS_[];
extern SC_API const char SC_ID_ABORT_[];

// backward compatibility with 2.0+
extern SC_API const char SC_ID_REGISTER_ID_FAILED_[];

} // namespace sc_core

#undef SC_NORETURN_

#if defined(_MSC_VER) && !defined(SC_WIN_DLL_WARN)
# pragma warning(pop)
#endif

#include "sysc/utils/sc_report_handler.h"

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Alex Riesen, Synopsys Inc., Jan 28, 2003
  Description of Modification: Implementation for SytemC 2.1

 *****************************************************************************/

// $Log: sc_report.h,v $
// Revision 1.8  2011/08/26 20:46:19  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
// Revision 1.7  2011/05/05 17:46:04  acg
//  Philip A. Hartmann: changes in "swap" support.
//
// Revision 1.6  2011/04/19 02:39:44  acg
//  Andy Goodrich: set proper name for get_verbosity().
//
// Revision 1.5  2011/03/23 16:16:48  acg
//  Andy Goodrich: finish message verbosity support.
//
// Revision 1.4  2011/02/18 20:38:44  acg
//  Andy Goodrich: Updated Copyright notice.
//
// Revision 1.3  2011/02/01 23:02:05  acg
//  Andy Goodrich: IEEE 1666 2011 changes.
//
// Revision 1.2  2008/05/20 20:42:50  acg
//  Andy Goodrich: added sc_core namespace prefix for ID value in sc_assert()
//  macro.
//
// Revision 1.1.1.1  2006/12/15 20:20:06  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:53:11  acg
// Andy Goodrich: Added $Log command so that CVS comments are reproduced in
// the source.
//

#endif // SC_REPORT_H
