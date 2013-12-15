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

  display.cpp -- 

  Original Author: Rocco Jonack, Synopsys, Inc., 1999-07-30

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "display.h"

void display::entry(){
    int i = 0;
    
    wait(2);
    while(1) {
	//  Reading Data, and Counter i,j is counted up.
	while (in_valid.read()==false) wait();
	cout << "Display : " << in_data1.read() << " "
	     << in_data2.read() << " "
	     << in_data3.read() << " "
	     << in_data4.read() << " "
	     << " at " << sc_time_stamp() << endl;
	i++;
	if(i == 24) 	    sc_stop();
        wait();
    }
}


// EOF
