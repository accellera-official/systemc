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

#include "systemc.h"

#include "tlm_initiator.h"
#include "tlm_target.h"


int sc_main(int argc, char* argv[])
{
    // module instances
    tlm_initiator*       initiator_1 = 0;
    tlm_target*          target_1 = 0;

    // construction
    initiator_1 = new tlm_initiator("initiator_1");
    target_1  = new tlm_target("target_1", 0, 1023);
    
    // connections
    initiator_1->bus_port(target_1->bus_port);
    
    sc_start();
    
    if (initiator_1) {delete initiator_1;}
    if (target_1) {delete target_1;}
    
    return 0;
}
