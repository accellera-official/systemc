/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
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

  sc_ver.h -- Version and copyright information.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_VER_H
#define SC_VER_H


namespace sc_core {

extern const char* sc_copyright();
extern const char* sc_release();
extern const char* sc_version();

#define SYSTEMC_VERSION 20070314

#define SC_RELEASE_STRING "2.2.0"
#define SC_API_VERSION_STRING sc_api_version_2_2_0


// THIS CLASS AND STATIC INSTANCE BELOW DETECTS BAD REV OBJECTS AT LINK TIME
//
// Each source file which includes this file for the current SystemC version 
// will have a static instance of the class sc_api_version_XXX defined
// in it. That object instanciation will cause the constructor below
// to be invoked. If the version of the SystemC being linked against
// does not contain the constructor below a linkage error will occur.

class SC_API_VERSION_STRING {
  public:
	SC_API_VERSION_STRING ();
};


static SC_API_VERSION_STRING api_version_check;

} // namespace sc_core

#endif
