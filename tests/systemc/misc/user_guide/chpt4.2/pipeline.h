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

  pipeline.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename pipeline.h */
/* This is the interface file for module `pipeline' */

#include "systemc.h"

struct pipeline : public sc_module { 
  sc_signal<double> prod;
  sc_signal<double> quot;

  stage1_2 S1_2; // component
  stage3 S3; //component

  //Constructor 
  pipeline(const char *NAME,
	   sc_clock& CLK,
	   const sc_signal<double>& IN1,
	   const sc_signal<double>& IN2,
	   sc_signal<double>& OUT_)
    : sc_module(NAME), 
      S1_2("Stage1_2", CLK, IN1, IN2, prod, quot),
      S3("Stage3", CLK, prod, quot, OUT_)
  {
    end_module();
  }
};


