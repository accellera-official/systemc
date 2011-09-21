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

  a2901_alu.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "a2901_alu.h"

void
a2901_alu::entry()
{
    int I53 = I.read().range(5,3);

    R_ext_v = (I53 == 0x1) ? sc_int<5> (0xf & ~(RE.read()))
                           : sc_int<5>( RE.read() );
    S_ext_v = (I53 == 0x2) ? sc_int<5> (0xf & ~(S.read()) )
                           : sc_int<5>( S.read() );
    R_ext.write(R_ext_v);
    S_ext.write(S_ext_v);
    
    switch (I53) {
    case 0x0:
    case 0x1:
    case 0x2:  
      result = R_ext_v + S_ext_v + C0.read();
      break;
    case 0x3:  
      result = R_ext_v | S_ext_v;
      break;
    case 0x4:  
      result = R_ext_v & S_ext_v;
      break;
    case 0x5:  
      result = ~(R_ext_v) & S_ext_v;
      break;
    case 0x6:  
      result = R_ext_v ^ S_ext_v;
      break;
    default:
      result = ~(R_ext_v ^ S_ext_v);
    }

    F.write(result);
    OVR.write(!(R_ext_v[3] ^ S_ext_v[3]) & (R_ext_v[3] ^ result[3]));
    //C4.write(result[4]);
    C4.write((bool)result[4]);
    temp_p = R_ext_v | S_ext_v;
    temp_g = R_ext_v & S_ext_v;
    Pbar.write((temp_p[0] & temp_p[1] & temp_p[2] & temp_p[3]) ? 0 : 1);
    Gbar.write((temp_g[3] |
	       (temp_p[3] & temp_g[2]) |
	       (temp_p[3] & temp_p[2] & temp_g[1]) |
	       (temp_p[3] & temp_p[2] & temp_p[1] & temp_g[0])) ? 0 : 1);
    //F3.write(result[3]);
    F3.write((bool)result[3]);
    F30.write((result[3] | result[2] | result[1] | result[0]) ? 0 : 1);
}
