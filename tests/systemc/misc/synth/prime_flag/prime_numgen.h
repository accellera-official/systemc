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

  prime_numgen.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

                /***************************************/
                /* Interface Filename:	prime_numgen.h */
                /***************************************/

#include "common.h"

SC_MODULE( prime_numgen )
{

    sc_in_clk clk;

  // Inputs
        const sc_signal<bool>&	reset;
  // Outputs
        sc_signal<bool>&	prime_ready;
        signal_bool_vector&      prime;  

  // Constructor
  prime_numgen (sc_module_name		NAME,
	sc_clock&			TICK,
        const sc_signal<bool>&        	RESET,
        sc_signal<bool>&        	PRIME_READY,
        signal_bool_vector&      	PRIME )
	
      : 
	reset		(RESET),
	prime_ready	(PRIME_READY),
    	prime		(PRIME)

  	{
	    clk	(TICK); 
          SC_CTHREAD( entry, clk.pos() );
	  reset_signal_is(reset,false);
	}

  void entry();
};
