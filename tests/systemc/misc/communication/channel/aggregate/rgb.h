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

  rgb.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

struct rgb_t {
  unsigned red;
  unsigned green;
  unsigned blue;

  inline bool operator == (const rgb_t& rhs) const
  {
    return (red == rhs.red && green == rhs.green && blue == rhs.blue);
  }

  friend ostream& operator<<(ostream& os, struct rgb_t &r);
};

SC_MODULE( some_process )
{
  SC_HAS_PROCESS( some_process );

  sc_in_clk clk;

  sc_fifo<rgb_t>& color_in;
  sc_fifo<rgb_t>& color_out;

  // Constructor

  some_process( sc_module_name NAME,
	        sc_clock& CLK,
	        sc_fifo<rgb_t>& COLOR_IN,
	        sc_fifo<rgb_t>& COLOR_OUT )
    : clk(CLK), color_in(COLOR_IN),color_out(COLOR_OUT)
  {
    SC_CTHREAD( entry, clk.pos() );
  }

  void entry();
};
