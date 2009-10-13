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

  add_chain.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#define LIMIT 	257       // Last stimulus vector memory address location
#define WIDTH 	8         // Width of stimulus vector
#define LATENCY 3	  // Latency of sum
 


extern bool_vector8 mem[];

extern void f_RESET_STIM 	(const char*, 
			  	sc_clock&, 
			  	sc_signal<bool>&, 
			  	sc_signal<bool>&, 
			  	sc_signal<int>& );

extern void f_DATA_GEN 		(const char*, 
				sc_clock&, 
				const sc_signal<bool>&, 
				signal_bool_vector8&, 
				sc_signal<int>& );

extern void f_add_chain 	(const char*, 
				sc_clock&, 
				const sc_signal<bool>&, 
				const signal_bool_vector8&, 
				signal_bool_vector4&, 
				sc_signal<bool>& );

extern void f_DISPLAY 		(const char*,
                		const sc_signal<bool>&,
                		const signal_bool_vector8&,
                		const signal_bool_vector4&);

