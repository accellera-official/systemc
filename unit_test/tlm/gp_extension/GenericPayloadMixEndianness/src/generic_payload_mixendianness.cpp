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

#include "SimpleBus.h"


int sc_main(int argc, char* argv[])
{
	// construction
	tlm_initiator initiator_le("initiator_little_endian", tlm::TLM_LITTLE_ENDIAN, true);
	tlm_initiator initiator_be("initiator_big_endian", tlm::TLM_BIG_ENDIAN, false);
	
	tlm_target target_le("target_little_endian", 0, 1023, tlm::TLM_LITTLE_ENDIAN);
	tlm_target target_be("target_big_endian", 0, 1023, tlm::TLM_BIG_ENDIAN);

	SimpleBus<2,2> bus("bus");

	// connections
	initiator_le.socket(bus.target_socket[0]);
	initiator_be.socket(bus.target_socket[1]);
	bus.initiator_socket[0](target_le.socket);
	bus.initiator_socket[1](target_be.socket);

	sc_start();

    return 0;
}
