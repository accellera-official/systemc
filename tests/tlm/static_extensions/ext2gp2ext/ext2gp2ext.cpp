/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/
#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "tlm.h"

#include "SimpleLTInitiator_ext.h"
#include "SimpleBusLT.h"
#include "SimpleLTTarget_ext.h"
#include "extension_adaptors.h"


int sc_main(int argc, char* argv[])
{
  SimpleLTInitiator_ext initiator("initiator1", 10, 0x0);
  adapt_ext2gp<32>       bridge1("bridge1");
  SimpleBusLT<1,1>       bus("bus");
  adapt_gp2ext<32>       bridge2("bridge2");
  SimpleLTTarget_ext     target("target1");

  initiator.socket(bridge1.target_socket);
  bridge1.initiator_socket(bus.target_socket[0]);
  bus.initiator_socket[0](bridge2.target_socket);
  bridge2.initiator_socket(target.socket);

  sc_core::sc_start();
  sc_core::sc_stop();

  return 0;
}
