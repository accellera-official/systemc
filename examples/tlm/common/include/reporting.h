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
#ifndef __REPORTING_H__
#define __REPORTING_H__

#include <sstream>
#include <iomanip>

using std::setfill;
using std::setw;
using namespace std;

#define REPORT_INFO(source, routine, text)                 \
  { ostringstream os;                         \
    os << sc_core::sc_time_stamp() << " - " << routine << endl << "      " << text; \
    SC_REPORT_INFO(source, os.str().c_str());    \
  }

#define REPORT_WARNING(source, routine, text)              \
  { ostringstream os;                         \
    os << sc_core::sc_time_stamp() << " - " << routine << endl << "      " << text; \
    SC_REPORT_WARNING(source, os.str().c_str()); \
  }

#define REPORT_ERROR(source, routine, text)                \
  { ostringstream os;                         \
    os << sc_core::sc_time_stamp() << " - " << routine << endl << "      " << text; \
    SC_REPORT_ERROR(source, os.str().c_str());   \
  }

#define REPORT_FATAL(source, routine, text)                \
  { ostringstream os;                         \
    os << sc_core::sc_time_stamp() << " - " << routine << endl << "      " << text; \
    SC_REPORT_FATAL(source, os.str().c_str());   \
  }

#endif /* __REPORTING_H__ */
