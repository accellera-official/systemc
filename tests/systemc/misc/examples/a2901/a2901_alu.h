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

  a2901_alu.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef A2901_ALU_H
#define A2901_ALU_H

#include "common.h"

SC_MODULE( a2901_alu )
{
    SC_HAS_PROCESS( a2901_alu );

    // inputs
    const sig9& I;
    const sig4& RE;
    const sig4& S;
    const sig1& C0;

    // outputs
    sig5& R_ext;
    sig5& S_ext;
    sig4& F;
    sig1& OVR;
    sig1& C4;
    sig1& Pbar;
    sig1& Gbar;
    sig1& F3;
    sig1& F30;

    // temporaries
    int5 result;
    int5 R_ext_v;
    int5 S_ext_v;
    int5 temp_p;
    int5 temp_g;

    // constructor
    a2901_alu( sc_module_name,
               const sig9& I_,
               const sig4& RE_,
               const sig4& S_,
               const sig1& C0_,
               sig5&       R_ext_,
               sig5&       S_ext_,
               sig4&       F_,
               sig1&       OVR_,
               sig1&       C4_,
               sig1&       Pbar_,
               sig1&       Gbar_,
               sig1&       F3_,
               sig1&       F30_ )
    : I( I_ ),
      RE( RE_ ),
      S( S_ ),
      C0( C0_ ),
      R_ext( R_ext_ ),
      S_ext( S_ext_ ),
      F( F_ ),
      OVR( OVR_ ),
      C4( C4_ ),
      Pbar( Pbar_ ),
      Gbar( Gbar_ ),
      F3( F3_ ),
      F30( F30_ )
    {
        SC_METHOD( entry );
        sensitive( I );
        sensitive( RE );
        sensitive( S );
        sensitive( C0 );
    }

    void entry();
};

#endif

