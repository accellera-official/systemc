/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

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

  sc_in_clk clk;

  sc_fifo<rgb_t>& color_in;
  sc_fifo<rgb_t>& color_out;

  // Constructor

  some_process( sc_module_name NAME,
	        sc_clock& CLK,
	        sc_fifo<rgb_t>& COLOR_IN,
	        sc_fifo<rgb_t>& COLOR_OUT )
    : color_in(COLOR_IN),color_out(COLOR_OUT)
  {
    clk(CLK);
	SC_THREAD( entry );
	sensitive <<  clk.pos();
  }

  void entry();
};
