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

#ifndef __SIMPLE_TARGET_WRAPPER_H__
#define __SIMPLE_TARGET_WRAPPER_H__

#include "tlm.h"
#include "SimpleLTTarget1.h"

class SimpleTargetWrapper : public sc_core::sc_module
{
public:
  typedef tlm::tlm_generic_payload transaction_type;
  typedef tlm::tlm_phase phase_type;
  typedef tlm::tlm_sync_enum sync_enum_type;
  typedef tlm::tlm_fw_transport_if<> fw_interface_type;
  typedef tlm::tlm_bw_transport_if<> bw_interface_type;
  typedef tlm::tlm_target_socket<> target_socket_type;

public:
  target_socket_type socket;
  
public:
  SimpleTargetWrapper(sc_core::sc_module_name name) : 
    sc_core::sc_module(name),
    socket("socket"),
    child("child")
  {
    socket(child.socket);
  }

protected:
  SimpleLTTarget1 child;

};

#endif


