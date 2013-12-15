
//******************************************************************************
//
//  The following code is derived, directly or indirectly, from the SystemC
//  source code Copyright (c) 1996-2014 by all Contributors.
//  All Rights reserved.
//
//  The contents of this file are subject to the restrictions and limitations
//  set forth in the SystemC Open Source License (the "License");
//  You may not use this file except in compliance with such restrictions and
//  limitations. You may obtain instructions on how to receive a copy of the
//  License at http://www.accellera.org/. Software distributed by Contributors
//  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
//  ANY KIND, either express or implied. See the License for the specific
//  language governing rights and limitations under the License.
//******************************************************************************

// sc_verbosity.cpp -- test for 
//
//  Original Author: John Aynsley, Doulos, Inc.
//
// MODIFICATION LOG - modifiers, enter your name, affiliation, date and
//
// $Log: sc_verbosity.cpp,v $
// Revision 1.2  2011/05/08 19:18:46  acg
//  Andy Goodrich: remove extraneous + prefixes from git diff.
//

// Verbosity level

#include <systemc>
using namespace sc_core;
using std::cout;
using std::endl;


void report_with_verbosity(const char* msg_type)
{
  SC_REPORT_INFO_VERB(msg_type, "verbosity=SC_NONE",   SC_NONE);
  SC_REPORT_INFO_VERB(msg_type, "verbosity=SC_LOW",    SC_LOW);
  SC_REPORT_INFO_VERB(msg_type, "verbosity=SC_MEDIUM", SC_MEDIUM);
  SC_REPORT_INFO_VERB(msg_type, "verbosity=SC_HIGH",   SC_HIGH);
  SC_REPORT_INFO_VERB(msg_type, "verbosity=SC_FULL",   SC_FULL);
  SC_REPORT_INFO_VERB(msg_type, "verbosity=SC_DEBUG",  SC_DEBUG);
}


SC_MODULE(Top)
{
  SC_CTOR(Top)
  {
    report_with_verbosity("DEFAULT");
  
    sc_report_handler::set_verbosity_level( SC_NONE );
    report_with_verbosity("SC_NONE");
    sc_assert( sc_report_handler::get_verbosity_level() == 0 );
  
    sc_report_handler::set_verbosity_level( SC_MEDIUM );
    report_with_verbosity("SC_MEDIUM");
    sc_assert( sc_report_handler::get_verbosity_level() == 200 );
  
    sc_report_handler::set_verbosity_level( SC_FULL );
    report_with_verbosity("SC_FULL");
    sc_assert( sc_report_handler::get_verbosity_level() == 400 );
  
    SC_THREAD(T);
    
    f = 0;
  }
  
  int f;
  
  void T()
  {
    sc_report_handler::set_verbosity_level( SC_LOW );
    report_with_verbosity("SC_LOW");
    sc_assert( sc_report_handler::get_verbosity_level() == 100 );
  
    sc_report_handler::set_verbosity_level( SC_HIGH );
    report_with_verbosity("SC_HIGH");
    sc_assert( sc_report_handler::get_verbosity_level() == 300 );
    
    sc_report_handler::set_verbosity_level( SC_DEBUG );
    report_with_verbosity("SC_DEBUG");
    sc_assert( sc_report_handler::get_verbosity_level() == 500 );
    
    try {
      SC_REPORT_ERROR("msg_type", "msg");
    }
    catch (const sc_report& e) {
      sc_assert( e.get_verbosity() == SC_MEDIUM );
      f = 1;
    }
  }
  
};

int sc_main(int argc, char* argv[])
{
  Top top("top");
  sc_start();

  sc_assert( top.f );
  
  cout << endl << "Success" << endl;
  return 0;
}
