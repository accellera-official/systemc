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

  T_1_1_2_5.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

SC_MODULE( foo )
{
    SC_HAS_PROCESS( foo );

    const sc_signal<bool> &input;
    sc_signal<bool> &output;

    int state;

    foo( sc_module_name NAME,
	 const sc_signal<bool>& INPUT,
	 sc_signal<bool>& OUTPUT )
        : input(INPUT), output(OUTPUT)
    {
        SC_THREAD( entry );
	sensitive(input);
    }
    void entry();
};

int sc_main(int argc, char* argv[] )
{
  return 0;
}
