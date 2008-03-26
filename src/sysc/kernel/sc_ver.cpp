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

    sc_ver.cpp -- Version and copyright information.

    Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_ver.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

#include "sysc/kernel/sc_ver.h"
#include "sysc/utils/sc_iostream.h"

namespace sc_core {

static
const char copyright[] =
    "        Copyright (c) 1996-2006 by all Contributors\n"
    "                    ALL RIGHTS RESERVED";

static
const char systemc_version[] =
    "             SystemC " SC_RELEASE_STRING " --- " __DATE__ " " __TIME__;

const char*
sc_copyright()
{
    return copyright;
}


const char*
sc_release()
{
    return SC_RELEASE_STRING "-OSCI";
}


const char*
sc_version()
{
    return systemc_version;
}


// THIS CONSTRUCTOR ROOTS OUT OLD OBJECTS AT LINK TIME
//
// Each source file which includes sc_ver.h for this SystemC version 
// will have a static instance of the class sc_api_version_XXX defined 
// in it. That object instanciation will cause the constructor below 
// to be invoked. If the version of the SystemC being linked against
// does not contain the constructor below a linkage error will occur.

SC_API_VERSION_STRING::SC_API_VERSION_STRING ()
{
}

} // namespace sc_core
// Taf!
