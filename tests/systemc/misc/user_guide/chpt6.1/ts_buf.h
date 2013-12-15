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

  ts_buf.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename ts_buf.h */
/* This is the interface file for asynchronous process `ts_buf' */

#include "common.h"

SC_MODULE( ts_buf )
{
  SC_HAS_PROCESS( ts_buf );

  const sc_signal<bool>& in; //input
  const sc_signal<bool>& control; //input
  signal_std_logic& ts_out; //output

  //Constructor 
  ts_buf(sc_module_name NAME,
	 const sc_signal<bool>& IN_,
	 const sc_signal<bool>& CONTROL,
	 signal_std_logic& TS_OUT)
    : in(IN_), control(CONTROL), ts_out(TS_OUT)
  {
    SC_METHOD( entry );
    sensitive << in;
    sensitive << control;
  }

  // Process functionality in member function below
  void entry();
};


