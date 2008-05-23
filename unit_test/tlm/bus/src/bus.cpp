/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/
#include <systemc>
#include "tlm.h"

#include "SimpleLTInitiator1.h"
#include "SimpleLTTarget1.h"
#include "SimpleLTInitiator2.h"
#include "SimpleLTTarget2.h"
#include "SimpleATInitiator1.h"
#include "SimpleATInitiator2.h"
#include "SimpleATTarget1.h"
#include "SimpleATInitiator2.h"
#include "SimpleATTarget2.h"
#include "CoreDecouplingLTInitiator.h"
#include "ExplicitLTTarget.h"
#include "ExplicitATTarget.h"
#include "SimpleBusAT.h"

int sc_main(int argc, char* argv[])
{
  SimpleLTInitiator1 initiator1("initiator1", 10, 0x00000000);
  SimpleLTTarget1 target1("target1");

  SimpleLTInitiator2 initiator2("initiator2", 10, 0x10000000);
  SimpleLTTarget2 target2("target2");

  SimpleATInitiator1 initiator3("initiator3", 10, 0x20000000);
  SimpleATTarget1 target3("target3");

  SimpleATInitiator2 initiator4("initiator4", 10, 0x30000000);
  SimpleATTarget2 target4("target4");

  SimpleATInitiator2 initiator5("initiator5", 10, 0x40000000);
  SimpleATTarget2 target5("target5");

  CoreDecouplingLTInitiator initiator6("initiator6", 10, 0x50000000);
  ExplicitLTTarget target6("target6");

  CoreDecouplingLTInitiator initiator7("initiator7", 10, 0x60000000);
  ExplicitATTarget target7("target7");

  SimpleBusAT<7, 7> bus("bus");

  initiator1.socket(bus.target_socket[0]);
  initiator2.socket(bus.target_socket[1]);
  initiator3.socket(bus.target_socket[2]);
  initiator4.socket(bus.target_socket[3]);
  initiator5.socket(bus.target_socket[4]);
  initiator6.socket(bus.target_socket[5]);
  initiator7.socket(bus.target_socket[6]);
  bus.initiator_socket[0](target1.socket);
  bus.initiator_socket[1](target2.socket);
  bus.initiator_socket[2](target3.socket);
  bus.initiator_socket[3](target4.socket);
  bus.initiator_socket[4](target5.socket);
  bus.initiator_socket[5](target6.socket);
  bus.initiator_socket[6](target7.socket);

  sc_core::sc_start();
  sc_core::sc_stop();

  return 0;
}
