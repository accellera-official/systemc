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

  display.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "define.h"


/******************************************************************************/
/***************************    Output Display Function  **********************/
/******************************************************************************/

SC_MODULE( DISPLAY )
{
    SC_HAS_PROCESS( DISPLAY );

  /*** Input and Output Ports ***/
  const sc_signal<bool>&       	ready;	// Input
  const signal_bool_vector8&   	data;	// Input
  const signal_bool_vector4&   	sum;	// Input
 
  /*** Constructor ***/
  DISPLAY ( 	sc_module_name               	NAME,
          	const sc_signal<bool>&    	READY,
               	const signal_bool_vector8& 	DATA,
               	const signal_bool_vector4& 	SUM   )
 
    : ready (READY),
      data  (DATA),	// 8 bits
      sum   (SUM)	

    {
        SC_METHOD( entry );
	sensitive(ready);
	sensitive(data);
	sensitive(sum);
    }
 
  /*** Call to Process Functionality ***/
  void entry();
 
};
 
void
DISPLAY::entry()
{
//  DISPLAY NUMBER OF 1'S IN DATA AT NEGEDGE ready

  if( ready.posedge() ) {
	cout << "Sum of "
	     << data.read()
             << " is  " << sum.read().to_uint()
             << endl;
     }
}
