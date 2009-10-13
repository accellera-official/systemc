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

  a2901_edge.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef A2901_EDGE_H
#define A2901_EDGE_H

#include "common.h"

SC_MODULE( a2901_edge )
{
    SC_HAS_PROCESS( a2901_edge );

    // clock
    const sc_clock& CLK;

    // shared state
    long* RAM;

    // inputs
    const sig9& I;
    const sig4& Badd;
    const sig4& F;
    const sig1& Q3;
    const sig1& Q0;
    const sig1& RAM3;
    const sig1& RAM0;

    // outputs
    sig4& Q;

    // temporaries
    sc_uint<3> i86;
    sc_uint<3> i87;
    sc_uint<3> q31, q20;
    sc_uint<3> f31, f20;

    // constructor
    a2901_edge( sc_module_name,
                const sc_clock& CLK_,
                long*           RAM_,
                const sig9&     I_,
                const sig4&     Badd_,
                const sig4&     F_,
                const sig1&     Q3_,
                const sig1&     Q0_,
                const sig1&     RAM3_,
                const sig1&     RAM0_,
                sig4&           Q_ )
    : CLK( CLK_ ),
      RAM( RAM_ ),
      I( I_ ),
      Badd( Badd_ ),
      F( F_ ),
      Q3( Q3_ ),
      Q0( Q0_ ),
      RAM3( RAM3_ ),
      RAM0( RAM0_ ),
      Q( Q_ )
    {
        SC_METHOD( entry );
        sensitive << CLK.posedge_event();
    }

    void entry();
};

#endif

