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

  nor.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename nor.h */
/* This is the interface file for asynchronous process 'nor' */

#include "systemc.h"

SC_MODULE( nor )
{
  SC_HAS_PROCESS( nor );

  sc_in<bool>  a;
  sc_in<bool>  b;
  sc_out<bool> c;

  // Constructor 
  nor( sc_module_name NAME,
       sc_signal<bool>& A,
       sc_signal<bool>& B,
       sc_signal<bool>& C )
  {
    a(A);
	b(B);
	c(C);
    SC_METHOD( entry );
    sensitive << a << b;
  }

  // Process functionality in member function below
  void entry();
};

