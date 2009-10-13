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

  a2901_output_and_shifter.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "a2901_output_and_shifter.h"

void
a2901_output_and_shifter::entry()
{
    bool i8, i7, f0, f3, q0, q3;
    sc_uint<3> i86;
    int4 z4;

    z4  = 0x0;
    i86 = I.read().range(8,6);
    i8  = I.read()[8];
    i7  = I.read()[7];
    f0  = F.read()[0];
    f3  = F.read()[3];
    q0  = Q.read()[0];
    q3  = Q.read()[3];

    Y.write( ( ( i86 == 0x2 ) && ( OEbar.read() == 0x0)) ? A.read() :
	     (!( i86 == 0x2 ) && ( OEbar.read() == 0x0)) ? F.read() : z4);

    t_RAM0 .write( (( i8 == 0x1) && ( i7 == 0x0 )) ? f0 : 0x0);             
    t_RAM3 .write( (( i8 == 0x1) && ( i7 == 0x1 )) ? f3 : 0x0);           
    t_Q3   .write( (( i8 == 0x1) && ( i7 == 0x1))  ? q3 : 0x0);         
    t_Q0   .write( (( i8 == 0x1) && ( i7 == 0x0))  ? q0 : 0x0);         
}
