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
 
  simple_bus_fast_mem.h : The memory (slave) without wait states.
 
  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/

#ifndef __simple_bus_fast_mem_h
#define __simple_bus_fast_mem_h

#include <systemc.h>

#include "simple_bus_types.h"
#include "simple_bus_slave_if.h"


class simple_bus_fast_mem
  : public simple_bus_slave_if
  , public sc_module
{
public:
  // constructor
  simple_bus_fast_mem(sc_module_name name_
		      , unsigned int start_address
		      , unsigned int end_address)
    : sc_module(name_)
    , m_start_address(start_address)
    , m_end_address(end_address)
  {
    sc_assert(m_start_address <= m_end_address);
    sc_assert((m_end_address-m_start_address+1)%4 == 0);
    unsigned int size = (m_end_address-m_start_address+1)/4;
    MEM = new int [size];
    for (unsigned int i = 0; i < size; ++i)
      MEM[i] = 0;
  }

  // destructor
  ~simple_bus_fast_mem();

  // direct Slave Interface
  bool direct_read(int *data, unsigned int address);
  bool direct_write(int *data, unsigned int address);

  // Slave Interface
  simple_bus_status read(int *data, unsigned int address);
  simple_bus_status write(int *data, unsigned int address);

  unsigned int start_address() const;
  unsigned int end_address() const;

private:
  int * MEM;
  unsigned int m_start_address;
  unsigned int m_end_address;

}; // end class simple_bus_fast_mem

inline bool simple_bus_fast_mem::direct_read(int *data, unsigned int address)
{
  return (read(data, address) == SIMPLE_BUS_OK);
}

inline bool simple_bus_fast_mem::direct_write(int *data, unsigned int address)
{
  return (write(data, address) == SIMPLE_BUS_OK);
}

inline simple_bus_status simple_bus_fast_mem::read(int *data
						   , unsigned int address)
{
  *data = MEM[(address - m_start_address)/4];
  return SIMPLE_BUS_OK;
}

inline simple_bus_status simple_bus_fast_mem::write(int *data
						    , unsigned int address)
{
  MEM[(address - m_start_address)/4] = *data;
  return SIMPLE_BUS_OK;
}

inline  simple_bus_fast_mem::~simple_bus_fast_mem()
{
  if (MEM) delete [] MEM;
  MEM = (int *)0;
}

inline unsigned int simple_bus_fast_mem::start_address() const
{
  return m_start_address;
}

inline unsigned int simple_bus_fast_mem::end_address() const
{
  return m_end_address;
}

#endif
