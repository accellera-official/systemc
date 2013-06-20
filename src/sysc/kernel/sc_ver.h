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

  sc_ver.h -- Version and copyright information.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 NO AUTOMATIC CHANGE LOG IS GENERATED, EXPLICIT CHANGE LOG AT END OF FILE
 *****************************************************************************/


#ifndef SC_VER_H
#define SC_VER_H

#include <string>

namespace sc_core {

extern const char* sc_copyright();
extern const char* sc_release();
extern const char* sc_version();

extern const unsigned int sc_version_major;
extern const unsigned int sc_version_minor;
extern const unsigned int sc_version_patch;

extern const std::string  sc_version_originator;
extern const std::string  sc_version_release_date;
extern const std::string  sc_version_prerelease;
extern const bool         sc_is_prerelease;
extern const std::string  sc_version_string;
extern const std::string  sc_copyright_string;

#define SYSTEMC_2_3_0
#define SC_API_VERSION_STRING sc_api_version_2_3_0

#define SYSTEMC_VERSION       20120701
#define SC_VERSION_ORIGINATOR "ASI"
#define SC_VERSION_MAJOR      2
#define SC_VERSION_MINOR      3
#define SC_VERSION_PATCH      0
#define SC_IS_PRERELEASE      0

/// compliancy with IEEE 1666-2011 (see 8.6.5)
#define IEEE_1666_SYSTEMC     201101L

#define SC_COPYRIGHT "        Copyright (c) 1996-2012 by all Contributors,\n        ALL RIGHTS RESERVED\n"

// token stringification

#define SC_STRINGIFY_HELPER_( Arg ) \
  SC_STRINGIFY_HELPER_DEFERRED_( Arg )
#define SC_STRINGIFY_HELPER_DEFERRED_( Arg ) \
  SC_STRINGIFY_HELPER_MORE_DEFERRED_( Arg )
#define SC_STRINGIFY_HELPER_MORE_DEFERRED_( Arg ) \
  #Arg 

#define SC_VERSION_RELEASE_DATE \
  SC_STRINGIFY_HELPER_( SYSTEMC_VERSION )

#if ( SC_IS_PRERELEASE == 1 )
#  define SC_VERSION_PRERELEASE "pub_rev"
#  define SC_VERSION \
    SC_STRINGIFY_HELPER_( SC_VERSION_MAJOR.SC_VERSION_MINOR.SC_VERSION_PATCH ) \
    "_" SC_VERSION_PRERELEASE "_" SC_VERSION_RELEASE_DATE \
    "-" SC_VERSION_ORIGINATOR
#else
#  define SC_VERSION_PRERELEASE "" // nothing
#  define SC_VERSION \
    SC_STRINGIFY_HELPER_( SC_VERSION_MAJOR.SC_VERSION_MINOR.SC_VERSION_PATCH ) \
    "-" SC_VERSION_ORIGINATOR
#endif

// THIS CLASS AND STATIC INSTANCE BELOW DETECTS BAD REV OBJECTS AT LINK TIME
//
// Each source file which includes this file for the current SystemC version 
// will have a static instance of the class sc_api_version_XXX defined
// in it. That object instance will cause the constructor below
// to be invoked. If the version of the SystemC being linked against
// does not contain the constructor below a linkage error will occur.

class SC_API_VERSION_STRING {
  public:
    SC_API_VERSION_STRING ();
};


static SC_API_VERSION_STRING api_version_check;

} // namespace sc_core

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
#endif
