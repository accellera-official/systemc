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

#include "systemc.h"

SC_MODULE( ts_buf )
{
  SC_HAS_PROCESS( ts_buf );

  sc_in<bool> in;
  sc_in<bool> control;
  sc_signal_resolved& ts_out; //output

  // Constructor 
  ts_buf( sc_module_name NAME,
	  sc_signal<bool>& IN1,
	  sc_signal<bool>& CONTROL,
	  sc_signal_resolved& TS_OUT )
    : in(IN1), control(CONTROL), ts_out(TS_OUT)
  {
    SC_METHOD( entry );
    sensitive << in << control;
  }

  // Process functionality in member function below
  void entry();
};

