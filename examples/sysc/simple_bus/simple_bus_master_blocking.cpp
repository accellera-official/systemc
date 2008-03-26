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
 
  simple_bus_master_blocking.cpp : The master using the blocking BUS interface.
 
  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/

#include "simple_bus_master_blocking.h"

void simple_bus_master_blocking::main_action()
{
  const unsigned int mylength = 0x10; // storage capacity/burst length in words
  int mydata[mylength];
  unsigned int i;
  simple_bus_status status;

  while (true)
    {
      wait(); // ... for the next rising clock edge
      status = bus_port->burst_read(m_unique_priority, mydata, 
				    m_address, mylength, m_lock);
      if (status == SIMPLE_BUS_ERROR)
	sb_fprintf(stdout, "%g %s : blocking-read failed at address %x\n",
		   sc_time_stamp().to_double(), name(), m_address);

      for (i = 0; i < mylength; ++i)
	{
	  mydata[i] += i;
	  wait();
	}

      status = bus_port->burst_write(m_unique_priority, mydata, 
				     m_address, mylength, m_lock);
      if (status == SIMPLE_BUS_ERROR)
	sb_fprintf(stdout, "%g %s : blocking-write failed at address %x\n",
		   sc_time_stamp().to_double(), name(), m_address);

      wait(m_timeout, SC_NS);
    }
}
