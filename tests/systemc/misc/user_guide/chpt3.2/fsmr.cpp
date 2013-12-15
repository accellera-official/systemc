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

  fsmr.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* File containing functionality of the FSM recognizer */

#include "fsmr.h"

void fsm_recognizer::entry()
{
  char c;
  int state = 0;
  bool out;

  while (true) {
    do { wait(); } while (data_ready != true);
    c = input_char.read();
    cout << c;

    switch(state) {
    case 0: 
      if (c == pattern[0]) state = 1;
      else state = 0;
      out = false;
      break;
    case 1:
      if (c == pattern[1]) state = 2;
      else state = 0;
      out = false;
      break;
    case 2:
      if (c == pattern[2]) state = 3;
      else state = 0;
      out = false;
      break;
    case 3:
      if (c == pattern[3]) out = true;
      else out = false;
      state = 0;
      break;
    default:
      cout << "Error: FSM in bad state." << endl;
      break;
    }

    found.write(out);
    wait(); // for writing the found signal
    found.write(false); // reset the found signal 
  }
}
