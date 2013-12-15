/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
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

                /*****************************************/
                /* Implementation Filename:  stimgen.cc  */
                /*****************************************/
 
#include "stimgen.h"
 
void
stimgen::entry()
{
  int i;
  int j;

  ready.write(0);

  for (i = 0; i < 64; i++) {		// integer in1 (6 bits of data)
    for (j = 0; j < 64; j++) {		// integer in2 (6 bits of data)
      in1.write(i);
      in2.write(j);
      ready.write(1);
      wait();
 
      ready.write(0);
      wait();

      cout << in1.read() << " + " << in2.read() 
	   << " = " << result.read() << endl;
    }
  }

  sc_stop();
}
