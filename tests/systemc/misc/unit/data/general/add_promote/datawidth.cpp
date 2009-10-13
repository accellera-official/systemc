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
  bool_vector6   tmp_a;
  bool_vector6   tmp_b;
  bool_vector7   tmp_result;
  bool_vector6   tmp_add;

  while (true) {
    
    // HANDSHAKING
    do { wait(); } while (ready != 1);

    // COMPUTATION
    tmp_a = in1.read();
    tmp_b = in2.read();
    tmp_add = tmp_a.to_uint() + tmp_b.to_uint();
    tmp_result = ("0", tmp_add);

    // WRITE OUTPUT
    result.write(tmp_result);		// result = in1 + in2
    wait();
  }
}
