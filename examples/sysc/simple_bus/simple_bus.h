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
 
  simple_bus.h : The bus.

		 The bus is derived from the following interfaces, and
	         contains the implementation of these: 
		 - blocking : burst_read/burst_write
		 - non-blocking : read/write/get_status
		 - direct : direct_read/direct_write
 
  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/

#ifndef __simple_bus_h
#define __simple_bus_h

#include <systemc.h>

#include "simple_bus_types.h"
#include "simple_bus_request.h"
#include "simple_bus_direct_if.h"
#include "simple_bus_non_blocking_if.h"
#include "simple_bus_blocking_if.h"
#include "simple_bus_arbiter_if.h"
#include "simple_bus_slave_if.h"


class simple_bus
  : public simple_bus_direct_if
  , public simple_bus_non_blocking_if
  , public simple_bus_blocking_if
  , public sc_module
{
public:
  // ports
  sc_in_clk clock;
  sc_port<simple_bus_arbiter_if> arbiter_port;
  sc_port<simple_bus_slave_if, 0> slave_port;

  SC_HAS_PROCESS(simple_bus);

  // constructor
  simple_bus(sc_module_name name_
             , bool verbose = false)
    : sc_module(name_)
    , m_verbose(verbose)
    , m_current_request(0)
  {
    // process declaration
    SC_METHOD(main_action);
    dont_initialize();
    sensitive << clock.neg();
  }

  // process
  void main_action();

  // direct BUS interface
  bool direct_read(int *data, unsigned int address);
  bool direct_write(int *data, unsigned int address);

  // non-blocking BUS interface
  void read(unsigned int unique_priority
	    , int *data
	    , unsigned int address
	    , bool lock = false);
  void write(unsigned int unique_priority
	     , int *data
	     , unsigned int address
	     , bool lock = false);
  simple_bus_status get_status(unsigned int unique_priority);

  // blocking BUS interface
  simple_bus_status burst_read(unsigned int unique_priority
			       , int *data
			       , unsigned int start_address
			       , unsigned int length = 1
			       , bool lock = false);
  simple_bus_status burst_write(unsigned int unique_priority
				, int *data
				, unsigned int start_address
				, unsigned int length = 1
				, bool lock = false);

private:
  void handle_request();
  void end_of_elaboration();
  simple_bus_slave_if * get_slave(unsigned int address);
  simple_bus_request * get_request(unsigned int priority);
  simple_bus_request * get_next_request();
  void clear_locks();

private:
  bool m_verbose;
  simple_bus_request_vec m_requests;
  simple_bus_request *m_current_request;

}; // end class simple_bus

#endif
