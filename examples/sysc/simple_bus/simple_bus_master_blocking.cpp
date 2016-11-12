/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

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
	sb_fprintf(stdout, "%s %s : blocking-read failed at address %x\n",
		   sc_time_stamp().to_string().c_str(), name(), m_address);

      for (i = 0; i < mylength; ++i)
	{
	  mydata[i] += i;
	  wait();
	}

      status = bus_port->burst_write(m_unique_priority, mydata, 
				     m_address, mylength, m_lock);
      if (status == SIMPLE_BUS_ERROR)
	sb_fprintf(stdout, "%s %s : blocking-write failed at address %x\n",
		   sc_time_stamp().to_string().c_str(), name(), m_address);

      wait(m_timeout, SC_NS);
    }
}
