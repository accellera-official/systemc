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
// Revision 1.7  2011/04/05 20:50:57  acg
//  Andy Goodrich:
//    (1) changes to make sure that event(), posedge() and negedge() only
//        return true if the clock has not moved.
//    (2) fixes for method self-resumes.
//    (3) added SC_PRERELEASE_VERSION
//    (4) removed kernel events from the object hierarchy, added
//        sc_hierarchy_name_exists().
//
// Revision 1.6  2011/02/18 20:27:14  acg
//  Andy Goodrich: Updated Copyrights.
//
// Revision 1.5  2011/02/13 21:47:38  acg
//  Andy Goodrich: update copyright notice.
//
// Revision 1.4  2011/01/18 20:10:45  acg
//  Andy Goodrich: changes for IEEE1666_2011 semantics.
//
// Revision 1.3  2010/11/20 17:10:57  acg
//  Andy Goodrich: reset processing changes for new IEEE 1666 standard.
//
// Revision 1.2  2008/05/22 17:06:27  acg
//  Andy Goodrich: updated copyright notice to include 2008.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:44:30  acg
// Added $Log to record CVS changes into the source.
//

#include "sysc/kernel/sc_ver.h"
#include "sysc/utils/sc_iostream.h"

namespace sc_core {

static
const char copyright[] =
    "        " SC_COPYRIGHT "\n"
    "                    ALL RIGHTS RESERVED";

static
const char systemc_version[] =
    "             SystemC " SC_VERSION
    " --- " __DATE__ " " __TIME__;

const unsigned int sc_version_major = SC_VERSION_MAJOR;
const unsigned int sc_version_minor = SC_VERSION_MINOR;
const unsigned int sc_version_patch = SC_VERSION_PATCH;
const bool         sc_is_prerelease = SC_IS_PRERELEASE;

const std::string  sc_version_originator   = SC_VERSION_ORIGINATOR;
const std::string  sc_version_release_date = SC_VERSION_RELEASE_DATE;
const std::string  sc_version_prerelease   = ( SC_VERSION_PRERELEASE + 1 );
const std::string  sc_version_string       = SC_VERSION;
const std::string  sc_copyright_string     = SC_COPYRIGHT;

const char*
sc_copyright()
{
    return copyright;
}


const char*
sc_release()
{
    return SC_RELEASE_STRING;
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
