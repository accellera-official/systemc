/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************
 
  stimulus.cpp -- 
 
  Original Author: Rocco Jonack, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#include <systemc.h>
#include "stimulus.h"

void stimulus::entry() {

  cycle++;
  // sending some reset values
  if (cycle<4) {
    reset.write(true);
    input_valid.write(false);
  } else {
    reset.write(false);
    input_valid.write( false );
    // sending normal mode values
    if (cycle%10==0) {
      input_valid.write(true);
      sample.write( (int)send_value1 );
      cout << "Stimuli : " << (int)send_value1 << " at time "
	   /* << sc_time_stamp() << endl; */
	   << sc_time_stamp().to_double() << endl;
      send_value1++;
    };
  }
}

