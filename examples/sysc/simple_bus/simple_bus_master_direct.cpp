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
 
  simple_bus_master_direct.cpp : The monitor (master) using the direct BUS
                                 interface.
 
  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/

#include "simple_bus_master_direct.h"
#include "simple_bus_types.h"

void simple_bus_master_direct::main_action()
{
  int mydata[4];
  while (true)
    {
      bus_port->direct_read(&mydata[0], m_address);
      bus_port->direct_read(&mydata[1], m_address+4);
      bus_port->direct_read(&mydata[2], m_address+8);
      bus_port->direct_read(&mydata[3], m_address+12);

      if (m_verbose)
      	sb_fprintf(stdout, "%s %s : mem[%x:%x] = (%x, %x, %x, %x)\n",
		      sc_time_stamp().to_string().c_str(), name(), m_address, 
		      m_address+15, 
		      mydata[0], mydata[1], mydata[2], mydata[3]);

      wait(m_timeout, SC_NS);
    }
}
