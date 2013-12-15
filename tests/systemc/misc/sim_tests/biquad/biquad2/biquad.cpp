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

  biquad.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename biquad.cc */
/* This is the implementation file for synchronous process `biquad' */

#include "biquad.h"

void biquad::entry()
{
  float Acc; // Accumulator
  float sample; // Input sample

  if ((bool) reset == true) {
    Del[0] = Del[1] = Del[2] = Del[3] = 0.0;
    out.write(0.0);
    wait();
  }
  
  while (true) {
    sample = in.read();
    Acc = Cte[0] * sample;
    for (int i = 0; i < 4; i++) {
      Acc += Cte[i+1] * Del[i];
    }
    Acc = Acc / 1024.0;
    Del[1] = Del[0]; Del[0] = sample;
    Del[3] = Del[2]; Del[2] = Acc;
    out.write(Acc);
    wait();
  }
  
} // end of entry function
