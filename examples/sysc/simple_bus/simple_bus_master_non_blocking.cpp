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
 
  simple_bus_master_non_blocking.cpp : The master using the non-blocking BUS
                                       interface.
 
  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/

#include "simple_bus_master_non_blocking.h"

void simple_bus_master_non_blocking::main_action()
{
  int mydata;
  int cnt = 0;
  unsigned int addr = m_start_address;

  wait(); // ... for the next rising clock edge
  while (true)
    {
      bus_port->read(m_unique_priority, &mydata, addr, m_lock);
      while ((bus_port->get_status(m_unique_priority) != SIMPLE_BUS_OK) &&
	     (bus_port->get_status(m_unique_priority) != SIMPLE_BUS_ERROR))
	wait();
      if (bus_port->get_status(m_unique_priority) == SIMPLE_BUS_ERROR)
	sb_fprintf(stdout, "%s %s : ERROR cannot read from %x\n",
		   sc_time_stamp().to_string().c_str(), name(), addr);

      mydata += cnt;
      cnt++;

      bus_port->write(m_unique_priority, &mydata, addr, m_lock);
      while ((bus_port->get_status(m_unique_priority) != SIMPLE_BUS_OK) &&
	     (bus_port->get_status(m_unique_priority) != SIMPLE_BUS_ERROR))
	wait();
      if (bus_port->get_status(m_unique_priority) == SIMPLE_BUS_ERROR)
	sb_fprintf(stdout, "%s %s : ERROR cannot write to %x\n",
		   sc_time_stamp().to_string().c_str(), name(), addr);
 
      wait(m_timeout, SC_NS);
      wait(); // ... for the next rising clock edge

      addr+=4; // next word (byte addressing)
      if (addr > (m_start_address+0x80)) {
        addr = m_start_address; cnt = 0;
      }
    }
}
