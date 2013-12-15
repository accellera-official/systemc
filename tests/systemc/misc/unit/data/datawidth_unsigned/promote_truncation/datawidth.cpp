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

  datawidth.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

                /*******************************************/
                /* Implementation Filename:  datawidth.cc  */
                /*******************************************/
 
#include "datawidth.h"
 
void
datawidth::entry()
{
  sc_unsigned   tmp_a (in1_width);
  sc_unsigned   tmp_b (in2_width);
  sc_unsigned   tmp_result (result_width);

  while (true) {
    
    // HANDSHAKING
    do { wait(); } while (ready != 1);

    // COMPUTATION
    tmp_a = in1.read();
    tmp_b = in2.read();
    tmp_result = tmp_a + tmp_b;

    // WRITE OUTPUT
    result.write(tmp_result);		// result = in1 + in2
    wait();
  }
}
