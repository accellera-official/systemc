/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
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
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
*****************************************************************************/

#ifndef MASTER_HEADER
#define MASTER_HEADER

#include <systemc.h>

//#include "bus_types.h"
#include "basic_initiator_port.h"


typedef unsigned int ADDRESS_TYPE;
typedef unsigned int DATA_TYPE;

using basic_protocol::basic_initiator_port;

class tlm_1_master : public sc_module
{
public:
  tlm_1_master( sc_module_name module_name );

  SC_HAS_PROCESS( tlm_1_master );

  basic_initiator_port<ADDRESS_TYPE,DATA_TYPE> initiator_port;

private:
  void run();
  
};


#endif
