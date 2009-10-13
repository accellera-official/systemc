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

  proc1.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef PROC1_H
#define PROC1_H

#include "systemc.h"

SC_MODULE( proc1 )
{
    SC_HAS_PROCESS( proc1 );

    sc_in_clk     clk;
    sc_in<float>  ready;
    sc_out<float> ack;

    proc1( sc_module_name    NAME,
	   sc_clock&         CLK,
	   sc_signal<float>& READY,
	   sc_signal<float>& ACK )
    : clk( CLK ),
      ready( READY ),
      ack( ACK )
    {
	SC_CTHREAD( entry, clk.pos() );
    }

    void entry();
};

#endif
