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

#ifndef __SIMPLE_INITIATOR_WRAPPER_H__
#define __SIMPLE_INITIATOR_WRAPPER_H__

#include "tlm.h"
#include "SimpleLTInitiator1.h"

class SimpleInitiatorWrapper : public sc_core::sc_module
{
public:
  typedef tlm::tlm_generic_payload transaction_type;
  typedef tlm::tlm_phase phase_type;
  typedef tlm::tlm_sync_enum sync_enum_type;
  typedef tlm::tlm_fw_transport_if<> fw_interface_type;
  typedef tlm::tlm_bw_transport_if<> bw_interface_type;
  typedef tlm::tlm_initiator_socket<> initiator_socket_type;

public:
  initiator_socket_type socket;
  
public:
  SimpleInitiatorWrapper(sc_core::sc_module_name name) : 
    sc_core::sc_module(name),
    socket("socket"),
    child("child")
  {
    child.socket(socket);
  }

protected:
  SimpleLTInitiator1 child;

};

#endif


