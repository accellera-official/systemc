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

  concat_port.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

                /**************************************/
                /* Interface Filename:	concat_port.h */
                /**************************************/

#include "common.h"

SC_MODULE( concat_port )
{

  sc_in_clk clk;

  // Inputs
	const signal_bool_vector8&	a;
	const signal_bool_vector8&	b;
	const sc_signal<int>&		mode;
	const sc_signal<bool>&		ready;
  // Outputs
	signal_bool_vector8&		c;
	signal_bool_vector16&		d;
	sc_signal<bool>&		done;

  // Constructor
  concat_port (sc_module_name 		NAME,
	sc_clock&			TICK,
	const signal_bool_vector8&	A,
	const signal_bool_vector8&	B,
	const sc_signal<int>&		MODE,
	const sc_signal<bool>&		READY,
	signal_bool_vector8&		C,
	signal_bool_vector16&		D,
	sc_signal<bool>&		DONE )
	
      : 
	a	(A),
	b	(B),
	mode	(MODE),
	ready	(READY),
	c	(C),
	d	(D),
 	done	(DONE)

  	{
          clk	(TICK);
	  SC_CTHREAD( entry, clk.pos() );
        }

  void entry();
};
