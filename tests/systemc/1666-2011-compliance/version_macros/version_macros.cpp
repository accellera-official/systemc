
//******************************************************************************
//
//  The following code is derived, directly or indirectly, from the SystemC
//  source code Copyright (c) 1996-2011 by all Contributors.
//  All Rights reserved.
//
//  The contents of this file are subject to the restrictions and limitations
//  set forth in the SystemC Open Source License Version 2.4 (the "License");
//  You may not use this file except in compliance with such restrictions and
//  limitations. You may obtain instructions on how to receive a copy of the
//  License at http://www.systemc.org/. Software distributed by Contributors
//  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
//  ANY KIND, either express or implied. See the License for the specific
//  language governing rights and limitations under the License.
//******************************************************************************

// version_macros.cpp -- test for 
//
//  Original Author: John Aynsley, Doulos, Inc.
//
// MODIFICATION LOG - modifiers, enter your name, affiliation, date and
//
// $Log: version_macros.cpp,v $
// Revision 1.3  2011/05/08 19:18:46  acg
//  Andy Goodrich: remove extraneous + prefixes from git diff.
//

// Process control method throw_it

#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <systemc>

using namespace sc_core;
using std::cout;
using std::endl;

int sc_main(int argc, char* argv[])
{
  sc_assert(  SC_VERSION_MAJOR        == sc_version_major );
  sc_assert(  SC_VERSION_MINOR        == sc_version_minor );
  sc_assert(  SC_VERSION_PATCH        == sc_version_patch );
  sc_assert(  SC_VERSION_ORIGINATOR   == sc_version_originator );
  sc_assert(  SC_VERSION_RELEASE_DATE == sc_version_release_date );
  sc_assert(  SC_VERSION_PRERELEASE   == sc_version_prerelease );
  sc_assert(  SC_IS_PRERELEASE        == sc_is_prerelease );
  sc_assert(  SC_VERSION              == sc_version_string );
  sc_assert(  SC_COPYRIGHT            == sc_copyright_string );

  cout << "IEEE_1666_SYSTEMC = "       << IEEE_1666_SYSTEMC << endl;
  cout << endl;
  cout << "sc_version_major = "        << sc_version_major << endl;  
  cout << "sc_version_minor = "        << sc_version_minor << endl;  
  cout << "sc_version_patch = "        << sc_version_patch << endl;  
  cout << "sc_version_originator = "   << sc_version_originator << endl;  
  cout << "sc_version_release_date = " << sc_version_release_date << endl;  
  cout << "sc_version_prerelease = "   << sc_version_prerelease << endl;  
  cout << "sc_is_prerelease = "        << sc_is_prerelease << endl;  
  cout << "sc_version_string = "       << sc_version_string << endl;  
  cout << "sc_copyright_string = "     << sc_copyright_string << endl;  
  cout << endl;
  cout << "sc_release() = "            << sc_release() << endl;  

  std::string ver = sc_version();
  unsigned int i = ver.find("---");
  cout << "i = " << i << endl;
  cout << "ver.size() = " << ver.size() << endl;
  for (unsigned int j = i+3; j < ver.size(); j++)
    ver[j] = 'X';
  cout << "sc_version() = "            << ver << endl;  

  cout << "sc_copyright() = "          << sc_copyright() << endl;  
  
  //sc_start();
 
  cout << endl << "Success" << endl;
  return 0;
}
  
