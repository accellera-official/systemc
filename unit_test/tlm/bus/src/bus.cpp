/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
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

#include <systemc>
#include "tlm.h"

#include "SimpleLTMaster1.h"
#include "SimpleLTSlave1.h"
#include "SimpleLTMaster2.h"
#include "SimpleLTSlave2.h"
#include "SimpleLTMaster3.h"
#include "SimpleATMaster1.h"
#include "SimpleATSlave1.h"
#include "SimpleATMaster2.h"
#include "SimpleATSlave2.h"
#include "CoreDecouplingLTMaster.h"
#include "ExplicitLTSlave.h"
#include "SimpleBus.h"

template <int X, int Y>
class AbstractionSwitch : public sc_core::sc_module
{
public:
  SC_HAS_PROCESS(AbstractionSwitch);
  AbstractionSwitch(sc_core::sc_module_name name, SimpleBus<X, Y>& m) :
    sc_core::sc_module(name),
    mModule(m)
  {
    SC_THREAD(doSwitch);
  }

  void doSwitch()
  {
    wait(1000, sc_core::SC_NS);
    mModule.setATMode();
  }

private:
  SimpleBus<X, Y>& mModule;
};

int sc_main(int argc, char* argv[])
{
  SimpleLTMaster1 master1("master1", 10, 0x0);
  SimpleLTSlave1 slave1("slave1");

  SimpleLTMaster2 master2("master2", 10, 0x10000000);
  SimpleLTSlave2 slave2("slave2");

  SimpleLTMaster3 master3("master3", 10, 0x20000000);
  SimpleLTSlave2 slave3("slave3");

  SimpleATMaster1 master4("master4", 10, 0x30000000);
  SimpleATSlave1 slave4("slave4");

  SimpleATMaster2 master5("master5", 10, 0x40000000);
  SimpleATSlave2 slave5("slave5");

  CoreDecouplingLTMaster master6("master6", 10, 0x50000000);
  ExplicitLTSlave slave6("slave6");

  SimpleBus<6, 6> bus("bus");
  AbstractionSwitch<6,6> abstractionSwitch("abstractionSwitch", bus);

  master1.socket(bus.slave_socket[0]);
  master2.socket(bus.slave_socket[1]);
  master3.socket(bus.slave_socket[2]);
  master4.socket(bus.slave_socket[3]);
  master5.socket(bus.slave_socket[4]);
  master6.socket(bus.slave_socket[5]);
  bus.master_socket[0](slave1.socket);
  bus.master_socket[1](slave2.socket);
  bus.master_socket[2](slave3.socket);
  bus.master_socket[3](slave4.socket);
  bus.master_socket[4](slave5.socket);
  bus.master_socket[5](slave6.socket);

  sc_core::sc_start();

  return 0;
}
