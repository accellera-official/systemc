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
 
  simple_bus_master_direct.h : The monitor (master) using the direct BUS
                               interface.
 
  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/

#ifndef __simple_bus_master_direct_h
#define __simple_bus_master_direct_h

#include <systemc.h>

#include "simple_bus_direct_if.h"


SC_MODULE(simple_bus_master_direct)
{
  // ports
  sc_in_clk clock;
  sc_port<simple_bus_direct_if> bus_port;

  SC_HAS_PROCESS(simple_bus_master_direct);

  // constructor
  simple_bus_master_direct(sc_module_name name_
                           , unsigned int address
                           , int timeout
                           , bool verbose = true)
    : sc_module(name_)
    , m_address(address)
    , m_timeout(timeout)
    , m_verbose(verbose)
  {
    // process declaration
    SC_THREAD(main_action);
  }

  // process
  void main_action();

private:
  unsigned int m_address;
  int m_timeout;
  bool m_verbose;

}; // end class simple_bus_master_direct

#endif
