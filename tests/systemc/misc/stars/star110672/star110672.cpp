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
Please compile the program below with SC5.0. The output of the program is:

sc_bit(1).to_char() = `

where the last char in the line is a caro in my shell. It should be

sc_bit(1).to_char() = 1
*/


#include <systemc.h>

int sc_main(int argc, char* arg[]) 
{
    sc_bit dummy; // to force the deprecation message to come out first
                  // instead of in the middle of the next message
    cout << "sc_bit(1).to_char() = " << sc_bit(1).to_char() << endl;

    return 0;
}
