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

  main.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "proc1.h"
#include "proc2.h"

int
sc_main( int, char*[] )
{
    sc_signal<int> ack;
    sc_signal<int> ready;

    ack = 1;
    ready = 1;

    sc_clock clk( "Clock", 20, SC_NS, 0.5, 0.0, SC_NS );

    proc1 P1( "P1", clk, ack, ready );
    proc2 P2( "P2", clk, ready, ack );

    sc_start( 500, SC_NS );

    return 0;
}
  
  
