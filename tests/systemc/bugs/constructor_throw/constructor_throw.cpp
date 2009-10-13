/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
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

  test01.cpp -- 

  Original Author: Andy Goodrich, Forte Design Systems, 14 March 2006

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

  $Log: constructor_throw.cpp,v $
  Revision 1.1.1.1  2006/12/15 20:25:56  acg
  systemc_tests-2.3

  Revision 1.1  2006/03/15 00:12:08  acg
   Andy Goodrich: Forte Design Systems
   First check in.

 *****************************************************************************/

//  This tests a bug when an exception is thrown in 
// sc_module::sc_module() for a dynamically allocated sc_module 
//  object. We are calling sc_module::end_module() on a module that has 
// already been deleted. The scenario runs like this: 
// 
// a) the sc_module constructor is entered
// b) the exception is thrown
// c) the exception processor deletes the storage for the sc_module 
// d) the stack is unrolled causing the sc_module_name instance to be deleted
// e) ~sc_module_name() calls end_module() with its pointer to the sc_module
// f) because the sc_module has been deleted its storage is corrupted, 
// either by linking it to a free space chain, or by reuse of some sort
// g) the m_simc field is garbage
// h) the m_object_manager field is also garbage
// i) an exception occurs
// 
// This does not happen for automatic sc_module instances since the 
// storage for the module is not reclaimed its just part of the stack. 
// 
// I am fixing this by having the destructor for sc_module clear the 
// module pointer in its sc_module_name instance. That cuts things at 
// step (e) above, since the pointer will be null if the module has 
// already been deleted. To make sure the module stack is okay, I call 
// end-module() in ~sc_module in the case where there is an 
// sc_module_name pointer lying around. 

#include "systemc.h"

SC_MODULE(X)
{
    SC_CTOR(X)
    {
        SC_REPORT_ERROR(SC_ID_SET_TIME_RESOLUTION_,"");
    }
};

int sc_main(int argc, char* argv[])
{
    sc_module* x_p = new X("x");

    cout << "Program completed" << endl;
    return 0;
}

