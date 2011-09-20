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
 
  simple_bus_test.h : The test bench.
 
  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/

#ifndef __simple_bus_test_h
#define __simple_bus_test_h

#include <systemc.h>

#include "simple_bus_master_blocking.h"
#include "simple_bus_master_non_blocking.h"
#include "simple_bus_master_direct.h"
#include "simple_bus_slow_mem.h"
#include "simple_bus.h"
#include "simple_bus_fast_mem.h"
#include "simple_bus_arbiter.h"
#include "simple_bus_test.h"

SC_MODULE(simple_bus_test)
{
  // channels
  sc_clock C1;

  // module instances
  simple_bus_master_blocking     *master_b;
  simple_bus_master_non_blocking *master_nb;
  simple_bus_master_direct       *master_d;
  simple_bus_slow_mem            *mem_slow;
  simple_bus                     *bus;
  simple_bus_fast_mem            *mem_fast;
  simple_bus_arbiter             *arbiter;

  // constructor
  SC_CTOR(simple_bus_test)
    : C1("C1")
  {
    // create instances
    master_b = new simple_bus_master_blocking("master_b", 4, 0x4c, false, 300);
    master_nb = new simple_bus_master_non_blocking("master_nb", 3, 0x38, false, 20);
    master_d = new simple_bus_master_direct("master_d", 0x78, 100);
    mem_fast = new simple_bus_fast_mem("mem_fast", 0x00, 0x7f);
    mem_slow = new simple_bus_slow_mem("mem_slow", 0x80, 0xff, 1);
    // bus = new simple_bus("bus",true); // verbose output
    bus = new simple_bus("bus");
    // arbiter = new simple_bus_arbiter("arbiter",true); // verbose output
    arbiter = new simple_bus_arbiter("arbiter");

    // connect instances
    master_d->clock(C1);
    bus->clock(C1);
    master_b->clock(C1);
    master_nb->clock(C1);
    mem_slow->clock(C1);
    master_d->bus_port(*bus);
    master_b->bus_port(*bus);
    master_nb->bus_port(*bus);
    bus->arbiter_port(*arbiter);
    bus->slave_port(*mem_slow);
    bus->slave_port(*mem_fast);
  }

  // destructor
  ~simple_bus_test()
  {
    if (master_b) {delete master_b; master_b = 0;}
    if (master_nb) {delete master_nb; master_nb = 0;}
    if (master_d) {delete master_d; master_d = 0;}
    if (mem_slow) {delete mem_slow; mem_slow = 0;}
    if (bus) {delete bus; bus = 0;}
    if (mem_fast) {delete mem_fast; mem_fast = 0;}
    if (arbiter) {delete arbiter; arbiter = 0;}
  }

}; // end class simple_bus_test

#endif
