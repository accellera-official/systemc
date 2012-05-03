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
 
  pic.cpp -- Programmable Interrupt Unit.
 
  Original Author: Martin Wang, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/


#include "systemc.h"
#include "pic.h"

void pic::entry(){

	if (ireq0.read() == true) {
		intreq.write(true);
		vectno.write(0);
	} else if (ireq1.read() == true) {
			intreq.write(true);
			vectno.write(1);
		} else if (ireq2.read() == true) {
				intreq.write(true);
				vectno.write(2);
			} else if (ireq3.read() == true) {
						intreq.write(true);
						vectno.write(2);
				} else {
				} 
	if ((intack_cpu.read() == true) && (cs.read() == true)) {
			intreq.write(false);
	}
  }

// EOF

