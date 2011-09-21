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

  test.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* 
   Verifies directive line_label.
   Author: PRP 
   From Test Case misc: 22.sc
*/

#include "systemc.h"
#include "test.h"

int foobar (int a);

void test::entry() 
{
  int i,j=0,y=0,z=0;
 
 
        for (i = 0; i < 7; i++)
        {
                z = z +
                        y;
                y = z + j;
                y = foobar (z);
                wait();
        }
        wait();

}

int foobar (int a)
{
        return a + 1;
}
