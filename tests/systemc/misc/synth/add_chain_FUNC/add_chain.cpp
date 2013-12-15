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

  add_chain.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "common.h"

/******************************************************************************/
/***************************   add_chain Class Definition  ********************/
/******************************************************************************/

SC_MODULE( add_chain )
{
    SC_HAS_PROCESS( add_chain );

    sc_in_clk clk;

    const sc_signal<bool>& 	rst;
    const signal_bool_vector8&	a_in;
          signal_bool_vector4& 	sum_out;
    	  sc_signal<bool>& 	ready;
 
    add_chain( 	sc_module_name  		NAME,
	       	      sc_clock& 		TICK_P,

    		const sc_signal<bool>&		RST,
    		const signal_bool_vector8&	A_IN,
          	      signal_bool_vector4&	SUM_OUT,
    	  	      sc_signal<bool>& 		READY
              ) 
        :
		rst	(RST), 
		a_in	(A_IN), 
		sum_out	(SUM_OUT),
		ready	(READY)	
    {
        clk(TICK_P); 
        SC_CTHREAD( entry, clk.pos() );
	reset_signal_is(rst,0);
    }
    void entry();
};

/******************************************************************************/
/***************************   add_chain Entry Function  **********************/
/******************************************************************************/
/**									     **/
/**  This function sums the number of 1's contained in a 8-bit data stream   **/ 
/**									     **/
/******************************************************************************/
void
add_chain::entry()
{
  bool_vector4     sum; 
  bool_vector8     a; 

    /***** Reset Initialization *****/
    sum_out.write(0);
    ready.write(1);
    wait(); 

    /***** MAIN LOOP *****/
    while(true) { 
      a = a_in;	
 
      /***** Handshake *****/
      ready.write(0);
      wait();
 
      /***** Computation *****/
      sum = 0;
 
      for (int i=0; i<=7; i=i+1) {
        sum = sum.to_uint() + a[i].to_bool();
        } 
 
      sum_out.write(sum);
 
      /***** Handshake *****/
      ready.write(1);
      wait();
    }
}

void
f_add_chain(  const char*  	NAME,
                    sc_clock&     	TICK,
              const sc_signal<bool>&    RST,
              const signal_bool_vector8& A_IN,
                    signal_bool_vector4& SUM_OUT,
                    sc_signal<bool>&     READY      )

{
        new add_chain(NAME, TICK, RST, A_IN, SUM_OUT, READY);
}

