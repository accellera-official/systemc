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
/*
 * @file
 *  reporting.h
 *  
 * @brief
 * Convenience macros to simplify reporting.
 */

/*****************************************************************************
  Original Authors:
    Charles Wilson, ESLX
    Bill Bunton, ESLX
*****************************************************************************/

#ifndef __REPORTING_H__
#define __REPORTING_H__

#include "tlm.h"                                        ///< TLM headers
#include <sstream>                                      ///< string streams
#include <iomanip>                                      ///< I/O manipulation
#include <stdio.h>                                      ///< standard I/O

using std::setfill;
using std::setw;
using namespace std;

#if ( defined ( REPORT_DEFINE_GLOBALS ) )
#   define RDG_EXTERN
#else /* REPORT_DEFINE_GLOBALS */
#   define RDG_EXTERN   extern
#endif /* REPORT_DEFINE_GLOBALS */

RDG_EXTERN bool          tlm_enable_info_reporting;     ///< info level reporting enable
RDG_EXTERN bool          tlm_enable_warning_reporting;  ///< warning level reporting enable
RDG_EXTERN bool          tlm_enable_error_reporting;    ///< error level reporting enable
RDG_EXTERN bool          tlm_enable_fatal_reporting;    ///< fatal level reporting enable

RDG_EXTERN ostringstream reporting_os;                  ///< reporting output string

#define REPORT_ENABLE_INFO_REPORTING()     (tlm_enable_info_reporting    = true)
#define REPORT_ENABLE_WARNING_REPORTING()  (tlm_enable_warning_reporting = true)
#define REPORT_ENABLE_ERROR_REPORTING()    (tlm_enable_error_reporting   = true)
#define REPORT_ENABLE_FATAL_REPORTING()    (tlm_enable_fatal_reporting   = true)

#define REPORT_DISABLE_INFO_REPORTING()    (tlm_enable_info_reporting    = false)
#define REPORT_DISABLE_WARNING_REPORTING() (tlm_enable_warning_reporting = false)
#define REPORT_DISABLE_ERROR_REPORTING()   (tlm_enable_error_reporting   = false)
#define REPORT_DISABLE_FATAL_REPORTING()   (tlm_enable_fatal_reporting   = false)

#define REPORT_SET_ENABLES(ri,rw,re,rf) \
{ \
  if (ri) {REPORT_ENABLE_INFO_REPORTING    ();} else {REPORT_DISABLE_INFO_REPORTING    ();} \
  if (rw) {REPORT_ENABLE_WARNING_REPORTING ();} else {REPORT_DISABLE_WARNING_REPORTING ();} \
  if (re) {REPORT_ENABLE_ERROR_REPORTING   ();} else {REPORT_DISABLE_ERROR_REPORTING   ();} \
  if (rf) {REPORT_ENABLE_FATAL_REPORTING   ();} else {REPORT_DISABLE_FATAL_REPORTING   ();} \
}

#define REPORT_ENABLE_ALL_REPORTING() \
{ \
  REPORT_SET_ENABLES(true,true,true,true); \
}

#define REPORT_DISABLE_ALL_REPORTING() \
{ \
  REPORT_SET_ENABLES(false,false,false,false); \
}

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
  SC_REPORT_##severity (source, os.str().c_str()); \
}
#endif /* REPORTING_OFF */

#define REPORT_INFO(source, routine, text) \
{ ostringstream os; \
  string routine_string (routine); \
  int colon_location; \
  if ((colon_location = routine_string.find("::")) != -1) \
  { \
    routine_string.erase(0, colon_location + 2); \
  } \
  os << sc_core::sc_time_stamp() << " - " << routine_string << endl << "      " << text; \
  if (tlm_enable_info_reporting) \
  { \
    SC_REPORT_INFO(source, os.str().c_str()); \
  } \
}

#define REPORT_WARNING(source, routine, text) \
{ ostringstream os; \
  string routine_string (routine); \
  int colon_location; \
  if ((colon_location = routine_string.find("::")) != -1) \
  { \
    routine_string.erase(0, colon_location + 2); \
  } \
  os << sc_core::sc_time_stamp() << " - " << routine_string << endl << "      " << text; \
  if (tlm_enable_warning_reporting) \
  { \
    SC_REPORT_WARNING(source, os.str().c_str()); \
  } \
}

#define REPORT_ERROR(source, routine, text) \
{ ostringstream os; \
  string routine_string (routine); \
  int colon_location; \
  if ((colon_location = routine_string.find("::")) != -1) \
  { \
    routine_string.erase(0, colon_location + 2); \
  } \
  os << sc_core::sc_time_stamp() << " - " << routine_string << endl << "      " << text; \
  if (tlm_enable_error_reporting) \
  { \
    SC_REPORT_ERROR(source, os.str().c_str()); \
  } \
}

#define REPORT_FATAL(source, routine, text) \
{ ostringstream os; \
  string routine_string (routine); \
  int colon_location; \
  if ((colon_location = routine_string.find("::")) != -1) \
  { \
    routine_string.erase(0, colon_location + 2); \
  } \
  os << sc_core::sc_time_stamp() << " - " << routine_string << endl << "      " << text; \
  if (tlm_enable_fatal_reporting) \
  { \
    SC_REPORT_FATAL(source, os.str().c_str()); \
  } \
}
  
namespace report
{
  std::string print (const tlm::tlm_sync_enum status); 
  std::string print (const tlm::tlm_phase phase);
  std::string print (const tlm::tlm_response_status);


  void print
  ( const int                 &ID              ///< Target ID/Initiator/?ID
  , tlm::tlm_generic_payload  &trans           ///< transaction to be printed
  , const char*               calling_filename= "print.cpp"
  );
  
  void print_full
  ( const int                 &ID              ///< Target ID/Initiator/?ID
  , tlm::tlm_generic_payload  &trans           ///< transaction to be printed
  , const char*               filename = "print.cpp"
  );
  
  void  
  print
  ( const int                 &ID              ///< Target ID/Initiator/?ID
  , tlm::tlm_dmi              &dmi_parameters     ///< dmi transaction to be printed
  , const char*               calling_filename = "print.cpp"
  );    
}


#endif /* __REPORTING_H__ */
