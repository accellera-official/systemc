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

  test03.cpp -- 

  Original Author: Andy Goodrich, Forte Design Systems, 27 July 2005

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// This tests that one can get process handles on static processes.

#include "systemc.h"


SC_MODULE(Test) {
    sc_in<bool> m_clk;

    void method() {
      sc_process_handle handle = sc_get_current_process_handle(); 
      cout << handle.name() << " " << handle.proc_kind() << endl; 
    }
    void thread() {
		for (;;)
		{
			wait();
			sc_process_handle handle = sc_get_current_process_handle(); 
			cout << handle.name() << " " << handle.proc_kind() << endl; 
		}
    }
    SC_CTOR(Test) {
        SC_METHOD(method);
        sensitive << m_clk.neg();
        sc_process_handle method_handle = sc_get_current_process_handle(); 
        cout << name() << ".method " << method_handle.proc_kind() << endl; 
        SC_CTHREAD(thread,m_clk.pos());
        sc_process_handle thread_handle = sc_get_current_process_handle(); 
        cout << name() << ".thread " << thread_handle.proc_kind() << endl; 
    }
};


int sc_main(int argc,char *argv[]) {
  
    Test t1("t1");
    sc_clock clk("clk",10,SC_NS);
    
    t1.m_clk(clk);
    
    sc_start(50,SC_NS);
    return 0;
}
