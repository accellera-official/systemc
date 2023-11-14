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
 
  simple_bus_master_blocking.h : The master using the blocking BUS interface.
 
  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/

#ifndef __simple_bus_master_blocking_h
#define __simple_bus_master_blocking_h

#include <systemc.h>

#include "simple_bus_types.h"
#include "simple_bus_blocking_if.h"


SC_MODULE(simple_bus_master_blocking)
{
  // ports
  sc_in_clk clock;
  sc_port<simple_bus_blocking_if> bus_port;


  // constructor
  simple_bus_master_blocking(sc_module_name name_
			     , unsigned int unique_priority
			     , unsigned int address
                             , bool lock
                             , int timeout)
    : sc_module(name_)
    , m_unique_priority(unique_priority)
    , m_address(address)
    , m_lock(lock)
    , m_timeout(timeout)
  {
    // process declaration
    SC_THREAD(main_action);
    sensitive << clock.pos();
  }
  
  // process
  void main_action();

private:
  unsigned int m_unique_priority;
  unsigned int m_address;
  bool m_lock;
  int m_timeout;

}; // end class simple_bus_master_blocking

#endif
