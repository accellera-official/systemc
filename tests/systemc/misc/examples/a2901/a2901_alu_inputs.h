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

  a2901_alu_inputs.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef A2901_ALU_INPUTS_H
#define A2901_ALU_INPUTS_H

#include "common.h"

SC_MODULE( a2901_alu_inputs )
{
    SC_HAS_PROCESS( a2901_alu_inputs );

    // shared state
    long* RAM;

    // inputs
    const sig9& I;
    const sig4& Aadd;
    const sig4& Badd;
    const sig4& D;
    const sig4& Q;

    // outputs
    sig4& RE;
    sig4& S;
    sig4& A;

    // constructor
    a2901_alu_inputs( sc_module_name,
                      long* RAM_,
                      const sig9& I_,
                      const sig4& Aadd_,
                      const sig4& Badd_,
                      const sig4& D_,
                      const sig4& Q_,
                      sig4&       RE_,
                      sig4&       S_,
                      sig4&       A_ )
    : RAM( RAM_ ),
      I( I_ ),
      Aadd( Aadd_ ),
      Badd( Badd_ ),
      D( D_ ),
      Q( Q_ ),
      RE( RE_ ),
      S( S_ ),
      A( A_ )
    {
        SC_METHOD( entry );
        sensitive << I;
        sensitive << Aadd;
        sensitive << Badd;
        sensitive << D;
        sensitive << Q;
    }

    void entry();
};

#endif

