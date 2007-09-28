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

#include "systemc"

#include "tlm_master.h"
#include "tlm_slave.h"

int sc_main(int argc, char* argv[])
{
    // module instances
    tlm_master*       master_1 = 0;
    tlm_slave*        slave_1 = 0;

    // construction
    master_1 = new tlm_master("master_1");
    slave_1  = new tlm_slave("slave_1", 0, 1023);
    
    // connections
    master_1->bus_port(slave_1->bus_port);
    
    sc_core::sc_start();
    
    if (master_1) {delete master_1;}
    if (slave_1) {delete slave_1;}
 
    return 0;
}
