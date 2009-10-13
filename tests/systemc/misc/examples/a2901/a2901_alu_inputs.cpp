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

  a2901_alu_inputs.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "a2901_alu_inputs.h"

void
a2901_alu_inputs::entry()
{
    int4 Av;
    int4 B;

    Av = RAM[Aadd.read()];
    B  = RAM[Badd.read()];
    A.write(Av);

    switch((int)(I.read().range(2,0))) {
    case 0x0:
    case 0x1:  
      RE.write(Av);
      break;
    case 0x2:
    case 0x3:
    case 0x4:
      RE.write(0x0);
      break;
    default:
      RE.write(D.read());
    }

    switch((int)(I.read().range(2,0))) {
    case 0x4:
    case 0x5:  
      S.write(Av);
      break;
    case 0x1:
    case 0x3:
      S.write(B);
      break;
    case 0x7:
      S.write(0x0);
      break;
    default:
      S.write(Q.read());
    }
}
