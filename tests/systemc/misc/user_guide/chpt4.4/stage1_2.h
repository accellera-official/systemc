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

  stage1_2.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename stage1_2.h */
/* This is the interface file for module `stage1_2' */

#include "systemc.h"

struct stage1_2 : public sc_module { 
  sc_signal<double> sum; // internal signal
  sc_signal<double> diff; // internal signal

  //Constructor 
  stage1_2(const char *NAME,
	   sc_clock& TICK,
	   const sc_signal<double>& IN1,
	   const sc_signal<double>& IN2,
	   sc_signal<double>& PROD,
	   sc_signal<double>& QUOT)
    : sc_module(NAME)
  {
    f_stage1("Stage1", TICK, IN1, IN2, sum, diff);
    f_stage2("Stage2", TICK, sum, diff, PROD, QUOT);
    end_module();
  }
};


