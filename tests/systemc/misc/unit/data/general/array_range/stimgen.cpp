/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  stimgen.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

                /*******************************************/
                /* Implementation Filename:  stimgen.cc  */
                /*******************************************/
 
#include "stimgen.h"
 
void
stimgen::entry()
{
  bool_vector8	d;

//  in1.write("0101_1001");
  in1.write("01011001");
  wait(2);

  cout << "IN1 = "  << in1.read() << endl;
  cout << "O1 = "   << o1.read() 
       << "  O2 = " << o2.read() << endl;
  cout << "O3 = "   << o3.read() << endl;
  cout << "O4 = "   << o4.read() << endl;
  cout << "O5 = "   << o5.read() << endl;

  sc_stop();
}
