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

  test05.cpp -- 

  Original Author: Andy Goodrich, Forte Design Systems, 2005-09-12

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of error detection: interface being supplied twice to a multi-port.

#include "systemc.h"

SC_MODULE(TB)
{
	SC_CTOR(TB)
	{
		m_port(m_signal);
		m_multi_port(m_signal);
		m_multi_port(m_port);
	}
	sc_port<sc_signal_inout_if<int>,0> m_multi_port;
	sc_inout<int>                      m_port;
	sc_signal<int>                     m_signal;
};

int sc_main(int, char**) 
{
	sc_clock clock;
	TB       tb("tb");

	sc_start(1, SC_NS);
    return 0;
} 

