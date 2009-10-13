/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
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

  pipeline.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename pipeline.cc */
/* This is the implementation file for module `pipeline' */

#include "systemc.h"
#include "f_stage1.h"
#include "f_stage2.h"
#include "stage1_2.h"
#include "f_stage3.h"
#include "pipeline.h"
#include "f_pipeline.h"

void  f_pipeline(const char *NAME,
		 sc_clock& CLK,
		 const sc_signal<double>& IN1,
		 const sc_signal<double>& IN2,
		 sc_signal<double>& OUT_)
{
  SC_NEW(pipeline(NAME, CLK, IN1, IN2, OUT_));
}
