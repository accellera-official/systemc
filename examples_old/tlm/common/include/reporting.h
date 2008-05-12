/*****************************************************************************
 *
 *   The following code is derived, directly or indirectly, from the SystemC
 *   source code Copyright (c) 1996-2007 by all Contributors.
 *   All Rights reserved.
 *
 *   The contents of this file are subject to the restrictions and limitations
 *   set forth in the SystemC Open Source License Version 3.0 (the "License");
 *   You may not use this file except in compliance with such restrictions and
 *   limitations. You may obtain instructions on how to receive a copy of the
 *   License at http://www.systemc.org/. Software distributed by Contributors
 *   under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 *   ANY KIND, either express or implied. See the License for the specific
 *   language governing rights and limitations under the License.
 *
 *   *****************************************************************************/
/*
 * @file
 *  reporting.h
 *  
 * @brief
 * Convenience macros to simplify reporting.
 */

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
*****************************************************************************/

#ifndef __REPORTING_H__
#define __REPORTING_H__

#include "tlm.h"                                      ///< TLM headers
#include <sstream>                                    ///< string streams
#include <iomanip>                                    ///< IO manipulation

using std::setfill;
using std::setw;
using namespace std;

extern ostringstream reporting_os;                     ///< reporting output string

#if ( defined ( REPORTING_OFF ) )

#define REPORT_NEW(text) {}
#define REPORT_APPEND(text) {}
#define REPORT_OUTPUT(severity, source) {}

#else /* REPORTING_OFF */

#define REPORT_NEW(text) \
{ \
  reporting_os.str (""); \
  reporting_os << text; \
}

#define REPORT_APPEND(text) \
{ \
  reporting_os << text; \
}

#define REPORT_OUTPUT(severity, source) \
{ \
  ostringstream os; \
  os << sc_core::sc_time_stamp() << " - " << __FUNCTION__ << endl << "      " << reporting_os.str(); \
  SC_REPORT_##severity (source, os.str().c_str());    \
}
#endif /* REPORTING_OFF */

#define REPORT_INFO(source, routine, text) \
  { ostringstream os; \
    string        routine_string (routine); \
    int  colon_location; \
    if ((colon_location = routine_string.find("::")) != -1) \
    { \
      routine_string.erase(0, colon_location + 2); \
    } \
    os << sc_core::sc_time_stamp() << " - " << routine_string << endl << "      " << text; \
    SC_REPORT_INFO(source, os.str().c_str()); \
  }

#define REPORT_WARNING(source, routine, text) \
  { ostringstream os; \
    string        routine_string (routine); \
    int  colon_location; \
    if ((colon_location = routine_string.find("::")) != -1) \
    { \
      routine_string.erase(0, colon_location + 2); \
    } \
    os << sc_core::sc_time_stamp() << " - " << routine_string << endl << "      " << text; \
    SC_REPORT_WARNING(source, os.str().c_str()); \
  }

#define REPORT_ERROR(source, routine, text) \
  { ostringstream os; \
    string        routine_string (routine); \
    int  colon_location; \
    if ((colon_location = routine_string.find("::")) != -1) \
    { \
      routine_string.erase(0, colon_location + 2); \
    } \
    os << sc_core::sc_time_stamp() << " - " << routine_string << endl << "      " << text; \
    SC_REPORT_ERROR(source, os.str().c_str());   \
  }

#define REPORT_FATAL(source, routine, text)                \
  { ostringstream os;                         \
    string        routine_string (routine); \
    int  colon_location; \
    if ((colon_location = routine_string.find("::")) != -1) \
    { \
      routine_string.erase(0, colon_location + 2); \
    } \
    os << sc_core::sc_time_stamp() << " - " << routine_string << endl << "      " << text; \
    SC_REPORT_FATAL(source, os.str().c_str());   \
  }

#endif /* __REPORTING_H__ */
