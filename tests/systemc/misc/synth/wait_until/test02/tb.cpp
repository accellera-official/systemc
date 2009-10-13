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

  tb.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "tb.h"
#include "define.h"
 
void tb::entry() 
{
  cout << "Begin Simulation" << endl;

        reset_sig = 1;
	cont1 = 0;
        i1 = 0;
        i2 = 0;
        single_cycle;
        reset_sig = 0;
 
	i1 = 5;
        single_cycle;
        single_cycle;
        single_cycle;
        single_cycle;
        single_cycle;
        single_cycle;

        set_value(cont1,1);
        single_cycle;
        single_cycle;
	
        test_value(o1,2);
        test_value(o2,3);
 
        // 2nd iteration. Test 'else' clause.
        i1 = 0;
        i2 = 0;
	cont1 = 0;
        single_cycle;
        single_cycle;
        single_cycle;
        single_cycle;
 
        test_value(o1,2);
        test_value(o2,3);
 
        long_wait;
 

  cout << "End Simulation" << endl;

  sc_stop();

}

